#ifndef _ROBOT_ROBOT_HTTP_H__
#define _ROBOT_ROBOT_HTTP_H__

#include <string>
#include <vector>
#include <map>
#include "curl/curl.h"
#include "basic/basictypes.h"
#include "log/mig_log.h"
#if defined (GOOGLE_URL)
typedef GURL            MIG_URL;
#else
typedef std::string     MIG_URL;
#endif

namespace robot_logic{
class HttpMethodGet{
public:
	HttpMethodGet(const MIG_URL& url);
    virtual ~HttpMethodGet(void);
    bool Get();
    const MIG_URL& GetUrl(void){return url_;}
    bool GetContent(std::string& content);
    void SetHeaders(std::string& value);
    bool GetHeader(const std::string& key,std::string& value);
private:
	const MIG_URL& url_;

	int code_;
	std::vector<char> content_;
	std::map<std::string,std::string> header_;
	struct curl_slist* headers_;
};

class HttpMethodPost{
public:
	HttpMethodPost(const MIG_URL& url);
    virtual ~HttpMethodPost(void){}
    bool Post(const char* post,const int port = 0);
    bool GetContent(std::string& content);
    void SetHeaders(std::string& value);
private:
	const MIG_URL& url_;

	int code_;
	std::vector<char> content_;
	std::map<std::string,std::string> header_;
	struct curl_slist* headers_;
};	
}

#endif
