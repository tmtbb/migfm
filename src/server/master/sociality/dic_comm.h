#ifndef _MASTER_PLUGIN_MUSIC_MGR_DIC_COMM_H__
#define _MASTER_PLUGIN_MUSIC_MGR_DIC_COMM_H__
#include "storage/storage.h"
#include "basic/basic_info.h"
#include "basic/radom_in.h"
#include <list>
#include <vector>
#include "json/json.h"

namespace storage{

class RedisComm{
public:
	RedisComm(){}
	virtual ~RedisComm(){}
	static base_storage::DictionaryStorageEngine *GetConnection();
public:
	static void Init(std::list<base::ConnAddr>& addrlist);
	static void Dest();

	static bool SetUserPushConfig(int64 uid, const std::string &device_token,
			int is_receive, unsigned begin_time, unsigned end_time);

	static bool GetUserPushConfig(int64 uid, std::string &device_token,
			bool &is_receive, unsigned &begin_time, unsigned &end_time);

	static bool GetMusicInfos(const std::string& songid, std::string &music_infos);

	static bool GenaratePushMsgID(int64 uid, int64 &msg_id);

	static bool StagePushMsg(int64 uid, int64 msg_id, const std::string &msg);

	static bool GetStagedPushMsg(int64 uid, int page_index, int page_size, std::list<std::string> &msgs);

	static bool AddFriend(int64 uid, int64 touid);

	static bool GetFriensList(int64 uid, std::list<std::string> &friends);

private:
	static std::list<base::ConnAddr>   addrlist_;
	static base::MigRadomIn*              radom_num_;
};

class MemComm{
public:
	MemComm(){}
	virtual ~MemComm(){}
	static void Init(std::list<base::ConnAddr>& addrlist);
	static void Dest();

	static bool SetString(const char* key,const size_t key_len,
		                  const char* data, size_t len);

	static bool GetString(const char* key,const size_t key_len,
		                  char** data,size_t* len);
private:
	static base_storage::DictionaryStorageEngine* engine_;
};

}
#endif