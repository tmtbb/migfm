#include "protocol.h"
#include "comm_head.h"
#include "protocol/data_packet.h"
#include "logic/logic_comm.h"


namespace robot_logic{


bool ProtocolPack::UnpackStream(const void *packet_stream, int len, 
                                        struct PacketHead**packhead ){

	bool r = true;
	if (len < PACKET_HEAD_LENGTH)
		return false;

	int32 packet_length = ((struct PacketHead *) packet_stream)->packet_length;
	int32 packet_operate = ((struct PacketHead *) packet_stream)->operate_code;
	int32 data_length = ((struct PacketHead *) packet_stream)->data_length;
	int32 is_zip = ((struct PacketHead *) packet_stream)->is_zip;
	char* data = NULL;
	if (packet_length < PACKET_HEAD_LENGTH
		||packet_length!= len
		||data_length != packet_length - PACKET_HEAD_LENGTH){
			LOG_ERROR2("packet_length [%d] data_length[%d] PACK_HEAD_LENGTH [%d]",
				packet_length,data_length,PACKET_HEAD_LENGTH);
			return false;
	}


	if (is_zip)
		data = (char*)packet_stream + PACKET_HEAD_LENGTH;
	else
		data = (char*)packet_stream + PACKET_HEAD_LENGTH;

	switch(packet_operate){
		case NOTICE_USER_LOGIN:
		{
			struct NoticeUserLogin* vNoticeUserLogin =
					new struct NoticeUserLogin;
			*packhead = (struct PacketHead*)vNoticeUserLogin;
			BUILDPACKHEAD();

			vNoticeUserLogin->platform_id = in.Read64();
			vNoticeUserLogin->uid = in.Read64();
			vNoticeUserLogin->latitude = in.Read32();
			vNoticeUserLogin->longitude = in.Read32();
		}
		break;
		case NOTICE_USER_DEFAULT_SONG:
		{
			struct NoticeUserDefaultSong* vNoticeUserDefaultSong =
					new struct NoticeUserDefaultSong;
			*packhead = (struct PacketHead*)vNoticeUserDefaultSong;
			BUILDPACKHEAD();

			vNoticeUserDefaultSong->platform_id = in.Read64();
			vNoticeUserDefaultSong->uid = in.Read64();
			vNoticeUserDefaultSong->songid = in.Read64();
			vNoticeUserDefaultSong->type_id = in.Read32();
			int temp = 0;
			int len = vNoticeUserDefaultSong->data_length  - sizeof(int64) * 3 - sizeof(int32);
			char* str = new char[len];
			memcpy(str,in.ReadData(len,temp),len);
			vNoticeUserDefaultSong->mode.assign(str,len);
			if(str){
				delete [] str;
				str = NULL;
			}

		}
		break;
		case NOTICE_USER_CURRENT_SONG:
		{
			struct NoticeUserCurrentSong* vNoticeUserCurrentSong =
					new struct NoticeUserCurrentSong;
			*packhead = (struct PacketHead*)vNoticeUserCurrentSong;
			BUILDPACKHEAD();

			vNoticeUserCurrentSong->platform_id = in.Read64();
			vNoticeUserCurrentSong->uid = in.Read64();
			vNoticeUserCurrentSong->songid = in.Read64();
			vNoticeUserCurrentSong->type_id = in.Read32();
			int temp = 0;
			int len = vNoticeUserCurrentSong->data_length  - sizeof(int64) * 3 - sizeof(int32);
			char* str = new char[len];
			memcpy(str,in.ReadData(len,temp),len);
			vNoticeUserCurrentSong->mode.assign(str,len);
			if(str){
				delete [] str;
				str = NULL;
			}
		}
		break;
		case NOTICE_USER_ROBOT_LOGIN:
		{
			struct NoticeRobotLogin* vNoticeRobotLogin =
					new struct NoticeRobotLogin;
			*packhead = (struct PacketHead*)vNoticeRobotLogin;
			BUILDPACKHEAD();

			vNoticeRobotLogin->uid = in.Read64();
			int list_size = data_length - sizeof(int64);
			int i = 0;
			int nums = list_size/ROBOTINFO_SIZE;
			int len = 0;

			for(;i<nums;i++){
				struct RobotInfo* robotinfo = new struct RobotInfo;
				int temp = 0;
				robotinfo->uid = in.Read64();
				robotinfo->latitude = in.Read32();
				robotinfo->longitude = in.Read32();
				memcpy(robotinfo->nickname,in.ReadData(NICKNAME_LEN,temp),
						NICKNAME_LEN);
				robotinfo->nickname[NICKNAME_LEN - 1] = '\0';
				vNoticeRobotLogin->robot_list.push_back(robotinfo);
			}
		}
		break;
		case SCHEDULER_LOGIN:
		{
			struct SchedulerLogin* vSchedulerLogin =
					new struct SchedulerLogin;
			*packhead = (struct PacketHead*)vSchedulerLogin;
			BUILDPACKHEAD();
			vSchedulerLogin->platform_id = in.Read64();
			int temp = 0;
			int len = vSchedulerLogin->data_length  - sizeof(int64);
			char* str = new char[len];
			memcpy(str,in.ReadData(len,temp),len);
			vSchedulerLogin->machine_id.assign(str,len);
			if(str){
				delete [] str;
				str = NULL;
			}
		}
		break;
		case ROBOT_LOGIN:
		{
			struct RobotLogin* vRobotLogin =
					new struct RobotLogin;
			*packhead = (struct RobotLogin*)vRobotLogin;
			BUILDPACKHEAD();
			vRobotLogin->platform_id = in.Read64();
			vRobotLogin->uid = in.Read64();
			vRobotLogin->robot_id = in.Read64();
		}
		break;
		case NOTICE_USER_ROBOT_HANDSEL_SONG:
		{
			struct NoticeUserRobotHandselSong* vNoticeUserRobotHandselSong =
					new struct NoticeUserRobotHandselSong;
			*packhead = (struct PacketHead*)vNoticeUserRobotHandselSong;
			BUILDPACKHEAD();
			vNoticeUserRobotHandselSong->platform_id = in.Read64();
			vNoticeUserRobotHandselSong->uid = in.Read64();
			vNoticeUserRobotHandselSong->robot_id = in.Read64();
			vNoticeUserRobotHandselSong->robot_id = in.Read64();
		}
		break;
		case NOTICE_USER_ROBOT_LISTEN_SONG:
		{
			struct NoticeUserListenSong* vNoticeUserListenSong =
					new struct NoticeUserListenSong;
			*packhead = (struct PacketHead*)vNoticeUserListenSong;
			BUILDPACKHEAD();
			vNoticeUserListenSong->platform_id = in.Read64();
			vNoticeUserListenSong->songid = in.Read64();
			vNoticeUserListenSong->typid = in.Read32();
			int temp = 0;
			memcpy(vNoticeUserListenSong->mode,in.ReadData(MODE_LEN,temp),
					MODE_LEN);
			vNoticeUserListenSong->mode[MODE_LEN - 1] = '\0';
			memcpy(vNoticeUserListenSong->name,in.ReadData(NAME_LEN,temp),
					NAME_LEN);
			vNoticeUserListenSong->name[NAME_LEN - 1] = '\0';
			memcpy(vNoticeUserListenSong->singer,in.ReadData(SINGER_LEN,temp),
					SINGER_LEN);
			vNoticeUserListenSong->singer[SINGER_LEN - 1] = '\0';
		}
		break;
		case NOTICE_ASSISTANT_LOGIN:
		{
			struct NoticeAssistantLogin* vNoticeAssistantLogin =
					new struct NoticeAssistantLogin;
			*packhead = (struct PacketHead*)vNoticeAssistantLogin;
			BUILDPACKHEAD();
			vNoticeAssistantLogin->platform_id = in.Read64();
			vNoticeAssistantLogin->assistant_id = in.Read64();
			int temp = 0;
			memcpy(vNoticeAssistantLogin->nickname,in.ReadData(NICKNAME_LEN,temp),
					NICKNAME_LEN);
			vNoticeAssistantLogin->nickname[NICKNAME_LEN - 1] = '\0';
		}
		break;
		case ASSISTANT_LOGIN_SUCCESS:
		{
			struct AssistantLoginSuccess* vAssistantLoginSuccess =
					new struct AssistantLoginSuccess;
			*packhead = (struct AssistantLoginSuccess*)vAssistantLoginSuccess;
			BUILDPACKHEAD();
			vAssistantLoginSuccess->platform_id = in.Read64();
			vAssistantLoginSuccess->assistant_id = in.Read64();
		}
		break;
		/*case NOTICE_USER_ROBOT_SCHEDULER_CHAT_LOGIN:
		{
			struct NoticeUserRobotSchedulerChatLogin* vNoticeUserRobotSchedulerChatLogin =
					new struct NoticeUserRobotSchedulerChatLogin;
			*packhead = (struct NoticeUserRobotSchedulerChatLogin*)vNoticeUserRobotSchedulerChatLogin;
			BUILDPACKHEAD();
			vNoticeUserRobotSchedulerChatLogin->platform_id = in.Read64();
			vNoticeUserRobotSchedulerChatLogin->uid = in.Read64();
			vNoticeUserRobotSchedulerChatLogin->robot_id = in.Read64();
			vNoticeUserRobotSchedulerChatLogin->flag = in.Read8();
		}
		break;
		*/
		case NOTICE_USER_ROBOT_SCHEDULER_CHAT_LOGIN:
		case NOTICE_USER_ROBOT_CHAT_LOGIN:
		{
			struct NoticeUserRobotChatLogin* vNoticeUserRobotChatLogin =
					new struct NoticeUserRobotChatLogin;
			*packhead= (struct NoticeUserRobotChatLogin*)vNoticeUserRobotChatLogin;
			BUILDPACKHEAD();
			vNoticeUserRobotChatLogin->platform_id = in.Read64();
			vNoticeUserRobotChatLogin->uid = in.Read64();
			vNoticeUserRobotChatLogin->robot_id = in.Read64();

		}
		break;
		case NOTICE_ASSISTANT_HANDSEL_SONG:
		{
			struct NoticeAssistantHandselSong* vNoticeAssistanHandselSong =
					new struct NoticeAssistantHandselSong;
			*packhead = (struct NoticeAssistantHandselSong*)vNoticeAssistanHandselSong;
			BUILDPACKHEAD();
			vNoticeAssistanHandselSong->platform_id = in.Read64();
			vNoticeAssistanHandselSong->assistant_id = in.Read64();

			int list_size = data_length - sizeof(int64) * 2;
			int i = 0;
			int nums = list_size / HANDLESONG_SIZE;
			int len = 0;
			for(;i<nums;i++){
				struct HandleSongInfo * handlesong = new struct HandleSongInfo;
				int temp = 0;
				handlesong->uid = in.Read64();
				handlesong->songid = in.Read8();
				memcpy(handlesong->message,in.ReadData(MESSAGE_LEN,temp),
						MESSAGE_LEN);
				handlesong->message[MESSAGE_LEN - 1] = '\0';
				vNoticeAssistanHandselSong->list.push_back(handlesong);
			}
		}
		break;
		case NOTICE_USER_ROBOT_GIFT_LUCK:
		{
			struct NoticeUserRobotGiftLuck* vNoticeUserRobotGiftLuck =
					new struct NoticeUserRobotGiftLuck;
			*packhead= (struct NoticeUserRobotGiftLuck*)vNoticeUserRobotGiftLuck;
			BUILDPACKHEAD();
			int temp = 0;
			vNoticeUserRobotGiftLuck->platform_id = in.Read64();
			vNoticeUserRobotGiftLuck->uid = in.Read64();
			vNoticeUserRobotGiftLuck->share_plat = in.Read32();
			vNoticeUserRobotGiftLuck->prize = in.Read32();
			memcpy(vNoticeUserRobotGiftLuck->name,in.ReadData(NAME_LEN,temp),
					NAME_LEN);
			vNoticeUserRobotGiftLuck->name[NAME_LEN - 1] = '\0';
			memcpy(vNoticeUserRobotGiftLuck->singer,in.ReadData(SINGER_LEN,temp),
					SINGER_LEN);
			vNoticeUserRobotGiftLuck->singer[SINGER_LEN - 1] = '\0';
		}
		break;
		case NOTICE_USER_READY_GIFT_LUCK:
		{
			struct NoticeUserReadyGiftLuck* vNoticeUserReadyGiftLuck =
					new struct NoticeUserReadyGiftLuck;
			*packhead= (struct NoticeUserReadyGiftLuck*)vNoticeUserReadyGiftLuck;
			BUILDPACKHEAD();
			vNoticeUserReadyGiftLuck->platform_id = in.Read64();
			vNoticeUserReadyGiftLuck->uid = in.Read64();
			vNoticeUserReadyGiftLuck->songid = in.Read64();
			vNoticeUserReadyGiftLuck->share_plat = in.Read32();
		}
		break;
		case HEART_PACKET:
		{
			r = true;
			struct PacketHead* packet = new struct PacketHead;
			*packhead = packet;
			BUILDPACKHEAD();
		}
		break;
		default:
			r = false;
			break;

	}
	return r;
}

bool ProtocolPack::PackStream(const struct PacketHead* packhead,void** packet_stream,
							  int32* packet_stream_length){
	bool r = true;

	int32 packet_length = packhead->packet_length;
	int32 operate_code = packhead->operate_code;
	int32 data_length = packhead->data_length;
	int32 current_time = packhead->current_time;
	int8  is_zip = packhead->is_zip;
	int16 msg_type = packhead->msg_type;
	int64 msg_id =packhead->msg_id;
	int32 reserverd = packhead->reserverd;

	char* packet = NULL;
	char* data = NULL;

	switch(operate_code){
		case NOTICE_USER_LOGIN:
		{
			struct NoticeUserLogin* vNoticeUserLogin =
					(struct NoticeUserLogin*)packhead;
			BUILDHEAD(NOTICEUSERLOGIN_SIZE);
			out.Write64(vNoticeUserLogin->platform_id);
			out.Write64(vNoticeUserLogin->uid);
			out.Write32(vNoticeUserLogin->latitude);
			out.Write32(vNoticeUserLogin->longitude);
			packet = (char*)out.GetData();
		}
		break;

		case NOTICE_USER_DEFAULT_SONG:
		{
			struct NoticeUserDefaultSong* vNoticeUserDefaultSong =
					(struct NoticeUserDefaultSong*)packhead;
			BUILDHEAD(NOTICEUSERDEFAULTSONG_SIZE);
			out.Write64(vNoticeUserDefaultSong->platform_id);
			out.Write64(vNoticeUserDefaultSong->uid);
			out.Write64(vNoticeUserDefaultSong->songid);
			out.Write32(vNoticeUserDefaultSong->type_id);
			out.WriteData(vNoticeUserDefaultSong->mode.c_str(),vNoticeUserDefaultSong->mode.length());
			packet = (char*)out.GetData();
		}
		break;

		case NOTICE_USER_CURRENT_SONG:
		{
			struct NoticeUserCurrentSong* vNoticeUserCurrentSong =
					(struct NoticeUserCurrentSong*)packhead;
			BUILDHEAD(NOTICEUSERCURRENTSONG_SIZE);
			out.Write64(vNoticeUserCurrentSong->platform_id);
			out.Write64(vNoticeUserCurrentSong->uid);
			out.Write64(vNoticeUserCurrentSong->songid);
			out.Write32(vNoticeUserCurrentSong->type_id);
			out.WriteData(vNoticeUserCurrentSong->mode.c_str(),vNoticeUserCurrentSong->mode.length());
			packet = (char*)out.GetData();
		}
		break;

		case NOTICE_USER_ROBOT_LOGIN:
		{
			struct NoticeRobotLogin* vNoticeRobotLogin =
					(struct NoticeRobotLogin*)packhead;
			int len = 0;
			data_length = vNoticeRobotLogin->robot_list.size() * ROBOTINFO_SIZE + sizeof(int64);
			BUILDHEAD(data_length);
			out.Write64(vNoticeRobotLogin->uid);
			std::list<struct RobotInfo*>::iterator it = vNoticeRobotLogin->robot_list.begin();
			for(;it!=vNoticeRobotLogin->robot_list.end();it++){
				out.Write64((*it)->uid);
				out.Write64((*it)->latitude);
				out.Write64((*it)->longitude);
				out.WriteData((*it)->nickname,NICKNAME_LEN);
			}
			packet = (char*)out.GetData();
		}
		break;
		case SCHEDULER_LOGIN:
		{
			struct SchedulerLogin* vSchedulerLogin =
					(struct SchedulerLogin*)packhead;
			BUILDHEAD(SCHEDULER_LOGIN_SIZE);
			out.Write64(vSchedulerLogin->platform_id);
			out.WriteData(vSchedulerLogin->machine_id.c_str(),
					vSchedulerLogin->machine_id.length());
			packet = (char*)out.GetData();
		}
		break;
		case ROBOT_LOGIN:
		{
			struct RobotLogin* vRobotLogin =
					(struct RobotLogin*)packhead;
			BUILDHEAD(ROBOT_LOGIN_SIZE);
			out.Write64(vRobotLogin->platform_id);
			out.Write64(vRobotLogin->uid);
			out.Write64(vRobotLogin->robot_id);
			packet = (char*)out.GetData();
		}
		break;
		case NOTICE_USER_ROBOT_HANDSEL_SONG:
		{
			struct NoticeUserRobotHandselSong* vNoticeUserRobotHandselSong =
					(struct NoticeUserRobotHandselSong*)packhead;
			BUILDHEAD(NOTICEUSERROBOTHANDSELSONG_SIZE);
			out.Write64(vNoticeUserRobotHandselSong->platform_id);
			out.Write64(vNoticeUserRobotHandselSong->uid);
			out.Write64(vNoticeUserRobotHandselSong->robot_id);
			out.Write64(vNoticeUserRobotHandselSong->song_id);
			packet = (char*)out.GetData();
		}
		break;
		case NOTICE_USER_ROBOT_LISTEN_SONG:
		{
			struct NoticeUserListenSong* vNoticeUserListenSong =
					(struct NoticeUserListenSong*)packhead;
			BUILDHEAD(NOTICEUSERROBOTLISTENSONG_SIZE);
			out.Write64(vNoticeUserListenSong->platform_id);
			out.Write64(vNoticeUserListenSong->songid);
			out.Write32(vNoticeUserListenSong->typid);
			out.WriteData(vNoticeUserListenSong->mode,MODE_LEN);
			out.WriteData(vNoticeUserListenSong->name,NAME_LEN);
			out.WriteData(vNoticeUserListenSong->singer,SINGER_LEN);
			packet = (char*)out.GetData();
		}
		break;
		case NOTICE_ASSISTANT_LOGIN:
		{
			struct NoticeAssistantLogin* vNoticeAssistantLogin =
					(struct NoticeAssistantLogin*)packhead;
			BUILDHEAD(NOTICEASSISTANTLOGIN_SIZE);
			out.Write64(vNoticeAssistantLogin->platform_id);
			out.Write64(vNoticeAssistantLogin->assistant_id);
			out.WriteData(vNoticeAssistantLogin->nickname,NICKNAME_LEN);
			packet = (char*)out.GetData();
		}
		break;
		case ASSISTANT_LOGIN_SUCCESS:
		{
			struct AssistantLoginSuccess* vAssistantLoginSuccess =
					(struct AssistantLoginSuccess*)packhead;
			BUILDHEAD(ASSISTANTLOGINSUCCESS_SIZE);
			out.Write64(vAssistantLoginSuccess->platform_id);
			out.Write64(vAssistantLoginSuccess->assistant_id);
			packet = (char*)out.GetData();
		}
		break;
		case NOTICE_USER_ROBOT_CHAT_LOGIN:
		case NOTICE_USER_ROBOT_SCHEDULER_CHAT_LOGIN:
		{
			struct NoticeUserRobotChatLogin* vNoticeUserRobotChatLogin =
					(struct NoticeUserRobotChatLogin*)packhead;
			BUILDHEAD(NOTICEUSERROBOTCHATLOGI_SIZE);
			out.Write64(vNoticeUserRobotChatLogin->platform_id);
			out.Write64(vNoticeUserRobotChatLogin->uid);
			out.Write64(vNoticeUserRobotChatLogin->robot_id);
			packet = (char*)out.GetData();
		}
		break;
		case NOTICE_USER_ROBOT_GIFT_LUCK:
		{
			struct NoticeUserRobotGiftLuck* vNoticeUserRobotGiftLuck =
					(struct NoticeUserRobotGiftLuck*)packhead;
			BUILDHEAD(NOTICEUSERROBOTGIFTLUCK_SIZE);
			out.Write64(vNoticeUserRobotGiftLuck->platform_id);
			out.Write64(vNoticeUserRobotGiftLuck->uid);
			out.Write32(vNoticeUserRobotGiftLuck->share_plat);
			out.Write32(vNoticeUserRobotGiftLuck->prize);
			out.WriteData(vNoticeUserRobotGiftLuck->name,NAME_LEN);
			out.WriteData(vNoticeUserRobotGiftLuck->singer,SINGER_LEN);
			packet = (char*)out.GetData();
		}
		break;
		case NOTICE_USER_READY_GIFT_LUCK:
		{
			struct NoticeUserReadyGiftLuck* vNoticeUserReadyGiftLuck =
					(struct NoticeUserReadyGiftLuck*)packhead;
			BUILDHEAD(NOTICEUSERREADYGIFTLUCK_SIZE);
			out.Write64(vNoticeUserReadyGiftLuck->platform_id);
			out.Write64(vNoticeUserReadyGiftLuck->uid);
			out.Write64(vNoticeUserReadyGiftLuck->songid);
			out.Write32(vNoticeUserReadyGiftLuck->share_plat);
			packet = (char*)out.GetData();
		}
		break;
		/*
		case NOTICE_USER_ROBOT_SCHEDULER_CHAT_LOGIN:
		{
			struct NoticeUserRobotSchedulerChatLogin* vNoticeUserRobotSchedulerChatLogin =
					(struct NoticeUserRobotSchedulerChatLogin*)packhead;
			BUILDHEAD(NOTICEUSERROBOTSCHEDULERCHATLOGIN_SIZE);
			out.Write64(vNoticeUserRobotSchedulerChatLogin->platform_id);
			out.Write64(vNoticeUserRobotSchedulerChatLogin->uid);
			out.Write64(vNoticeUserRobotSchedulerChatLogin->robot_id);
			out.Write8(vNoticeUserRobotSchedulerChatLogin->flag);
			packet = (char*)out.GetData();
		}
		break;
		*/
		case NOTICE_ASSISTANT_HANDSEL_SONG:
		{
			struct NoticeAssistantHandselSong* vNoticeAssistanHandselSong =
					(struct NoticeAssistantHandselSong*)packhead;
			int len = 0;
			data_length = vNoticeAssistanHandselSong->list.size() * HANDLESONG_SIZE + sizeof(int64) * 2;
			BUILDHEAD(data_length);
			out.Write64(vNoticeAssistanHandselSong->platform_id);
			out.Write64(vNoticeAssistanHandselSong->assistant_id);
			std::list<struct HandleSongInfo*>::iterator it = vNoticeAssistanHandselSong->list.begin();
			for(;it!=vNoticeAssistanHandselSong->list.end();it++){
				out.Write64((*it)->uid);
				out.Write64((*it)->songid);
				out.WriteData((*it)->message,MESSAGE_LEN);
			}
			packet = (char*)out.GetData();
		}
		break;
		case HEART_PACKET:
		{
			BUILDHEAD(0);
			packet = (char*)out.GetData();
		}
		break;
		default:
			r = false;
			break;
	}
	*packet_stream = (void *)packet;

	if (packhead->packet_length == PACKET_HEAD_LENGTH
		&& packhead->data_length == 0)
		*packet_stream_length = PACKET_HEAD_LENGTH;
	else
		*packet_stream_length = packet_length;
	return r;
}

#define DUMPPACKBUF     4096

#define PRINT_TITLE(v)   \
	j += snprintf (buf + j, DUMPPACKBUF - j, "\n-------- %s --------\n", v)

#define PRINT_END(v)     \
	j += snprintf (buf + j, DUMPPACKBUF - j, "-------- %s --------\n", v)

#define PRINT_INT(v)    \
	j += snprintf (buf + j, DUMPPACKBUF - j, "\t%s = %d\n", #v, (int)v)

#define PRINT_INT64(v)    \
	j += snprintf (buf + j, DUMPPACKBUF - j, "\t%s = %lld\n", #v, (int64_t)v)

#define PRINT_STRING(v) \
	j += snprintf (buf + j, DUMPPACKBUF - j, "\t%s = %s\n", #v, (const char *)v)

#define DUMPHEAD()	            \
	PRINT_INT (packet_length);	\
	PRINT_INT (operate_code);	\
	PRINT_INT (data_length);    \
	PRINT_INT (current_time);   \
	PRINT_INT (msg_type);       \
	PRINT_INT (is_zip);         \
	PRINT_INT (msg_id);         \
	PRINT_INT (reserved);       \
	if (packet_length == PACKET_HEAD_LENGTH \
	&& data_length == 0)    \
	break;


void ProtocolPack::DumpPacket(const struct PacketHead *packhead){
#if !defined DEBUG || defined _DEBUG
	int32 packet_length = packhead->packet_length;
	int32 operate_code = packhead->operate_code;
	int32 data_length = packhead->data_length;
	int32 current_time = packhead->current_time;
	int16 msg_type = packhead->msg_type;
	int8 is_zip = packhead->is_zip;
	int64 msg_id = packhead->msg_id;
	int32 reserved = packhead->reserverd;
	/*
   int32 packet_length;
   int32 operate_code;
   int32 data_length;
   int32 current_time;
   int16 msg_type;
   int8  is_zip;
   int64 msg_id;
   int32 reserverd;
	 */
	char buf[DUMPPACKBUF];
	bool r = false;
	buf[0] = '\0';
	int j = 0;

	switch(operate_code){
		case NOTICE_USER_LOGIN:
		{
			struct NoticeUserLogin*  vNoticeUserLogin =
					(struct NoticeUserLogin*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct NoticeUserLogin DumpBegin");
			PRINT_INT64(vNoticeUserLogin->platform_id);
			PRINT_INT64(vNoticeUserLogin->uid);
			PRINT_INT(vNoticeUserLogin->latitude);
			PRINT_INT(vNoticeUserLogin->longitude);
			PRINT_END("struct NoticeUserLogin DumpEnd");

		}
		break;
		case  NOTICE_USER_DEFAULT_SONG:
		{
			struct NoticeUserDefaultSong* vNoticeUserDefaultSong =
					(struct NoticeUserDefaultSong*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct NoticeUserDefaultSong DumpBegin");
			PRINT_INT64(vNoticeUserDefaultSong->platform_id);
			PRINT_INT64(vNoticeUserDefaultSong->uid);
			PRINT_INT64(vNoticeUserDefaultSong->songid);
			PRINT_INT64(vNoticeUserDefaultSong->type_id);
			PRINT_STRING(vNoticeUserDefaultSong->mode.c_str());
			PRINT_END("struct NoticeUserDefaultSong DumpEnd");
		}
		break;
		case  NOTICE_USER_CURRENT_SONG:
		{
			struct NoticeUserCurrentSong* vNoticeUserCurrentSong =
					(struct NoticeUserCurrentSong*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct NoticeUserCurrentSong DumpBegin");
			PRINT_INT64(vNoticeUserCurrentSong->platform_id);
			PRINT_INT64(vNoticeUserCurrentSong->uid);
			PRINT_INT64(vNoticeUserCurrentSong->songid);
			PRINT_INT64(vNoticeUserCurrentSong->type_id);
			PRINT_STRING(vNoticeUserCurrentSong->mode.c_str());
			PRINT_END("struct NoticeUserCurrentSong DumpEnd");
		}
		break;
		case NOTICE_USER_ROBOT_LOGIN:
		{
			struct NoticeRobotLogin* vNoticeUserLogin =
					(struct NoticeRobotLogin*)packhead;
			std::list<struct RobotInfo*>::iterator it = vNoticeUserLogin->robot_list.begin();
			DUMPHEAD();
			PRINT_TITLE("struct NoticeRobotLogin DumpBegin");
			PRINT_INT64(vNoticeUserLogin->uid);
			for(;it!=vNoticeUserLogin->robot_list.end();it++){
				PRINT_INT64((*it)->uid);
				PRINT_INT((*it)->latitude);
				PRINT_INT((*it)->longitude);
				PRINT_STRING((*it)->nickname);
			}
			PRINT_END("struct NoticeRobotLogin DumpEnd");
		}
		break;
		case ROBOT_LOGIN:
		{
			struct RobotLogin* vRobotLogin =
					(struct RobotLogin*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct RobotLogin DumpBegin");
			PRINT_INT64(vRobotLogin->platform_id);
			PRINT_INT64(vRobotLogin->uid);
			PRINT_INT64(vRobotLogin->robot_id);
			PRINT_END("struct RobotLogin DumpEnd");
		}
		break;
		case NOTICE_USER_ROBOT_HANDSEL_SONG:
		{
			struct NoticeUserRobotHandselSong* vNoticeUserRobotHandselSong =
					(struct NoticeUserRobotHandselSong*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct NoticeUserRobotHandselSong DumpBegin");
			PRINT_INT64(vNoticeUserRobotHandselSong->platform_id);
			PRINT_INT64(vNoticeUserRobotHandselSong->uid);
			PRINT_INT64(vNoticeUserRobotHandselSong->robot_id);
			PRINT_INT64(vNoticeUserRobotHandselSong->song_id);
			PRINT_END("struct NoticeUserRobotHandselSong DumpEnd");
		}
		break;
		case SCHEDULER_LOGIN:
		{
			struct SchedulerLogin* vSchedulerLogin =
					(struct SchedulerLogin*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct SchedulerLogin DumpBegin");
			PRINT_INT64(vSchedulerLogin->platform_id);
			PRINT_STRING(vSchedulerLogin->machine_id.c_str());
			PRINT_END("struct SchedulerLogin DumpEnd");
		}
		break;
		case NOTICE_USER_ROBOT_GIFT_LUCK:
		{
			struct NoticeUserRobotGiftLuck* vNoticeUserRobotGiftLuck =
					(struct NoticeUserRobotGiftLuck*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct NoticeUserRobotGiftLuck DumpBegin");
			PRINT_INT64(vNoticeUserRobotGiftLuck->platform_id);
			PRINT_INT64(vNoticeUserRobotGiftLuck->uid);
			PRINT_INT(vNoticeUserRobotGiftLuck->share_plat);
			PRINT_INT(vNoticeUserRobotGiftLuck->prize);
			PRINT_STRING(vNoticeUserRobotGiftLuck->name);
			PRINT_STRING(vNoticeUserRobotGiftLuck->singer);
			PRINT_END("struct NoticeUserRobotGiftLuck DumpEnd");
		}
		break;
		case NOTICE_USER_READY_GIFT_LUCK:
		{
			struct NoticeUserReadyGiftLuck* vNoticeUserReadyGiftLuck =
					(struct NoticeUserReadyGiftLuck*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct NoticeUserReadyGiftLuck DumpBegin");
			PRINT_INT64(vNoticeUserReadyGiftLuck->platform_id);
			PRINT_INT64(vNoticeUserReadyGiftLuck->uid);
			PRINT_INT64(vNoticeUserReadyGiftLuck->songid);
			PRINT_INT(vNoticeUserReadyGiftLuck->share_plat);
			PRINT_END("struct NoticeUserReadyGiftLuck DumpEnd");
		}
		break;
		case NOTICE_USER_ROBOT_LISTEN_SONG:
		{
			struct NoticeUserListenSong* vNoticeUserListenSong =
					(struct NoticeUserListenSong*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct NoticeUserListenSong DumpBegin");
			PRINT_INT64(vNoticeUserListenSong->platform_id);
			PRINT_INT64(vNoticeUserListenSong->songid);
			PRINT_INT(vNoticeUserListenSong->typid);
			PRINT_STRING(vNoticeUserListenSong->mode);
			PRINT_STRING(vNoticeUserListenSong->name);
			PRINT_STRING(vNoticeUserListenSong->singer);
			PRINT_END("struct NoticeUserListenSong DumpEnd");
		}
		break;
		case NOTICE_ASSISTANT_LOGIN:
		{
			struct NoticeAssistantLogin* vNoticeAssistantLogin =
					(struct NoticeAssistantLogin*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct NoticeAssistantLogin DumpBegin");
			PRINT_INT64(vNoticeAssistantLogin->platform_id);
			PRINT_INT64(vNoticeAssistantLogin->assistant_id);
			PRINT_STRING(vNoticeAssistantLogin->nickname);
			PRINT_END("struct NoticeAssistantLogin DumpEnd");
		}
		break;
		case ASSISTANT_LOGIN_SUCCESS:
		{
			struct AssistantLoginSuccess* vAssistantLoginSuccess =
					(struct AssistantLoginSuccess*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct AssistantLoginSuccess DumpBegin");
			PRINT_INT64(vAssistantLoginSuccess->platform_id);
			PRINT_INT64(vAssistantLoginSuccess->assistant_id);
			PRINT_END("struct AssistantLoginSuccess DumpEnd");
		}
		break;
		case NOTICE_USER_ROBOT_CHAT_LOGIN:
		case NOTICE_USER_ROBOT_SCHEDULER_CHAT_LOGIN:
		{
			struct NoticeUserRobotChatLogin* vNoticeUserRobotChatLogin =
					(struct NoticeUserRobotChatLogin*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct NoticeUserRobotChatLogin DumpBegin");
			PRINT_INT64(vNoticeUserRobotChatLogin->platform_id);
			PRINT_INT64(vNoticeUserRobotChatLogin->uid);
			PRINT_INT64(vNoticeUserRobotChatLogin->robot_id);
			PRINT_END("struct NoticeUserRobotChatLogin DumpEnd");
		}
		break;
		/*
		case NOTICE_USER_ROBOT_SCHEDULER_CHAT_LOGIN:
		{
			struct NoticeUserRobotSchedulerChatLogin* vNoticeUserRobotSchedulerChatLogin =
					(struct NoticeUserRobotSchedulerChatLogin*)packhead;
			DUMPHEAD();
			PRINT_TITLE("struct NoticeUserRobotSchedulerChatLogin DumpBegin");
			PRINT_INT64(vNoticeUserRobotSchedulerChatLogin->platform_id);
			PRINT_INT64(vNoticeUserRobotSchedulerChatLogin->uid);
			PRINT_INT64(vNoticeUserRobotSchedulerChatLogin->robot_id);
			PRINT_INT(vNoticeUserRobotSchedulerChatLogin->flag);
			PRINT_END("struct NoticeUserRobotSchedulerChatLogin DumpEnd");
		}
		break;
		*/
		case NOTICE_ASSISTANT_HANDSEL_SONG:
		{
			struct NoticeAssistantHandselSong* vNoticeAssistanHandselSong=
					(struct NoticeAssistantHandselSong*)packhead;
			DUMPHEAD();
			std::list<HandleSongInfo*>::iterator it = vNoticeAssistanHandselSong->list.begin();
			PRINT_TITLE("struct NoticeAssistanHandselSong DumpBegin");
			PRINT_INT64(vNoticeAssistanHandselSong->platform_id);
			PRINT_INT64(vNoticeAssistanHandselSong->assistant_id);
			for(;it!=vNoticeAssistanHandselSong->list.end();it++){
				PRINT_INT64((*it)->uid);
				PRINT_INT64((*it)->songid);
				PRINT_STRING((*it)->message);
			}
			PRINT_END("struct NoticeAssistanHandselSong DumpEnd");
		}
		break;
		case HEART_PACKET:
		{
			PRINT_TITLE("struct HEART_PACKET DumpBegin");
			PRINT_END("struct HEART_PACKET DumpEnd");
		}
		break;
		default:
			r = false;
			break;
	}
	if (buf[0]!='\0')
		LOG_DEBUG2("%s\n",buf);
#endif
}



void ProtocolPack::HexEncode(const void *bytes, size_t size){
	struct PacketHead* head = (struct PacketHead*)bytes;
	static const char kHexChars[] = "0123456789ABCDEF";
	std::string sret(size*3,'\0');
	for (size_t i = 0;i<size;++i){
		char b = reinterpret_cast<const char*>(bytes)[i];
		sret[(i*3)] = kHexChars[(b>>4) & 0xf];
		sret[(i*3)+1]=kHexChars[b&0xf];
		if ((((i*3)+2+1)%12)!=0)
			sret[(i * 3) + 2] = '\40';
		else
			sret[(i * 3) + 2] = '\n';
	}
	LOG_DEBUG2("===start====\nopcode[%d]:\n%s\n====end====\n",
			head->operate_code,sret.c_str());
}

}
