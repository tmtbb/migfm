/*
 * lbs_logic.cc

 *
 *  Created on: 2013-7-15
 *      Author: huaiyu
 */
#include <string>
#include <map>
#include <sstream>
#include "lbs_logic.h"
#include "json/json.h"
#include "log/mig_log.h"
#include "config/config.h"
#include "basic/base64.h"
#include "basic/basic_util.h"
#include "storage/db_storage.h"
#include "storage/dic_storage.h"
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
	return 0;
	using namespace std;

	if (0 == user_id)
		return -1;

	WLockGd lk(lock_);
	int64 poi_id = redis_conn_.FindUserPOIID(user_id);

	if (0 == poi_id) {
		// 不存在POI,创建
		if (0 != bd_lbs_coon_.CreatePOI(user_id, latitude,longitude , poi_id, response, err_msg)) {
			return -1;
		}

		/*ASSERT(poi_id != 0);
		bd_lbs_coon_.CreatePOIEX(user_id, poi_id, data, response, err_msg);*/

		redis_conn_.BindUserPOI(user_id, poi_id);
	} else {
		// 已存在,更新
		bd_lbs_coon_.UpdatePOI(user_id,poi_id, latitude, longitude, response, err_msg);

		//bd_lbs_coon_.UpdatePOIEX(poi_id, data, response, err_msg);
	}

	return 0;
}

int LBSLogic::DelPOI(int64 user_id, std::string& response,
		std::string& err_msg) {
	using namespace std;

	if (0 == user_id)
		return -1;

	WLockGd lk(lock_);
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
	
	std::string q;
	bd_lbs_coon_.SearchNearby(longitude, latitude, radius, q,filter,
			page_index, page_size, result, response, err_msg);

	return 0;
}


int LBSLogic::SearchNearbyV2(double longitude, double latitude, uint32 radius,uint32 page_index,
		uint32 page_size, Json::Value &result,std::string &response, std::string &err_msg){

	return storage::DBComm::GetUserInfosLBS(result,latitude,longitude,
			radius,page_size);

}


bool LBSLogic::Init() {
   return true;
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
	} else if (type=="nearuser"){
		OnMsgSearchNearbyV2(packet, root, ret_status, ret_msg);
	}else if (type=="searchcollect"){
		OnMsgNearCollect(packet,root,ret_status,ret_msg);
	}else if (type=="nearmusic"){
		OnMsgNearMusic(packet,root,ret_status,ret_msg);
	}else if (type=="musicfri"){
		OnMsgMusicFri(packet,root,ret_status,ret_msg);
	}else if (type=="samemusic"){
		OnMsgSameMusic(packet,root,ret_status,ret_msg);
	}
	else
		return true;

	root["status"] = ret_status;
	if (ret_status != 1)
		root["msg"] = ret_msg;
	else
		root["msg"] = "";

	Json::FastWriter wr;
	std::string res = wr.write(root);
	SomeUtils::SendFull(socket, res.c_str(), res.length());

	MIG_DEBUG(USER_LEVEL, "lbs request:%s, response:%s", type.c_str(), res.c_str());

    return true;
}


bool LBSLogic::OnMsgSearchNearbyV2(packet::HttpPacket& packet, Json::Value &result, 
								   int &status, std::string &msg){

    return OnMsgPublicLbs(packet,result,status,msg,1);
}


bool LBSLogic::OnMsgSameMusic(packet::HttpPacket& packet, Json::Value &result, 
							  int &status, std::string &msg){
    return OnMsgPublicLbs(packet,result,status,msg,2);
}

bool LBSLogic::OnMsgMusicFri(packet::HttpPacket& packet, Json::Value &result, 
							 int &status, std::string &msg){
	
     return OnMsgPublicLbs(packet,result,status,msg,3);
}

bool LBSLogic::OnMsgNearMusic(packet::HttpPacket& packet, Json::Value &result, 
							  int &status, std::string &msg){
	
    return OnMsgPublicLbs(packet,result,status,msg,4);

}


bool LBSLogic::OnMsgNearCollect(packet::HttpPacket &packet, Json::Value &result, int &status, std::string &msg)
{


	return OnMsgPublicLbs(packet,result,status,msg,5);

}

bool LBSLogic::OnMsgPublicLbs(packet::HttpPacket& packet, Json::Value &result, 
							  int &status, std::string &msg,int flag){
	status = 0;
	msg.clear();
	bool r = false;

	std::string uid_str, location_str, radius_str, page_index_str, page_size_str;
	if (!packet.GetAttrib("uid", uid_str)) {
	  msg = "uid未指定";
	  return false;
	}
	if (!packet.GetAttrib("location", location_str)) {
	  msg = "location未指定";
	  return false;
	}
	//if (!packet.GetAttrib("radius", radius_str)) {
	radius_str = DEFAULT_MAX_RADIUS;
	//}

	if (!packet.GetAttrib("page_index", page_index_str)) {
	  page_index_str = "0";
	}

	if (!packet.GetAttrib("page_size", page_size_str)) {
	  page_size_str = "10";
	}
	if(atol(page_size_str.c_str())<=0)
		page_size_str = "10";

	int64 uid = atoll(uid_str.c_str());
	if (0 == uid) {
	  msg = "无效uid";
	  return false;
	}

	double current_latitude = 0;
	double current_longitude = 0;
	std::vector<std::string> location_pair;
	if (2 != SplitStringChr(location_str.c_str(), ",", location_pair)) {
	  msg = "location参数格式错误";
	  return false;
	}

	current_latitude = atof(location_pair[1].c_str());
	current_longitude = atof(location_pair[0].c_str());

	double latitude = 0;
	double longitude = 0;
	/*r = storage::DBComm::GetUserLbsPos(atol(uid_str.c_str()),latitude,longitude);
	if (!r||((latitude==0)&&(longitude==0))){
		//result["result"] = "";
		//msg = "周围没有用户";
		//status = 0;
		//return true;
		latitude = current_latitude;
		longitude = current_longitude;
	}*/

	latitude = current_latitude;
	longitude = current_longitude;
	storage::DBComm::RecordUserLbs(uid,latitude,longitude);

	uint32 radius = atoi(radius_str.c_str());
	int page_index = atoi(page_index_str.c_str());
	int page_size = atoi(page_size_str.c_str());
	std::string response;
	Json::Value content;

	Json::Value temp_users;
	std::vector<std::string> vec_users;
	typedef std::map<std::string, std::string> UserSongMap;
	UserSongMap map_songs;
	Json::Value same_music_users;
	std::map<std::string,std::string> collect_musices;
	Json::Value usersmusic;
	Json::Value samemusers;

/* 因为暂时无法判断进入范围的用户，故暂时取消
	//对比当前距离和存储距离如果未超过500米     暂时不做请求
	r = IsOverRange(uid,current_latitude,current_longitude); //大于500米为false 重新请求距离

	if(r){//未超过500米 缓存获取
		 //vec_users.push_back(uid_str);
		r = GetCacheLBSUserInfos(uid,latitude,longitude,temp_users,vec_users);
		//为false 说明并未存入缓存
	}
*/
	r = false;
	if(!r){ //重新获取距离

		/*if (0 != SearchNearby(longitude, latitude, radius, "", page_index, page_size,
				content, response, msg)) {
			return false;
		}*/
		if(!SearchNearbyV2(longitude, latitude, radius, page_index, page_size,
				content, response, msg)){
			return false;
		}
		int i = content["size"].asInt();
		const Json::Value &items = content["contents"];
		if (items.empty()){
			result["result"] = "";
			msg = "周围没有用户";
			status = 1;
			return true;
		}


		std::map<std::string, bool> mapExist;
		std::string nick_name, sex,pic;
		int jk = 0;
		Json::Value temp_id;

		for (Json::Value::iterator it = items.begin();
			it != items.end();
	  	  ++it,jk++) {
		  	  const Json::Value &item = *it;
		  	  Json::Value val;

		  	  std::string uid_str = item["usr_id"].asString();
		  	  if (uid_str.empty())
			  	  continue;
		  	  int64 tar_uid = atoll(uid_str.c_str());
		  	  if (uid==tar_uid)
			  	  continue;
		  	  if (mapExist.end() != mapExist.find(uid_str))
			  	  continue;

		  	  mapExist[uid_str] = true;
		  	  val["userinfo"]["userid"] = uid_str;
		  	  //获取坐标
		  	  base::UserInfo usrinfo;
		  	  r = base::BasicUtil::GetUserInfo(uid_str,usrinfo);
		  	  if (r){

			  	  const Json::Value &pos_items = item["location"];
			  	  Json::Value json_latitude;
			  	  Json::Value json_logitude;
			  	  int index = 0;
			  	  for (Json::Value::iterator itr =pos_items.begin();
					  itr!= pos_items.end();++itr){
				  	if (index==0)
  						json_latitude = (*itr);
  					else if (index==1)
  						json_logitude = (*itr);
  					index++;
			  	  }
			  	  val["userinfo"]["latitude"] = json_latitude;
			  	  val["userinfo"]["longitude"] = json_logitude;


			  	  //因和百度算出的距离有差异，避免在我的好友显示距离不一样，故统一用自行计算方式
			  	  double tar_latitude
			  	  	  = json_latitude.asDouble();

			  	  double tar_longitude
			  	  	  = json_logitude.asDouble();
			  	  val["userinfo"]["distance"]
			                  =  base::BasicUtil::CalcGEODistance(latitude,longitude,
				                          tar_latitude,tar_longitude);
			  	  val["userinfo"]["nickname"] = usrinfo.nickname();
			  	  val["userinfo"]["sex"] = usrinfo.sex();
			  	  val["userinfo"]["head"] = usrinfo.head();
			  	  val["userinfo"]["birthday"] = usrinfo.birthday();
			  	  val["userinfo"]["plat"] = usrinfo.source();
			  	  vec_users.push_back(uid_str);
			  	  temp_users.append(val);
			  	  temp_id.append(uid_str);
		  	  }
		}
		//AddCacheLBSUserInfos(uid,temp_id);

	}

	//if (temp_users.size()>0)
		//storage::DBComm::UpDateUserLbsPos(temp_users,uid);

	if (vec_users.size()>0)
		storage::MemComm::GetUserCurrentSong(vec_users, map_songs);

	//获取该用户用户红心歌曲名单
	if ((flag==2)||(flag==3)||(flag==5)){
		r = storage::RedisComm::GetCollectSongs(uid_str,collect_musices);
	}


	for (Json::Value::iterator it = temp_users.begin();
	  it != temp_users.end(); ++it) {
		  Json::Value &item = *it;
		  const std::string uid_str = item["userinfo"]["userid"].asString();
		  UserSongMap::const_iterator find = map_songs.find(uid_str);
		  if (map_songs.end() != find){
			  bool is_user_like = false;
			  if ((flag==2)||(flag==3))
				  r = GetUserCurrentMusic(find->second,item,is_user_like,
										 &collect_musices,true);
			  else
				  r = GetUserCurrentMusic(find->second,item,is_user_like);
			  if (r){
				  if (flag==2){
					  if (is_user_like)
						  samemusers.append(item); //周围歌曲是否是用户红心歌曲
				  }else{
					  usersmusic.append(item);
				  }
			  }

		  }else{
			  if (flag==1){//
				//value {"songid":"10000","state":"1","type":"mm","tid":"1","name":"鑹抽槼澶�,"singer":"绐﹀敮"}
 				  const std::string default_content = "{\"songid\":\"0\",\"state\":\"1\",\"type\":\"mm\",\"tid\":\"1\",\"name\":\"黑梦\",\"singer\":\"窦唯\"}";
 				  bool is_user_like = false;
 				  r = GetUserCurrentMusic(default_content,item,is_user_like);
 				  if(r)
 					  usersmusic.append(item);

			  }else {
				  item["music"] = 0;
			  }
			  
		  }
	}


	//获取相同类型的歌曲
	if(flag==2||flag==3){
		storage::DBComm::GetSameMusic(same_music_users,uid,latitude,longitude);
		AddSameMusicUsers(same_music_users,samemusers,uid_str);

	}

	if (flag==3){
		std::string str_friend_num;
		int new_msg_num = 0;
		storage::RedisComm::GetNewMsgNum(uid_str,new_msg_num);
		storage::DBComm::GetMusicFriendNum(uid_str,str_friend_num);
		int friend_num = atol(str_friend_num.c_str());
		int near_num = temp_users.size();

		//friend
		result["result"]["fri_num"] = friend_num;
		//same_music
		result["result"]["music_num"] = samemusers.size();
		//near friend
		result["result"]["near_num"] = usersmusic.size();
		//message_num
		result["result"]["msg_num"] = GetMsgCount(uid_str);
		//new message
		result["result"]["new_msg_num"] = new_msg_num;
	}else if (flag==5){
		//int collect_num = GetEffectCollectNum(collect_musices)/*collect_musices.size()*/;
		int collect_num = collect_musices.size();
		int near_num = usersmusic.size();
		int music_num = usersmusic.size();
		result["result"]["mynum"] = collect_num;
		result["result"]["nearnum"] = music_num;
	}else if(flag==2){
		result["result"]["nearUser"] = samemusers;
	}else{
		result["result"]["nearUser"] = usersmusic;
	}

	status = 1;
	return true;
}



int32 LBSLogic::GetEffectCollectNum(std::map<std::string,std::string>& songs){
	int32 count = 0;
	storage::DBComm::GetEffectCollectCount(songs,count);
	return count;
}
void LBSLogic::AddSameMusicUsers(Json::Value& same_music_users,
		   Json::Value& user_music,
		   const std::string& str_uid){

	int flag = 0;
	for (Json::Value::iterator it = same_music_users.begin();
		  it != same_music_users.end(); ++it) {
		Json::Value item = *it;
		flag = 0;
		if(str_uid!=item["userinfo"]["userid"].asString()){
			for(Json::Value::iterator itr = user_music.begin();
				  itr != user_music.end(); ++itr){
				Json::Value itrem = *itr;
				if(item["userinfo"]["userid"].asString()==itrem["userinfo"]["userid"].asString())
					flag = 1;
			}
			if(flag==0){
				//获取音乐
				GetUserCurrentMusic(item,str_uid);
				user_music.append(item);
			}
		}
	}
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

	double latitude = atof(location_pair[1].c_str());
	double longitude = atof(location_pair[0].c_str());

	std::string response;
	if (0 != SetPOI(uid, longitude, latitude, "", response, msg)) {
		return false;
	}

	//用于客户端，服务端baidu坐标有差异，所以更新自身坐标
	//UpdateUserInfoPoi(uid,latitude,longitude,msg);
	storage::DBComm::RecordUserLbs(uid,latitude,longitude);
	status = 1;
	return true;
}

bool LBSLogic::UpdateUserInfoPoi(int64 user_id,double longitude, 
								 double latitude,std::string &msg){

	 Json::Value content;
	 std::string response;
	 Json::Value temp_users;
	 bool r =false;
	 std::map<std::string, bool> mapExist;
	 if (0 != SearchNearby(longitude, latitude, 500000, "", 0, 4,
		 content, response, msg)) {
			 return false;
	 }

	 int i = content["size"].asInt();
	 const Json::Value &items = content["contents"];
	 if (items.empty()){
		 msg = "周围没有用户";
		 return false;
	 }

	 for (Json::Value::iterator it = items.begin();
		 it != items.end();++it) {
			 const Json::Value &item = *it;
			 Json::Value val;
			 std::string uid_str = item["usr_id"].asString();
			 if (uid_str.empty())
				 continue;
			 if (mapExist.end() != mapExist.find(uid_str))
				 continue;
			 mapExist[uid_str] = true;
			 val["userinfo"]["userid"] = uid_str;
			 //判断用户后是否存在
// 			 base::UserInfo usrinfo;
// 			 r = base::BasicUtil::GetUserInfo(uid_str,usrinfo);
			 //存储过程中已经判断是否存在

			 //获取坐标
			 const Json::Value &pos_items = item["location"];
			 Json::Value json_latitude;
			 Json::Value json_logitude;
			 int index = 0;
			 for (Json::Value::iterator itr =pos_items.begin();
				 itr!= pos_items.end();++itr){
					 if (index==0)
						 json_latitude = (*itr);
					 else if (index==1)
						 json_logitude = (*itr);
					 index++;
			 }
			 val["userinfo"]["latitude"] = json_latitude;
			 val["userinfo"]["longitude"] = json_logitude;
			 temp_users.append(val);
	 }
	 //缓存用户的经度 纬度 存入mysql
	 if (temp_users.size()>0)
		 storage::DBComm::UpDateUserLbsPos(temp_users,user_id);
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
		msg ="无效uid";
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
	base_storage::MYSQLDB::Init(config->mysql_db_list_);
	base_storage::MemDic::Init(config->mem_list_);

	InitThreadrw(&lock_);

}

LBSLogic::~LBSLogic() {
	storage::DBComm::Dest();
	storage::MemComm::Dest();
	storage::RedisComm::Dest();

	DeinitThreadrw(lock_);
	ThreadKey::DeinitThreadKey ();
}

bool LBSLogic::GetUserCurrentMusic(Json::Value& item,const std::string& str_uid){
	bool r = false;
	Json::Value music;
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
	std::string songid = item["music"]["id"].asString();
	r = storage::RedisComm::GetMusicInfos(songid,music_info);
	if (!r){
		LOG_ERROR("song no vailed");
		return false;
	}
	r =smi.UnserializedJson(music_info);
	if (!r){
		LOG_ERROR("song parser error");
		return false;
	}

	storage::DBComm::GetMusicAboutInfo(smi.id(),hq_content_url,content_url,clt_num,cmt_num,hot_num);
	smi.set_hq_url(hq_content_url);
	smi.set_url(content_url);
	smi.set_music_time(0);
	Base64Decode(smi.title(),&b64title);
	Base64Decode(smi.artist(),&b64artist);
	Base64Decode(smi.album_title(),&b64album);

	uid = item["userinfo"]["userid"].asString();
	LOG_DEBUG2("uid[%s]",uid.c_str());
	r = storage::RedisComm::IsCollectSong(uid,songid);
	if (r)
		is_like = 1;
	else
		is_like = 0;

	smi.set_music_clt(clt_num);
	smi.set_music_cmt(cmt_num);
	smi.set_music_hot(hot_num);

	item["music"]["id"] = smi.id();
	item["music"]["title"] = b64title;
	item["music"]["artist"] = b64artist;
	item["music"]["url"] = smi.url();
	item["music"]["hqurl"] = smi.hq_url();
	item["music"]["pub_time"] = smi.pub_time();
	item["music"]["album"] = b64album;
	item["music"]["pic"] = smi.pic_url();
	item["music"]["type"] = mode;
	item["music"]["tid"] = tid;
	item["music"]["like"] = is_like;
	item["music"]["hot"] = hot_num;
	item["music"]["clt"] = clt_num;
	item["music"]["cmt"] = cmt_num;
	item["songstat"] = 2;
	return r;
}

bool LBSLogic::GetUserCurrentMusic(const std::string &content, Json::Value &item,
								   bool& is_user_like,
								   std::map<std::string,std::string>* collect_musices,
								   bool is_collect){
//value {"songid":"10000","state":"1","type":"mm","tid":"1","name":"窦唯","singer":"来听一听"}
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

	//瑙ｆ瀽json
	r = reader.parse(content.c_str(),root);
	if (!r){
		LOG_ERROR("parser json error");
		return false;
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


	if (is_collect){
		std::map<std::string,std::string>::const_iterator itr 
				 = (*collect_musices).find(songid);

		if(itr == (*collect_musices).end())
			is_user_like = false;
		else
			is_user_like = true;
	}

	if (state!="2"){
		r = storage::RedisComm::GetMusicInfos(songid,music_info);
		if (!r){
			LOG_ERROR("song no vailed");
			return false;
		}
		r =smi.UnserializedJson(music_info);
		if (!r){
			LOG_ERROR("song parser error");
			return false;
		}
		r = storage::DBComm::GetMusicAboutInfo(smi.id(),hq_content_url,content_url,clt_num,cmt_num,hot_num);
		if(!r)
			return r;
		smi.set_hq_url(hq_content_url);
		smi.set_url(content_url);
		smi.set_music_time(0);
		Base64Decode(smi.title(),&b64title);
		Base64Decode(smi.artist(),&b64artist);
		Base64Decode(smi.album_title(),&b64album);

		uid = item["userinfo"]["userid"].asString();
		LOG_DEBUG2("uid[%s]",uid.c_str());
		r = storage::RedisComm::IsCollectSong(uid,songid);
		if (r)
			is_like = 1;
		else
			is_like = 0;
		//鑾峰彇璇勮 鐑害 鏀惰棌鏁�

		/*GetMusicHotCltCmt(songid,hot_num,cmt_num,clt_num);*/
		smi.set_music_clt(clt_num);
		smi.set_music_cmt(cmt_num);
		smi.set_music_hot(hot_num);

		item["music"]["id"] = smi.id();
		item["music"]["title"] = b64title;
		item["music"]["artist"] = b64artist;
		item["music"]["url"] = smi.url();
		item["music"]["hqurl"] = smi.hq_url();
		item["music"]["pub_time"] = smi.pub_time();
		item["music"]["album"] = b64album;
		item["music"]["pic"] = smi.pic_url();
		item["music"]["type"] = mode;
		item["music"]["tid"] = tid;
		item["music"]["like"] = is_like;
		item["music"]["hot"] = hot_num;
		item["music"]["clt"] = clt_num;
		item["music"]["cmt"] = cmt_num;
	}else{//鏈湴缂撳瓨闊充箰
		item["music"]["arits"] = singer;
		item["music"]["title"] = name;
	}
	item["songstat"] = state;
	return true;
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

int LBSLogic::GetMsgCount(const std::string &uid){
	int count = 0;
	//bool r = storage::RedisComm::GetMsgCount(uid,count);
	bool r = storage::DBComm::GetMsgCount(atoll(uid.c_str()),count);
	return count;
}

bool LBSLogic::IsOverRange(const int64 uid,
		double current_latitude,
        double current_longitude){

	double stor_latitude = 0;
	double stor_longitude = 0;
	//storage::DBComm::GetUserLbsPos(uid,stor_latitude,stor_longitude);
	//获取当前存储距离

	//key uidlbscurrent 10001lbscurrent
	std::stringstream key;
	key<<uid<<"lbscurrent";
	char* value;
	size_t value_len;
	Json::Reader reader;
	Json::Value root;

	bool r = storage::MemComm::GetString(key.str().c_str(),key.str().length(),
			  &value,&value_len);
	//存储
	Json::Value val;
	Json::FastWriter wr;
	val["latitude"] = current_latitude;
	val["longitude"] = current_longitude;
	std::string res = wr.write(val);
	storage::MemComm::SetString(key.str().c_str(),key.str().length(),res.c_str(),res.length());

	if(!r){
		return false;
	}

	r = reader.parse(value,root);
	if (!r){
		MIG_ERROR(USER_LEVEL,"json parser error");
		return false;
	}

	if(value){
		delete [] value;
		value = NULL;
	}

	stor_latitude = root["latitude"].asDouble();
	stor_longitude = root["longitude"].asDouble();
	//计算距离
	double distance
	  =  base::BasicUtil::CalcGEODistance(current_latitude,current_longitude,
			  stor_latitude,stor_longitude);

	if(distance>500)
		return false;
	else
		return true;
}

bool LBSLogic::AddCacheLBSUserInfos(const int64 uid,Json::Value& temp_userid){

	Json::FastWriter wr;
	bool r = false;
	std::string res = wr.write(temp_userid);
	//存入memcache
	//key uidlbs 10001lbs
	std::stringstream key;
	key<<uid<<"lbs";
	LOG_DEBUG2("%s",res.c_str());
	return storage::MemComm::SetString(key.str().c_str(),key.str().length(),res.c_str(),res.length());
}

bool LBSLogic::GetCacheLBSUserInfos(const int64 uid,const double uid_latitude,
		const double uid_longitude,Json::Value& temp_users,
		std::vector<std::string>& vec_users){

   char* value;
   size_t value_len;
  	//从memcache 获取
	//key uidlbs 10001lbs
  std::stringstream key;
  key<<uid<<"lbs";
  bool r = storage::MemComm::GetString(key.str().c_str(),key.str().length(),
		  &value,&value_len);
  if(!r)
  	return false;
  //读取号码
  Json::Reader reader;
  Json::Value root;
  r = reader.parse(value,root);
  if (!r){
	  MIG_ERROR(USER_LEVEL,"json parser error");
	  return false;
  }
  for (Json::Value::iterator it = root.begin();
  	  it != root.end();++it){
	  const Json::Value item = (*it);
	  std::string str_uid = item.asString();
	  Json::Value val;
	  //获取用户信息
	  std::string sex;
	  std::string nickname;
	  std::string head;
	  std::string birthday;
	  double latitude;
	  double longitude;
	  r = storage::DBComm::GetLBSAboutInfos(str_uid,sex,nickname,head,birthday,
			  	  latitude,longitude);
	  if(!r)
		  continue;
	  vec_users.push_back(str_uid);
	  val["userinfo"]["nickname"] = nickname;
	  val["userinfo"]["sex"] = sex;
	  val["userinfo"]["head"] = head;
	  val["userinfo"]["birthday"] = birthday;
	  val["userinfo"]["latitude"] = latitude;
	  val["userinfo"]["longitude"] = longitude;
	  val["userinfo"]["userid"] = str_uid;
	  double distance = base::BasicUtil::CalcGEODistance(latitude,
  	  	  	  longitude,uid_latitude,uid_longitude);
	  val["userinfo"]["distance"] =  base::BasicUtil::CalcGEODistance(latitude,
			  	  	  	  	  	  longitude,uid_latitude,uid_longitude);
	  temp_users.append(val);
  }
  if(value){
	  delete [] value;
	  value = NULL;
  }
  return true;
}


} /* namespace mig_lbs */
