#ifndef _MASTER_PLUGIN_USR_MGR_LOGIC_COMM_H__
#define _MASTER_PLUGIN_USR_MGR_LOGIC_COMM_H__

#include "thread_handler.h"
#include "thread_lock.h"
#include "log/mig_log.h"
#include "storage/storage.h"

#if defined _DEBUG || defined DEBUG
#define LOG_DEBUG(s)        \
	MIG_DEBUG (USER_LEVEL, "%s", s)
#define LOG_DEBUG2(f, ...)  \
	MIG_DEBUG (USER_LEVEL, f, __VA_ARGS__)
#else
#define LOG_DEBUG(s)
#define LOG_DEBUG2(f, ...)
#endif

#if defined USR_LOG_ERROR
#define LOG_ERROR(s)        \
	MIG_ERROR (USER_LEVEL, "%s", s)
#define LOG_ERROR2(f, ...)  \
	MIG_ERROR (USER_LEVEL, f, __VA_ARGS__)
#else
#define LOG_ERROR(s)
#define LOG_ERROR2(f, ...)
#endif

#if defined USR_LOG_MSG
#define LOG_MSG(s)          \
	MIG_LOG (USER_LEVEL, "%s", s)
#define LOG_MSG2(f, ...)    \
	MIG_LOG (USER_LEVEL, f, __VA_ARGS__)
#else
#define LOG_MSG(s)
#define LOG_MSG2(f, ...)
#endif

#define  USERNAME           "username"
#define  PASSWORD           "password"
#define  NICKNAME           "nickname"
#define  SOURCE             "source"
#define  SESSION            "session"
#define  UID                "uid"
#define  SEX                "sex"
#define  GENDER             "gender"
#define  BIRTHDAY           "birthday"
#define  LOCATION           "location"
#define  HEAD               "head"


#define IDENTITY "migfm_identity"
#define USERINFOS "migfm_user_infos"

#define MIGLAB_PLAT   "0"
#define WEIBO_PLAT    "1"
#define WEIXIN_PLAT   "2"
#define QZONE_PLAT    "3"
#define DOUBAN_PALT   "4"



#define DEFAULT_CONFIG_PATH     "./plugins/user_manager/user_manager_config.xml"
namespace usr_logic{
class ThreadKey {
public:
	static void InitThreadKey ();
	static base_storage::DBStorageEngine *GetStorageDBConn ();
	static void SetStorageDBConn (base_storage::DBStorageEngine *conn);
	static base_storage::DictionaryStorageEngine* GetStorageDicConn ();
	static void SetStorageDicConn(base_storage::DictionaryStorageEngine *conn);
	static void DeinitThreadKey ();
	static threadkey_handler_t *db_key_;
	static threadkey_handler_t *dic_key_;
};

class SomeUtils{
public:
    static int SendFull (int socket, const char *buffer, size_t nbytes);

	static void GetResultMsg(std::string &status, std::string &msg,
		                     std::string &result,std::string &out_str,
		                     int32 flag = 0);

	static void GetCurrntTimeFormat(std::string& current_time);

private:
	static FILE *m_urandomfp;
};

class RLockGd
{
private:
	threadrw_t *m_lock;

public:
	RLockGd (threadrw_t *lock)
	{
		assert (lock);
		m_lock = lock;
		RlockThreadrw (lock);
	}

	virtual ~RLockGd ()
	{
		UnlockThreadrw (m_lock);
	}
};

class WLockGd
{
private:
	threadrw_t *m_lock;

public:
	WLockGd (threadrw_t *lock)
	{
		assert (lock);
		m_lock = lock;
		WlockThreadrw (lock);
	}

	virtual ~WLockGd ()
	{
		UnlockThreadrw (m_lock);
	}
};


}
#endif
