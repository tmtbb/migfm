#include "mysql_storage_impl.h"
#include "log/mig_log.h"
namespace base_storage{

MysqlStorageEngineImpl::MysqlStorageEngineImpl(){
    
    conn_.reset(new db_conn_t);
    result_.reset(new db_res_t);
    rows_.reset(new db_row_t);
    conn_.get()->proc = result_.get()->proc = rows_.get()->proc = NULL;
}

MysqlStorageEngineImpl::~MysqlStorageEngineImpl(){
	Release();
}
	
bool MysqlStorageEngineImpl::Connections(std::list<base::ConnAddr>& addrlist){
    connected_ = false;
    base::ConnAddr addr;
    MYSQL* mysql = NULL;
    mysql = mysql_init((MYSQL*)0);
    mysql_options(mysql,MYSQL_SET_CHARSET_NAME,"utf8");
    std::list<base::ConnAddr>::iterator it = addrlist.begin();
    while(it!=addrlist.end()){
        addr = (*it);
    	if(mysql_real_connect(mysql,addr.host().c_str(),
    		                  addr.usr().c_str(),addr.pwd().c_str(),
    		                  addr.source().c_str(),addr.port(),0,
    		                  /*CLIENT_INTERACTIVE*/CLIENT_MULTI_STATEMENTS)==NULL){
            MIG_ERROR(USER_LEVEL,"mysql:connection to database failed failed %s",
                      mysql_error(mysql));
    		return false;
    	}
        
        //mysql_set_character_set(&mysql,"gbk");
        MIG_INFO(USER_LEVEL,"mysql ip[%s] port[%d] user[%s] pwd[%s] db[%s]",
                addr.host().c_str(),addr.port(),addr.usr().c_str(),
                addr.pwd().c_str(),addr.source().c_str());
    	break;
    }
    mysql_set_server_option(mysql, MYSQL_OPTION_MULTI_STATEMENTS_ON);
    mysql_query(mysql,"SET NAMES 'binary'");
    connected_ = true;
    conn_.get()->proc = mysql;
    return true;
}

bool MysqlStorageEngineImpl::Release(){

	FreeRes();
    return true;
}

bool MysqlStorageEngineImpl::FreeRes(){
    MYSQL_RES * result = (MYSQL_RES *)result_.get()->proc;
    MYSQL* mysql = (MYSQL*)conn_.get()->proc;
	do{
		result = mysql_store_result(mysql);
		mysql_free_result(result);
		result_.get()->proc = NULL;
	}while(!mysql_next_result(mysql));
    return true;
}

bool MysqlStorageEngineImpl::SQLExec(const char* sql){
	bool r = CheckConnect();
	if(!r){
		MIG_ERROR(USER_LEVEL,"lost connection");
		return false;
	}
    FreeRes();
    MYSQL* mysql = (MYSQL*)conn_.get()->proc;
    mysql_query(mysql,sql);
    result_.get()->proc = mysql_store_result(mysql);
	if (mysql->net.last_errno!=0){
		MIG_ERROR(USER_LEVEL,"mysql error code [%d] [%s]",
			mysql_errno(mysql),mysql_error(mysql));
	}
    return true;
}

uint32 MysqlStorageEngineImpl::RecordCount(){
    //unsigned long ulCount = (unsigned long)mysql_num_rows((MYSQL_RES *)(result_.get()->proc));
	unsigned long ulCount = (unsigned long)mysql_affected_rows((MYSQL*)conn_.get()->proc);
	MIG_DEBUG(USER_LEVEL,"ulconut ==%d===\n",ulCount);
	return ulCount;
}
bool MysqlStorageEngineImpl::Affected(unsigned long& rows){
	rows = (unsigned long)mysql_affected_rows((MYSQL*)conn_.get()->proc);
	return true;
}

db_row_t* MysqlStorageEngineImpl::FetchRows(){
	MYSQL_RES* result = (MYSQL_RES*)(result_.get()->proc);
	row_ = mysql_fetch_row(result);
    rows_.get()->proc = &row_;
	return rows_.get();
}

bool MysqlStorageEngineImpl::CheckConnect(void){
	MYSQL* mysql = (MYSQL*)conn_.get()->proc;
    return (mysql_ping(mysql)==0);
}


}
