#ifndef _MASTER_PLUGIN_CHAT_MGR_DB_COMM__H__
#define _MASTER_PLUGIN_CHAT_MGR_DB_COMM__H__
#include "thread_handler.h"
#include "thread_lock.h"
#include "logic_comm.h"
#include "storage/storage.h"
#include "basic/basictypes.h"
#include "basic/basic_info.h"
#include <list>
namespace chat_storage{

class DBComm{
public:
	DBComm();
	virtual ~DBComm();
	static base_storage::DBStorageEngine *GetConnection();
public:
	static void Init(std::list<base::ConnAddr>& addrlist,const int32 db_conn_num = 5);

	static void Dest();

public:
	static bool GetLeaveMessage(const int64 platform_id,const int64 uid,const int64 oppid,
			 const int32 from,const int32 count,const int64 msgid,std::list<struct GetLeaveMessage*>& list);
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
