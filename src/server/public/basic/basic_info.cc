#include "basic_info.h"
#include "json/json.h"
#include "log/mig_log.h"
#include <sstream>

namespace base{
	
WordAttrInfo::WordAttrInfo(){
	data_ = new Data();
}

WordAttrInfo::WordAttrInfo(const std::string &word_id, const std::string &name){
	data_ = new Data(word_id,name);
}

WordAttrInfo& WordAttrInfo::operator=(const WordAttrInfo& mi){
	if(mi.data_!=NULL){
		mi.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}

	data_ = mi.data_;
	return *this;
}

WordAttrInfo::WordAttrInfo(const WordAttrInfo& mi)
:data_(mi.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

MusicUsrInfo::MusicUsrInfo(){
    data_ = new Data();
}

MusicUsrInfo::MusicUsrInfo(const std::string& name,const std::string& phone,
                           const std::string& content){
    data_ = new Data(name,phone,content);
}


MusicUsrInfo& MusicUsrInfo::operator=(const MusicUsrInfo& mi){
    if(mi.data_!=NULL){
        mi.data_->AddRef();
    }
    
    if(data_!=NULL){
        data_->Release();
    }

    data_ = mi.data_;
    return *this;
}

MusicUsrInfo::MusicUsrInfo(const MusicUsrInfo& mi)
:data_(mi.data_){
    if(data_!=NULL){
        data_->AddRef();
    }
}

ChannelInfo::ChannelInfo(){
	data_ = new Data();
}

ChannelInfo::ChannelInfo(const std::string& index,
						 const std::string& douban_index, 
						 const std::string& channel_name,
						 const std::string& channel_dec,
						 const std::string pic/* = "http://fm.miglab.com"*/){
    data_ = new Data(index,douban_index,channel_name,pic,channel_dec);
}

ChannelInfo& ChannelInfo::operator=(const ChannelInfo& mi){
	if(mi.data_!=NULL){
		mi.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}

	data_ = mi.data_;
	return *this;
}

ChannelInfo::ChannelInfo(const ChannelInfo& mi)
:data_(mi.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}



MusicInfo::MusicInfo(){
	data_ = new Data();
}

MusicInfo::MusicInfo(const std::string id,const std::string& sid,const std::string& ssid,
					 const std::string& album_title,const std::string& title,
					 const std::string& hq_url,const std::string& pub_time,
					 const std::string& artist,const std::string& pic_url,
					 const std::string& url,int32 time){

		data_ = new Data(id,sid,ssid,album_title,title,hq_url,pub_time,artist,pic_url,url,time);
}

MusicInfo& MusicInfo::operator=(const MusicInfo& mi){
	if(mi.data_!=NULL){
		mi.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}

	data_ = mi.data_;
	return *this;
}

MusicInfo::MusicInfo(const MusicInfo& mi)
:data_(mi.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}
/*
{"sid":"1911362","ssid_":"9310","album_title":"VGhlIFVsdGltYXRlIEp1bmcuLi4=","titile_":"TGltYiBCeSBMaW1iIChESiBTUyBNaXgp","pub_time_":"2008","artist_":"Q3V0dHkgUmFua3M="}
*/
bool MusicInfo::SerializedJson(std::string &json){
	std::stringstream os;
	os<<"{\"id\":"
		<<"\""<<id().c_str()<<"\",\"sid\":"
		<<"\""<<sid().c_str()<<"\",\"ssid\":"
		<<"\""<<ssid().c_str()<<"\",\"album_title\":"
		<<"\""<<album_title().c_str()<<"\",\"titile\":"
		<<"\""<<title().c_str()<<"\",\"pub_time\":"
		<<"\""<<pub_time().c_str()<<"\",\"artist\":"
		<<"\""<<artist().c_str()<<"\",\"hq_url\":"
		<<"\""<<hq_url().c_str()<<"\",\"pic_url\":"
		<<"\""<<pic_url().c_str()<<"\"}";
	json.assign(os.str().c_str(),os.str().length());
	return true;

}

/*
{\"id\":\"147\",\"sid\":\"1\",\"ssid\":\"79dd\",\"album_title_\":\"Umlub2Nlcm9zZQ==\",\"titile_\":\"Q3ViaWNsZSA=\",\"pub_time_\":\"2006\",\"artist_\":\"Umlub2Nlcm9zZQ==\"}
*/
bool MusicInfo::UnserializedJson(std::string& str){
	Json::Reader reader;
	Json::Value root;
	bool r = false;
	r = reader.parse(str.c_str(),root);
	if (!r){
		MIG_ERROR(USER_LEVEL,"json parser error");
		return false;
	}
	
	set_id(root["id"].asString());
	set_sid(root["sid"].asString());
	set_ssid(root["ssid"].asString());
	set_album_title(root["album_title"].asString());
	set_title(root["titile"].asString());
	set_pub_time(root["pub_time"].asString());
	set_artist(root["artist"].asString());
	set_pic_url(root["pic_url"].asString());
	set_hq_url(root["hq_url"].asString());
	return true;
}

ConnAddr::ConnAddr(){
  
    data_ = new Data();
}

ConnAddr::ConnAddr(const std::string& host,const int port,
					const std::string& usr,const std::string& pwd,
					const std::string& source){
    
    data_ = new Data(host,port,usr,pwd,source);
}

ConnAddr& ConnAddr::operator=(const ConnAddr& ca){
	if(ca.data_!=NULL){
		ca.data_->AddRef();
    }
    if(data_!=NULL){
    	data_->Release();
    }

    data_ = ca.data_;
       return *this;
}

ConnAddr::ConnAddr(const ConnAddr& ca)
:data_(ca.data_){
	if(data_!=NULL){
	    data_->AddRef();
	}
}


CompareInfo::CompareInfo(){

	data_ = new Data();
}

CompareInfo::CompareInfo(const std::string& info_id,const std::string& info_index){

	data_ = new Data(info_id,info_index);
}

CompareInfo& CompareInfo::operator=(const CompareInfo& ci){
	if(ci.data_!=NULL){
		ci.data_->AddRef();
	}
	if(data_!=NULL){
		data_->Release();
	}

	data_ = ci.data_;
	return *this;
}


CompareInfo::CompareInfo(const CompareInfo& ci)
:data_(ci.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}





RecordingLocalMusic::RecordingLocalMusic(){

	data_ = new Data();
}

RecordingLocalMusic::RecordingLocalMusic(const std::string& name,const std::string& singer){

	data_ = new Data(name,singer);
}

RecordingLocalMusic& RecordingLocalMusic::operator=(const RecordingLocalMusic& ci){
	if(ci.data_!=NULL){
		ci.data_->AddRef();
	}
	if(data_!=NULL){
		data_->Release();
	}

	data_ = ci.data_;
	return *this;
}


RecordingLocalMusic::RecordingLocalMusic(const RecordingLocalMusic& ci)
:data_(ci.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}



}