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

#define DEFAULT_CONFIG_PATH		"./plugins/lbs/config.xml"
#define DEFAULT_URL "http://42.121.14.108/wx/lp5.mp3"
#define DEFAULT_MAX_RADIUS "2000000"

namespace mig_lbs {

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
		                     int32 flag = 1);

	static bool InitRandom ();

	static int GetRandomID ();

	static bool DeinitRandom ();

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
#endif // mig_lbs
