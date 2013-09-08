#include "sociality_mgr_engine.h"
#include "db_comm.h"
#include "dic_comm.h"
#include "logic_comm.h"
#include "basic/constants.h"
#include "basic/basic_util.h"
#include "config/config.h"
#include "basic/base64.h"
#include "basic/errno_comm.h"
#include "http_comm.h"
#include <sstream>
#include <ctime>

#define		TIME_TEST		1025

#define		LOGIC_PROLOG()	\
		status = 0;	\
		err_code = 0

namespace mig_sociality{


SocialityMgrEngine::SocialityMgrEngine(){

	bool r = false;
	std::string path = DEFAULT_CONFIG_PATH;
	mig_sociality::ThreadKey::InitThreadKey();
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	if(config==NULL){
		return;
	}
	r = config->LoadConfig(path);
	storage::DBComm::Init(config->mysql_db_list_);
	storage::MemComm::Init(config->mem_list_);
	storage::RedisComm::Init(config->redis_list_);
}

SocialityMgrEngine::~SocialityMgrEngine(){
	storage::DBComm::Dest();
	storage::MemComm::Dest();
	storage::RedisComm::Dest();

	mig_sociality::ThreadKey::DeinitThreadKey();
}

SocialityMgrEngine* SocialityMgrEngine::instance_ = NULL;
SocialityMgrEngine* SocialityMgrEngine::GetInstance(){
   if (instance_==NULL){
        instance_ = new SocialityMgrEngine();
   }

   return instance_;
}

void SocialityMgrEngine::FreeInstance(){
	delete instance_;
}

bool SocialityMgrEngine::OnBroadcastClose(struct server *srv, int socket){
	return true;
}

bool SocialityMgrEngine::OnBroadcastConnect(struct server *srv, int socket, 
									  void *data, int len){
    return true;
}

bool SocialityMgrEngine::OnBroadcastMessage(struct server *srv, int socket, 
										void *msg, int len){
    return true;
}

bool SocialityMgrEngine::OnIniTimer(const struct server *srv){
	//srv->add_time_task(srv, "user_manager", TIME_TEST, 300, 1);
    return true;
}

bool SocialityMgrEngine::OnTimeout(struct server *srv, char *id, int opcode, int time){

	return true;
}

bool SocialityMgrEngine::OnClose(struct server *srv, int socket){
    return true;
}

bool SocialityMgrEngine::OnConnect(struct server *srv, int socket){
    return true;
}

bool SocialityMgrEngine::OnReadMessage(struct server *srv, int socket,
								       const void *msg, int len){

    const char* packet_stream = (char*)(msg);
	packet::HttpPacket packet(packet_stream,len-1);
	std::string type;
	packet.GetPacketType(type);

	Json::Value root(Json::objectValue);
	int ret_status = 0;
	int err_code = 0;
	if (type == "setuserconfigofpush") {
		OnMsgSetUserConfigOfPush(packet, root, ret_status, err_code);
	} else if (type == "presentsong") {
		OnMsgPresentSong(packet, root, ret_status, err_code);
	} else if (type == "getpushmsg") {
		OnMsgGetPushMsg(packet, root, ret_status, err_code);
	} else if (type == "getpushmsgsummary") {
		OnMsgGetPushMsgSummary(packet, root, ret_status, err_code);
	} else if (type == "getfriendlist") {
		OnMsgGetFriendList(packet, root, ret_status, err_code);
	} else if (type == "sendfriendmsg") {
		OnMsgSendFriendMsg(packet, root, ret_status, err_code);
	} else if (type == "sayhello") {
		OnMsgSayHello(packet, root, ret_status, err_code);
	} else if (type == "adduserbacklist") {
		OnMsgAddUserBacklist(packet, root, ret_status, err_code);
	} else if (type == "addfriend") {
		OnMsgAddFriend(packet, root, ret_status, err_code);
	} else if (type == "importsonglist") {
		OnMsgImportSongList(packet, root, ret_status, err_code);
	} else if (type == "importfriend") {
		OnMsgImportFriend(packet, root, ret_status, err_code);
	} else {
		return true;
	}

	root["status"] = ret_status;
	if (ret_status != 1)
		root["msg"] = migfm_strerror(err_code);
	else
		root["msg"] = "";

	Json::FastWriter wr;
	std::string res = wr.write(root);
	SomeUtils::SendFull(socket, res.c_str(), res.length());

	MIG_DEBUG(USER_LEVEL, "lbs request:%s, response:%s", type.c_str(), res.c_str());

    return true;
}

bool SocialityMgrEngine::OnMsgSetUserConfigOfPush(packet::HttpPacket& packet,
		Json::Value& result, int& status, int &err_code) {
	LOGIC_PROLOG();

	std::string user_id, device_token, is_receive, begin_time, end_time;
	if (!packet.GetAttrib("uid", user_id)) {
		err_code = MIG_FM_HTTP_USER_NO_EXITS;
		return false;
	}
	if (!packet.GetAttrib("devicetoken", device_token)) {
		err_code = MIG_FM_HTTP_DEVICE_TOKEN_NOT_EXIST;
		return false;
	}
	if (!packet.GetAttrib("isreceive", is_receive)) {
		is_receive = "1";
	}
	if (!packet.GetAttrib("begintime", begin_time)) {
		begin_time = "08:00";
	}
	if (!packet.GetAttrib("endtime", end_time)) {
		end_time = "23:00";
	}

	int64 uid = atoll(user_id.c_str());
	if (0 == uid) {
		err_code = MIG_FM_HTTP_INVALID_USER_ID;
		return false;
	}

	unsigned btime = 0, etime = 0;
	if (!CheckAndTransHMTime(begin_time, btime)) {
		err_code = MIG_FM_HTTP_INVALID_TIME_FORMAT;
		return false;
	}
	if (!CheckAndTransHMTime(end_time, etime)) {
		err_code = MIG_FM_HTTP_INVALID_TIME_FORMAT;
		return false;
	}

	if (!storage::RedisComm::SetUserPushConfig(uid, device_token,
		atoi(is_receive.c_str()), btime, etime)) {
		err_code = MIG_FM_DB_SAVE_PUSH_CONFIG_FAILED;
		status = -1;
		return false;
	}

	status = 1;
	return true;
}

bool SocialityMgrEngine::OnMsgPresentSong(packet::HttpPacket& packet,
		Json::Value& result, int& status, int &err_code) {
	LOGIC_PROLOG();

	std::string user_id, to_user_id, song_id_str, ext_msg;
	if (!packet.GetAttrib("uid", user_id)) {
		err_code = MIG_FM_HTTP_USER_NO_EXITS;
		return false;
	}
	if (!packet.GetAttrib("touid", to_user_id)) {
		err_code = MIG_FM_HTTP_USER_NO_EXITS;
		return false;
	}
	if (!packet.GetAttrib("songid", song_id_str)) {
		err_code = MIG_FM_HTTP_SONG_ID_NO_VALID;
		return false;
	}
	if (!packet.GetAttrib("msg", ext_msg)) {
	}

	int64 uid = atoll(user_id.c_str());
	if (0 == uid) {
		err_code = MIG_FM_HTTP_INVALID_USER_ID;
		return false;
	}
	int64 to_uid = atoll(to_user_id.c_str());
	if (0 == to_uid) {
		err_code = MIG_FM_HTTP_INVALID_USER_ID;
		return false;
	}

	int64 song_id = strtoll(song_id_str.c_str(), NULL, 10);
	if (0 == song_id) {
		err_code = MIG_FM_HTTP_SONG_ID_NO_VALID;
		return false;
	}

	std::string device_token;
	bool is_recv = false;
	unsigned btime=0, etime=0;
	if (!storage::RedisComm::GetUserPushConfig(to_uid, device_token,
		is_recv, btime, etime)) {
		err_code = MIG_FM_DB_READ_PUSH_CONFIG_FAILED;
		status = -1;
		return false;
	}

	if (!is_recv) {
		err_code = MIG_FM_OTHER_PUSH_SERVICE_CLOSED;
		return false;
	}

	time_t cur_time = time(NULL);
	tm cur_tm = *localtime(&cur_time);
	unsigned ct = 60 * cur_tm.tm_hour + cur_tm.tm_min;
	bool enable = true;
	if (btime < etime)
		enable = (btime<=ct) && (ct<=etime);
	else
		enable = (ct<=etime) || (btime<=ct);
	if (!enable) {
		err_code = MIG_FM_OTHER_ANTI_HARASSMENT;
		return false;
	}

	int64 msg_id = 0;
	if (!storage::RedisComm::GenaratePushMsgID(uid, msg_id)) {
		err_code = MIG_FM_DB_ACCESS_FAILED;
		status = -1;
		return false;
	}

	std::string detail, summary;
	if (!MakePresentSongContent(user_id, to_user_id, song_id, msg_id, ext_msg,
			detail, summary)) {
		err_code = MIG_FM_DB_ACCESS_FAILED;
		status = -1;
		return false;
	}

	if (!storage::RedisComm::StagePushMsg(to_uid, msg_id, detail)) {
		err_code = MIG_FM_DB_ACCESS_FAILED;
		status = -1;
		return false;
	}

	if (!HttpComm::PushMessage(device_token, summary)) {
		err_code = MIG_FM_PUSH_MSG_FAILED;
		status = -1;
		return false;
	}

	status = 1;
	return true;
}

bool SocialityMgrEngine::OnMsgGetPushMsg(packet::HttpPacket& packet,
		Json::Value& result, int& status, int &err_code) {
	LOGIC_PROLOG();

	std::string user_id, page_index_str, page_size_str;
	if (!packet.GetAttrib("uid", user_id)) {
		err_code = MIG_FM_HTTP_USER_NO_EXITS;
		return false;
	}
	if (!packet.GetAttrib("page_index", page_index_str)) {
		page_index_str = "0";
	}
	if (!packet.GetAttrib("page_size", page_size_str)) {
		page_size_str = "10";
	}

	int64 uid = atoll(user_id.c_str());
	if (0 == uid) {
		err_code = MIG_FM_HTTP_INVALID_USER_ID;
		return false;
	}

	int page_index = atoi(page_index_str.c_str());
	int page_size = atoi(page_size_str.c_str());

	typedef std::list<std::string> MsgList;
	MsgList msg_list;
	if (!storage::RedisComm::GetStagedPushMsg(uid, page_index, page_size, msg_list)) {
		err_code = MIG_FM_DB_ACCESS_FAILED;
		status = -1;
		return false;
	}

	if (msg_list.empty()) {
		err_code = MIG_FM_MSG_LIST_EMPTY;
		return false;
	}

	Json::Value &content = result["result"];
	for (MsgList::iterator it=msg_list.begin(); it!=msg_list.end(); ++it) {
		Json::Value item;
		if (GetPushMsgDetail(*it, item))
			content.append(item);
	}

	status = 1;
	return true;
}

bool SocialityMgrEngine::OnMsgGetPushMsgSummary(packet::HttpPacket& packet,
		Json::Value& result, int& status, int &err_code) {
	LOGIC_PROLOG();
	return true;
}

bool SocialityMgrEngine::OnMsgGetFriendList(packet::HttpPacket& packet,
		Json::Value& result, int& status, int &err_code) {
	LOGIC_PROLOG();

	std::string uid_str, page_index_str, page_size_str;
	if (!packet.GetAttrib("uid", uid_str)) {
		err_code = MIG_FM_HTTP_USER_NO_EXITS;
		return false;
	}
//	if (!packet.GetAttrib("page_index", page_index_str)) {
//		page_index_str = "0";
//	}
//	if (!packet.GetAttrib("page_size", page_size_str)) {
//		page_size_str = "10";
//	}

	int64 uid = atoll(uid_str.c_str());
	if (0 == uid) {
		err_code = MIG_FM_HTTP_INVALID_USER_ID;
		return false;
	}

//	int page_index = atoi(page_index_str.c_str());
//	int page_size = atoi(page_size_str.c_str());

	typedef storage::DBComm::FriendInfoList FriendList;
	FriendList friends;
	if (!storage::DBComm::GetFriendList(uid_str, friends)) {
		err_code = MIG_FM_DB_ACCESS_FAILED;
		status = -1;
		return false;
	}



	status = 1;
	return true;
}

bool SocialityMgrEngine::OnMsgSendFriendMsg(packet::HttpPacket& packet,
		Json::Value& result, int& status, int &err_code) {
	LOGIC_PROLOG();
	return true;
}

bool SocialityMgrEngine::OnMsgSayHello(packet::HttpPacket& packet,
		Json::Value& result, int& status, int &err_code) {
	LOGIC_PROLOG();
	return true;
}

bool SocialityMgrEngine::OnMsgAddUserBacklist(packet::HttpPacket& packet,
		Json::Value& result, int& status, int &err_code) {
	LOGIC_PROLOG();
	return true;
}

bool SocialityMgrEngine::OnMsgAddFriend(packet::HttpPacket& packet,
		Json::Value& result, int& status, int &err_code) {
	LOGIC_PROLOG();

	std::string uid_str, touid_str;
	if (!packet.GetAttrib("uid", uid_str)) {
		err_code = MIG_FM_HTTP_USER_NO_EXITS;
		return false;
	}
	if (!packet.GetAttrib("touid", touid_str)) {
		err_code = MIG_FM_HTTP_USER_NO_EXITS;
		return false;
	}

	int64 uid = atoll(uid_str.c_str());
	if (0 == uid) {
		err_code = MIG_FM_HTTP_INVALID_USER_ID;
		return false;
	}

	int64 to_uid = atoll(touid_str.c_str());
	if (0 == to_uid) {
		err_code = MIG_FM_HTTP_INVALID_USER_ID;
		return false;
	}

//	if (!storage::RedisComm::AddFriend(uid, to_uid)) {
//		err_code = MIG_FM_DB_ACCESS_FAILED;
//		status = -1;
//		return false;
//	}

	if (!storage::DBComm::AddFriend(uid_str, touid_str)) {
		err_code = MIG_FM_DB_ACCESS_FAILED;
		status = -1;
		return false;
	}

	status = 1;
	return true;
}

bool SocialityMgrEngine::OnMsgImportSongList(packet::HttpPacket& packet,
		Json::Value& result, int& status, int &err_code) {
	LOGIC_PROLOG();
	return true;
}

bool SocialityMgrEngine::OnMsgImportFriend(packet::HttpPacket& packet,
		Json::Value& result, int& status, int &err_code) {
	LOGIC_PROLOG();
	return true;
}

bool SocialityMgrEngine::CheckAndTransHMTime(const std::string &str, unsigned &time) {
	time = 0;

	if (str.empty())
		return false;

	std::vector<std::string> vec_str;
	if (2 != SomeUtils::SplitStringChr(str.c_str(), ":", vec_str)) {
		return false;
	}

	int hour = atoi(vec_str[0].c_str());
	int minute = atoi(vec_str[1].c_str());

	if (hour<0 || hour>=24)
		return false;
	if (minute<0 || minute>=60)
		return false;

	time = hour*60 + minute;
	return true;
}

bool SocialityMgrEngine::MakePresentSongContent(const std::string& send_uid,
		const std::string& to_uid, int64 song_id,
		int64 msg_id, const std::string& msg,
		std::string &detail, std::string &summary) {
	detail.clear();
	summary.clear();

	std::string sd_nick, sd_sex, sd_head;
	std::string to_nick, to_sex, to_head;

	if (!storage::DBComm::GetUserInfos(send_uid, sd_nick, sd_sex, sd_head))
		return false;
	if (!storage::DBComm::GetUserInfos(to_uid, to_nick, to_sex, to_head))
		return false;

	std::stringstream ss;
	ss << sd_nick << "(" << send_uid << ")" << "赠送您一首歌";
	summary.assign(ss.str());

	char tmp[256] = {0};
	Json::FastWriter wr;
	Json::Value value;
	value["action"] = "presentsong";
	snprintf(tmp, arraysize(tmp), "%lld", msg_id);
	value["msgid"] = tmp;
	Json::Value &content = value["content"];
	content["send_uid"] = send_uid.c_str();
	content["to_uid"] = to_uid.c_str();
	snprintf(tmp, arraysize(tmp), "%lld", song_id);
	content["song_id"] = tmp;
	content["msg"] = msg;
	std::string cur_time;
	SomeUtils::GetCurrntTimeFormat(cur_time);
	content["time"] = cur_time;

	detail = wr.write(value);

	return true;
}

bool SocialityMgrEngine::GetPresentSongDetail(Json::Value& content) {
	std::string song_id = content["song_id"].asString();

	if (!GetMusicInfos(song_id, content["song"])) {
		return false;
	}

	return true;
}

bool SocialityMgrEngine::GetPushMsgDetail(const std::string& msg,
		Json::Value& content) {
	content.clear();

	Json::Reader rd;
	Json::Value root;
	if(!rd.parse(msg.c_str(), root)) {
		return false;
	}

	content["type"] = root["action"];
	Json::Value detail_node = content["detail"] = root["content"];

	std::string type = content["type"].asString();
	if (type == "presentsong") {
		GetPresentSongDetail(detail_node);
	} else if (type == "presentsong") {

	} else {
		// do nothing
	}

	return true;
}

bool SocialityMgrEngine::GetMusicInfos(const std::string& songid, Json::Value &info) {
	std::stringstream os;
	std::stringstream os1;
	std::string result_out;
	std::string status;
	std::string msg;
	std::string result;
	std::string mode;
	std::string wordid;
	std::string dec;
	std::string dec_id;
	std::string dec_word;
	std::string music_info;
	std::string content_url;
	base::MusicInfo smi;
	std::string b64title;
	std::string b64artist;
	std::string b64album;
	bool r = false;

	r = storage::RedisComm::GetMusicInfos(songid,music_info);

	if (!r)
		return false;

	r = smi.UnserializedJson(music_info);
	if (!r)
		return false;
	MIG_DEBUG(USER_LEVEL,"artist[%s] title[%s]",smi.artist().c_str(),
		smi.title().c_str());

	storage::DBComm::GetWXMusicUrl(smi.id(),content_url,dec,dec_id,dec_word);

	smi.set_url(content_url);
	Base64Decode(smi.title(),&b64title);
	Base64Decode(smi.artist(),&b64artist);
	Base64Decode(smi.album_title(),&b64album);

	info["id"] = smi.id();
	info["title"] = b64title;
	info["artist"] = b64artist;
	info["url"] = smi.url();
	info["pub_time"] = smi.pub_time();
	info["album"] = b64album;
	info["pic"] = smi.pic_url();
	info["like"] = "0";
	info["id"] = smi.id();

	return true;
}

}