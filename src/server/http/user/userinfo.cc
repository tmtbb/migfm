#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include "client/linux/handler/exception_handler.h"

#if defined (FCGI_STD)
#include "fcgi_stdio.h"
#elif defined(FCGI_PLUS)
#include "fcgio.h"
#include "fcgi_config.h"
#endif

#include "log/mig_log.h"
#include "config/config.h"
#include "basic/basic_util.h"
#include "user_engine.h"


static bool DumpCallBack(const char* dump_path,const char* minidump_id,
			 void* contect,bool succeeded){
			 	    
    printf("Dump path:%s/%s.dump\n",dump_path,minidump_id);
    MIG_LOG(USER_LEVEL,"Dump path:%s/%s.dump\n",dump_path,minidump_id);
    return succeeded;
}

#if defined (FCGI_STD)
static void GetRequestMethod(const char* query){
    
   // char* header = "Content-type:text/html\r\n\r\n<title>FastCGI echo</title><h1>444444444</h1>\n";
    //printf("%s",header);
    //write(STDOUT_FILENO,header,strlen(header)+1);
    /*printf("Content-type: text/html\r\n"
           "\r\n"
           "<title>FastCGI echo</title>"*/
           /*"<h1>FastCGI echo</h1>\n"*/
           /*"Process ID: %d %s<p>\n"
           ,getpid(),query);*/
    std::string result;
    std::string request;
    request.assign(query);
    //check token
    bool r = base::BasicUtil::CheckToken(request);
    if(!r){    
        printf("Content-type: text/html\r\n"
           "\r\n"
           "get userinfo errno\n");
        return ;
    }

    userinfo::UserInfoEngine::GetEngine()->GetUserInfo(query,result);


    printf("Content-type: text/html\r\n"
           "\r\n"
           "%s\n"
           ,result.c_str());
    /*printf("Content-type: text/html\r\n"
           "\r\n"
           "<title>FastCGI echo</title>"
           "<h1>FastCGI echorrrr</h1>\n"
           "Process ID: %d %s %s<p>\n"
           ,getpid(),query,result.c_str());*/
    
}

static void PostRequestMethod(std::string& content){

}

static void PutRequestMethod(std::string& content){

}

static void DeleteRequestMethod(std::string& content){

}
#elif defined (FCGI_PLUS)

static void GetRequestMethod(FCGX_Request * request){
	
}

static void PostRequestMethod(FCGX_Request * request){
    char * clenstr = FCGX_GetParam("CONTENT_LENGTH", request->envp);
    if(clenstr){
        unsigned long clen = strtol(clenstr, &clenstr, 10);
        char* content = new char[clen];
        std::cin.read(content, clen);
        clen = std::cin.gcount();
       // UserMgr::GetInstance()->PostUserInfo(content,clen);
        if(content){
            delete content;
            content = NULL;
        }
    }
    return;
}

static void PutRequestMethod(FCGX_Request * request){

}

static void DeleteRequestMethod(FCGX_Request * request){

}
#endif

int main(int agrc,char* argv[]){
    
    //google_breakpad::ExceptionHandler eh(".",NULL,DumpCallBack,NULL,true);
    std::string path = "./config.xml";
    config::FileConfig file_config;
    std::string content;
    const char* query;
    bool r = false;
    MIG_INFO(USER_LEVEL,"init fastcgi id:%d",getpid());
#if defined (FCGI_PLUS)
    FCGX_Request request;
    FCGX_Init();
    FCGX_InitRequest(&request,0,0);
#endif

  //  if((!file_config.LoadConfig(path))){
       // return 1;
	//}
     r = userinfo::UserInfoEngine::GetEngine()->InitEngine(path);
     if(!r)
         return r;
#if defined (FCGI_STD)
    while(FCGI_Accept()==0){
#elif defined (FCGI_PLUS)
    while(FCGX_Accept_r(&request)==0){
#endif

#if defined (FCGI_PLUS)
    	fcgi_streambuf cin_fcgi_streambuf(request.in);
    	fcgi_streambuf cout_fcgi_streambuf(request.out);
    	fcgi_streambuf cerr_fcgi_streambuf(request.err);
    	
#if HAVE_IOSTREAM_WITHASSIGN_STREAMBUF
        std::cin  = &cin_fcgi_streambuf;
        std::cout = &cout_fcgi_streambuf;
        std::cerr = &cerr_fcgi_streambuf;
#else
        std::cin.rdbuf(&cin_fcgi_streambuf);
        std::cout.rdbuf(&cout_fcgi_streambuf);
        std::cerr.rdbuf(&cerr_fcgi_streambuf);
#endif

#endif

#if defined (FCGI_STD)
    
#if defined (TEST)
    char* request_method = "GET";
#else
    char *request_method = getenv("REQUEST_METHOD");
#endif    
    if(strcmp(request_method,"POST")==0){
        std::cin>>content;
        PostRequestMethod(content);
    }
		
    else if(strcmp(request_method,"GET")==0){
#if defined (TEST)
        query = "username=flaght@gmail.com";
#else
        query = getenv("QUERY_STRING");
#endif
        GetRequestMethod(query);
    }
		
    else if(strcmp(request_method,"PUT")==0){
        std::cin>>content;
        PutRequestMethod(content);
    }
		
    else if(strcmp(request_method,"DELETE")==0){
        std::cin>>content;
        DeleteRequestMethod(content);
    }
    
#elif defined (FCGI_PLUS)
    char *request_method = FCGX_GetParam("REQUEST_METHOD",request.envp);
	if(strcmp(request_method,"POST")==0)
		PostRequestMethod(&request);
	else if(strcmp(request_method,"GET")==0)
		GetRequestMethod(&request);
	else if(strcmp(request_method,"PUT")==0)
		PutRequestMethod(&request);
	else if(strcmp(request_method,"DELETE")==0)
		DeleteRequestMethod(&request);
#endif
    //std::stringstream output;
    //output<<"<iq id='v1' to='flaght' type='result'>"
         // <<"<vCard xmlns='vcard-temp'>";
    /*char* tmp ="<iq id='v1' to='flaght' type='result'><vCard xmlns='vcard-temp'>";
    printf("Content-type: text/html\r\n"
           "\r\n"
           "<title>FastCGI echo</title>"
           "<h1>%s</h1>\n"
           "Process ID: %d [%s]\n"
           ,tmp,getpid(),request_method);*/
    }
    
    MIG_INFO(USER_LEVEL,"deinit fastcgi id:%d",getpid());
    //UserMgr::FreeInstance();
    return 0;
}
