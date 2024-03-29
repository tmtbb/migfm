#include "wx_get_song.h"
#include "json/json.h"
#include "log/mig_log.h"
#include "http_response.h"
#include "basic/base64.h"
#include <sstream>
namespace wxinfo{

WXGetSongUrl* WXGetSongUrl::Create(int32 type){
	WXGetSongUrl* engine_ = NULL;
	switch (type)
	{
	case TYPE_BAIDU:
		engine_ = new wxinfo::WXBaiduGetSongImpl();
		break;
	}
	return engine_;
}




WXBaiduGetSongImpl::~WXBaiduGetSongImpl(){

}

void WXBaiduGetSongImpl::Init(std::string& song_url){
	requst_song_url_ = song_url;
}

bool WXBaiduGetSongImpl::GetSongInfo(const std::string& artist,const std::string& title,
									 const std::string album,std::string& song_url){
	std::string key;
	std::stringstream s_key;
	key = artist+title/*+album*/;
	bool r = false;
	s_key<<"keys="<<base64_decode(artist)<<" "<<base64_decode(title);
	key = s_key.str();
	r = HttpGetSongInfo(key,song_url);
	MIG_DEBUG(USER_LEVEL,"%s",song_url.c_str());
	if (song_url=="0")
		r = false;
	return r;
}

bool WXBaiduGetSongImpl::HttpGetSongInfo(const std::string& key,std::string& song_url){

	//std::stringstream baidu_content;
	std::string baidu_ack_content;
	bool r  =false;
	Json::Reader reader;
	Json::Value  root;

	//baidu_content<<"keys="<<base64_decode(key).c_str();

	wxinfo::HttpPost http_post(requst_song_url_);
	r = http_post.Post(key.c_str(),82);
	MIG_DEBUG(USER_LEVEL,"%s",key.c_str());
	if (!r){
	 MIG_ERROR(USER_LEVEL,"http post error");
	 return false;
	}

	http_post.GetContent(baidu_ack_content);
	r = reader.parse(baidu_ack_content.c_str(),root);
	if (r){
		song_url = root["result"].asString();
	}
	return true;
}



}