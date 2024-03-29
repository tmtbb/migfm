/*
 * soc_comm_head.h
 *
 *  Created on: 2014年12月10日
 *      Author: kerry
 */

#ifndef SOC_COMM_HEAD_H_
#define SOC_COMM_HEAD_H_
#include "net/comm_head.h"
#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include <list>
#include <string>
#include <sstream>

namespace netcomm_recv{
//获取弹幕和评论
class BarrageComm: public LoginHeadPacket{
public:
	BarrageComm(NetBase* m)
	:LoginHeadPacket(m)
	,platform_(0)
	,tid_(0)
	,msgid_(0)
	,from_(0)
	,count_(0){
		Init();
	}

	inline void Init(){
		bool r = false;
		GETBIGINTTOINT(L"platform",platform_);
		if(!r) error_code_ = PLATFORM_LACK;
		r = m_->GetString(L"ttype",&ttype_);
		if(!r) error_code_ = MUSIC_TYPE_LACK;
		GETBIGINTTOINT(L"tid",tid_);
		if(!r) error_code_ = MUSIC_TYPE_ID_LACK;
		GETBIGINTTOINT(L"msgid",msgid_);
		if(!r) error_code_ = MESSAGE_ID_LACK;
		GETBIGINTTOINT(L"from",from_);
		GETBIGINTTOINT(L"count",count_);
	}

	const inline int64 platform() const {return this->platform_;}
	const inline std::string& type() const {return this->ttype_;}
	const inline int64 tid() const {return this->tid_;}
	const inline int64 msgid() const {return this->msgid_;}
	const inline int64 from() const {return this->from_;}
	const inline int64 count() const {return this->count_==0?20:this->count_;}
private:
	int64       platform_;
	std::string ttype_;
	int64       tid_;
	int64       msgid_;
	int64       from_;
	int64       count_;
};

//获取本地相关信息
class LocationInfo:public LoginHeadPacket{
public:
	LocationInfo(NetBase* m)
	:LoginHeadPacket(m){}
};


//获取好友
class MyFriend:public LoginHeadPacket{
public:
	MyFriend(NetBase* m)
	:LoginHeadPacket(m){

		Init();
	}

	inline void Init(){
		bool r = false;
		GETBIGINTTOINT(L"from",from_);
		if(!r) from_ = 0;
		GETBIGINTTOINT(L"count",count_);
		if(!r) count_ = 5;
	}

	const int64 from() const {return this->from_;}
	const int64 count() const {return this->count_;}
private:
	int64 from_;
	int64 count_;
};

//打招呼
class SayHello:public LoginHeadPacket{
public:
	SayHello(NetBase* m)
	:LoginHeadPacket(m){
		 Init();
	}

	inline void Init(){
		bool r = false;
		GETBIGINTTOINT(L"touid",tid_);
		if(!r) error_code_ = TID_LACK;
		//
		int64 content = 0;
		r = m_->GetString(L"msg",&content_);
		//数字 //fixe me
		/*if(!r){
			GETBIGINTTOINT(L"msg",content);
			if(r)
				content_ = base::BasicUtil::Int64ToString(content);
			else
				error_code_ = MSG_CONTENT_LACK;
		}*/
	}

	const int64 tid() const {return this->tid_;}
	const std::string& content() const {return this->content_;}
private:
	int64          tid_;
	std::string    content_;
};

//送歌
class GivingSong:public LoginHeadPacket{
public:
	GivingSong(NetBase* m)
	:LoginHeadPacket(m){
		Init();
	}

	inline void Init(){
		bool r = false;
		GETBIGINTTOINT(L"touid",tid_);
		if(!r) error_code_ = TID_LACK;
		r = m_->GetString(L"msg",&content_);
		if(!r)error_code_ = MSG_CONTENT_LACK;
	}

	const std::string& content() const {return this->content_;}

	const int64 tid() const {return this->tid_;}
private:
	int64          tid_;
	std::string    content_;

};


}

namespace netcomm_send{
class BarrageComm:public HeadPacket{
public:
	BarrageComm(){
		barrage_com_.reset(new netcomm_send::NetBase());
		barrage_.reset(new base_logic::ListValue());
		comment_.reset(new base_logic::ListValue());
	}

	netcomm_send::NetBase* release(){
		if(!barrage_->empty())
			this->barrage_com_->Set(L"barrage",barrage_.release());
		if(!comment_->empty())
			this->barrage_com_->Set(L"comment",comment_.release());
		this->barrage_com_->Set(L"group_id",group_id_.release());
		head_->Set("result",barrage_com_.release());
		this->set_status(1);
		return head_.release();
	}
public:
	inline void set_barrage(const std::string& nickname,const int64 msgid,const std::string& type_name,
			const int64 type_id,const std::string& message){
		scoped_ptr<base_logic::DictionaryValue> barrage(new base_logic::DictionaryValue());
		barrage->SetString(L"nickname",nickname);
		barrage->SetBigInteger(L"msgid",msgid);
		barrage->SetString(L"type",type_name);
		barrage->SetBigInteger(L"tid",type_id);
		barrage->SetString(L"msg",message);
		barrage_->Append(barrage.release());
	}

	inline void set_group_id(const int64 group_id){
		group_id_.reset(new base_logic::FundamentalValue(group_id));
	}
private:
	scoped_ptr<netcomm_send::NetBase>             barrage_com_;
	scoped_ptr<base_logic::FundamentalValue>      group_id_;
	scoped_ptr<base_logic::ListValue>             barrage_;
	scoped_ptr<base_logic::ListValue>             comment_;
};

//发送地理相关信息

class Location:public HeadPacket{
public:
	Location(){
		location_.reset(new netcomm_send::NetBase());
	}

	netcomm_send::NetBase* release(){
		//this->location_->Set(L"weather",weather_.release());
		//this->location_->Set(L"city",city_.release());
		//this->location_->Set(L"temp",temp_.release());
		head_->Set("result",location_.release());
		this->set_status(1);
		return head_.release();
	}

	inline void set_weather(const std::string& weather){
		location_->SetString(L"weather",weather);
		//weather_.reset(new base_logic::StringValue(weather));
	}

	inline void set_temp(const std::string& temp){
		location_->SetString(L"temp",temp);
		//weather_.reset(new base_logic::StringValue(temp));
	}

	inline void set_city(const std::string& city){
		location_->SetString(L"city",city);
		//city_.reset(new base_logic::StringValue(city));
	}
private:
	scoped_ptr<netcomm_send::NetBase>               location_;
	//scoped_ptr<base_logic::StringValue>             weather_;
	//scoped_ptr<base_logic::StringValue>             temp_;
	//scoped_ptr<base_logic::StringValue>             city_;

};

//打招呼
class SayHello:public HeadPacket{
public:
	SayHello(){

	}
	netcomm_send::NetBase* release(){
		this->set_status(1);
		return head_.release();
	}
};

//送歌
class GivingSong:public HeadPacket{
public:
	GivingSong(){

	}

	netcomm_send::NetBase* release(){
		this->set_status(1);
		return head_.release();
	}
};


//我的好友
class MyFriend:public HeadPacket{
public:
	MyFriend(){
		base_.reset(new netcomm_send::NetBase());
		list_.reset(new base_logic::ListValue());
	}

	netcomm_send::NetBase* release(){
		if(!list_->empty())
			base_->Set(L"list",list_.release());

		head_->Set("result",base_.release());
		this->set_status(1);
		return head_.release();
	}

	void set_unit(base_logic::Value* info){
		list_->Append(info);
	}
private:
	scoped_ptr<netcomm_send::NetBase>         base_;
	scoped_ptr<base_logic::ListValue>         list_;
};
}




#endif /* SOC_COMM_HEAD_H_ */
