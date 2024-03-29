/*
 * data_comm.cc
 *
 *  Created on: 2015年5月25日
 *      Author: kerry
 *  Time: 下午11:06:49
 *  Project: migfm
 *  Year: 2015
 *  Instruction：
 */

#include "data_mysql_engine.h"
#include "db/base_db_mysql_auto.h"
#include "basic/basic_util.h"
#include "logic/base_values.h"
#include <sstream>
#include <mysql.h>
namespace base_logic{


DataMysqlEngne::DataMysqlEngne(){
	mysql_engine_.reset(base_logic::DataControllerEngine::Create(MYSQL_TYPE));
}

DataMysqlEngne::~DataMysqlEngne(){

}

bool DataMysqlEngne::ReadUserInfo(const int64 uid,base_logic::UserInfo& info){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	std::string sql = "call proc_V2GetUserInfo("+base::BasicUtil::BasicUtil::StringUtil::Int64ToString(uid)+")";
	dict->SetString(L"sql",sql);
	mysql_engine_->ReadData(0,(base_logic::Value*)(dict.get()),CallBackReadUserInfo);
	return info.ValueSerialization(dict.get());
}

bool DataMysqlEngne::BatchReadUserInfos(std::vector<int64>& uid_list,std::map<int64,base_logic::UserInfo>& usrinfo){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	base_logic::ListValue* list;
	int32 i = 0;
	//call migfm.proc_V2BatchGetUserInfo('10149,10150,10301')
	std::string sql = "call proc_V2BatchGetUserInfo(\'";
	while(i<uid_list.size()){
		int64 uid = uid_list.front();
		sql = sql+base::BasicUtil::BasicUtil::StringUtil::Int64ToString(uid_list[i]);
		i++;
		if(i<uid_list.size())
			sql = sql+",";

	}
	sql = sql+"\');";
	dict->SetString(L"sql",sql);
	mysql_engine_->ReadData(0,(base_logic::Value*)(dict.get()),CallBackBatchReadUserInfo);
	dict->GetList(L"resultvalue",&list);
	while(list->GetSize()>0){
		base_logic::UserInfo info;
		base_logic::Value* result_value;
		list->Remove(0,&result_value);
		base_logic::DictionaryValue* dict_result_value = (base_logic::DictionaryValue*)(result_value);
		info.ValueSerialization(dict_result_value);
		usrinfo[info.uid()] = info;
	}

	return true;
}

bool DataMysqlEngne::GetDimensions(const std::string& type,base_logic::Dimensions& dimensions){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	//call migfm.proc_V2GetDimensions('chl')
	std::string sql = "call migfm.proc_V2GetDimensions(\'"+type+"\');";
	base_logic::ListValue* list;
	dict->SetString(L"sql",sql);
	mysql_engine_->ReadData(0,(base_logic::Value*)(dict.get()),CallBackGetDimensions);
	dict->GetList(L"resultvalue",&list);
	while(list->GetSize()>0){
		base_logic::Dimension dimension;
		base_logic::Value* result_value;
		list->Remove(0,&result_value);
		base_logic::DictionaryValue* dict_result_value = (base_logic::DictionaryValue*)(result_value);
		dimension.ValueSerialization(dict_result_value);
		dimensions.set_dimension(dimension.id(),dimension);
	}
	return true;
}

bool DataMysqlEngne::GetAvailableMusicInfo(std::map<int64,base_logic::MusicInfo>& map){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	//call proc_V2GetAvailableMusicInfos()
	std::string sql = "call proc_V2GetAvailableMusicInfos()";
	base_logic::ListValue* list;
	dict->SetString(L"sql",sql);
	mysql_engine_->ReadData(0,(base_logic::Value*)(dict.get()),CallBackGetAvailableMusicInfo);
	dict->GetList(L"resultvalue",&list);

	while(list->GetSize()>0){
		base_logic::MusicInfo musicinfo;
		base_logic::Value* result_value;
		list->Remove(0,&result_value);
		base_logic::DictionaryValue* dict_result_value = (base_logic::DictionaryValue*)(result_value);
		musicinfo.ValueSerialization(dict_result_value);
		if(musicinfo.Isvalid())
			map[musicinfo.id()] = musicinfo;

	}
	return true;
}


bool DataMysqlEngne::GetAllDimension(std::list<base_logic::Dimension>& list){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	//call proc_V2GetAvailableMusicInfos()
	std::string sql = "call proc_V2GetAvailableMusicInfos()";
	base_logic::ListValue* listvalue;
	dict->SetString(L"sql",sql);
	mysql_engine_->ReadData(0,(base_logic::Value*)(dict.get()),CallBackGetAllDimension);
	dict->GetList(L"resultvalue",&listvalue);
	while(listvalue->GetSize()){
		base_logic::Dimension dimension;
		base_logic::Value* result_value;
		listvalue->Remove(0,&result_value);
		base_logic::DictionaryValue* dict_result_value = (base_logic::DictionaryValue*)(result_value);
		dimension.ValueSerialization(dict_result_value);
		list.push_back(dimension);
	}
	return true;
}
/////////////////////callback////////////////////////////////
void DataMysqlEngne::CallBackBatchReadUserInfo(void* param,base_logic::Value* value){
	base_logic::DictionaryValue* dict = (base_logic::DictionaryValue*)(value);
	base_logic::ListValue* list = new base_logic::ListValue();
	base_storage::DBStorageEngine* engine  = (base_storage::DBStorageEngine*)(param);
	MYSQL_ROW rows;
	int num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			base_logic::DictionaryValue* info_value = new base_logic::DictionaryValue();
			if(rows[0]!=NULL)
				info_value->SetBigInteger(L"uid",atoll(rows[0]));
			if(rows[1]!=NULL)
				info_value->SetInteger(L"sex",atol(rows[1]));
			if(rows[2]!=NULL)
				info_value->SetInteger(L"type",atol(rows[2]));
			if(rows[3]!=NULL)
				info_value->SetString(L"nickname",std::string(rows[3]));
			if(rows[4]!=NULL)
				info_value->SetInteger(L"source",atol(rows[4]));
			if(rows[5]!=NULL)
				info_value->SetInteger(L"machine",atol(rows[5]));
			if(rows[6]!=NULL)
				info_value->SetString(L"city",std::string(rows[6]));
			if(rows[7]!=NULL)
				info_value->SetString(L"birthday",std::string(rows[7]));
			if(rows[8]!=NULL)
				info_value->SetBigInteger(L"logintime",atoll(rows[8]));
			if(rows[9]!=NULL)
				info_value->SetString(L"head",std::string(rows[9]));
			list->Append((base_logic::Value*)(info_value));
		}
	}
	dict->Set(L"resultvalue",(base_logic::Value*)(list));
}

void DataMysqlEngne::CallBackReadUserInfo(void* param,base_logic::Value* value){
	base_logic::DictionaryValue* dict = (base_logic::DictionaryValue*)(value);
	base_storage::DBStorageEngine* engine  = (base_storage::DBStorageEngine*)(param);
	MYSQL_ROW rows;
	int num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			if(rows[0]!=NULL)
				dict->SetBigInteger(L"uid",atoll(rows[0]));
			if(rows[1]!=NULL)
				dict->SetInteger(L"sex",atol(rows[1]));
			if(rows[2]!=NULL)
				dict->SetInteger(L"type",atol(rows[2]));
			if(rows[3]!=NULL)
				dict->SetString(L"nickname",std::string(rows[3]));
			if(rows[4]!=NULL)
				dict->SetInteger(L"source",atol(rows[4]));
			if(rows[5]!=NULL)
				dict->SetInteger(L"machine",atol(rows[5]));
			if(rows[6]!=NULL)
				dict->SetString(L"city",std::string(rows[6]));
			if(rows[7]!=NULL)
				dict->SetString(L"birthday",std::string(rows[7]));
			if(rows[8]!=NULL)
				dict->SetBigInteger(L"logintime",atoll(rows[8]));
			if(rows[9]!=NULL)
				dict->SetString(L"head",std::string(rows[9]));
		}
		return;
	}
}

void DataMysqlEngne::CallBackGetDimensions(void* param,base_logic::Value* value){
	base_logic::DictionaryValue* dict = (base_logic::DictionaryValue*)(value);
	base_logic::ListValue* list = new base_logic::ListValue();
	base_storage::DBStorageEngine* engine  = (base_storage::DBStorageEngine*)(param);
	MYSQL_ROW rows;
	int num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			base_logic::DictionaryValue* info_value = new base_logic::DictionaryValue();
			if(rows[0]!=NULL)
				info_value->SetInteger(L"id",atol(rows[0]));
			if(rows[1]!=NULL)
				info_value->SetString(L"name",std::string(rows[1]));

			list->Append((base_logic::Value*)(info_value));
		}
	}

	dict->Set(L"resultvalue",(base_logic::Value*)(list));
}

void DataMysqlEngne::CallBackGetAvailableMusicInfo(void* param,base_logic::Value* value){
	base_logic::DictionaryValue* dict = (base_logic::DictionaryValue*)(value);
	base_logic::ListValue* list = new base_logic::ListValue();
	base_storage::DBStorageEngine* engine  = (base_storage::DBStorageEngine*)(param);
	MYSQL_ROW rows;
	int num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			base_logic::DictionaryValue* info_value = new base_logic::DictionaryValue();
			if(rows[0]!=NULL){
				dict->SetBigInteger(L"id",atoll(rows[0]));
				dict->SetBigInteger(L"songid",atoll(rows[0]));
			}
			if(rows[1]!=NULL)
				dict->SetBigInteger(L"hot",atoll(rows[1]));
			if(rows[2]!=NULL)
				dict->SetBigInteger(L"clt",atoll(rows[2]));
			if(rows[3]!=NULL)
				dict->SetBigInteger(L"cmt",atoll(rows[3]));
			if(rows[4]!=NULL){
				dict->SetString(L"title",std::string(rows[4]));
				dict->SetString(L"name",std::string(rows[4]));
			}
			if(rows[5]!=NULL)
				dict->SetString(L"album",std::string(rows[5]));
			if(rows[6]!=NULL)
				dict->SetString(L"pubtime",std::string(rows[6]));
			if(rows[7]!=NULL)
				dict->SetString(L"artist",std::string(rows[7]));
			if(rows[8]!=NULL){
				dict->SetString(L"url",std::string(rows[8]));
				dict->SetString(L"hqurl",std::string(rows[8]));
			}
			if(rows[9]!=NULL)
				dict->SetString(L"pic",std::string(rows[9]));
			if(rows[10]!=NULL)
				dict->SetString(L"lyric",std::string(rows[10]));

			list->Append((base_logic::Value*)(info_value));
		}
	}

	dict->Set(L"resultvalue",(base_logic::Value*)(list));
}

void DataMysqlEngne::CallBackGetAllDimension(void* param,base_logic::Value* value){
	base_logic::DictionaryValue* dict = (base_logic::DictionaryValue*)(value);
	base_logic::ListValue* list = new base_logic::ListValue();
	base_storage::DBStorageEngine* engine  = (base_storage::DBStorageEngine*)(param);
	MYSQL_ROW rows;
	int num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			base_logic::DictionaryValue* info_value = new base_logic::DictionaryValue();
			if(rows[0]!=NULL)
				info_value->SetBigInteger(L"id",atoll(rows[0]));
			if(rows[1]!=NULL)
				info_value->SetString(L"class_name",std::string(rows[1]));
			if(rows[2]!=NULL)
				info_value->SetString(L"name",std::string(rows[2]));
			if(rows[3]!=NULL)
				info_value->SetString(L"desc",std::string(rows[3]));

			list->Append((base_logic::Value*)(info_value));
		}
	}

	dict->Set(L"resultvalue",(base_logic::Value*)(list));
}


/*
DataDBComm::DataDBComm(){

}

DataDBComm::~DataDBComm(){

}

void DataDBComm::Init(std::list<base::ConnAddr>& addrlist){
#if defined (_DB_POOL_)
	base_db::MysqlDBPool::Init(addrlist);
#endif
}

void DataDBComm::Dest(){
#if defined (_DB_POOL_)
	base_db::MysqlDBPool::Dest();
#endif
}

bool DataDBComm::GetUserInfo(const int64 uid,base_logic::UserInfo& info){
	bool r = false;
#if defined (_DB_POOL_)
	base_db::AutoMysqlCommEngine auto_engine;
	base_storage::DBStorageEngine* engine  = auto_engine.GetDBEngine();
#endif
	std::stringstream os;
	MYSQL_ROW rows;

	if (engine==NULL){
		LOG_ERROR("GetConnection Error");
		return false;
	}

    //call migfm.proc_V2GetUserInfo(10108)
	os<<"call proc_V2GetUserInfo("<<uid<<")";
	std::string sql = os.str();
	LOG_MSG2("[%s]", sql.c_str());
	r = engine->SQLExec(sql.c_str());

	if (!r) {
		LOG_ERROR("exec sql error");
		return false;
	}


	int num = engine->RecordCount();
	if(num>0){
		while(rows = (*(MYSQL_ROW*)(engine->FetchRows())->proc)){
			if(rows[0]!=NULL)
				info.set_uid(atoll(rows[0]));
			if(rows[1]!=NULL)
				info.set_sex(atol(rows[1]));
			if(rows[2]!=NULL)
				info.set_type(atol(rows[2]));
			if(rows[3]!=NULL)
				info.set_nickname(rows[3]);
			if(rows[4]!=NULL)
				info.set_source(atol(rows[4]));
			if(rows[5]!=NULL)
				info.set_machine(atol(rows[5]));
			if(rows[6]!=NULL)
				info.set_city(rows[6]);
			if(rows[7]!=NULL)
				info.set_birthday(rows[7]);
			if(rows[8]!=NULL)
				info.set_logintime(atoll(rows[8]));
			if(rows[9]!=NULL)
				info.set_head(rows[9]);
		}
		return true;
	}
	return false;
}*/


}



