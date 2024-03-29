/*
 * logic_infos.cc
 *
 *  Created on: 2014年12月2日
 *      Author: kerry
 */
#include "logic_infos.h"
#include "logic_unit.h"
#include "lbs/lbs_logic_unit.h"
#include "basic/base64.h"

namespace base_logic{

UserInfo::UserInfo(){
	data_ = new Data();
}


UserInfo::UserInfo(const UserInfo& userinfo)
:data_(userinfo.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

UserInfo& UserInfo::operator =(const UserInfo& userinfo){
	if(userinfo.data_!=NULL){
		userinfo.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}
	data_ = userinfo.data_;
	return (*this);
}

bool UserInfo::cmptime(UserInfo& t_info,UserInfo& r_info){
	return t_info.logintime()<r_info.logintime();
}

bool UserInfo::JsonDeSerialization(std::string& info){
	bool r  = false;
	int error_code;
	std::string error_str;
	scoped_ptr<base_logic::ValueSerializer> serializer(base_logic::ValueSerializer::Create(base_logic::IMPL_JSON,&info));
	base_logic::DictionaryValue* dic =  (base_logic::DictionaryValue*)(serializer->Deserialize(&error_code,&error_str));
	int64 uid;
	r = dic->GetBigInteger(L"uid",&data_->uid_);
	r = dic->GetInteger(L"sex",&data_->sex_);
	r = dic->GetInteger(L"machine",&data_->machine_);
	r = dic->GetBigInteger(L"loginttime",&data_->logintime_);
	r = dic->GetInteger(L"type",&data_->type_);
	r = dic->GetInteger(L"source",&data_->source_);

	r = dic->GetString(L"session",&data_->session_);
	r = dic->GetString(L"imei",&data_->imei_);
	r = dic->GetString(L"nickname",&data_->nickname_);
	r = dic->GetString(L"city",&data_->city_);
	r = dic->GetString(L"head",&data_->head_);
	r = dic->GetString(L"birthday",&data_->birthday_);
	r = dic->GetString(L"location",&data_->location_);
	r = dic->GetString(L"token",&data_->token_);
	return true;
}

bool UserInfo::ValueSerialization(base_logic::DictionaryValue* dict){
	bool r = false;
	r = dict->GetBigInteger(L"uid",&data_->uid_);
	r = dict->GetInteger(L"sex",&data_->sex_);
	r = dict->GetInteger(L"machine",&data_->machine_);
	r = dict->GetBigInteger(L"loginttime",&data_->logintime_);
	r = dict->GetInteger(L"type",&data_->type_);
	r = dict->GetInteger(L"source",&data_->source_);

	r = dict->GetString(L"session",&data_->session_);
	r = dict->GetString(L"imei",&data_->imei_);
	r = dict->GetString(L"nickname",&data_->nickname_);
	r = dict->GetString(L"city",&data_->city_);
	r = dict->GetString(L"head",&data_->head_);
	r = dict->GetString(L"birthday",&data_->birthday_);
	r = dict->GetString(L"location",&data_->location_);
	r = dict->GetString(L"token",&data_->token_);

	return true;
}

bool UserInfo::JsonSerialization(std::string& info){
	scoped_ptr<base_logic::ValueSerializer> serializer(base_logic::ValueSerializer::Create(base_logic::IMPL_JSON,&info));
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	if(data_->uid_!=0)
		dict->SetBigInteger(L"uid",data_->uid_);
	if(data_->sex_!=0)
		dict->SetInteger(L"sex",data_->sex_);
	if(data_->machine_!=-1)
		dict->SetInteger(L"machine",data_->machine_);
	if(data_->logintime_!=0)
		dict->SetBigInteger(L"logintime",data_->logintime_);
	if(data_->type_!=0)
		dict->SetInteger(L"type",data_->type_);
	if(data_->source_!=0)
		dict->SetInteger(L"source",data_->source_);

	if(!data_->session_.empty())
		dict->SetString(L"session",data_->session_);
	if(!data_->imei_.empty())
		dict->SetString(L"imei",data_->imei_);
	if(!data_->nickname_.empty())
		dict->SetString(L"nickname",data_->nickname_);
	if(!data_->city_.empty())
		dict->SetString(L"city",data_->city_);
	if(!data_->head_.empty())
		dict->SetString(L"head",data_->head_);
	if(!data_->birthday_.empty())
		dict->SetString(L"birthday",data_->birthday_);
	if(!data_->location_.empty())
		dict->SetString(L"location",data_->location_);

	base_logic::Value* value = (base_logic::Value*)dict.get();
	return serializer->Serialize(*value);;
}

base_logic::DictionaryValue* UserInfo::Release(){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	if(data_->uid_!=0)
		dict->SetBigInteger(L"uid",data_->uid_);
#if defined(__OLD_VERSION__)
		dict->SetString(L"userid",base::BasicUtil::StringUtil::Int64ToString(data_->uid_));
#endif
	if(data_->sex_!=-1){
#if defined(__OLD_VERSION__)
		int64 i64sex = data_->sex_;
		dict->SetString(L"sex",base::BasicUtil::StringUtil::Int64ToString(i64sex));
#else
		dict->SetBigInteger(L"sex",data_->sex_);
#endif
		dict->SetInteger(L"gender",data_->sex_);
	}
	if(data_->source_!=0){
#if defined(__OLD_VERSION__)
		int64 i64souce = data_->source_;
		dict->SetString(L"source",base::BasicUtil::StringUtil::Int64ToString(i64souce));
#else
		dict->SetInteger(L"source",data_->source_);
#endif
		dict->SetInteger(L"plat",data_->source_);
	}
	if(data_->machine_!=0)
		dict->SetInteger(L"machine",data_->machine_);
	if(data_->type_!=0)
		dict->SetInteger(L"type",data_->type_);
	if(!data_->session_.empty())
		dict->SetString(L"session",data_->session_);
	if(!data_->imei_.empty())
		dict->SetString(L"imei",data_->imei_);
	if(!data_->nickname_.empty())
		dict->SetString(L"nickname",data_->nickname_);
	if(!data_->city_.empty()){
		dict->SetString(L"city",data_->city_);
		dict->SetString(L"location",data_->city_);
	}
	if(!data_->head_.empty())
		dict->SetString(L"head",data_->head_);
	if(!data_->birthday_.empty())
		dict->SetString(L"birthday",data_->birthday_);
	if(!data_->location_.empty())
		dict->SetString(L"loaction",data_->location_);
	if(!data_->token_.empty())
		dict->SetString(L"token",data_->token_);
	return dict.release();
}

//音乐
MusicInfo::MusicInfo(){
	data_ = new Data();
}

MusicInfo::MusicInfo(const MusicInfo& music_info)
:data_(music_info.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

MusicInfo& MusicInfo::operator =(const MusicInfo& music_info){
	if(music_info.data_!=NULL){
		music_info.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}
	data_ = music_info.data_;
	return (*this);
}

MusicInfo& MusicInfo::BaseCopy(const MusicInfo& music_info){
	//type(class_name)和typeid(class)具有特殊性，故不拷贝
	this->set_id(music_info.id());
	this->set_clt(music_info.clt());
	this->set_hot(music_info.hot());
	this->set_like(music_info.like());
	data_->ablum_ = music_info.ablum();
	data_->artist_ = music_info.artist();
	data_->title_ = music_info.title();
	this->set_hq_url(music_info.hq_url());
	this->set_pic(music_info.pic());
	this->set_url(music_info.url());
	this->set_lyric(music_info.lyric());
	return (*this);
}
void MusicInfo::set_ablum(const std::string& ablum){
#if defined(__BASE64_MUSIC__)
	std::string b64ablum;
	Base64Decode(ablum,&b64ablum);
	data_->ablum_ = b64ablum;
#else
	data_->ablum_ = ablum;
#endif
}

void MusicInfo::set_artist(const std::string& artist){
#if defined(__BASE64_MUSIC__)
	std::string b64artist;
	Base64Decode(artist,&b64artist);
	data_->artist_ = b64artist;
#else
	data_->artist_ = artist;
#endif
}

void MusicInfo::set_title(const std::string& title){
#if defined(__BASE64_MUSIC__)
	std::string b64title;
	Base64Decode(title,&b64title);
	data_->title_ = b64title;
#else
	data_->title_ = title;
#endif
}

bool MusicInfo::ValueSerialization(base_logic::DictionaryValue* dict){
	bool r = false;
	r = dict->GetBigInteger(L"id",&data_->id_);
	r = dict->GetBigInteger(L"hot",&data_->hot_);
	r = dict->GetBigInteger(L"clt",&data_->clt_);
	r = dict->GetBigInteger(L"cmt",&data_->cmt_);
	r = dict->GetString(L"title",&data_->title_);
	r = dict->GetString(L"album",&data_->ablum_);
	r = dict->GetString(L"pubtime",&data_->pubtime_);
	r = dict->GetString(L"artist",&data_->artist_);
	r = dict->GetString(L"url",&data_->url_);
	r = dict->GetString(L"hqurl",&data_->hq_url_);
	r = dict->GetString(L"pic",&data_->pic_);
	r = dict->GetString(L"lyric",&data_->lyric_);
	return true;
}


void MusicInfo::JsonSeralize(std::string& str){
	bool r  = false;
	int error_code;
	std::string error_str;
	scoped_ptr<base_logic::ValueSerializer> serializer(base_logic::ValueSerializer::Create(base_logic::IMPL_JSON,&str));
	base_logic::DictionaryValue* dic =  (base_logic::DictionaryValue*)(serializer->Deserialize(&error_code,&error_str));
	std::string str_class;
	std::string str_class_name;
	std::string str_songid;
	int64 songid;
	//老版本中songid用的十字符串估计需要兼容新版和老版本
	r = dic->GetString(L"songid",&str_songid);
	if(!r)
		r = dic->GetBigInteger(L"songid",&songid);
	else
		songid = atoll(str_songid.c_str());
	r = dic->GetString(L"type",&str_class_name);
	r = dic->GetString(L"typeid",&str_class);
	this->set_id(songid);
	this->set_class_name(str_class_name);
	this->set_class(atoll(str_class.c_str()));
}

void MusicInfo::JsonDeserialize(std::string& str,int32 dimenon){
	scoped_ptr<base_logic::ValueSerializer> serializer(base_logic::ValueSerializer::Create(base_logic::IMPL_JSON,&str));
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	if(data_->id_!=0){
		dict->SetBigInteger(L"id",data_->id_);
		dict->SetBigInteger(L"songid",data_->id_);
	}
	if(data_->clt_!=-1)
		dict->SetBigInteger(L"clt",data_->clt_);
	if(data_->cmt_!=-1)
		dict->SetBigInteger(L"cmt",data_->cmt_);
	if(data_->hot_!=-1)
		dict->SetBigInteger(L"hot",data_->hot_);
	if(data_->like_!=-1)
		dict->SetInteger(L"like",data_->like_);
	if(data_->class_!=-1)
		dict->SetInteger(L"tid",data_->class_);
	if(!data_->class_name_.empty())
		dict->SetString(L"type",data_->class_name_);
	if(!data_->ablum_.empty())
		dict->SetString(L"album",data_->ablum_);
	if(!data_->artist_.empty())
		dict->SetString(L"artist",data_->artist_);
	if(!data_->title_.empty()){
		dict->SetString(L"title",data_->title_);
		dict->SetString(L"name",data_->title_);
#if defined(__OLD_VERSION__)
		dict->SetString(L"titile",data_->title_);
#endif
	}
	if(!data_->hq_url_.empty())
		dict->SetString(L"hq_url",data_->hq_url_);
	if(!data_->pic_.empty())
		dict->SetString(L"pic",data_->pic_);
	if(!data_->pubtime_.empty())
		dict->SetString(L"pubtime",data_->pubtime_);
	if(!data_->url_.empty())
		dict->SetString(L"url",data_->url_);
	if(dimenon==1){//允许设置类别
#if defined(__OLD_VERSION__)
	if(data_->class_!=-1)
		dict->SetBigInteger(L"typeid",data_->class_);
	if(!data_->class_name_.empty())
		dict->SetString(L"type",data_->class_name_);
#endif
	}

	base_logic::Value* value = (base_logic::Value*)dict.get();
	serializer->Serialize(*value);

}



base_logic::DictionaryValue* MusicInfo::Release(bool all){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());

	//若是全获即all为true 取则URL 若为空则不添加
	if(all&&(data_->hq_url_.empty()||data_->url_.empty()))
		return NULL;

	if(data_->id_!=0)
		dict->SetBigInteger(L"id",data_->id_);
	if(data_->clt_!=-1)
		dict->SetBigInteger(L"clt",data_->clt_);
	if(data_->cmt_!=-1)
		dict->SetBigInteger(L"cmt",data_->cmt_);
	if(data_->hot_!=-1)
		dict->SetBigInteger(L"hot",data_->hot_);
	if(data_->like_!=-1)
		dict->SetInteger(L"like",data_->like_);
	else
		dict->SetInteger(L"like",0);
	if(data_->class_!=-1)
		dict->SetInteger(L"tid",data_->class_);
	if(!data_->class_name_.empty())
		dict->SetString(L"type",data_->class_name_);
	if(!data_->ablum_.empty())
		dict->SetString(L"ablum",data_->ablum_);
	if(!data_->artist_.empty())
		dict->SetString(L"artist",data_->artist_);
	if(!data_->title_.empty()){
		dict->SetString(L"title",data_->title_);
		dict->SetString(L"name",data_->title_);
#if defined(__OLD_VERSION__)
		dict->SetString(L"titile",data_->title_);
#endif
	}

	if(!data_->hq_url_.empty())
		dict->SetString(L"hq_url",data_->hq_url_);
	if(!data_->pic_.empty())
		dict->SetString(L"pic",data_->pic_);

	if(!data_->pubtime_.empty())
		dict->SetString(L"pubtime",data_->pubtime_);
	if(!data_->url_.empty())
		dict->SetString(L"url",data_->url_);

	//返回
	return dict.release();
}



LBSInfos::LBSInfos(){
	data_ = new Data();
}

LBSInfos::LBSInfos(const std::string& host,const double& latitude,
			const double& longitude,const std::string& city,const std::string& district,
			const std::string& province,std::string& street){
	data_ = new Data(host,latitude,longitude,city,district,province,street);
}

LBSInfos::LBSInfos(const LBSInfos& lbs_basic_info)
:data_(lbs_basic_info.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

LBSInfos& LBSInfos::operator =(const LBSInfos& lbs_basic_info){
	if(lbs_basic_info.data_!=NULL){
		lbs_basic_info.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}
	data_ = lbs_basic_info.data_;
	return (*this);
}

base_logic::DictionaryValue* LBSInfos::Release(const double latitude,
			const double longitude){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	if(data_->latitude_!=0.0)
		dict->SetReal(L"latitude",data_->latitude_);
	if(data_->longitude_!=0.0)
		dict->SetReal(L"longitude",data_->longitude_);
	if(data_->longitude_!=0.0&&data_->latitude_!=0.0&&
			latitude!=0.0&&longitude!=0.0)
		dict->SetReal(L"distance",base_lbs::CalcGEO::CalcGEODistance(data_->latitude_,data_->longitude_,
				latitude,longitude));
	if(!data_->city_.empty())
		dict->SetString(L"city",data_->city_);
	if(!data_->district_.empty())
		dict->SetString(L"district",data_->district_);
	if(!data_->province_.empty())
		dict->SetString(L"province",data_->province_);
	if(!data_->street_.empty())
		dict->SetString(L"street",data_->street_);

	return dict.release();
}

Dimension::Dimension(){
	data_ = new Data();
}

Dimension::Dimension(const int64 id,const std::string& name,const std::string& description){
	data_ = new Data(id,name,description);
}

Dimension::Dimension(const Dimension& dimension_info)
:data_(dimension_info.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

Dimension& Dimension::operator =(const Dimension& dimension_info){
	if(dimension_info.data_!=NULL){
		dimension_info.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}
	data_ = dimension_info.data_;
	return (*this);
}

bool Dimension::ValueSerialization(base_logic::DictionaryValue* dict){
	bool r = false;
	r = dict->GetString(L"class_name",&data_->class_name_);
	r = dict->GetString(L"desc",&data_->description_);
	r = dict->GetBigInteger(L"id",&data_->id_);
	r = dict->GetString(L"name",&data_->name_);
	return true;
}

base_logic::DictionaryValue* Dimension::Release(){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	if(!data_->class_name_.empty())
		dict->SetString(L"class_name",data_->class_name_);
	if(!data_->description_.empty())
		dict->SetString(L"desc",data_->description_);
	if(data_->id_!=0)
		dict->SetBigInteger(L"id",data_->id_);
	if(!data_->name_.empty())
		dict->SetString(L"name",data_->name_);

	return dict.release();
}

Dimensions::Dimensions(){
	data_ = new Data();
}

Dimensions::Dimensions(const int64 id,const std::string& name,const std::string& alias){
	data_ = new Data(id,name,alias);
}

Dimensions::Dimensions(const Dimensions& dimensions_info)
:data_(dimensions_info.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

Dimensions& Dimensions::operator =(const Dimensions& dimensions_info){
	if(dimensions_info.data_!=NULL){
		dimensions_info.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}
	data_ = dimensions_info.data_;
	return (*this);
}

void Dimensions::dimension_name(const int64 id,std::string& name){
	std::string empty;
	std::map<int64,base_logic::Dimension>::iterator it = data_->dimensions_map_.find(id);
	if(it!=data_->dimensions_map_.end())
		name =  it->second.name();
	else
		name = UNKONW_DIMENSIONS;
}


DetailMessage::DetailMessage(){
	data_ = new Data();
}

DetailMessage::DetailMessage(const DetailMessage& message)
:data_(message.data_){
}

DetailMessage& DetailMessage::operator =(const DetailMessage& message){
	if(message.data_!=NULL){
		message.data_->AddRef();
	}
	if(data_!=NULL){
		data_->Release();
	}
	data_ = message.data_;
	return (*this);
}

base_logic::DictionaryValue* DetailMessage::Release(){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	if(data_->id_!=0)
		dict->SetBigInteger(L"id",data_->id_);
	if(data_->type_!=0)
		dict->SetBigInteger(L"type",data_->type_);
	if(data_->send_id_!=0)
		dict->SetBigInteger(L"send_id",data_->send_id_);
	if(data_->recv_id_!=0)
		dict->SetBigInteger(L"recv_id",data_->recv_id_);
	if(data_->msg_time_!=0)
		dict->SetBigInteger(L"msg_time",data_->msg_time_);
	if(!data_->message_.empty())
		dict->SetString(L"msg",data_->message_);

	return dict.release();
}

////组合////
UserAndMusic::UserAndMusic(){

}

UserAndMusic::UserAndMusic(const UserAndMusic& user_music){
	this->userinfo_ = user_music.userinfo_;
	this->musicinfo_ = user_music.musicinfo_;
	this->lbsinfo_ = user_music.lbsinfo_;
}

UserAndMusic& UserAndMusic::operator =(const UserAndMusic& user_music){
	this->userinfo_ = user_music.userinfo_;
	this->musicinfo_ = user_music.musicinfo_;
	this->lbsinfo_ = user_music.lbsinfo_;
	return (*this);
}

base_logic::DictionaryValue* UserAndMusic::Release(bool all,double latitude,
		double longitude){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	if(musicinfo_.id()!=0)
		dict->Set(L"music",musicinfo_.Release(all));
	if(userinfo_.uid()!=0)
		dict->Set(L"userinfo",userinfo_.Release());
	if(lbsinfo_.latitude()!=0.0&lbsinfo_.longitude()!=0.0)
		dict->Set(L"poi",lbsinfo_.Release(latitude,longitude));
	dict->SetString(L"songstat","0");
	return dict.release();
}


PersonalMessage::PersonalMessage(){

}

PersonalMessage::PersonalMessage(const PersonalMessage& personal){
	this->userinfo_ = personal.userinfo_;
	this->musicinfo_ = personal.musicinfo_;
	this->listen_musicinfo_ = personal.listen_musicinfo_;
	this->lbsinfo_ = personal.lbsinfo_;
	this->detail_message_ = personal.detail_message_;
}

PersonalMessage& PersonalMessage::operator =(const PersonalMessage& personal){
	this->userinfo_ = personal.userinfo_;
	this->musicinfo_ = personal.musicinfo_;
	this->listen_musicinfo_ = personal.listen_musicinfo_;
	this->lbsinfo_ = personal.lbsinfo_;
	this->detail_message_ = personal.detail_message_;
	return (*this);
}

base_logic::DictionaryValue* PersonalMessage::Release(double latitude,double longitude){
	scoped_ptr<base_logic::DictionaryValue> dict(new base_logic::DictionaryValue());
	if(musicinfo_.Isvalid())
		dict->Set(L"music",musicinfo_.Release());
	if(listen_musicinfo_.Isvalid())
		dict->Set(L"listen",listen_musicinfo_.Release());
	if(userinfo_.Isvalid())
		dict->Set(L"userinfo",userinfo_.Release());
	if(lbsinfo_.latitude()!=0.0&lbsinfo_.longitude()!=0.0)
		dict->Set(L"poi",lbsinfo_.Release(latitude,longitude));
	if(!detail_message_.message().empty())
		dict->Set(L"detail",detail_message_.Release());
	return dict.release();
}

}



