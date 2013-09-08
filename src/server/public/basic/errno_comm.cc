/*
*
* Returns: a static string.
*/
#include "errno_comm.h"
const char*
migfm_strerror(int error_code){
	
	switch (error_code) {
	case MIG_FM_HTTP_MOOD_NO_VALID:
		return "请求中未包含心情信息";
	case MIG_FM_HTTP_USER_NO_EXITS:
		return "请求中未包含用户信息";
	case MIG_FM_USER_MOOD_NO_EXITS:
		return "用户心情未生成";
	case MIG_FM_HTTP_MODE_NO_VALID:
		return "请求中未包含模式信息";
	case MIG_FM_HTTP_MOOD_DEC_NO_EXITS:
		return "请求中未包含心情描述信息";
	case MIG_FM_HTTP_DEC_NO_VALID:
		return "请求中未包含模式描述信息";
	case MIG_FM_HTTP_CHANNLE_NO_VALID:
		return "请求中未包含频道信息";
	case MIG_FM_HTTP_SONG_ID_NO_VALID:
		return "请求中未包含歌曲信息";
	case MIG_FM_USER_NO_COLLECT_SONG:
		return "用户未收藏歌曲";
	case MIG_FM_MOOD_NO_VALID:
		return "系统中心情不存在";
	case MIG_FM_SYSTEM_DEC_NO_VALID:
		return "系统中描述词不存在";
	case MIG_FM_MOODID_NO_VALID:
		return "请求中未包含心情序号";
	case MIG_FM_MOODINDEX_NO_VALID:
		return "请求中未包含心情顺序";
	case MIG_FM_SCENEID_NO_VALID:
		return "请求中未包含场景序号";
	case MIG_FM_SCENEINDEX_NO_VALID:
		return "请求中未包含场景顺序";
	case MIG_FM_CHANNELID_NO_VALID:
		return "请求中未包含频道序号";
	case MIG_FM_CHANNELINDEX_NO_VALID:
		return "请求中未包含频道顺序";
	case MIG_FM_SOUCE_NO_VALID:
		return "请求中未包含来源";
	case MIG_FM_CONTENT_NO_EXITS:
		return "请求中未包含歌曲信息";
	case MIG_FM_VERSION_NO_EXITS:
		return "请求中未包含版本信息";
	case MIG_FM_NET_STATE_NO_EXITS:
		return "请求中未包含网络状态";
	case MIG_FM_HTTP_INVALID_USER_ID:
		return "请求中ID字段非法";
	case MIG_FM_HTTP_DEVICE_TOKEN_NOT_EXIST:
		return "请求中未包含devicetoken字段";
	case MIG_FM_HTTP_INVALID_TIME_FORMAT:
		return "请求中时间格式非法";
	case MIG_FM_DB_SAVE_PUSH_CONFIG_FAILED:
		return "数据库保存推送设置失败";
	case MIG_FM_DB_READ_PUSH_CONFIG_FAILED:
		return "数据库读取推送设置失败";
	case MIG_FM_OTHER_PUSH_SERVICE_CLOSED:
		return "对方推送服务关闭";
	case MIG_FM_OTHER_ANTI_HARASSMENT:
		return "对方开启防打扰";
	case MIG_FM_DB_ACCESS_FAILED:
		return "数据库访问失败";
	case MIG_FM_PUSH_MSG_FAILED:
		return "推送消息失败";
	case MIG_FM_MSG_LIST_EMPTY:
		return "消息列表为空";
	case MIG_FM_SOURCE_SESSION_NO_VAILED:
		return "请求中未包含第三方标示符";
	case MIG_FM_USERNAME_NO_VAILED:
		return "请求中未包含用户名";
	case MIG_FM_PASSWORD_NO_VAILED:
		return "请求中未包含密码";
	case MIG_FM_USER_EXITS:
		return "用户已经存在";
	default:
		return "未知错误码";
	}
}
