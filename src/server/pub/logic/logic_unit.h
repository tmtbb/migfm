/*
 * logic_unit.h
 *
 *  Created on: 2014年12月2日
 *      Author: kerry
 */
#ifndef _BUDDHA_LOGIC_UNIT_H__
#define _BUDDHA_LOGIC_UNIT_H__

#include "logic/logic_infos.h"
#include "net/comm_head.h"
#include "basic/radom_in.h"

#define UNKONW_DIMENSIONS    "未知维度"
#define MOOD_NAME            "心情"
#define SCENE_NAME           "场景"
#define CHANNEL_NAME         "频道"
#define PLATFORM_NAME        "miyo"

#define UNKONW_DIMENSIONS_ID 100000

#define MUSICINFO_MAP         std::map<int64,base_logic::MusicInfo>
#define MUSICINFONLIST_MAP    std::map<int64,std::map<int64,base_logic::MusicInfo> >

//维度存储歌曲
#define DIMENSION_VEC         std::vector<base_logic::MusicInfo>  /*歌曲*/
#define MULTI_DIMENSION_VEC   std::map<int64,DIMENSION_VEC > /*维度ID -- 歌曲存储*/

#define DIMENSION_MAP         std::map<int64,base_logic::MusicInfo> /* 歌曲ID--歌曲信息*/
#define MULTI_DIMENSION_MAP   std::map<int64,DIMENSION_MAP > /*维度ID -- 歌曲存储*/


#define RADOMIN_MAP           std::map<int,base::MigRadomInV2*>

#define USER_INFO_MAP         std::map<int64,base_logic::UserInfo> /*ID----用户信息*/


namespace base_logic{

class LogicUnit{
public:
	static bool IPToGeocoderAndAddress(netcomm_recv::HeadPacket* packet,
			base_logic::LBSInfos* lbs_infos);

	static base_logic::LBSInfos*  GetGeocderAndAddress(netcomm_recv::HeadPacket* packet);

	static bool GetGeocoderWeather(base_logic::LBSInfos* lbsinfo,std::string& weather,std::string& temp);

	static void CreateToken(const int64 uid,std::string& token);

	static void SendMessage(const int socket,netcomm_send::HeadPacket* packet);

	static void SendErrorMsg(const int32 error_code,const int socket);

	static void RecordBehavior(const int32 cat,const int64 uid,const std::string& str);
};

//批量获取
class GetBatchData{
public:
	//先从memcached 批量获取 没有的数据从数据库获取再写入memcached
	bool BatchGetUserinfo(std::vector<int64>& vec,
			std::map<int64,base_logic::UserInfo>& info);
};

}

#define send_error      base_logic::LogicUnit::SendErrorMsg
#define send_message    base_logic::LogicUnit::SendMessage

#endif

