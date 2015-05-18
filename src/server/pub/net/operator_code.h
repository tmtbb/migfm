#ifndef _NET_OPERATOR_CODE_H_
#define _NET_OPERATOR_CODE_H_

enum operatorcode{

	//USER 1000
	QUICK_LOGIN = 1000, //快速登录
	THIRID_LOGIN = 1001, //第三方登陆
	LOGIN_RECORD = 1002,//登陆记录
	UPDATE_USERINFO = 1003,//更新用户信息
	BD_BIND_PUSH = 1100, //百度推送绑定

	//MUSIC 4000
	MUSIC_GAIN_COLLECT_LIST = 4001,//获取红心歌单
	MUSIC_GAIN_DIMENSION_LIST = 4002,//获取单维度歌曲
	MUSIC_GAIN_DIMENSIONS_LIST = 4003,//获取多维度歌曲
	MUSIC_GAIN_SET_COLLECT = 4004,//新增红心歌单
	MUSIC_GAIN_DEL_COLLECT = 4005,//删除红心歌单
	MUSIC_GAIN_NEAR_MUSIC = 4006,//周围音乐
	MUSIC_GAIN_NEAR_USER = 4007,//周围用户
	MUSIC_GAIN_SET_HATE = 4008,//新增垃圾歌曲
	MUSIC_SET_CURRENT_MUSIC = 4009,//记录当前歌曲
	MUSIC_GAIN_MY_MUSIC_FRIEND = 4010,//获取音乐好友
	MUSIC_GAIN_DIMENSION_INFO = 4011,//获取类别信息

	//SOC 5000
	SOC_GAIN_CURRENT_TYPE_BARRAGECOMM = 5001,//获取当前音乐列别弹幕及评论
	SOC_GAIN_CURRENT_LOCATION_INFO = 5002,//获取当前地理信息
	SOC_GAIN_SAY_HELLO = 5003,//打招呼
	SOC_GAIN_GIVE_SONG = 5004,//赠送歌曲

	//CHAT 6000
	CHAT_GAIN_GROUP_MESSAGE = 6001,//获取群组历史聊天记录
	CHAT_GAIN_ALONE_MESSAGE = 6002, //获取单聊历史聊天记录
	CHAT_GAIN_IDLE_CHAT_SERVER = 6003 //获取最空闲聊天服务端

};


#endif
