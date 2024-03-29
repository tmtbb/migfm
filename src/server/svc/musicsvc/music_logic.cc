#include "music_logic.h"
#include "whole_manager.h"
#include "db_comm.h"
#include "music_dic_comm.h"
#include "data_cache_manager.h"
#include "logic/pub_db_comm.h"
#include "logic/pub_dic_comm.h"
#include "logic/logic_unit.h"
#include "logic/logic_comm.h"
#include "logic/behavior.h"
#include "config/config.h"
#include "basic/native_library.h"
#include "intertface/robot_interface.h"
#include "common.h"

namespace musicsvc_logic{

Musiclogic*
Musiclogic::instance_=NULL;

Musiclogic::Musiclogic(){
   if(!Init())
      assert(0);
}

Musiclogic::~Musiclogic(){
}

bool Musiclogic::Init(){

	bool r = false;
	std::string path = DEFAULT_CONFIG_PATH;
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	if(config==NULL)
		return false;
	r = config->LoadConfig(path);
	basic_logic::PubDBComm::Init(config->mysql_db_list_);
	basic_logic::PubDicComm::Init(config->redis_list_);

	musicsvc_logic::MusicDicComm::Init(config->redis_list_);
	musicsvc_logic::MemComm::Init(config->mem_list_);


	musicsvc_logic::DBComm::Init(config->mysql_db_list_);
	musicsvc_logic::CacheManagerOp::GetWholeManager();

	//读取信息
	musicsvc_logic::CacheManagerOp::FetchAvailableMusicInfo();

	//
	musicsvc_logic::CacheManagerOp::FetchDimensionMusic();

	musicsvc_logic::CacheManagerOp::GetWholeManager()->CreateRadomin();

    return true;
}

Musiclogic*
Musiclogic::GetInstance(){

    if(instance_==NULL)
        instance_ = new Musiclogic();

    return instance_;
}



void Musiclogic::FreeInstance(){
    delete instance_;
    instance_ = NULL;
}

bool Musiclogic::OnMusicConnect(struct server *srv,const int socket){

    return true;
}



bool Musiclogic::OnMusicMessage(struct server *srv, const int socket, const void *msg,const int len){
	const char* packet_stream = (char*)(msg);
	std::string http_str(packet_stream,len);
	std::string error_str;
	int error_code = 0;
	LOG_DEBUG2("%s",packet_stream);

	scoped_ptr<base_logic::ValueSerializer> serializer(base_logic::ValueSerializer::Create(base_logic::IMPL_HTTP,&http_str));


	netcomm_recv::NetBase*  value = (netcomm_recv::NetBase*)(serializer.get()->Deserialize(&error_code,&error_str));
	if(value==NULL){
		error_code = STRUCT_ERROR;
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	scoped_ptr<netcomm_recv::HeadPacket> packet(new netcomm_recv::HeadPacket(value));
	int32 type = packet->GetType();

	switch(type){
	case MUSIC_GAIN_COLLECT_LIST:
		OnCollectList(srv,socket,value);
		break;
	case MUSIC_GAIN_DIMENSION_LIST:
		OnDimensionList(srv,socket,value);
		break;
	case MUSIC_GAIN_DIMENSIONS_LIST:
		OnDimensionsList(srv,socket,value);
		break;
	case MUSIC_GAIN_SET_COLLECT:
		OnSetCollection(srv,socket,value);
		break;
	case MUSIC_GAIN_DEL_COLLECT:
		OnDelCollection(srv,socket,value);
		break;
	case MUSIC_GAIN_NEAR_MUSIC:
		OnNearMusic(srv,socket,value);
		break;
	case MUSIC_GAIN_NEAR_USER:
		OnSocNearUser(srv,socket,value);
		break;
	case MUSIC_GAIN_SET_HATE:
		OnHateList(srv,socket,value);
		break;
	case MUSIC_SET_CURRENT_MUSIC:
		OnRecordMusic(srv,socket,value);
		break;
	case MUSIC_GAIN_MY_MUSIC_FRIEND:
		OnSocMyMusicFriend(srv,socket,value);
		break;
	case MUSIC_GAIN_DIMENSION_INFO:
		OnDimensionInfo(srv,socket,value);
		break;
	}
    return true;
}

bool Musiclogic::OnMusicClose(struct server *srv,const int socket){

    return true;
}



bool Musiclogic::OnBroadcastConnect(struct server *srv, const int socket, const void *msg,const int len){
	robot_server_socket_ = socket;
    return true;
}

bool Musiclogic::OnBroadcastMessage(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}



bool Musiclogic::OnBroadcastClose(struct server *srv, const int socket){

    return true;
}

bool Musiclogic::OnIniTimer(struct server *srv){

    return true;
}



bool Musiclogic::OnTimeout(struct server *srv, char *id, int opcode, int time){

    return true;
}

bool Musiclogic::OnDimensionList(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg,const int len){
	scoped_ptr<netcomm_recv::Dimension> dimension(new netcomm_recv::Dimension(netbase));
	int error_code = dimension->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	MUSICINFO_MAP music_list;

	musicsvc_logic::CacheManagerOp::GetWholeManager()->GetDimensionList(dimension->dimension_name(),
			dimension->dimension_sub_id(),music_list);
	scoped_ptr<netcomm_send::MusicList> smusiclist(new netcomm_send::MusicList());
	for(MUSICINFO_MAP::iterator it = music_list.begin();it!=music_list.end();){
		base_logic::MusicInfo info = it->second;
		base_logic::DictionaryValue* dict = info.Release();
		if(dict!=NULL)
			smusiclist->set_list(dict);
		music_list.erase(it++);
	}

	send_message(socket,(netcomm_send::HeadPacket*)smusiclist.get());
	return true;
}

bool Musiclogic:: OnDimensionsList(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
		const void* msg,const int len){
	scoped_ptr<netcomm_recv::Dimensions> dimension(new netcomm_recv::Dimensions(netbase));
	int error_code = dimension->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	MUSICINFO_MAP music_list;
	int index = dimension->ChannelDimension().dimension_index();
	if(dimension->ChannelDimension().dimension_index()!=-1)
		musicsvc_logic::CacheManagerOp::GetWholeManager()->GetDimensionList(dimension->ChannelDimension().dimension_alias_name(),
				dimension->ChannelDimension().dimension_id(),music_list,dimension->Num());
	if(dimension->MoodDimension().dimension_index()!=-1)
		musicsvc_logic::CacheManagerOp::GetWholeManager()->GetDimensionList(dimension->MoodDimension().dimension_alias_name(),
				dimension->MoodDimension().dimension_id(),music_list,dimension->Num());
	if(dimension->ScensDimension().dimension_index()!=-1)
		musicsvc_logic::CacheManagerOp::GetWholeManager()->GetDimensionList(dimension->ScensDimension().dimension_alias_name(),
				dimension->ScensDimension().dimension_id(),music_list,dimension->Num());

	scoped_ptr<netcomm_send::MusicList> smusiclist(new netcomm_send::MusicList());
	for(MUSICINFO_MAP::iterator it = music_list.begin();it!=music_list.end();){
		base_logic::MusicInfo info = it->second;
		base_logic::DictionaryValue* dict = info.Release();
		if(dict!=NULL)
			smusiclist->set_list(dict);
		music_list.erase(it++);
	}

	send_message(socket,(netcomm_send::HeadPacket*)smusiclist.get());
	return true;
}

bool Musiclogic::OnCollectList(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg,const int len){
	scoped_ptr<netcomm_recv::Collect> collect(new netcomm_recv::Collect(netbase));
	int error_code = collect->GetResult();
	int64 group_id = 0;
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	//获取红心歌单
	MUSICINFO_MAP music_list;

	musicsvc_logic::CacheManagerOp::GetWholeManager()->GetCollectList(collect->uid(),music_list);
	scoped_ptr<netcomm_send::MusicList> smusiclist(new netcomm_send::MusicList());
	for(MUSICINFO_MAP::iterator it = music_list.begin();it!=music_list.end();){
		base_logic::MusicInfo info = it->second;
		info.set_like(PFE_LIKE);
		base_logic::DictionaryValue* dict = info.Release();
		if(dict!=NULL)
			smusiclist->set_list(dict);
		music_list.erase(it++);
	}

	send_message(socket,(netcomm_send::HeadPacket*)smusiclist.get());
	return true;
}

bool Musiclogic::OnSetCollection(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg,const int len){
	scoped_ptr<netcomm_recv::SetCollect> collect(new netcomm_recv::SetCollect(netbase));
	int error_code = collect->GetResult();
	int64 group_id = 0;
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	//设置
	base_logic::MusicInfo musicinfo;
	musicinfo.set_id(collect->songid());
	musicinfo.set_class(collect->dimemsion_id());
	musicinfo.set_class_name(collect->dimension_alais());
	musicsvc_logic::CacheManagerOp::GetWholeManager()->SetCollectSong(collect->uid(),musicinfo);

	scoped_ptr<netcomm_send::HeadPacket> head(new netcomm_send::HeadPacket());
	head->set_status(1);
	send_message(socket,(netcomm_send::HeadPacket*)head.get());
	return true;
}

bool Musiclogic::OnDelCollection(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	scoped_ptr<netcomm_recv::DelCollect> delcollect(new netcomm_recv::DelCollect(netbase));
	int error_code = delcollect->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	musicsvc_logic::CacheManagerOp::GetWholeManager()->DelCollectSong(delcollect->uid(),delcollect->songid());
	scoped_ptr<netcomm_send::HeadPacket> head(new netcomm_send::HeadPacket());
	head->set_status(1);
	send_message(socket,(netcomm_send::HeadPacket*)head.get());
	return true;
}

bool Musiclogic::OnHateList(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg,const int len){
	scoped_ptr<netcomm_recv::SetHateCollect> collect(new netcomm_recv::SetHateCollect(netbase));
	int error_code = collect->GetResult();
	int64 group_id = 0;
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	//设置
	base_logic::MusicInfo musicinfo;
	musicinfo.set_id(collect->songid());
	musicsvc_logic::CacheManagerOp::GetWholeManager()->SetHatList(collect->uid(),musicinfo);

	scoped_ptr<netcomm_send::HeadPacket> head(new netcomm_send::HeadPacket());
	head->set_status(1);
	send_message(socket,(netcomm_send::HeadPacket*)head.get());
	return true;
}

bool Musiclogic::OnNearMusic(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len ){
	scoped_ptr<netcomm_recv::NearMusic> near_music(new netcomm_recv::NearMusic(netbase));
	int error_code = near_music->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	//std::map<int64,base_logic::UserAndMusic>infomap;
	std::list<base_logic::UserAndMusic> list;
	GetNearUserAndMusic(near_music->uid(),MUSIC_NEAR,near_music->latitude(),
			near_music->longitude(),0,0,list);

	scoped_ptr<netcomm_send::NearMusic> snear_music(new netcomm_send::NearMusic());
	while(list.size()>0){
		base_logic::UserAndMusic info = list.back();
		list.pop_back();
		if(info.musicinfo_.Isvalid()&&info.userinfo_.uid()!=near_music->uid_)
			snear_music->set_info(info.Release(true,near_music->latitude(),near_music->longitude()));
	}

	/*for(std::map<int64,base_logic::UserAndMusic>::iterator it = infomap.begin();
			it!=infomap.end();it++){
		base_logic::UserAndMusic info = it->second;
		//判断如果没有听歌不发送
		LOG_DEBUG2("uid %lld muisc %lld",info.userinfo_.uid(),info.musicinfo_.id());
		if(info.musicinfo_.Isvalid())
			snear_music->set_info(info.Release(true,near_music->latitude(),near_music->longitude()));
	}*/
	send_message(socket,(netcomm_send::HeadPacket*)snear_music.get());

	return true;
}

bool Musiclogic::OnSocNearUser(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	scoped_ptr<netcomm_recv::NearUser> near_user(new netcomm_recv::NearUser(netbase));
	int error_code = near_user->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	int i = 0;

	//std::map<int64,base_logic::UserAndMusic>infomap;
	std::list<base_logic::UserAndMusic> list;
	GetNearUserAndMusic(near_user->uid(),MUSIC_NEAR,near_user->latitude(),near_user->longitude(),0,0,list);

	scoped_ptr<netcomm_send::NearUser> snear_user(new netcomm_send::NearUser());
	while(list.size()>0){
		base_logic::UserAndMusic info = list.back();
		list.pop_back();
		if(info.userinfo_.uid()>0&&info.userinfo_.uid()!=near_user->uid_)
			snear_user->set_info(info.Release(true,near_user->latitude(),near_user->longitude()));
	}
	/*for(std::map<int64,base_logic::UserAndMusic>::iterator it = infomap.end();
			it!=infomap.begin()&&i<near_user->page_size();it--){
		base_logic::UserAndMusic info = it->second;
		if(info.userinfo_.uid()>0){
			snear_user->set_info(info.Release(true,near_user->latitude(),near_user->longitude()));
			i++;
		}
	}*/
	send_message(socket,(netcomm_send::HeadPacket*)snear_user.get());
}

bool Musiclogic::OnSocMyMusicFriend(struct server* srv,const int socket,netcomm_recv::NetBase* netbase,
   		const void* msg,const int len){

	scoped_ptr<netcomm_recv::MyMusicFriend> owen_freind(new netcomm_recv::MyMusicFriend(netbase));
	int error_code = owen_freind->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	std::list<base_logic::UserAndMusic> list;
	GetNearUserAndMusic(owen_freind->uid(),MUSIC_FRI,
			owen_freind->latitude(),owen_freind->longitude(),
			owen_freind->from(),owen_freind->count(),list);
	scoped_ptr<netcomm_send::MyMusicFriend> send_freind(new netcomm_send::MyMusicFriend());
	while(list.size()>0){
		base_logic::UserAndMusic info = list.back();
		list.pop_back();
		if(info.userinfo_.Isvalid()&&info.userinfo_.uid()!=owen_freind->uid())
			send_freind->set_unit(info.Release(true,owen_freind->latitude(),owen_freind->longitude()));
	}

	//发送
	send_message(socket,(netcomm_send::HeadPacket*)send_freind.get());
	return true;
}

bool Musiclogic::OnDimensionInfo(struct server* srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	scoped_ptr<netcomm_recv::LoginHeadPacket> login(new netcomm_recv::LoginHeadPacket(netbase));
	int error_code = login->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	//获取信息
	scoped_ptr<netcomm_send::DimensionInfo> dimension(new netcomm_send::DimensionInfo());
	musicsvc_logic::CacheManagerOp::GetWholeManager()->GetDimensionInfo(dimension.get());
	send_message(socket,(netcomm_send::HeadPacket*)dimension.get());
	return true;
}

bool Musiclogic::OnRecordMusic(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	scoped_ptr<netcomm_recv::RecordMusic> record(new netcomm_recv::RecordMusic(netbase));
	int error_code = record->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	bool r = false;
	std::string json;
	//创建音乐信息
	base_logic::MusicInfo music;
	music.set_id(record->current_song_id());
	music.set_title(record->name());
	music.set_artist(record->singer());
	music.set_class_name(record->dimension_name());
	music.set_class(record->dimension_sub_id());

	music.JsonDeserialize(json);
	MIG_DEBUG(USER_LEVEL,"%s",json.c_str());

	scoped_ptr<netcomm_send::HeadPacket> head(new netcomm_send::HeadPacket());
	head->set_status(1);
	send_message(socket,(netcomm_send::HeadPacket*)head.get());
	//通知用户听歌发生变化
	NoticeUserCurrentSong(robot_server_socket_,10000,record->uid(),
			record->current_song_id(),record->dimension_sub_id(),record->dimension_name().c_str());
	//写入memcached
	if(!json.empty()){
		musicsvc_logic::MemComm::SetUserCurrentMusic(record->uid(),json);
		//历史记录
		musicsvc_logic::DBComm::RecordMusicHistory(record->uid(),record->current_song_id());
		//行为分析
		//日期/uid.txt
		base_logic::BehaviorEngine::Instance()->RecordUserListenBehavior(record->uid(),record->current_song_id(),record->dimension_sub_id(),record->dimension_name().c_str());
		//base_logic::LogicUnit::RecordBehavior(LISTEN_MUSIC_BEH,record->uid(),json);
	}

	return true;
}

bool Musiclogic::OnSocGetPushMessage(struct server* srv,const int socket,
		netcomm_recv::NetBase* netbase,const void* msg,const int len){

}

void Musiclogic::GetNearUserAndMusic(const int64 uid,
		const int32 cat,const double latitude,const double longitude,
		const int32 from,const int32 count,
		std::list<base_logic::UserAndMusic>& infolist){

	//判断如果有坐标则获取周边的人 如果为0 根据登陆获取
	std::map<int64,base_logic::UserAndMusic> infomap;
	if(cat==0){
		musicsvc_logic::DBComm::GetMyFriend(uid,from,count,infomap);
	}else{
		if(latitude==0 || longitude==0)
			basic_logic::PubDBComm::GetUserInfoByLoginTime(infomap);
		else
			basic_logic::PubDBComm::GetUserInfoByLocation(infomap);
	}
	//获取对应用户的音乐
	musicsvc_logic::MemComm::BatchGetCurrentSong(infomap);
	for(std::map<int64,base_logic::UserAndMusic>::iterator it = infomap.end();
				it!=infomap.begin();it--){
		base_logic::UserAndMusic info = it->second;
		infolist.push_back(info);
	}

	//是否当前用户的红心歌单
	musicsvc_logic::CacheManagerOp::GetWholeManager()->CheckIsCollectSong(uid,infolist);
	infolist.sort(base_logic::UserAndMusic::cmptime);

}

}

