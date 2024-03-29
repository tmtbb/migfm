#include "db_comm.h"
#include "thread_handler.h"
#include "basic/basic_info.h"
#include "basic/base64.h"
#include "basic/basic_util.h"
#include "storage/storage.h"
#include <mysql.h>
#include <sstream>

namespace mig_sociality {

#if defined (_DB_POOL_)
threadrw_t* DBComm::db_pool_lock_;
std::list<base_storage::DBStorageEngine*>  DBComm::db_conn_pool_;
#endif

std::list<base::ConnAddr> DBComm::addrlist_;


AutoDBCommEngine::AutoDBCommEngine()
:engine_(NULL){
#if defined (_DB_POOL_)
	engine_ = mig_sociality::DBComm::DBConnectionPop();
#endif
}

AutoDBCommEngine::~AutoDBCommEngine(){
#if defined (_DB_POOL_)
	mig_sociality::DBComm::DBConnectionPush(engine_);
#endif
}

void DBComm::Init(std::list<base::ConnAddr>& addrlist,
				  const int32 db_conn_num/* = 5*/){
	addrlist_ = addrlist;

#if defined (_DB_POOL_)
	bool r =false;
	InitThreadrw(&db_pool_lock_);
	for (int i = 0; i<db_conn_num;i++){
		base_storage::DBStorageEngine* engine  =
				base_storage::DBStorageEngine::Create(base_storage::IMPL_MYSQL);
		if (engine==NULL){
			assert(0);
			LOG_ERROR("create db conntion error");
			continue;
		}

		r = engine->Connections(addrlist_);
		if (!r){
			assert(0);
			LOG_ERROR("db conntion error");
			continue;
		}

		db_conn_pool_.push_back(engine);

	}

#endif
}

#if defined (_DB_POOL_)

void DBComm::DBConnectionPush(base_storage::DBStorageEngine* engine){
	WLockGd lk(db_pool_lock_);
	db_conn_pool_.push_back(engine);
}

base_storage::DBStorageEngine* DBComm::DBConnectionPop(){
	if(db_conn_pool_.size()<=0)
		return NULL;
	WLockGd lk(db_pool_lock_);
    base_storage::DBStorageEngine* engine = db_conn_pool_.front();
    db_conn_pool_.pop_front();
    return engine;
}

#endif


void DBComm::Dest(){
#if defined (_DB_POOL_)
	WLockGd lk(db_pool_lock_);
	while(db_conn_pool_.size()>0){
		base_storage::DBStorageEngine* engine = db_conn_pool_.front();
		db_conn_pool_.pop_front();
		if(engine){
			engine->Release();
			delete engine;
			engine =NULL;
		}
	}
	DeinitThreadrw(db_pool_lock_);

#endif
}

base_storage::DBStorageEngine* DBComm::GetConnection(){


	try{
		bool r = false;
		base_storage::DBStorageEngine* engine = mig_sociality::ThreadKey::GetStorageDBConn();
		if (engine){
			if (!engine->CheckConnect()){
				LOG_ERROR("Database %s connection was broken");
				engine->Release();
				if (engine){
					delete engine;
					engine = NULL;
				}
			}else
				return engine;
		}

		engine = base_storage::DBStorageEngine::Create(base_storage::IMPL_MYSQL);
		if (engine==NULL){
			assert(0);
			return NULL;
		}
		r = engine->Connections(addrlist_);
		if (!r)
			return NULL;
		mig_sociality::ThreadKey::SetStorageDBConn(engine);
		LOG_DEBUG("Created database connection");
		return engine;
	}
	catch (...){
		LOG_ERROR("connect error");
		return NULL;
	}
}

bool DBComm::GetShareInfo(const int64 songid,const std::string& mode,const std::string& index,
			std::string& lyric,std::string& description){
	std::stringstream os;
	bool r = false;
	int num = 0;
#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	base_storage::db_row_t* db_rows;
	MYSQL_ROW rows = NULL;
	os<<"call proc_GetShareInfo("<<songid<<",\'"
			<<mode.c_str()<<"\',"<<index.c_str()<<");";
	std::string sql = os.str();
	LOG_DEBUG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());
	if(!r){
		MIG_ERROR(USER_LEVEL,"sqlexec error ");
		return r;
	}
	num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			if(rows[0]==NULL)
				return false;
			lyric = rows[0];
			if(rows[1]==NULL)
				return false;
			description = rows[1];
		}
		return true;
	}
}

bool DBComm::GetLyric(const int64 songid,std::string& lyric){
	std::stringstream os;
	bool r = false;
	int num = 0;
#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	base_storage::db_row_t* db_rows;
	MYSQL_ROW rows = NULL;
	os<<"call proc_GetLyric("<<songid<<");";
	r = engine->SQLExec(os.str().c_str());
	if(!r){
		MIG_ERROR(USER_LEVEL,"sqlexec error ");
		return r;
	}
	num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			if(rows[0]==NULL)
				return false;
			lyric = rows[0];
			//lyric = rows[0];
		}
		return true;
	}
}

bool DBComm::SetMusicHostCltCmt(const std::string& songid,
								const int32 flag, 
								const int32 value){

		std::stringstream os;
		bool r = false;
		MYSQL_ROW rows;
#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
		//base_storage::DBStorageEngine* engine = GetConnection();
		if (engine==NULL){
			LOG_ERROR("engine error");
			return false;
		}
		/*
		call proc_SetMuiscAbout`('100000',1,1);
		*/
		os<<"call proc_SetMuiscAbout(\'"<<songid.c_str()<<"\',"
			<<flag<<","<<value<<");";

		std::string sql = os.str();
		LOG_DEBUG2("[%s]", sql.c_str());
		r = engine->SQLExec(sql.c_str());

		if (!r) {
			LOG_ERROR2("exec sql error");
			return false;
		}
		return true;
}

bool DBComm::RecordUserMessageList(const int32 type,const int64 send_uid,
						const int64 to_uid,std::list<struct RecordMessage>& list){
	std::stringstream os;
	bool r = false;
	MYSQL_ROW rows;
#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	if (engine == NULL) {
		LOG_ERROR("GetConnection Error");
		return false;
	}


	while(list.size()){
		struct RecordMessage msg = list.front();
		char s_distance[256];
		list.pop_front();
		snprintf(s_distance, arraysize(s_distance),
					"%lf", msg.distance);
		os	<< "call proc_RecordMessageList("
			<<type<<",\'"<<msg.message<<"\',"<<send_uid
			<<","<<to_uid<<",\'"<<msg.songid<<"\',\'"<<s_distance<<"\');";
	}
	// call proc_RecordMessageList(1,'来一发',10108,10158,'123232.09999')

	std::string sql = os.str();
	LOG_DEBUG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());

	if (!r) {
		LOG_ERROR2("exec sql error");
		return false;
	}
	return true;

}
bool DBComm::RecordUserMessageList(const int32 type,int64 send_uid,int64 to_uid,
								const double distance,
								const std::string& message){
#if defined (_DB_POOL_)
	AutoDBCommEngine auto_engine;
	base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	std::stringstream os;
	bool r = false;
	MYSQL_ROW rows;
	if (engine == NULL) {
		LOG_ERROR("GetConnection Error");
		return false;
	}
	char s_distance[256];
	snprintf(s_distance, arraysize(s_distance),
			"%lf", distance);

	// call proc_RecordMessageList(1,'来一发',10108,10158,'0','123232.09999')
	os	<< "call proc_RecordMessageList("
		<<type<<",\'"<<message<<"\',"<<send_uid
		<<","<<to_uid<<",\'0\',\'"<<s_distance<<"\')";

	std::string sql = os.str();
	LOG_DEBUG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());

	if (!r) {
		LOG_ERROR2("exec sql error");
		return false;
	}
	return true;
}

bool DBComm::GetMusicUser(const std::string& uid, 
						  const std::string& fromid, 
						  const std::string& count, 
						  std::vector<std::string>& vec_users,
						  std::list<struct MusicFriendInfo>& userlist){
	  std::stringstream os;
	  std::string sql;
	  MYSQL_ROW rows;
	  double uid_latitude = 0;
	  double uid_longitude = 0;
#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	  if (engine==NULL){
		  LOG_ERROR("engine error");
		  return false;
	  }

	  //��ȡ����ľ���
	  os<<"select uid,latitude,longitude from migfm_lbs_pos where uid = \'" <<uid <<"\';";
	  sql = os.str();
	  LOG_DEBUG2("%s",sql.c_str());
	  engine->SQLExec(sql.c_str());
	  
	  int32 num = engine->RecordCount();
	  if (num>0){
		  while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			  uid_latitude = atof(rows[1]);
			  uid_longitude = atof(rows[2]);
		  }
	  }else{
		  return false;
	  }

	  os.str("");
		  // proc_GetHistoryFriends
	  os<< "call proc_GetHistoryFriends("
		  << uid.c_str() << ","
		  << fromid.c_str() << ","
		  << count.c_str() << ")";

	  sql = os.str();
	  engine->SQLExec(sql.c_str());
	  LOG_DEBUG2("%s",sql.c_str());
	  num = engine->RecordCount();
	  if(num>0){
		  while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){

			  base::UserInfo userinfo;
			  userinfo.set_nickname(rows[0]);
			  userinfo.set_sex(rows[1]);
			  userinfo.set_uid(rows[2]);
			  userinfo.set_head(rows[3]);
			  userinfo.set_source(rows[4]);
			  userinfo.set_birthday(rows[5]);
			  if (userinfo.uid()==uid)
				  continue;
			  vec_users.push_back(userinfo.uid());
			  struct MusicFriendInfo info;
			  info.userinfo = userinfo;
			  info.latitude = atof(rows[6]);
			  info.longitude = atof(rows[7]);
			  info.distance 
				  = base::BasicUtil::CalcGEODistance(uid_latitude,
				                                     uid_longitude,
													 info.latitude,
													 info.longitude);

			 /* LOG_DEBUG2("uid[%s] touid[%s] uid_latitude [%lld] uid_longitude[%lld] info.latitude[%lld] info.longitude[%lld] distance[%lld]",uid.c_str(),userinfo.uid().c_str(),
				  uid_latitude,uid_longitude,info.latitude,info.latitude,info.distance );*/
			  userlist.push_back(info);
		  }
		  return true;
	  }
	  return false;
}

bool DBComm::GetUserInfos(const std::string &uid, std::string &nickname, 
						  std::string &gender, std::string &head, 
						  double &latitude, double &longitude){
	nickname.clear();
	gender.clear();
	head.clear();

#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	std::stringstream os;
	bool r = false;
	MYSQL_ROW rows;
	int num = 0;
	if (engine == NULL) {
	  LOG_ERROR("GetConnection Error");
	  return false;
	}

	os <<"call proc_GetUserInfos(\'"<<uid<<"\')";

	LOG_DEBUG2("%s",os.str().c_str());
	r = engine->SQLExec(os.str().c_str());
	if(!r){
		MIG_ERROR(USER_LEVEL,"sqlexec error ");
		return r;
	}

	num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			latitude = atof(rows[0]);
			longitude = atof(rows[1]);
			nickname = rows[2];
			gender = rows[3];
			head = rows[4];
		}
		return true;
	}
	return false;
}

bool DBComm::GetUserInfos(const std::string& uid, std::string& nickname,
		std::string& gender, std::string& head) {
	nickname.clear();
	gender.clear();
	head.clear();

#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	std::stringstream os;
	bool r = false;
	MYSQL_ROW rows;
	if (engine == NULL) {
		LOG_ERROR("GetConnection Error");
		return false;
	}
	os	<< "select nickname,sex,head from " << USERINFOS
		<< " where usrid=\'" << uid.c_str() << "\';";
	const char* sql = os.str().c_str();
	LOG_DEBUG2("[%s]",os.str().c_str());
	r = engine->SQLExec(os.str().c_str());

	if (!r) {
		LOG_ERROR2("exec sql error");
		return false;
	}
	int32 num = engine->RecordCount();
	if (num > 0) {
		if (rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc)) {
			nickname = rows[0];
			gender = rows[1];
			head = rows[2];
		}
		return true;
	}
	return false;
}

bool DBComm::GetMusicUrl(const std::string& song_id,std::string& hq_url,
						 std::string& song_url){
	 std::stringstream os;
	 bool r = false;
#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	 if (engine==NULL){
		 LOG_ERROR("engine error");
		 return true;
	 }
	 base_storage::db_row_t* db_rows;
	 int num;
	 MYSQL_ROW rows = NULL;
	 os<<"select song_hifi_url,song_url from migfm_music_url where song_id =\'"
		 <<song_id.c_str()<<"\';";
	 r = engine->SQLExec(os.str().c_str());
	 if(!r){
		 MIG_ERROR(USER_LEVEL,"sqlexec error");
		 return r;
	 }

	 num = engine->RecordCount();
	 if(num>0){
		 while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			 if (rows[0] ==NULL)
				 hq_url = DEFAULT_URL;
			 else
				 hq_url = rows[0];

			 if (rows[1] ==NULL)
				 song_url = DEFAULT_URL;
			 else
				 song_url = rows[1];
		 }
		 song_url = hq_url;
		 return true;
	 }else{
		 hq_url = song_url = DEFAULT_URL;
		 return false;
	 }
	 return true;
}


bool DBComm::GetMusicOtherInfos(std::map<std::string,base::MusicInfo>&song_music_infos){

	std::stringstream os;
	bool r = false;
#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	base_storage::db_row_t* db_rows;
	MYSQL_ROW rows = NULL;
	int num = song_music_infos.size();
	if (num<=0)
		return true;

	os<<"call proc_GetMusicInfo('";
	for(std::map<std::string,base::MusicInfo>::iterator it = song_music_infos.begin();
		it!=song_music_infos.end();++it){

			os<<it->second.id();
			num--;
			if (num!=0){
				os<<",";
			}
	}

	os<<"');";
	LOG_DEBUG2("%s",os.str().c_str());
	r = engine->SQLExec(os.str().c_str());
	if(!r){
		MIG_ERROR(USER_LEVEL,"sqlexec error ");
		return r;
	}

	num = engine->RecordCount();
	if(num>0){
		std::map<std::string,base::MusicInfo>::iterator itr 
			= song_music_infos.begin();

		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			std::string songdid = rows[1];
			std::string hifi_url = rows[2];
			//std::string url = rows[3];
			std::string clt_num = rows[4];
			std::string cmt_num = rows[5];
			std::string hot_num = rows[6];
			if (itr!=song_music_infos.end()){
				itr->second.set_hq_url(hifi_url);
				itr->second.set_url(hifi_url);
				itr->second.set_music_clt(clt_num);
				itr->second.set_music_cmt(cmt_num);
				itr->second.set_music_hot(hot_num);
			}
			++itr;
		}
		return true;
	}

	return false;
}


bool DBComm::GetLBSAboutInfos(const std::string& uid,std::string& sex,std::string& nickname,
							  std::string& head,std::string& birthday,
							  double& latitude,double& longitude){
#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
#if defined (_DB_SINGLE_)
		mig_lbs::RLockGd lk(db_single_lock_);
		base_storage::DBStorageEngine* engine = db_conn_single_;
#endif
		std::stringstream os;
		bool r = false;
		MYSQL_ROW rows;
		if (engine == NULL) {
			LOG_ERROR("GetConnection Error");
			return false;
		}
		//call proc_GetLbsAboutInfos(10108);
		os<<"call proc_GetLbsAboutInfos("
			<<uid<<")";

		const char* sql = os.str().c_str();
		LOG_DEBUG2("[%s]",os.str().c_str());
		r = engine->SQLExec(os.str().c_str());

		if (!r) {
			LOG_ERROR2("exec sql error");
			return false;
		}
		int32 num = engine->RecordCount();
		if (num > 0) {
			if (rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
				sex = rows[0];
				nickname = rows[1];
				head = rows[2];
				birthday = rows[3];
				latitude = atof(rows[4]);
				longitude = atof(rows[5]);
			}
			return true;
		}
		return false;

}

bool DBComm::GetWXMusicUrl(const std::string& song_id,std::string& song_url,
						   std::string& dec,std::string& dec_id,std::string& dec_word){
	std::stringstream os;
	std::stringstream os1;
	bool r = false;
#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	if (engine==NULL){
	   LOG_ERROR("engine error");
	   return true;
	}
	base_storage::db_row_t* db_rows;
	int num;
	MYSQL_ROW rows = NULL;
	os<<"select song_url from migfm_music_url where song_id =\'"
	   <<song_id.c_str()<<"\';";
	r = engine->SQLExec(os.str().c_str());
	if(!r){
	   MIG_ERROR(USER_LEVEL,"sqlexec error");
	   return r;
	}

	num = engine->RecordCount();
	if(num>0){
	   while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
		   song_url = rows[0];
	   }
	}

	//mood scens
	int current = time(NULL)%2;


	dec = "mm";
	os.str("");
	os<<" select name,typeid from migfm_mood_word where typeid=1 ORDER BY RAND() limit 1;";
// 	if (current==0){//mood
// 		dec="mm";
// 		os.str("");
// 		os<<"select a.name,typeid from migfm_mood_word as a join migfm_mood as b where a.typeid = b.id  ORDER BY RAND() limit 1;";
// 	}else{//scens
// 		dec="ms";
// 		os.str("");
// 		os<<"select a.name,typeid from migfm_scene_word as a join migfm_scene as b where a.typeid = b.id  ORDER BY RAND() limit 1;";
// 	}

	MIG_DEBUG(USER_LEVEL,"%s",os.str().c_str());
	r = engine->SQLExec(os.str().c_str());
	if(!r){
		MIG_ERROR(USER_LEVEL,"sqlexec error");
		return r;
	}

	num = engine->RecordCount();
 	if(num>0){
 		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
 			dec_id = rows[1];
			Base64Decode(rows[0],&dec_word);
 		}
 		return true;
 	}

	return false;
}

bool DBComm::AddMusciFriend(const std::string& uid, 
							const std::string &touid){
#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	std::stringstream os;
	bool r = false;
	MYSQL_ROW rows;
	if (engine == NULL) {
		LOG_ERROR("GetConnection Error");
		return false;
	}

	// proc_AddFriend
	os	<< "call proc_RecordMusicFriend("
		<< uid.c_str() << ","
		<< touid.c_str() << ");";

	std::string sql = os.str();
	LOG_DEBUG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());

	if (!r) {
		LOG_ERROR2("exec sql error");
		return false;
	}
	return true;
}

bool DBComm::AddFriend(const std::string& uid, const std::string& touid) {
#if defined (_DB_POOL_)
	AutoDBCommEngine auto_engine;
	base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	std::stringstream os;
	bool r = false;
	MYSQL_ROW rows;
	if (engine == NULL) {
		LOG_ERROR("GetConnection Error");
		return false;
	}

	// proc_AddFriend
	os	<< "call proc_AddFriend("
		<< uid.c_str() << ","
		<< touid.c_str() << ")";
	std::string sql = os.str();
	LOG_DEBUG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());

	if (!r) {
		LOG_ERROR2("exec sql error");
		return false;
	}
	return true;
}

bool DBComm::GetFriendList(const std::string& uid, FriendInfoList& friends) {
#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	std::stringstream os;
	bool r = false;
	MYSQL_ROW rows;
	if (engine == NULL) {
		LOG_ERROR("GetConnection Error");
		return false;
	}

	// proc_GetUserFriends
	os	<< "call proc_GetUserFriends("
		<< uid.c_str() << ")";
	std::string sql = os.str();
	LOG_DEBUG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());

	if (!r) {
		LOG_ERROR2("exec sql error");
		return false;
	}

	int num = engine->RecordCount();
 	if(num > 0){
 		friends.reserve(num);
 		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)) {
 			FriendInfo finfo;
 			finfo.uid = rows[0];
 			finfo.name = rows[1];
 			finfo.type = atoi(rows[2]);
 			friends.push_back(finfo);
 		}
 		return true;
 	}

	return false;
}

bool DBComm::RecordShareInfo(const int64 uid,const int64 songid,const int64 plat){

#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	  std::stringstream os;
	  bool r = false;
	  MYSQL_ROW rows;
	  if (engine==NULL){
		  LOG_ERROR("GetConnection Error");
		  return false;
	  }

	  //proc_RecordShareLog(uid,songid,plat)
	  os<<"call proc_RecordShareLog("<<uid<<","<<songid<<","<<plat<<")";
	  const char* sql = os.str().c_str();
	  LOG_DEBUG2("[%s]",os.str().c_str());
	  r = engine->SQLExec(os.str().c_str());
	  if (!r){
		  LOG_ERROR2("exec sql error");
		  return false;
	  }
	  int32 num = engine->RecordCount();
	  if(num>0){
		  while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			  int reuslt = atol(rows[0]);
			  if(reuslt==1)
				  return true;
		  }
	  }
	  return false;
}

bool DBComm::GetMessageList(const int64 uid,const int64 count,const int64 from,
		std::list<struct MessageListInfo>& message_list){


#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	  std::stringstream os;
	  bool r = false;
	  MYSQL_ROW rows;
	  if (engine==NULL){
		  LOG_ERROR("GetConnection Error");
		  return false;
	  }

	  //call proc_GetMessageList(10112,10,0);
	  os<<"call proc_GetMessageList("
			  <<uid<<","<<from<<","<<count<<")";
	  const char* sql = os.str().c_str();
	  LOG_DEBUG2("[%s]",os.str().c_str());
	  r = engine->SQLExec(os.str().c_str());

	  if (!r){
		  LOG_ERROR2("exec sql error");
		  return false;
	  }
	  int32 num = engine->RecordCount();
	  if(num>0){
		  while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			  struct MessageListInfo msg_info;
			  msg_info.detail.uid = uid;
			  msg_info.detail.msg_id = atoll(rows[0]);
			  msg_info.detail.message_type = atol(rows[1]);
			  msg_info.detail.message = rows[2];
			  msg_info.detail.fromuid = atoll(rows[3]);
			  msg_info.userinfo.set_uid(rows[3]);
			  msg_info.detail.msg_time = rows[4];
			  msg_info.detail.distance = atof(rows[5]);
			  std::string songid = rows[6];
			  msg_info.musicinfo.set_id(songid);
			  msg_info.userinfo.set_birthday(rows[7]);
			  msg_info.userinfo.set_head(rows[8]);
			  msg_info.userinfo.set_crty(rows[9]);
			  msg_info.userinfo.set_nickname(rows[10]);
			  msg_info.userinfo.set_sex(rows[11]);
			  msg_info.userinfo.set_source(rows[12]);
			  msg_info.musicinfo.set_hq_url(rows[13]);
			  msg_info.musicinfo.set_url(rows[14]);
			  msg_info.musicinfo.set_music_clt(rows[15]);
			  msg_info.musicinfo.set_music_cmt(rows[16]);
			  msg_info.musicinfo.set_music_hot(rows[17]);

			  msg_info.current_musicinfo.set_id(rows[18]);
			  msg_info.current_musicinfo.set_title(rows[19]);
			  msg_info.current_musicinfo.set_artist(rows[20]);
			  msg_info.current_musicinfo.set_album_title(rows[21]);
			  //若送歌的url 不存在则不添加
			  if((msg_info.musicinfo.hq_url().empty()||msg_info.musicinfo.hq_url()=="0")
					  && msg_info.detail.message_type==PARENT_TYPE)
				  continue;
			  message_list.push_back(msg_info);
		  }
		  return true;
	  }
	  return false;
}

bool DBComm::GetUserInfos(int64 uid,
						  std::string& nickname,std::string& gender,
						  std::string& type,std::string& birthday,
						  std::string& location,std::string& source,
						  std::string& head){
	nickname.clear();
	gender.clear();
	type.clear();
	birthday.clear();
	location.clear();
	source.clear();
	head.clear();

#if defined (_DB_POOL_)
		AutoDBCommEngine auto_engine;
		base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	  std::stringstream os;
	  bool r = false;
	  MYSQL_ROW rows;
	  if (engine==NULL){
		  LOG_ERROR("GetConnection Error");
		  return false;
	  }
	  os<<"select sex,type,ctry,birthday,nickname,source,head from "
		  <<USERINFOS<<" where usrid=" << uid << ";";
	  const char* sql = os.str().c_str();
	  LOG_DEBUG2("[%s]",os.str().c_str());
	  r = engine->SQLExec(os.str().c_str());

	  if (!r){
		  LOG_ERROR2("exec sql error");
		  return false;
	  }
	  int32 num = engine->RecordCount();
	  if(num>0){
		  while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			  gender = rows[0];
			  type = rows[1];
			  location = rows[2];
			  birthday = rows[3];
			  nickname = rows[4];
			  source = rows[5];
			  head = rows[6];
		  }
		  return true;
	  }
	  return false;

}


} // mig_sociality
