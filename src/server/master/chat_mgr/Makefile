
###############################################################################
## 输出文件名
OUTPUT_FILE = ./../../bin/hfcrsrv/plugins/chat_manager/chat_manager.so

## 编译器
COMPIlER = g++
COMPILERC = gcc
## 编译选项
## for debug -DROOM_DEBUG_MCACHE
CPP_FLAG = -g -Wall -w -fPIC -c -D_LINUX -DDEBUG -D_DEBUG -D_DB_POOL_ -D_REENTRANT -D_GNU_SOURCE -DROOM_LOG_ERROR -DROOM_LOG_MSG
## for release
## CPP_FLAG = -O3 -Wall -s -w -fPIC -c -D_LINUX -DNDEBUG -D_REENTRANT -D_GNU_SOURCE -DROOM_LOG_ERROR -DROOM_LOG_MSG

## 链接选项
## for debug
LINK_FLAGS = -g -shared -fPIC
## for release
## LINK_FLAGS = -s -O3 -shared -fPIC

## 头文件
INCLUDE_PATHS = -I./ -I/usr/local/libmemcached-1.0.7/include/ -I../core -I../../public/ -I/usr/local/include\
				 -I/usr/include/mysql -I./../../../third_party/hiredis 

## 库
LINK_PATHS = -L./ -L/usr/local/libmemcached-1.0.7/lib/  -L/usr/lib64/mysql
LINK_LIBRARYS = -lmemcached -lmemcachedutil  -lexpat -lrt -lpthread -ldl -liconv -lmysqlclient -lcurl\
				../../lib/libbreakpad_client.a ../../lib/libhiredis.a

##3rd
THIRD_PATH = ../../public
# 编译
COMPILE = $(COMPIlER) $(CPP_FLAG) $(INCLUDE_PATHS) -o $@

# 链接
LINK = $(COMPIlER) $(LINK_FLAGS) $(LINK_PATHS) $? -o $(OUTPUT_FILE) $(LINK_LIBRARYS)

all: clean output delete-temp-file
output:	dic_comm.o logic_comm.o thread_handler.o chat_mgr_engine.o \
	chat_mgr_init.o thread_lock.o db_comm.o message_mgr.o srv_mgr.o\
	basic_info.o http_packet.o basictypes.o config.o \
	mig_log.o mem_storage_impl.o mysql_storage_impl.o redis_storage_impl.o\
	redis_warrper.o storage.o constants.o basic_util.o errno_comm.o json_reader.o\
	json_value.o json_writer.o
	$(LINK)

clean:
	-rm -f *.o
	-rm -f $(OUTPUT_FILE)

delete-temp-file:
	-rm -f *.o

### 编译
chat_mgr_engine.o:
	$(COMPILE) ./chat_mgr_engine.cc
	
chat_mgr_init.o:
	$(COMPILE) ./chat_mgr_init.cc
	
message_mgr.o:
	$(COMPILE) ./message_mgr.cc

srv_mgr.o:
	$(COMPILE) ./srv_mgr.cc
		
db_comm.o:
	$(COMPILE) ./db_comm.cc

dic_comm.o:
	$(COMPILE) ./dic_comm.cc

logic_comm.o:
	$(COMPILE) ./logic_comm.cc

thread_handler.o:
	$(COMPILE) ./thread_handler.cc

music_mgr_engine.o:
	$(COMPILE) ./music_mgr_engine.cc

music_mgr_init.o:
	$(COMPILE) ./music_mgr_init.cc

music_recording.o:
	$(COMPILE) ./music_recording.cc

get_song.o:
	$(COMPILE) ./get_song.cc

http_response.o:
	$(COMPILE) ./http_response.cc

music_cache_mgr.o:
	$(COMPILE) ./music_cache_mgr.cc

thread_lock.o:
	$(COMPILE) ./thread_lock.cc

basic_info.o:
	$(COMPILE) ./../../public/basic/basic_info.cc

http_packet.o:
	$(COMPILE) ./../../public/protocol/http_packet.cc


basictypes.o:
	$(COMPILE) ./../../public/basic/basictypes.cc

config.o:
	$(COMPILE) ./../../public/config/config.cc

mig_log.o:
	$(COMPILE) ./../../public/log/mig_log.cc

file_path.o:
	$(COMPILE) ./../../public/file/file_path.cc
	
file_util.o:
	$(COMPILE) ./../../public/file/file_util.cc

string_serialization.o:
	$(COMPILE) ./../../public/file/string_serialization.cc
	
mem_storage_impl.o:
	$(COMPILE) ./../../public/storage/mem_storage_impl.cc

mysql_storage_impl.o:
	$(COMPILE) ./../../public/storage/mysql_storage_impl.cc

redis_storage_impl.o:
	$(COMPILE) ./../../public/storage/redis_storage_impl.cc

redis_warrper.o:
	$(COMPILERC) -std=c99 -pedantic -c $(INCLUDE_PATHS) -O3 -fPIC  -Wall -W -Wstrict-prototypes -Wwrite-strings -g -ggdb ./../../public/storage/redis_warrper.c

storage.o:
	$(COMPILE) ./../../public/storage/storage.cc

algorithm_base.o:
	$(COMPILE) ./../../public/algorithm/algorithm_base.cc

constants.o:
	$(COMPILE) ./../../public/basic/constants.cc

basic_util.o:
	$(COMPILE) ./../../public/basic/basic_util.cc

errno_comm.o:
	$(COMPILE) ./../../public/basic/errno_comm.cc

json_reader.o:
	$(COMPILE) ./../../public/json/json_reader.cpp

json_value.o:
	$(COMPILE) ./../../public/json/json_value.cpp

json_writer.o:
	$(COMPILE) ./../../public/json/json_writer.cpp

