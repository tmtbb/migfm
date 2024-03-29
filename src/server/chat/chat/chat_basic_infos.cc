#include "chat_basic_infos.h"
#include "basic/constants.h"
#include "json/json.h"
#include "log/mig_log.h"
namespace chat_base{


GroupInfo::GroupInfo(){
	data_ = new Data();
}

GroupInfo::GroupInfo(const int64 platform_id,const int64 group_id,const int16 group_type,
		const int64 nicknumber,const int64 session,const std::string& name,
		const std::string& head_url){

	data_ = new Data(platform_id,group_id,group_type,nicknumber,session,name,head_url);
}

GroupInfo::GroupInfo(const GroupInfo& group_info)
:data_(group_info.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

GroupInfo& GroupInfo::operator =(const GroupInfo& group_info){

	if (group_info.data_!=NULL){
		group_info.data_->AddRef();
	}
	if (data_!=NULL){
		data_->Release();
	}
	data_ = group_info.data_;
	return *this;
}


UserInfo::UserInfo(){
	data_ = new Data();
}

UserInfo::UserInfo(const int64 platform_id,const int64 user_id, 
				   const int64 nicknumber,const int socket,const int64 session,
				   const std::string& nickname,const std::string& head_url, 
				   const std::string& platform_token, 
				   const std::string& chat_token){
    data_ = new Data(platform_id,user_id,nicknumber,socket,session,nickname,
					head_url,platform_token,chat_token);
}

UserInfo::UserInfo(const int64 platform_id,const int64 user_id,
					const int64 nicknumber,const std::string& nickname,
					const std::string& head_url){
	data_ = new Data(platform_id,user_id,nicknumber,nickname,head_url);
}

UserInfo::UserInfo(const UserInfo& user_info)
:data_(user_info.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

UserInfo& UserInfo::operator =(const UserInfo& user_info){

	if (user_info.data_!=NULL){
		user_info.data_->AddRef();
	}
	if (data_!=NULL){
		data_->Release();
	}
	data_ = user_info.data_;
	return *this;
}


bool UserInfo::UnserializedJson(const char* str){
	Json::Reader reader;
	Json::Value root;
	bool r = false;
	r = reader.parse(str,root);
	if (!r){
		MIG_ERROR(USER_LEVEL,"json parser error");
		return false;
	}

	int64 platform_id = 0;
	int64 userid = atoll(root["uid"].asString().c_str());
	int64 nicknumber = userid;
	std::string nickname = root["nickname"].asString();
	std::string head_url = root["head"].asString();
	chat_base::UserInfo usrinfo(platform_id,userid,nicknumber,nickname,
								head_url);
	*this = usrinfo;
	return true;
}

PlatformInfo::PlatformInfo(){
    data_ = new Data();
}

PlatformInfo::PlatformInfo(const int64 platform_id,
						   const std::string& platform_name){
	data_ = new Data(platform_id,platform_name);
}

PlatformInfo::PlatformInfo(const PlatformInfo& platform_info)
:data_(platform_info.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

PlatformInfo& PlatformInfo::operator =(const PlatformInfo& platfrom_info){
	
	if (platfrom_info.data_!=NULL){
		platfrom_info.data_->AddRef();
	}
	if (data_!=NULL){
		data_->Release();
	}
	data_ = platfrom_info.data_;
	return *this;
}


MessageInfos::MessageInfos(){
	data_ = new Data();
}

MessageInfos::MessageInfos(const int64 platform,const int64 msgid,const int64 fid,const int64 oppid,
		const int32 type,const std::string& nickname,const std::string& message,
		const std::string& lasstime,const std::string& head){
	data_ = new Data(platform,msgid,fid,oppid,type,nickname,message,
			lasstime,head);
}


MessageInfos::MessageInfos(const MessageInfos& msginfo)
:data_(msginfo.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

MessageInfos& MessageInfos::operator = (const MessageInfos& msginfo){
	if(msginfo.data_!=NULL){
		msginfo.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}
	data_ = msginfo.data_;
	return (*this);
}

}
