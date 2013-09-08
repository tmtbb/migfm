#include "basic_util.h"
#include "basic/basictypes.h"
#include "storage/dic_serialization.h"
#include "basic/basic_util.h"
#include "basic/base64.h"
#include <sstream>
#include <math.h>

namespace base{

int BasicUtil::SplitStringChr( const char *str, const char *char_set,
	std::vector<std::string> &out )
{
	using std::string;

	assert(str != NULL);
	assert(char_set != NULL);

	out.clear();

	if (0 == char_set[0]) {
		if (str[0])
			out.push_back(str);
		return (int)out.size();
	}

	const char *find_ptr = NULL;
	str += ::strspn(str, char_set);
	while (str && (find_ptr=::strpbrk(str, char_set))) {
		if (str != find_ptr)
			out.push_back(string(str, find_ptr));
		str = find_ptr + ::strspn(find_ptr, char_set);
	}
	if (str && str[0])
		out.push_back(str);

	return (int)out.size();
}


 void BasicUtil::GetHttpParamElement(const char* query,const char* name,
                         std::string& value){

    char* str = const_cast<char*>(query);
    char* format_str = str;
    int32 flag = 0;
    while(*str!='\0'){
       if(*str == '='){
           size_t str_name_len = str - format_str;
           std::string str_name(format_str,str_name_len);
           if(strcmp(str_name.c_str(),name)==0){
               flag = 1;
               format_str = str;
           }
       }else if(*str=='&'&&flag){
           size_t str_value_len = str - format_str;
           value.assign(format_str+1,str_value_len-1);
           flag = 0;
           format_str = str;
       }
       str++;
    }
    if(flag){
      size_t str_value_len = str - format_str;
      value.assign(format_str+1,str_value_len);
    }
}


void BasicUtil::PaserRecordPost(const std::string& request_string,std::string& enter,
    	                          std::string& url_code,std::string& content){
    std::map<std::string,std::string> http_map;
    std::map<std::string,std::string>::iterator it;
    BasicUtil::ParserHttpRequest(request_string,http_map);
    if(http_map.size()<=0)
     	return;
    it = http_map.find("enter");
    if(it!=http_map.end())
    	enter = it->second;
    it = http_map.find("urlcode");
    if(it!=http_map.end())
        url_code = it->second;
    it = http_map.find("content");
    if(it!=http_map.end())
        content = it->second; 
}

bool BasicUtil::ParserHttpRequest(const std::string& request,
		                  std::map<std::string,std::string >& http_map){
    std::string request_str = request;
    while (request_str.length()!=0){
        int32 start_pos = request_str.find("=");
	int32 end_pos = request_str.find("&")==-1?request.length():request_str.find("&");
	std::string key = request_str.substr(0,start_pos);
	std::string value = request_str.substr(key.length()+1,end_pos - key.length()-1);
	http_map[key] = value;
	if (request_str.find("&")!=-1)
	    request_str = request_str.substr(end_pos+1,request_str.length());
	else
            request_str.clear();
    }
    return true;
}

void BasicUtil::ParserIdpPost(const std::string& request_string,std::string& sso_info,
                              std::string& username,std::string& password){
   
    int32 start_pos = request_string.find("username");
    int32 end_pos = request_string.find("&");
    
    username = request_string.substr(start_pos+9,end_pos-9);
    
    std::string temp_str;
    temp_str = request_string.substr(end_pos+1,request_string.length());
    
    start_pos = temp_str.find("password");
    end_pos = temp_str.find("&");
  
    password = temp_str.substr(start_pos+9,end_pos-9);
   
    sso_info = temp_str.substr(end_pos+1,request_string.length());
}

void BasicUtil::ParserSpPost(const std::string& request_string,std::string& samlart,std::string& relay_state){
    int32 start_pos = request_string.find("SAMLart");
    int32 end_pos =request_string.find("&");
  
    samlart = request_string.substr(start_pos+8,end_pos-8);

    std::string temp_str;
    temp_str = request_string.substr(end_pos+1,request_string.length());

    start_pos = temp_str.find("RelayState");
    end_pos = temp_str.find("&");

    relay_state = temp_str.substr(start_pos+11,end_pos-11);
}


bool BasicUtil::CheckToken(const std::string& request){
     bool r =false;
     char* mem_value = NULL;
     size_t mem_value_length = 0;
     int32 token_pos = request.find("token");
    if((token_pos==std::string::npos))
    	return false;
     std::string temp_str = request.substr(token_pos,request.length());
     int32 and_pos = temp_str.find("&");
     int32 end_pos = and_pos>0?(and_pos-6):(request.length()-6);
     
     if((and_pos==std::string::npos)||(end_pos==std::string::npos))
    	return r;
    	
     std::string token = temp_str.substr(6,end_pos);
     r = base_storage::MemDicSerial::GetString(token.c_str(),token.length(),
                                                &mem_value,&mem_value_length);
     return r;
}

const std::string& BasicUtil::FormatCurrentTime(){
    std::stringstream os;
    time_t current = time(NULL);
    struct tm* local = localtime(&current);
    os<<local->tm_year+1900<<"-"<<local->tm_mon+1<<"-"
      <<local->tm_mday<<" "<<local->tm_hour<<":"
      <<local->tm_min<<":"<<local->tm_sec;

    return os.str();
}

std::string BasicUtil::GetRawString(std::string str){
    /*std::ostringstream out;
    //out<<'';
    out<<std::hex;
    for(std::string::const_iterator it = str.begin();
            it!=str.end();++it){
    
        out<<"\\\\x"<<(static_cast<short>(*it)&0xff);
    }

    //out<<'\"';
    return out.str();*/
    return base64_encode(reinterpret_cast<const unsigned char*>(str.c_str()),str.length());
}

std::string BasicUtil::GetSrcString(std::string str){
     return base64_decode(str);
}

bool BasicUtil::UrlDecode(std::string& content,std::string& out_str){

    char  const *in_str = content.c_str();
    int32 in_str_len = strlen(in_str);
    int32 out_str_len = 0;
    char *str;

    str = strdup(in_str);
    char *dest = str;
    char *data = str;

    while (in_str_len--){
        if (*data == '+'){
            *dest = ' ';
        }
        else if (*data == '%' && in_str_len >= 2 && isxdigit((int) *(data + 1))
                                && isxdigit((int) *(data + 2))){
            *dest = (char) php_htoi(data + 1);
            data += 2;
            in_str_len -= 2;
        } else{
            *dest = *data;
        }
            data++;
            dest++;
    }
    *dest = '\0';
    out_str_len =  dest - str;
    out_str = str;
    free(str);
    return true;
}

int32 BasicUtil::php_htoi(char *s){
    int32 value;
    int32 c;
             
    c = ((unsigned char *)s)[0];
    if (isupper(c))
        c = tolower(c);
    value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;
                         
    c = ((unsigned char *)s)[1];
    if (isupper(c))
        c = tolower(c);
    
    value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;
     
    return (value);
}

bool BasicUtil::GB2312ToUTF8 (const char *input, size_t inlen, char **output, size_t *outlen)
{
	char *ib;
	char *ob;
	size_t rc;

	iconv_t cd = iconv_open ("UTF-8", "GB2312");
	if (cd == 0) {
		*output = strdup (input);
		return true;
	} else if (cd == (iconv_t)-1)
		return false;
	*outlen = inlen * 8 + 1;
	ob = *output = (char *) malloc (*outlen);
	ib = (char *) input;
	rc = iconv (cd, &ib, &inlen, &ob, outlen);
	*ob = 0;
	iconv_close (cd);
	return rc == -1 ? false : true;
}

bool BasicUtil::UTF8ToGB2312 (const char *input, size_t inlen, char **output, size_t *outlen)
{
	char *ib;
	char *ob;
	size_t rc;

	iconv_t cd = iconv_open ("GB2312", "UTF-8");
	if (cd == 0) {
		*output = strdup (input);
		return true;
	} else if (cd == (iconv_t)-1)
		return false;
	*outlen = inlen * 8 + 1;
	ob = *output = (char *) malloc (*outlen);
	ib = (char *) input;
	rc = iconv (cd, &ib, &inlen, &ob, outlen);
	*ob = 0;
	iconv_close (cd);
	return rc == -1 ? false : true;
}

double BasicUtil::CalcGEODistance(double latitude1, double longitude1,
		double latitude2, double longitude2) {
    double dd = M_PI/180;
    double x1 = latitude1 * dd;
    double y1 = longitude1 * dd;

    double x2 = latitude2 * dd;
    double y2 = longitude1 * dd;

    double R = 6371004;

    double runDistance = (2*R*asin(sqrt(2-2*cos(x1)*cos(x2)*cos(y1-y2) - 2*sin(x1)*sin(x2))/2));
    runDistance = (runDistance < 0) ? (-runDistance) : runDistance;
	return runDistance;
}

}