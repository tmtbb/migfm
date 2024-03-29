#ifndef _MIG_FM_PUBLIC_BASIC_BASIC_INFO_H__
#define _MIG_FM_PUBLIC_BASIC_BASIC_INFO_H__
#include <stdio.h>
#include "constants.h"
#include "basictypes.h"
namespace base{

class MusicUsrInfo{
public:
    explicit MusicUsrInfo();
    explicit MusicUsrInfo(const std::string& name,const std::string& phone,
                          const std::string& content);

    MusicUsrInfo(const MusicUsrInfo& mi);
    MusicUsrInfo& operator=(const MusicUsrInfo& mi);

    ~MusicUsrInfo(){
        if(data_!=NULL){
            data_->Release();
        }
    }
    const std::string& name() const {return !data_?STR_EMPTY:data_->name_;}
    const std::string& phone() const {return !data_?STR_EMPTY:data_->phone_;}
    const std::string& content() const {return !data_?STR_EMPTY:data_->content_;}
private:
    class Data{
        public:
            Data():refcount_(1){}
            Data(const std::string& name,const std::string& phone,const std::string& content)
                :name_(name)
                ,phone_(phone)
                ,content_(content)
                ,refcount_(1){}
            void AddRef(){refcount_++;}
            void Release(){if(!--refcount_) delete this;}
            const std::string name_;
            const std::string phone_;
            const std::string content_;
        private:
            int refcount_;
    };

    Data*           data_;
};

class MusicInfo{
public:
	explicit MusicInfo();
	explicit MusicInfo(const std::string id,const std::string& sid,
					   const std::string& ssid,const std::string& album_title,
					   const std::string& title,const std::string& hq_url,
					   const std::string& pub_time,const std::string& artist,
					   const std::string& pic_url,const std::string& url = " ",
					   const int32 time = 0);

	MusicInfo(const MusicInfo& mi);
	MusicInfo& operator=(const MusicInfo& mi);

	~MusicInfo(){
		if(data_!=NULL){
			data_->Release();
		}
	}

	void set_id(const std::string& id) {data_->id_ = id;}
	void set_sid(const std::string& sid) {data_->sid_ = sid;}
	void set_ssid(const std::string& ssid) {data_->ssid_ = ssid;}
	void set_album_title(const std::string& album_title) {data_->album_title_ = album_title;}
	void set_title(const std::string& title) {data_->titile_ = title;}
	void set_hq_url(const std::string& hq_url){data_->hq_url_ = hq_url;}
	void set_url(const std::string& url) {data_->url_ = url;}
	void set_pub_time(const std::string& pub_time) {data_->pub_time_ = pub_time;}
	void set_artist(const std::string& artist) {data_->artist_ = artist;}
	void set_pic_url(const std::string& pic_url) {data_->pic_url_ = pic_url;}
	void set_music_time(const int32 music_time) {data_->music_time_ = music_time;}
	void set_music_clt(const std::string& clt_num){data_->clt_num_ = clt_num;}
	void set_music_cmt(const std::string& cmt_num) {data_->cmt_num_ = cmt_num;}
	void set_music_hot(const std::string& hot_num){data_->hot_num_ = hot_num;}

	const std::string& id() const {return !data_?STR_EMPTY:data_->id_;}
	const std::string& sid() const {return !data_?STR_EMPTY:data_->sid_;}
	const std::string& ssid() const {return !data_?STR_EMPTY:data_->ssid_;}
	const std::string& album_title() const {return !data_?STR_EMPTY:data_->album_title_;}
	const std::string& title() const {return !data_?STR_EMPTY:data_->titile_;}
	const std::string& url() const {return !data_?STR_EMPTY:data_->url_;}
	const std::string& pub_time() const {return !data_?STR_EMPTY:data_->pub_time_;}
	const std::string& artist() const {return !data_?STR_EMPTY:data_->artist_;}
	const std::string& pic_url() const {return !data_?STR_EMPTY:data_->pic_url_;}
	const std::string& hq_url() const {return !data_?STR_EMPTY:data_->hq_url_;}
	const int32 music_time() const {return !data_?0:data_->music_time_;}
	const std::string& clt_num() const {return (!data_||data_->clt_num_.empty())?STR_ZONE:data_->clt_num_;}
	const std::string& cmt_num() const {return !(!data_||data_->cmt_num_.empty())?STR_ZONE:data_->cmt_num_;}
	const std::string& hot_num() const {return !(!data_||data_->hot_num_.empty())?STR_ZONE:data_->hot_num_;}

	bool SerializedJson(std::string& json);
 	bool UnserializedJson(std::string& str);
// 
// 	bool SerializedXml(std::string& xml);
// 	bool UnserializedXml(std::string& str);
private:
	class Data{
	public:
		Data():refcount_(1){}
		Data(const std::string id,const std::string& sid,
			const std::string& ssid,const std::string& album_title,
			const std::string& title,const std::string& hq_url,const std::string& pub_time,
			const std::string& artist,const std::string& pic_url,const std::string& url,
			int32 time)
			:id_(id)
			,sid_(sid)
			,ssid_(ssid)
			,album_title_(album_title)
			,titile_(title)
			,hq_url_(hq_url)
			,pub_time_(pub_time)
			,artist_(artist)
			,pic_url_(pic_url)
			,url_(url)
			,music_time_(time)
			,refcount_(1){
				hot_num_ = "0";
				clt_num_ = "0";
				cmt_num_ = "0";
		}

		void AddRef(){refcount_++;}
		void Release(){if(!--refcount_) delete this;}
		std::string id_;
		std::string sid_;
		std::string ssid_;
		std::string album_title_;
		std::string titile_;
		std::string url_;
		std::string hq_url_;
		std::string pub_time_;
		std::string artist_;
		std::string pic_url_;
		std::string clt_num_;
		std::string cmt_num_;
		std::string hot_num_;
		int32       music_time_;
	private:
		int refcount_;
	};

	Data*           data_;
};


class WordAttrInfo{
public:
	explicit WordAttrInfo();
	explicit WordAttrInfo(const std::string& word_id,const std::string& name);

	~WordAttrInfo(){
		if(data_!=NULL){
			data_->Release();
		}
	}

	WordAttrInfo(const WordAttrInfo& mi);
	WordAttrInfo& operator=(const WordAttrInfo& mi);

	const std::string& id(){return data_->word_id_;}
	const std::string& name() {return data_->name_;}

private:
	class Data{
	public:
		Data():refcount_(1){}
		Data(const std::string& word_id,const std::string& name)
			:word_id_(word_id)
			,name_(name)
			,refcount_(1){}
		void AddRef(){refcount_++;}
		void Release() {if(!--refcount_) delete this;}
		const std::string word_id_;
		const std::string name_;
	private:
		int refcount_;
	};
	Data*    data_;
};

class ChannelInfo{
public:
	explicit ChannelInfo();
	explicit ChannelInfo(const std::string& index,
		const std::string& douban_index, 
		const std::string& channel_name,
		const std::string& channel_dec,
		const std::string pic = "http://fm.miglab.com");

	~ChannelInfo(){
		if(data_!=NULL){
			data_->Release();
		}
	} 

	ChannelInfo(const ChannelInfo& mi);
	ChannelInfo& operator=(const ChannelInfo& mi);

	const std::string& index() const {return data_->index_;}
	const std::string& douban_index() const {return data_->douban_index_;}
	const std::string& channel_name() const {return data_->channel_name_;}
	const std::string& channel_pic() const {return data_->channel_pic_;}
	const std::string& channel_dec() const {return data_->channel_dec_;}

private:
	class Data{
	public:
		Data():refcount_(1){}
		Data(const std::string& index,const std::string& douban_index,
			const std::string& channel_name,const std::string pic,
			const std::string& dec)
			:index_(index)
			,douban_index_(douban_index)
			,channel_name_(channel_name)
			,channel_pic_(pic)
			,channel_dec_(dec)
			,refcount_(1){}
		void AddRef(){refcount_++;}
		void Release() {if(!--refcount_) delete this;}
		const std::string index_;
		const std::string douban_index_;
		const std::string channel_name_;
		const std::string channel_pic_;
		const std::string channel_dec_;
	private:
		int refcount_;
	};

    Data*    data_;
};

class ConnAddr{
public:
    explicit ConnAddr();
    explicit ConnAddr(const std::string& host,const int port,
    				const std::string& usr="",const std::string& pwd="",const std::string& source="");

    ConnAddr(const ConnAddr& ca);
	ConnAddr& operator=(const ConnAddr& ca);
	
    ~ConnAddr(){
        if(data_!=NULL){
            data_->Release();
        }
    } 
    
    const std::string& host() const {return !data_?STR_EMPTY:data_->host_;}
    const int port() const {return !data_?0:data_->port_;}
    const std::string& usr() {return !data_?STR_EMPTY:data_->usr_;}
    const std::string& pwd() {return !data_?STR_EMPTY:data_->pwd_;}   
    const std::string& source() {return !data_?STR_EMPTY:data_->source_;} 	

private:
   class Data{
    public:
	Data():refcount_(1),port_(0){}
	Data(const std::string& host,const int port,const std::string& usr,const std::string& pwd,const std::string& source)
	   :host_(host)
	   ,port_(port)
	   ,usr_(usr)
	   ,pwd_(pwd)
	   ,source_(source)
       ,refcount_(1){}
    void AddRef(){refcount_++;}
    void Release() {if(!--refcount_) delete this;}
    const std::string host_;
    const int port_;
    const std::string usr_;
    const std::string pwd_;
    const std::string source_;
    private:
	int refcount_;
    };
    
    Data*    data_;
};


class CompareInfo{
public:
	explicit CompareInfo();
	explicit CompareInfo(const std::string& info_id,const std::string& info_index);
	CompareInfo(const CompareInfo& ci);
	CompareInfo& operator=(const CompareInfo& ci);
	~CompareInfo(){
		if(data_!=NULL){
			data_->Release();
		}
	}

	void set_info_num(const int32 info_num){data_->info_num_ = info_num;}
	void set_info_id(const std::string info_id){data_->info_id_ = info_id;}
	void set_info_index(const std::string info_index){data_->info_index_ = info_index;}

	const std::string& info_id() const {return data_->info_id_;}
	const std::string& info_index() const {return data_->info_index_;}
	const int32 info_num() const {return data_->info_num_;}
private:
	class Data{
	public:
		Data():refcount_(1){}
		Data(const std::string& info_id,const std::string& info_index)
			:info_id_(info_id)
			,info_index_(info_index)
			,refcount_(1){}
		void AddRef(){refcount_++;}
		void Release(){if(!--refcount_) delete this;}
		std::string info_id_;
		std::string info_index_;
		int info_num_;
	private:
		int refcount_;
	};

	Data*      data_;
};

class UserInfo{
public:
	explicit UserInfo();
	explicit UserInfo(const std::string& uid,const std::string& username,
		const std::string& sex,const std::string& type,
		const std::string& crty,const std::string& head,
		const std::string& birthday,const std::string& nickname,
		const std::string& source);

	UserInfo(const UserInfo& userinfo);
	UserInfo& operator=(const UserInfo& usr);
	~UserInfo(){
		if (data_!=NULL){
			data_->Release();
		}
	}
	

	bool SerializedJson(std::string& json);
	bool UnserializedJson(const char* str);
    
	const std::string& uid() const {return data_->uid_;}
	const std::string& username() const {return data_->username_;}
	const std::string& sex() const {return data_->sex_;}
	const std::string& type() const {return data_->type_;}
	const std::string& crty() const {return data_->crty_;}
	const std::string& head() const {return ((!data_)||(data_->head_=="0"))?DEFAULT_HEAD:data_->head_;}
	const std::string& birthday() const {return data_->birthday_;}
	const std::string& nickname() const {return data_->nickname_;}
	const std::string& source() const {return data_->source_;}

	void set_uid(const std::string& uid){data_->uid_ = uid;}
	void set_username(const std::string& username){data_->username_ = username;}
	void set_sex(const std::string& sex){data_->sex_ = sex;}
	void set_type(const std::string& type){data_->type_ = type;}
	void set_crty(const std::string& crty){data_->crty_ = crty;}
	void set_head(const std::string& head){data_->head_ = head;}
	void set_birthday(const std::string& birthday){data_->birthday_ = birthday;}
	void set_nickname(const std::string& nickname){data_->nickname_ = nickname;}
	void set_source(const std::string& source){data_->source_ = source;}


	void set_uid(const char* uid){data_->uid_.assign(uid);}
	void set_username(const char* username){data_->username_.assign(username);}
	void set_sex(const char* sex){data_->sex_.assign(sex);}
	void set_type(const char* type){data_->type_.assign(type);}
	void set_crty(const char* crty){data_->crty_.assign(crty);}
	void set_head(const char* head){data_->head_.assign(head);}
	void set_birthday(const char* birthday){data_->birthday_.assign(birthday);}
	void set_nickname(const char* nickname){data_->nickname_.assign(nickname);}
	void set_source(const char* source){data_->source_ .assign(source);}

private:
	class Data{
	public:
		Data():refcount_(1){}
		Data(const std::string& uid,const std::string& username,
			 const std::string& sex,const std::string& type,
			 const std::string& ctry,const std::string& head,
			 const std::string& birthday,const std::string& nickname,
			 const std::string& source)
			:uid_(uid)
			,username_(username)
			,sex_(sex)
			,type_(type)
			,crty_(ctry)
			,head_(head)
			,birthday_(birthday)
			,nickname_(nickname)
			,source_(source)
			,refcount_(1){}
		void AddRef(){refcount_++;}
		void Release(){if(!--refcount_) delete this;}
		std::string uid_;
		std::string username_;
		std::string sex_;
		std::string type_;
		std::string crty_;
		std::string head_;
		std::string birthday_;
		std::string nickname_;
		std::string source_;
	private:
		int refcount_;
	};
	Data*      data_;


};

class MusicCltHateInfo{
public:
	explicit MusicCltHateInfo();
	explicit MusicCltHateInfo(const std::string& songid,
		                      const std::string& type,
							  const std::string& tid);

	MusicCltHateInfo(const MusicCltHateInfo& mclti);

	MusicCltHateInfo& operator=(const MusicCltHateInfo& mclti);

	~MusicCltHateInfo(){

	}

	bool SerializedJson(std::string& json);
	bool UnserializedJson(std::string& str);

	const std::string& songid() const {return data_->songid_;}
	const std::string& type() const {return data_->type_;}
	const std::string& tid() const {return data_->typeid_;}

	void set_songid(const std::string& songid){data_->songid_ = songid;}
	void set_type(const std::string& type) {data_->type_ = type;}
	void set_tid(const std::string& tid) {data_->typeid_ = tid;}

private:
	class Data{
	public:
		Data():refcount_(1){}
		Data(const std::string& songid,
			 const std::string& type,
			 const std::string& tid)
			:songid_(songid)
			,type_(type)
			,typeid_(tid)
			,refcount_(1){}
		void AddRef(){refcount_++;}
		void Release(){if(!--refcount_) delete this;}
		std::string songid_;
		std::string type_;
		std::string typeid_;
	private:
		int refcount_;
	};
	Data*      data_;

};

class NormalMsgInfo{
public:
	explicit NormalMsgInfo();
	explicit NormalMsgInfo(const std::string& uid,const std::string& content,
						   const std::string& type,const std::string& param);

	NormalMsgInfo(const NormalMsgInfo& cmt);

	NormalMsgInfo& operator=(const NormalMsgInfo& cmt);
	~NormalMsgInfo(){
		if (data_!=NULL){
			data_->Release();
		}
	}

	bool SerializedJson(std::string& json);
	bool UnserializedJson(std::string& str);

	const std::string& uid() const {return data_->uid_;}
	const std::string& content() const {return data_->content_;}
	const std::string& type() const {return data_->type_;}
	const std::string& param() const {return data_->param_;}
	const int64 msg_id() const {return data_->msg_id_;}

	void set_uid(const std::string& uid){data_->uid_ = uid;}
	void set_content(const std::string& content) {data_->content_ = content;}
	void set_type(const std::string& type) {data_->type_ = type;}
	void set_param(const std::string& param) {data_->param_ = param;}
	void set_msg_id(const int64 msg_id) {data_->msg_id_ = msg_id;}

private:
	class Data{
	public:
		Data():msg_id_(0)
		  ,refcount_(1){}
		Data(const std::string& uid,const std::string& content,
			 const std::string& type,int64 msg_id,
			 const std::string& param)
			:uid_(uid)
			,content_(content)
			,type_(type)
			,msg_id_(msg_id)
			,param_(param)
			,refcount_(1){}
		void AddRef(){refcount_++;}
		void Release(){if(!--refcount_) delete this;}
		std::string uid_;
		std::string content_;
		std::string type_;
		int64 msg_id_;
		std::string param_;
	private:
		int refcount_;
	};
	Data*      data_;
};


class RecordingLocalMusic{
public:
	explicit RecordingLocalMusic();
	explicit RecordingLocalMusic(const std::string& name,const std::string& singer);
	RecordingLocalMusic(const RecordingLocalMusic& rlm);
	RecordingLocalMusic& operator=(const RecordingLocalMusic& rlm);
	~RecordingLocalMusic(){
		if(data_!=NULL){
			data_->Release();
		}
	}


	const std::string name() const {return data_->name_;}
	const std::string singer() const {return data_->singer_;}

private:
	class Data{
	public:
		Data():refcount_(1){}
		Data(const std::string& name,const std::string& singer)
			:name_(name)
			,singer_(singer)
			,refcount_(1){}
		void AddRef(){refcount_++;}
		void Release(){if(!--refcount_) delete this;}
		std::string name_;
		std::string singer_;
		int info_num_;
	private:
		int refcount_;
	};

	Data*      data_;
};



}

#endif
