#include "soc_logic.h"
#include "db_comm.h"
#include "logic/cache_manager.h"
#include "logic/pub_db_comm.h"
#include "logic/logic_unit.h"
#include "logic/logic_comm.h"
#include "weather/weather_engine.h"
#include "lbs/lbs_connector.h"
#include "common.h"

namespace socsvc_logic{

Soclogic*
Soclogic::instance_=NULL;

Soclogic::Soclogic(){
   if(!Init())
      assert(0);
}

Soclogic::~Soclogic(){
}

bool Soclogic::Init(){
	bool r = false;
	std::string path = DEFAULT_CONFIG_PATH;
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	if(config==NULL){
		return false;
	}
	r = config->LoadConfig(path);
	socsvc_logic::DBComm::Init(config->mysql_db_list_);
	basic_logic::PubDBComm::Init(config->mysql_db_list_);
	//初始化全局存储
	base_logic::WholeManager::GetWholeManager();
	base_lbs::LbsConnectorEngine::Create(base_lbs::IMPL_BAIDU);
	base_lbs::LbsConnector* engine = base_lbs::LbsConnectorEngine::GetLbsConnectorEngine();
	engine->Init(config->mysql_db_list_);

	base_weather::WeatherConnectorEngine::Create(base_weather::IMPL_CAIYUN);
	base_weather::WeatherConnector* weather_engine = base_weather::WeatherConnectorEngine::GetWeatherConnectorEngine();
	weather_engine->Init();
    return true;
}

Soclogic*
Soclogic::GetInstance(){

    if(instance_==NULL)
        instance_ = new Soclogic();

    return instance_;
}



void Soclogic::FreeInstance(){
    delete instance_;
    instance_ = NULL;
}

bool Soclogic::OnSocConnect(struct server *srv,const int socket){

    return true;
}



bool Soclogic::OnSocMessage(struct server *srv, const int socket, const void *msg,const int len){
	const char* packet_stream = (char*)(msg);
	std::string http_str(packet_stream,len);
	std::string error_str;
	int error_code = 0;
	LOG_MSG2("%s",packet_stream);

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
	 case SOC_GAIN_CURRENT_TYPE_BARRAGECOMM:
		 OnGainBarrageComm(srv,socket,value);
		 break;
	 case SOC_GAIN_CURRENT_LOCATION_INFO:
		 OnGainLocation(srv,socket,value);
		 break;
	}
    return true;
}

bool Soclogic::OnSocClose(struct server *srv,const int socket){

    return true;
}



bool Soclogic::OnBroadcastConnect(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}

bool Soclogic::OnBroadcastMessage(struct server *srv, const int socket, const void *msg,const int len){

}



bool Soclogic::OnBroadcastClose(struct server *srv, const int socket){

    return true;
}

bool Soclogic::OnIniTimer(struct server *srv){

    return true;
}



bool Soclogic::OnTimeout(struct server *srv, char *id, int opcode, int time){

    return true;
}

bool Soclogic::OnGainBarrageComm(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg,const int len){
	scoped_ptr<netcomm_recv::BarrageComm> barrage(new netcomm_recv::BarrageComm(netbase));
	std::list<socsvc_logic::BarrageInfos> list;
	int error_code = barrage->GetResult();
	int64 group_id = 0;
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	//获取对应描述
	std::string desc;

	group_id = base_logic::WholeManager::GetWholeManager()->dimensions_name(barrage->type(),
			barrage->tid(),desc);
	group_id = 20001;
	//读取聊天记录
	scoped_ptr<netcomm_send::BarrageComm> send_barragecom(new netcomm_send::BarrageComm());
	bool r = socsvc_logic::DBComm::GetTypeBarrage(barrage->platform(),group_id,50,list);
	while(r&&list.size()>0){
		socsvc_logic::BarrageInfos barragecom = list.front();
		list.pop_front();
		send_barragecom->set_barrage(barragecom.nickname(),barragecom.msg_id(),desc,
				barrage->tid(),barragecom.message());
	}
	send_barragecom->set_group_id(group_id);
	//发送
	send_message(socket,(netcomm_send::HeadPacket*)send_barragecom.get());
	return true;
}

bool Soclogic::OnGainLocation(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	scoped_ptr<netcomm_recv::LocationInfo> location(new netcomm_recv::LocationInfo(netbase));
	int error_code = location->GetResult();
	scoped_ptr<base_logic::LBSInfos> lbs_info;
	std::string temp;
	std::string weather;
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	//获取地区
	//获取地址
	lbs_info.reset(base_logic::LogicUnit::GetGeocderAndAddress((netcomm_recv::HeadPacket*)location.get()));
	location.get()->Reset();
	//获取天气
	base_logic::LogicUnit::GetGeocoderWeather(lbs_info.get(),weather,temp);

	scoped_ptr<netcomm_send::Location> send_location(new netcomm_send::Location());
	send_location->set_city(lbs_info->city());
	send_location->set_temp(temp);
	send_location->set_weather(weather);
	send_message(socket,(netcomm_send::HeadPacket*)send_location.get());
	return true;

}

}

