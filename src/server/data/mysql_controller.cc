/*
 * mysql_controller.cc
 *
 *  Created on: 2015年5月25日
 *      Author: Administrator
 */
#include "mysql_controller.h"
#include "db/base_db_mysql_auto.h"

namespace base_logic{


void MysqlController::InitParam(std::list<base::ConnAddr>& addrlist){

}

void MysqlController::Release(){

}


bool MysqlController::WriteData(const int32 type,base_logic::Value* value){
	return true;
}


bool MysqlController::ReadData(const int32 type,base_logic::Value* value,
		void (*storage_get)(void*,base_logic::Value*)){
	bool r = false;
	std::string sql;
	base_logic::DictionaryValue* dict =(base_logic::DictionaryValue*)(value);
	r = dict->GetString(L"sql",&sql);
	if(!r)
		return r;
	base_db::AutoMysqlCommEngine auto_engine;
	base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
	if (engine==NULL){
		LOG_ERROR("GetConnection Error");
		return false;
	}
	LOG_MSG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());

	if (!r) {
		LOG_ERROR("exec sql error");
		return false;
	}

	if(storage_get==NULL)
		return r;
	storage_get((void*)(engine),value);
	return r;
}


}



