/*
 * lbs_logic.cc

 *
 *  Created on: 2013-7-15
 *      Author: huaiyu
 */
#include <string>
#include <vector>
#include <map>
#include "lbs_logic.h"
#include "json/json.h"
#include "log/mig_log.h"
#include "config/config.h"
#include "basic/base64.h"
#include "logic_comm.h"
#include "dic_comm.h"
#include "db_comm.h"

namespace {
int SplitStringChr( const char *str, const char *char_set,
				   std::vector<std::string> &out )
{
	using std::string;

	assert(str != NULL);
	assert(char_set != NULL);

	out.clear();

	if (0 == char_set[0]) {
		if (str[0])
			out.push_back(str);
		return (int)out.size();
	}

	const char *find_ptr = NULL;
	str += ::strspn(str, char_set);
	while (str && (find_ptr=::strpbrk(str, char_set))) {
		if (str != find_ptr)
			out.push_back(string(str, find_ptr));
		str = find_ptr + ::strspn(find_ptr, char_set);
	}
	if (str && str[0])
		out.push_back(str);

	return (int)out.size();
}
}

namespace mig_lbs {

LBSLogic *LBSLogic::inst_ = NULL;

LBSLogic* LBSLogic::GetInstance() {
	if (NULL == inst_)
		inst_ = new LBSLogic();
	return inst_;
}

void LBSLogic::FreeInstance() {
	delete inst_;
}

int LBSLogic::SetPOI(int64 user_id, double longitude, double latitude,
		const std::string& data, std::string& response, std::string& err_msg) {
	using namespace std;

	if (0 == user_id)
		return -1;

	int64 poi_id = redis_conn_.FindUserPOIID(user_id);

	if (0 == poi_id) {
		// 不存在POI,创建
		if (0 != bd_lbs_coon_.CreatePOI(user_id, longitude, latitude, poi_id, response, err_msg)) {
			return -1;
		}

		ASSERT(poi_id != 0);
		bd_lbs_coon_.CreatePOIEX(user_id, poi_id, data, response, err_msg);

		redis_conn_.BindUserPOI(user_id, poi_id);
	} else {
		// 已存在,更新
		bd_lbs_coon_.UpdatePOI(poi_id, longitude, latitude, response, err_msg);

		bd_lbs_coon_.UpdatePOIEX(poi_id, data, response, err_msg);
	}

	return 0;
}

int LBSLogic::DelPOI(int64 user_id, std::string& response,
		std::string& err_msg) {
	using namespace std;

	if (0 == user_id)
		return -1;

	int64 poi_id = redis_conn_.FindUserPOIID(user_id);

	if (0 != poi_id) {
		//bd_lbs_coon_.DelPOIEX(poi_id, response, err_msg);
		bd_lbs_coon_.DelPOI(poi_id, response, err_msg);
	}

	redis_conn_.DeleteUserPOI(user_id);

	return 0;
}

int LBSLogic::SearchNearby(double longitude, double latitude, uint32 radius,
		const std::string& filter, uint32 page_index, uint32 page_size, Json::Value &result,
		std::string& response, std::string& err_msg) {

	bd_lbs_coon_.SearchNearby(longitude, latitude, radius, filter,
			page_index, page_size, result, response, err_msg);

	return 0;
}

bool LBSLogic::Init() {

}

bool LBSLogic::OnMsgRead(struct server* srv, int socket, const void* msg, int len) {
    const char* packet_stream = (const char*)(msg);
	packet::HttpPacket packet(packet_stream, len-1);
	std::string type;
	packet.GetPacketType(type);

	Json::Value root(Json::objectValue);
	int ret_status;
	std::string ret_msg;
	if (type=="setuserpos") {
		OnMsgSetPoi(packet, root, ret_status, ret_msg);
	} else if (type=="searchnearby") {
		OnMsgSearchNearby(packet, root, ret_status, ret_msg);
	} else if (type=="searchcollect"){
		OnMsgNearCollect(packet,root,ret_status,ret_msg);
	}else if (type=="nearmusic"){
		OnMsgNearMusic(packet,root,ret_status,ret_msg);
	}else
		return true;

	root["status"] = ret_status;
	if (ret_status != 1)
		root["msg"] = ret_msg;
	else
		root["msg"] = "success";

	Json::FastWriter wr;
	std::string res = wr.write(root);
	SomeUtils::SendFull(socket, res.c_str(), res.length());

	MIG_DEBUG(USER_LEVEL, "lbs request:%s, response:%s", type.c_str(), res.c_str());

    return true;
}

bool LBSLogic::OnMsgNearMusic(packet::HttpPacket& packet, Json::Value &result, 
							  int &status, std::string &msg){
	status = 0;
	msg.clear();

	std::string uid_str, location_str, radius_str, page_index_str, page_size_str;
	if (!packet.GetAttrib("uid", uid_str)) {
	  msg = "uid未指定";
	  return false;
	}
	if (!packet.GetAttrib("location", location_str)) {
	  msg = "location未指定";
	  return false;
	}
	if (!packet.GetAttrib("radius", radius_str)) {
	  radius_str = "1000";
	}
	if (!packet.GetAttrib("page_index", page_index_str)) {
	  page_index_str = "0";
	}
	if (!packet.GetAttrib("page_size", page_size_str)) {
	  page_size_str = "10";
	}

	int64 uid = atoll(uid_str.c_str());
	if (0 == uid) {
	  msg = "无效uid";
	  return false;
	}
	std::vector<std::string> location_pair;
	if (2 != SplitStringChr(location_str.c_str(), ",", location_pair)) {
		msg = "location参数格式错误";
		return false;
	}

	double latitude = atof(location_pair[0].c_str());
	double longitude = atof(location_pair[1].c_str());
	uint32 radius = atoi(radius_str.c_str());
	int page_index = atoi(page_index_str.c_str());
	int page_size = atoi(page_size_str.c_str());
	std::string response;
	Json::Value content;
	if (0 != SearchNearby(longitude, latitude, radius, "", page_index, page_size,
		content, response, msg)) {
			return false;
	}
	const Json::Value &items = content["content"];
	if (items.empty()){
		//Json::Value &usersmusic = result["result"];
		result["result"] = "";
		status = 1;
		return true;
	}

	Json::Value &usersmusic = result["result"]["nearUser"];
	std::map<std::string, bool> mapExist;
	std::vector<std::string> vec_users;
	typedef std::map<std::string, std::string> UserSongMap;
	UserSongMap map_songs;
	std::string nick_name, sex,pic;
	//test 100000 100001 100002
	int jk = 0;
	for (Json::Value::iterator it = items.begin();
		it != items.end();
		++it,jk++) {
			const Json::Value &item = *it;
			Json::Value val;
			if (!item.isMember("ext"))
				continue;
			std::string uid_str = item["ext"]["user_id"].asString();
			//test
		//////////////////////////////////////////////////////////////////////////
			if (jk==0){
				uid_str = "100000";
			}else if (jk==1){
				uid_str = "100001";
			}else{
				uid_str = "100002";
			}
			//////////////////////////////////////////////////////////////////////////
			if (uid_str.empty())
				continue;
			if (mapExist.end() != mapExist.find(uid_str))
				continue;

			mapExist[uid_str] = true;
			val["users"]["userid"] = uid_str;
			val["users"]["latitude"] = item["latitude"];
			val["users"]["longitude"] = item["longitude"];
			val["users"]["distance"] = item["distance"];
			storage::DBComm::GetUserInfos(uid_str, nick_name, sex,pic);
			val["users"]["nickname"] = nick_name;
			val["users"]["sex"] = sex;
			vec_users.push_back(uid_str);
			//users.append(val)
			usersmusic.append(val);
	}


	storage::MemComm::GetUserCurrentSong(vec_users, map_songs);
	for (Json::Value::iterator it = usersmusic.begin();
		it != usersmusic.end(); ++it) {
			Json::Value &item = *it;
			const std::string uid_str = item["users"]["userid"].asString();
			UserSongMap::const_iterator find = map_songs.find(uid_str);
			if (map_songs.end() != find){
				//item["cur_music"] = ::atoi(find->second.c_str());
				GetUserCurrentMusic(find->second,item);
			}
 			else
 				item["music"] = 0;
	}
	status = 1;
	return true;
}

bool LBSLogic::OnMsgNearCollect(packet::HttpPacket &packet, Json::Value &result, int &status, std::string &msg)
{

	status = 0;

	msg.clear();
	std::string uid_str, taruid_str,location_str, radius_str, page_index_str, page_size_str;
	if (!packet.GetAttrib("uid", uid_str)) {
		msg = "uid未指定";
		return false;
	}

	if (!packet.GetAttrib("taruid", taruid_str)) {
		msg = "taruid未指定";
		return false;
	}

	if (!packet.GetAttrib("location", location_str)) {
		msg = "location未指定";
		return false;
	}
	if (!packet.GetAttrib("radius", radius_str)) {
		radius_str = "1000";
	}
	if (!packet.GetAttrib("page_index", page_index_str)) {
		page_index_str = "0";
	}
	if (!packet.GetAttrib("page_size", page_size_str)) {
		page_size_str = "10";
	}

	std::vector<std::string> location_pair;
	if (2 != SplitStringChr(location_str.c_str(), ",", location_pair)) {
		msg = "location参数格式错误";
		return false;
	}

	double latitude = atof(location_pair[0].c_str());
	double longitude = atof(location_pair[1].c_str());
	uint32 radius = atoi(radius_str.c_str());
	int page_index = atoi(page_index_str.c_str());
	int page_size = atoi(page_size_str.c_str());

	int64 uid = atoll(uid_str.c_str());
	int64 taruid = atoll(taruid_str.c_str());
	std::string response;
	Json::Value content;
	if (0 != SearchNearby(longitude, latitude, radius, "", page_index, page_size,
		content, response, msg)) {
			return false;
	}

	//Json::Value &users = result["result"]["nearUser"];
	std::map<std::string, bool> mapExist;
	const Json::Value &items = content["content"];//多少用户即多少歌曲
	//获取收藏列表大小
	int32 collect_num = redis_conn_.GetCollect(taruid);

	for (Json::Value::iterator it = items.begin();//获取每个用户收藏列表
		it != items.end();
		++it) {
			const Json::Value &item = *it;
			Json::Value val;
			if (!item.isMember("ext"))
				continue;
			std::string uid_str = item["ext"]["user_id"].asString();
			if (uid_str.empty())
				continue;
			if (mapExist.end() != mapExist.find(uid_str))
				continue;
			if (atoll(uid_str.c_str())!=taruid)
			    collect_num +=redis_conn_.GetCollect(atoll(uid_str.c_str()));
	}

	result["result"]["mynum"] = collect_num;
	result["result"]["nearnum"] = items.size();

	status = 1;
	return true;
}

bool LBSLogic::OnMsgSetPoi(packet::HttpPacket& packet, Json::Value &result,
		int &status, std::string &msg) {
	status = 0;
	msg.clear();

	std::string uid_str, location_str;
	if (!packet.GetAttrib("uid", uid_str)) {
		msg = "uid未指定";
		return false;
	}
	if (!packet.GetAttrib("location", location_str)) {
		msg = "location未指定";
		return false;
	}

	int64 uid = atoll(uid_str.c_str());
	if (0 == uid) {
		msg = "无效uid";
		return false;
	}

	std::vector<std::string> location_pair;
	if (2 != SplitStringChr(location_str.c_str(), ",", location_pair)) {
		msg = "location参数格式错误";
		return false;
	}

	double latitude = atof(location_pair[0].c_str());
	double longitude = atof(location_pair[1].c_str());

	std::string response;
	if (0 != SetPOI(uid, longitude, latitude, "", response, msg)) {
		return false;
	}

	status = 1;
	return true;
}

bool LBSLogic::OnMsgSearchNearby(packet::HttpPacket& packet, Json::Value &result,
		int &status, std::string &msg) {
	status = 0;
	msg.clear();

	std::string uid_str, location_str, radius_str, page_index_str, page_size_str;
	if (!packet.GetAttrib("uid", uid_str)) {
		msg = "uid未指定";
		return false;
	}
	if (!packet.GetAttrib("location", location_str)) {
		msg = "location未指定";
		return false;
	}
	if (!packet.GetAttrib("radius", radius_str)) {
		radius_str = "1000";
	}
	if (!packet.GetAttrib("page_index", page_index_str)) {
		page_index_str = "0";
	}
	if (!packet.GetAttrib("page_size", page_size_str)) {
		page_size_str = "10";
	}

	int64 uid = atoll(uid_str.c_str());
	if (0 == uid) {
		msg = "无效uid";
		return false;
	}

	std::vector<std::string> location_pair;
	if (2 != SplitStringChr(location_str.c_str(), ",", location_pair)) {
		msg = "location参数格式错误";
		return false;
	}

	double latitude = atof(location_pair[0].c_str());
	double longitude = atof(location_pair[1].c_str());
	uint32 radius = atoi(radius_str.c_str());
	int page_index = atoi(page_index_str.c_str());
	int page_size = atoi(page_size_str.c_str());

	std::string response;
	std::string pic;
	Json::Value content;
	if (0 != SearchNearby(longitude, latitude, radius, "", page_index, page_size,
			content, response, msg)) {
		return false;
	}

	Json::Value &users = result["result"]["nearUser"];
	std::map<std::string, bool> mapExist;
	std::vector<std::string> vec_users;
	typedef std::map<std::string, std::string> UserSongMap;
	UserSongMap map_songs;
	std::string nick_name, sex;
	const Json::Value &items = content["content"];
	for (Json::Value::iterator it = items.begin();
		it != items.end();
		++it) {
		const Json::Value &item = *it;
		Json::Value val;
		if (!item.isMember("ext"))
			continue;
		std::string uid_str = item["ext"]["user_id"].asString();
		if (uid_str.empty())
			continue;
		if (mapExist.end() != mapExist.find(uid_str))
			continue;

		mapExist[uid_str] = true;
		val["userid"] = uid_str;
		val["latitude"] = item["latitude"];
		val["longitude"] = item["longitude"];
		val["distance"] = item["distance"];

		storage::DBComm::GetUserInfos(uid_str, nick_name, sex,pic);
		val["nickname"] = nick_name;
		val["sex"] = sex;

		vec_users.push_back(uid_str);

		users.append(val);
	}

	storage::MemComm::GetUserCurrentSong(vec_users, map_songs);
	for (Json::Value::iterator it = users.begin();
		it != users.end(); ++it) {
		Json::Value &item = *it;
		const std::string uid_str = item["userid"].asString();
		UserSongMap::const_iterator find = map_songs.find(uid_str);
		if (map_songs.end() != find)
			item["cur_music"] = 0;
		else
			item["cur_music"] = 0;
	}

	result["size"] = users.size();
	result["total"] = content["total"];

	status = 1;
	LOG_DEBUG("+++++++++++++++++++++");
	return true;
}

LBSLogic::LBSLogic() {
	bool r = false;
	std::string path = DEFAULT_CONFIG_PATH;
	ThreadKey::InitThreadKey();
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	if(config==NULL)
		return;

	r = config->LoadConfig(path);
	storage::DBComm::Init(config->mysql_db_list_);
	storage::MemComm::Init(config->mem_list_);
	storage::RedisComm::Init(config->redis_list_);
}

LBSLogic::~LBSLogic() {
	storage::DBComm::Dest();
	storage::MemComm::Dest();
	storage::RedisComm::Dest();

	ThreadKey::DeinitThreadKey ();
}

void LBSLogic::GetUserCurrentMusic(const std::string &content, Json::Value &item){
//value {"songid":"10000","state":"1","type":"mm","tid":"1","name":"艳阳天","singer":"窦唯"}
	bool r = false;
	Json::Reader reader;
	Json::Value  root;
	Json::Value music;
	std::string songid;
	std::string state;
	std::string name;
	std::string singer;
	std::string mode;
	std::string tid;
	std::string hq_content_url;
	std::string content_url;
	std::string music_info;
	std::string hot_num;
	std::string clt_num;
	std::string cmt_num;
	int is_like = 0;
	std::string uid;
	base::MusicInfo smi;
	std::string b64title;
	std::string b64artist;
	std::string b64album;

	//解析json
	r = reader.parse(content.c_str(),root);
	if (!r){
		LOG_ERROR("parser json error");
		return;
	}
	LOG_DEBUG2("%s",content.c_str());
	if (root.isMember("songid")){
		songid = root["songid"].asString();
	}
	
	if (root.isMember("state")){
		state = root["state"].asString();
	}

	if (root.isMember("name")){
		name = root["name"].asString();
	}

	if (root.isMember("singer")){
		singer = root["singer"].asString();
	}

	if (root.isMember("type")){
		mode = root["type"].asString();
	}

	if (root.isMember("tid")){
		tid = root["tid"].asString(); 
	}

	if (state!="2"){//非本地缓存歌曲
		r = storage::RedisComm::GetMusicInfos(songid,music_info);
		if (!r){
			LOG_ERROR("song no vailed");
			return;
		}
		r =smi.UnserializedJson(music_info);
		if (!r){
			LOG_ERROR("song parser error");
			return;
		}
		//获取URL
		storage::DBComm::GetMusicUrl(smi.id(),hq_content_url,content_url);
		smi.set_hq_url(hq_content_url);
		smi.set_url(content_url);
		smi.set_music_time(0);
		Base64Decode(smi.title(),&b64title);
		Base64Decode(smi.artist(),&b64artist);
		Base64Decode(smi.album_title(),&b64album);

		uid = item["users"]["userid"].asString();
		LOG_DEBUG2("uid[%s]",uid.c_str());
		r = storage::RedisComm::IsCollectSong(uid,songid);
		if (r)
			is_like = 1;
		else
			is_like = 0;
		//获取评论 热度 收藏数

		GetMusicHotCltCmt(songid,hot_num,cmt_num,clt_num);
		smi.set_music_clt(clt_num);
		smi.set_music_cmt(cmt_num);
		smi.set_music_hot(hot_num);

		item["music"]["id"] = smi.id();
		item["music"]["title"] = b64title;
		item["music"]["artis"] = b64artist;
		item["music"]["url"] = smi.url();
		item["music"]["hq_url"] = smi.hq_url();
		item["music"]["pub_time"] = smi.pub_time();
		item["music"]["album"] = b64album;
		item["music"]["pic"] = smi.pic_url();
		item["music"]["type"] = mode;
		item["music"]["tid"] = tid;
		item["music"]["like"] = is_like;
		item["music"]["host"] = smi.hot_num();
		item["music"]["clt"] = smi.clt_num();
		item["music"]["comment"] = smi.cmt_num();
	}else{//本地缓存音乐
		item["music"]["arits"] = singer;
		item["music"]["title"] = name;
	}
	item["songstat"] = state;
}

bool LBSLogic::GetMusicHotCltCmt(const std::string &songid, 
									   std::string &hot_num, 
									   std::string &cmt_num, 
									   std::string &clt_num){
   std::string content;
   bool r = false;
   Json::Reader reader;
   Json::Value  root;
   Json::Value songinfo;
   r = storage::RedisComm::GetMusicAboutUser(songid,content);
   if (!r){
	   hot_num = clt_num = cmt_num = "0";
	   return false;
   }

   //value {"hot":"123","cmt":"231",clt:"2312"}
   r = reader.parse(content.c_str(),root);
   if (!r)
	   return false;
   if (root.isMember("hot"))
	   hot_num = root["hot"].asString();
   else
	   hot_num = "0";

   if (root.isMember("cmt"))
	   cmt_num = root["cmt"].asString();
   else
	   cmt_num = "0";

   if (root.isMember("clt"))
	   clt_num = root["typeid"].asString();
   else
	   clt_num = "0";
}

} /* namespace mig_lbs */
