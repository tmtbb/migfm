#ifndef __ROBOT_DB_COMM_H__
#define __ROBOT_DB_COMM_H__

#include "logic_unit.h"
#include "robot_cache_manager.h"
#include "logic/logic_comm.h"
#include "basic/basictypes.h"
#include "storage/storage.h"
#include "basic/basic_info.h"

//class robot_logic::CacheManagerOp;

namespace robot_storage{

class DBComm{
public:
	DBComm(){};
	virtual ~DBComm(){};
	static base_storage::DBStorageEngine *GetConnection();
public:
	static void Init(std::list<base::ConnAddr>& addrlist,const int32 db_conn_num = 5);

	static void Dest();

	static bool GetAssistant(RobotInfosMap& robot_infos);

	static bool GetRobotInfos(const int from,const int count,RobotInfosMap& robot_infos);

	static bool GetUserLbsPos(const int64 uid,double& latitude,double& longitude);

	static bool UpdateRobotLbsPos(const int64 uid,double latitude,double longitude);

	static bool GetRobotLoginListenSong(const int64 uid,int64& songid);

	static bool GetChannelInfos(std::list<int64>& list);

	static bool GetMoodInfos(std::list<int64>& list);

	static bool GetSceneInfos(std::list<int64>& list);

	static bool GetUsersLBSPos(std::list<robot_base::UserLbsInfo>& user_lbs_list,const int from,
			const int count);

	static bool UpdateRobotLoginTime(const int64 uid);

	static bool GetLuckGiftInfo(robot_logic::CacheManagerOp* global_mgr);


public:
#if defined (_DB_POOL_)
	static base_storage::DBStorageEngine* DBConnectionPop(void);
	static void DBConnectionPush(base_storage::DBStorageEngine* engine);
#endif
public:
	static std::list<base::ConnAddr>  addrlist_;
#if defined (_DB_POOL_)
	static std::list<base_storage::DBStorageEngine*>  db_conn_pool_;
	static threadrw_t*                                db_pool_lock_;
#endif
};

class AutoDBCommEngine{
public:
	AutoDBCommEngine();
	virtual ~AutoDBCommEngine();
	base_storage::DBStorageEngine*  GetDBEngine(){
		if(engine_){
			engine_->Release();//释放多余记录集
			if(!engine_->CheckConnect()){//失去连接重新连接
				//重新创建连接
				LOG_DEBUG("lost connection");
				if(!engine_->Connections(DBComm::addrlist_))
					return NULL;
			}
			return engine_;
		}
		return engine_;
	}
private:
	base_storage::DBStorageEngine*  engine_;
};


}
#endif
