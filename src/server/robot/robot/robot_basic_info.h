#ifndef _ROBOT_ROBOT_BASIC_INFO_H__
#define _ROBOT_ROBOT_BASIC_INFO_H__

#include <map>
#include <string>

#include "basic/basictypes.h"
#include <map>

namespace robot_base{

class LuckGiftInfo{
public:
	explicit LuckGiftInfo();
	explicit LuckGiftInfo(const int64 plat,const int32 rate,const int prize);

	LuckGiftInfo(const LuckGiftInfo& luck_gift);
	LuckGiftInfo& operator = (const LuckGiftInfo& luck_gift);

	const int32 plat() const {return data_->plat_;}
	const int32 rate() const {return data_->rate_;}
	const int32 prize() const {return data_->prize_;}

private:
	class Data{
	public:
		Data():refcount_(1)
			,plat_(0)
			,rate_(0)
			,prize_(0){}
		Data(const int64 plat,const int32 rate,const int prize)
			:refcount_(1)
			,plat_(plat)
			,rate_(rate)
			,prize_(prize){}

		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}
	public:
		const int32          plat_;
		const int32    		 rate_;
		const int32          prize_;
	private:
		int                  refcount_;
	};
	Data*                    data_;
};


class PlatformInfo{
public:
	explicit PlatformInfo();
	explicit PlatformInfo(const int64 platform_id,
		         const std::string& platform_name);

	PlatformInfo(const PlatformInfo& platform_info);
	PlatformInfo& operator = (const PlatformInfo& platform_info);

private:
	class Data{
	public:
		Data():refcount_(1)
			,platform_id_(0){}
		Data(const int64 platform_id,const std::string& platform_name)
			:refcount_(1)
			,platform_id_(platform_id)
			,platform_name_(platform_name){}

		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}
	public:
		int64                platform_id_;
		const std::string    platform_name_;
	private:
		int                  refcount_;
	};

	Data*                    data_;
};


class UserInfo{
public:
	explicit UserInfo();
	explicit UserInfo(const int64 uid);

	UserInfo(const UserInfo& userinfo);
	UserInfo& operator = (const UserInfo& userinfo);

	void set_type_id(const int32 type_id){data_->type_id_ = type_id;}
	void set_mode(const std::string& mode){data_->mode_ = mode;}

	const int64 uid() const {return data_->uid_;}
	const int32 type_id() const {return data_->type_id_;}
	const std::string& mode() const {return data_->mode_;}

private:
	class Data{
	public:
		Data():refcount_(1)
			,uid_(0){}
		Data(const int64 uid)
			:refcount_(1)
			,uid_(uid){}

		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}

	public:
		int64            uid_;
		int32            type_id_;
		std::string      mode_;

	private:
		int                  refcount_;
	};

	Data*                  data_;
};


class UserLbsInfo{
public:
	explicit UserLbsInfo();
	explicit UserLbsInfo(const int64 uid,const std::string& latitude,const std::string& longitude);

	UserLbsInfo(const UserLbsInfo& lbs_info);
	UserLbsInfo& operator = (const UserLbsInfo& lbs_info);

	const int64 uid() const {return data_->uid_;}
	const std::string& latitude() const {return data_->latitude_;}
	const std::string& longitude() const {return data_->longitude_;}

private:
	class Data{
	public:
		Data():refcount_(1)
			,uid_(0){}
		Data(const int64 uid,const std::string& latitude,const std::string& longitude)
			:refcount_(1)
			,uid_(uid)
			,latitude_(latitude)
			,longitude_(longitude){}

		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}

	public:
		int64   uid_;
		const std::string latitude_;
		const std::string longitude_;
	private:
		int                  refcount_;
	};

	Data*                  data_;

};

class UserBasicInfo{
public:
	explicit UserBasicInfo(const int64 uid);
	explicit UserBasicInfo();

	UserBasicInfo(const UserBasicInfo& user_info);
	UserBasicInfo& operator = (const UserBasicInfo& user_info);

	const int64 uid() const {return data_->uid_;}
	const double latitude() const {return data_->latitude_;}
	const double longitude() const {return data_->longitude_;}
	const std::string& city() const {return data_->city_;}
	const std::string& province() const {return data_->province_;}
	const std::string& street() const {return data_->street_;}
	const std::string& distict() const {return data_->district_;}
	const int last_weather_status() const {return data_->last_weather_status_;}
	const int32 push_day() const {return data_->push_day_;}
	const int32 push_month() const {return data_->push_month_;}

	void set_latitude(const double& latitude){data_->latitude_ = latitude;}
	void set_longitude(const double& longitude){data_->longitude_ = longitude;}
	void set_city(const std::string& city){data_->city_ = city;}
	void set_province(const std::string& province){data_->province_ = province;}
	void set_street(const std::string& street){data_->street_ = street;}
	void set_distict(const std::string& distict){data_->district_ = distict;}
	void set_last_weather_status(const double last_weather_status){data_->last_weather_status_ = last_weather_status;}
	void set_push_day(const int32 push_day){data_->push_day_ = push_day;}
	void set_push_month(const int32 push_month){data_->push_month_ = push_month;}

private:
	class Data{
	public:
		Data():refcount_(1)
			,uid_(0)
			,push_day_(0)
			,push_month_(0)
			,latitude_(0)
			,longitude_(0)
			,last_weather_status_(-1){}
		Data(const int64 uid_):refcount_(1)
			,uid_(uid_)
			,latitude_(0)
			,longitude_(0)
			,push_day_(0)
			,push_month_(0)
			,last_weather_status_(0){}
		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}
	public:
		const int64     uid_;
		double          latitude_;
		double          longitude_;
		int32           push_day_;
		int32           push_month_;
		std::string     city_;
		std::string     province_;
		std::string     street_;
		std::string     district_;
		int             last_weather_status_;
	private:
		int refcount_;
	};
	Data*               data_;
};

class RobotBasicInfo{
public:
	explicit RobotBasicInfo(const int64 uid,const int32 sex,const double latitude,
            const double longitude,const int64 songid,const std::string nickname,
            const std::string& head_url);

	explicit RobotBasicInfo();


	RobotBasicInfo(const RobotBasicInfo& robot_info);
	RobotBasicInfo& operator = (const RobotBasicInfo& robot_info);

	const int64 uid() const {return data_->uid_;}
	const int32 sex() const {return data_->sex_;}
	const int64 songid() const {return data_->songid_;}
	const double latitude() const {return data_->latitude_;}
	const double longitude() const {return data_->longitude_;}
	const std::string& nickname() const {return data_->nickname_;}
	const std::string& head_url() const {return data_->head_url_;}
	const int socket() const {return data_->socket_;}
	const int32 task_count() const {return data_->task_count_;}
	const int32 song_task_count() const {return data_->song_task_count_;}
	const int32 say_hello_count() const {return data_->say_hello_count_;}
	const int32 listen_task_count() const {return data_->listen_task_count_;}
	const time_t send_last_time() const {return data_->send_last_time_;}
	const time_t recv_last_time() const {return data_->recv_last_time_;}
	const int32 send_error_count() const {return data_->send_error_count_;}
	std::map<int64,int64>& follow_uid() const  {return data_->follow_map_; }
	const time_t follower_user_last_time() const {return data_->follower_user_last_time_;}



	void set_sex(const int32 sex) {data_->sex_ = sex;}
	void set_songid(const int64 songid) {data_->songid_ = songid;}
	void set_latitude(const double latitude) {data_->latitude_ = latitude;}
	void set_longitude(const double longitude) {data_->longitude_ = longitude;}
	void set_nickname(const std::string& nickname) {data_->nickname_ = nickname;}
	void set_head_url(const std::string& head_url) {data_->head_url_ = head_url;}
	void set_socket(const int socket){data_->socket_ = socket;}
	void set_follow_uid(const int64 uid){data_->follow_map_[uid] = uid;}
	void set_send_last_time(const time_t send_last_time){data_->send_last_time_ = send_last_time;}
	void set_recv_last_time(const time_t recv_last_time){data_->recv_last_time_ = recv_last_time;}
	void set_send_error_count(const int32 send_error_count){data_->send_error_count_ = send_error_count;}
	void set_login_status(const int32 login_status){data_->login_ = login_status;}
	void set_follower_user_last_time(const time_t follower_user_last_time) {data_->follower_user_last_time_ = follower_user_last_time;}

	void add_task_count(){data_->task_count_++;}
	void add_song_task_count(){data_->song_task_count_++;add_task_count();}
	void add_hello_task_count(){data_->say_hello_count_++;add_task_count();}
	void add_listen_task_count(){data_->listen_task_count_++;add_task_count();}

	bool check_song_task();


	void add_send_error_count(){data_->send_error_count_++;}
private:
		class Data{
		public:
			Data():refcount_(1)
				,task_count_(0)
				,song_task_count_(0)
				,say_hello_count_(0)
				,listen_task_count_(0)
				,uid_(0)
				,sex_(0)
				,latitude_(0)
				,longitude_(0)
				,songid_(0)
				,socket_(0)
				,login_(0)
				,send_last_time_(time(NULL))
				,recv_last_time_(time(NULL))
				,send_error_count_(0)
				,follower_user_last_time_(0){}


			Data(const int64 uid,const int32 sex,const double latitude,const double longitude,
	                const int64 songid,const std::string nickname,const std::string& head_url)
			:refcount_(1)
			,task_count_(0)
			,song_task_count_(0)
			,say_hello_count_(0)
			,listen_task_count_(0)
			,uid_(uid)
			,sex_(sex)
			,latitude_(latitude)
			,longitude_(longitude)
			,songid_(songid)
			,nickname_(nickname)
			,head_url_(head_url)
			,socket_(0)
			,login_(0)
			,send_last_time_(time(NULL))
			,recv_last_time_(time(NULL))
			,send_error_count_(0)
			,follower_user_last_time_(0){}

			void AddRef(){refcount_ ++;}
			void Release(){if (!--refcount_)delete this;}

		public:
			const int64 uid_;
			int32  sex_;
			double latitude_;
			double longitude_;
			int64 songid_;
			int socket_;
			int task_count_;
			int song_task_count_;
			int say_hello_count_;
			int listen_task_count_;
			int send_error_count_;
			int login_;
			std::string nickname_;
			std::string head_url_;
			std::map<int64,int64> follow_map_;
			time_t send_last_time_;
			time_t recv_last_time_;
			time_t follower_user_last_time_;
		private:
			int refcount_;
		};

		Data*        data_;
	};


class SchedulerInfo{
public:
	SchedulerInfo(const int64 platform_id,const int socket,const std::string& ip,
			const std::string& machine_id);
	SchedulerInfo();
	SchedulerInfo(const SchedulerInfo& scheduler_info);
	SchedulerInfo& operator = (const SchedulerInfo& scheduler_info);
public:
	const std::string& machine_id() const {return data_->machine_id_;}
	const std::string& ip() const {return data_->ip_;}
	const int socket() const {return data_->socket_;}
	const int client_count() const {return data_->client_count_;}

	void add_client_count(){data_->client_count_++;}
	void relase_client_count(){data_->client_count_--;}

	void set_machine_id(const std::string& machine_id){data_->machine_id_ = machine_id;}
	void set_ip(const std::string& ip){data_->ip_ = ip;}
	void set_socket(const int socket){data_->socket_ = socket;}

	const int32 send_error_count() const {return data_->send_error_count_;}
	void add_send_error_count(){data_->send_error_count_++;}

	void set_send_last_time(const time_t send_last_time){data_->send_last_time_ = send_last_time;}
	void set_recv_last_time(const time_t recv_last_time){data_->recv_last_time_ = recv_last_time;}
	const time_t send_last_time() const {return data_->send_last_time_;}
	const time_t recv_last_time() const {return data_->recv_last_time_;}


public:
	class Data{
	public:
		Data(const int64 platform_id,const int socket,const std::string& ip,
				const std::string& machine_id)
		:refcount_(1)
		,platform_id_(platform_id)
		,socket_(socket)
		,ip_(ip)
		,client_count_(0)
		,send_error_count_(0)
		,machine_id_(machine_id)
		,send_last_time_(0)
		,recv_last_time_(0){}

		Data():refcount_(1),platform_id_(0),client_count_(0),send_error_count_(0),send_last_time_(0)
		,recv_last_time_(0){}

		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}

	public:
		int64 platform_id_;
		int socket_;
		int client_count_;
		int send_error_count_;
		time_t send_last_time_;
		time_t recv_last_time_;
		std::string ip_;
		std::string machine_id_;
	private:
		int refcount_;

	};
	Data*   data_;
};


}

#endif
