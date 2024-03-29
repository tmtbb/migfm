###############################################################################
## 输出文件名
OUTPUT_FILE = ../../bin/hfcrsrv/plugins/lbs/lbs.so

## 编译器
COMPILERC = gcc
COMPIlER = g++

LIB_DIR = ./../../../library
USER_DIR = ../../pub
CUSTOM_THIRD_PATH = $(LIB_DIR)/third_party
## 编译选项
## for debug -DROOM_DEBUG_MCACHE
## CPP_FLAG = -g -Wall -w -fPIC -c -D_LINUX -DDEBUG -D_DEBUG -D_DB_POOL_ -D_DIC_POOL_ -D_REENTRANT -D_GNU_SOURCE -DCHAR_BIT=8
## for release
CPP_FLAG = -O3 -Wall -s -w -fPIC -c -D_LINUX -DNDEBUG -D_REENTRANT -D_DB_POOL_ -D_DIC_POOL_ -D_GNU_SOURCE -DROOM_LOG_ERROR -DCHAR_BIT=8  -DROOM_LOG_MSG

## 链接选项
## for debug
LINK_FLAGS = -g -shared -fPIC
## for release
## LINK_FLAGS = -s -O3 -shared -fPIC

## 头文件
INCLUDE_PATHS = -I./ -I$(LIB_DIR)/core -I$(LIB_DIR)/public/ -I$(LIB_DIR)/third_party/hiredis/ \
	-I/usr/include/mysql -I/usr/local/include/libmemcached-1.0 \
	-I$(USER_DIR) -I$(CUSTOM_THIRD_PATH)/hiredis -I$(CUSTOM_THIRD_PATH)\

## 库
LINK_PATHS = -L./ -L/usr/lib64/mysql -L/usr/local/lib
LINK_LIBRARYS = -lcurl -lmysqlclient -lmemcached -lmemcachedutil \
	-lexpat -lrt -lpthread -ldl -liconv \
	../../lib/libbreakpad_client.a ../../lib/libhiredis.a ../../lib/robot_interface.a $(CUSTOM_THIRD_PATH)/dmg_fp/lib/dmg_fp.a

##3rd
THIRD_PATH = $(LIB_DIR)/public/
# 编译
COMPILE = $(COMPIlER) $(CPP_FLAG) $(INCLUDE_PATHS) -o $@

# 链接
LINK = $(COMPIlER) $(LINK_FLAGS) $(LINK_PATHS) $? -o $(OUTPUT_FILE) $(LINK_LIBRARYS)

all: clean output delete-temp-file
output: lbs_init.o mig_log.o lbs_logic.o http_response.o base64.o baidu_lbs_connector.o redis_connector.o \
	json_reader.o json_value.o json_writer.o  mem_storage_impl.o mysql_storage_impl.o \
	storage.o redis_storage_impl.o redis_warrper.o basic_info.o constants.o basic_util.o dic_storage.o db_storage.o\
	db_comm.o dic_comm.o logic_comm.o thread_handler.o thread_lock.o http_packet.o basictypes.o config.o
	$(LINK)

clean:
	-rm -f *.o
	-rm -f $(OUTPUT_FILE)

delete-temp-file:
	-rm -f *.o

### 编译

lbs_init.o:
	$(COMPILE) ./lbs_init.cc

mig_log.o:
	$(COMPILE) $(THIRD_PATH)/log/mig_log.cc

lbs_logic.o:
	$(COMPILE) ./lbs_logic.cc

http_response.o:
	$(COMPILE) ./http_response.cc

baidu_lbs_connector.o:
	$(COMPILE) ./baidu_lbs_connector.cc

redis_connector.o:
	$(COMPILE) ./redis_connector.cc

json_reader.o:
	$(COMPILE) $(THIRD_PATH)/json/json_reader.cpp

json_value.o:
	$(COMPILE) $(THIRD_PATH)/json/json_value.cpp

json_writer.o:
	$(COMPILE) $(THIRD_PATH)/json/json_writer.cpp

basic_info.o:
	$(COMPILE) $(THIRD_PATH)/basic/basic_info.cc
	
storage.o:
	$(COMPILE) $(THIRD_PATH)/storage/storage.cc

mysql_storage_impl.o:
	$(COMPILE) $(THIRD_PATH)/storage/mysql_storage_impl.cc

mem_storage_impl.o:
	$(COMPILE) $(THIRD_PATH)/storage/mem_storage_impl.cc

redis_storage_impl.o:
	$(COMPILE) $(THIRD_PATH)/storage/redis_storage_impl.cc

db_storage.o:
	$(COMPILE) $(THIRD_PATH)/storage/db_storage.cc

dic_storage.o:
	$(COMPILE) $(THIRD_PATH)/storage/dic_storage.cc

db_comm.o:
	$(COMPILE) ./db_comm.cc

dic_comm.o:
	$(COMPILE) ./dic_comm.cc

logic_comm.o:
	$(COMPILE) ./logic_comm.cc

thread_handler.o:
	$(COMPILE) ./thread_handler.cc

thread_lock.o:
	$(COMPILE) ./thread_lock.cc
http_packet.o:
	$(COMPILE) $(THIRD_PATH)/protocol/http_packet.cc

basictypes.o:
	$(COMPILE) $(THIRD_PATH)/basic/basictypes.cc

config.o:
	$(COMPILE) $(THIRD_PATH)/config/config.cc

constants.o:
	$(COMPILE) $(THIRD_PATH)/basic/constants.cc

basic_util.o:
	$(COMPILE) $(THIRD_PATH)/basic/basic_util.cc

base64.o:
	$(COMPILE) $(THIRD_PATH)/basic/base64.cc


redis_warrper.o:
	$(COMPILERC) -std=c99 -pedantic -c $(INCLUDE_PATHS) -O3 -fPIC  -Wall -W -Wstrict-prototypes -Wwrite-strings -g -ggdb $(THIRD_PATH)/storage/redis_warrper.c
