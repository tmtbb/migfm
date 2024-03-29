/*
 * whole_manager.cc
 *
 *  Created on: 2015年2月1日
 *      Author: pro
 */

#include "whole_manager.h"
#include "db_comm.h"
#include "music_dic_comm.h"
#include "basic/template.h"
#include "logic/pub_dic_comm.h"
#include "logic/pub_db_comm.h"
#include "logic/logic_unit.h"
#include "logic/logic_comm.h"
#include <sstream>
namespace musicsvc_logic{

WholeManager* CacheManagerOp::whole_mgr_ = NULL;
CacheManagerOp* CacheManagerOp::cache_manager_op_ = NULL;


WholeManager::WholeManager()
:base_logic::BaseWholeManager(){
	music_cache_ = new MusicCache();
	Init();
}

WholeManager::~WholeManager(){
}

void WholeManager::Init(){
	base_logic::BaseWholeManager::Init();
	CreateDimensions(10000,"ms",MOOD_NAME);
	CreateDimensions(20000,"mm",SCENE_NAME);
	CreateDimensions(30000,"chl",CHANNEL_NAME);
}

void  WholeManager::CreateDimensions(const int64 id,
		const std::string& type,const std::string& name){
	base_logic::Dimensions dimensions(id,name,type);
	basic_logic::PubDBComm::GetDimensions(type,dimensions);
	music_cache_->dimensions_[type] = dimensions;
}

void WholeManager::CreateRadomin(){
	CreateRadomin(music_cache_->channel_dimension_);
	CreateRadomin(music_cache_->mood_dimension_);
	CreateRadomin(music_cache_->scene_dimension_);
}


void WholeManager::GetDimensionInfo(netcomm_send::DimensionInfo* net_dimension){
	GetDimensionInfos("ms",net_dimension);
	GetDimensionInfos("mm",net_dimension);
	GetDimensionInfos("chl",net_dimension);
}

void WholeManager::GetDimensionInfos(const std::string& type,
		netcomm_send::DimensionInfo* net_dimension){
	base_logic::Dimensions dimensions = music_cache_->dimensions_[type];
	std::map<int64,base_logic::Dimension> dimension;
	dimensions.swap(dimension);
	GetDimensionInfoUnit(type,dimension,net_dimension);
}

void WholeManager::GetDimensionInfoUnit(const std::string& type,
		std::map<int64,base_logic::Dimension>& map,
		netcomm_send::DimensionInfo* net_dimension){

	std::map<int64,base_logic::Dimension>::iterator it =map.begin();
	for(;it!=map.end();it++){
		base_logic::Dimension info = it->second;
		if(type=="chl")
			net_dimension->set_channel(info.Release());
		else if(type=="ms")
			net_dimension->set_scens(info.Release());
		else if(type=="mm")
			net_dimension->set_mood(info.Release());
	}
}


void WholeManager::GetDimensionList(const std::string& name,const int64 id,MUSICINFO_MAP& music_list,const int64 num
		){
	//
	bool r = false;
	/*MULTI_DIMENSION_MAP multi_dimension_map;
	{
		base_logic::RLockGd lk(lock_);
		if(name=="chl")
			multi_dimension_map = music_cache_->channel_dimension_;
		else if(name=="ms")
			multi_dimension_map = music_cache_->scene_dimension_;
		else if(name=="mm")
			multi_dimension_map = music_cache_->mood_dimension_;

		DIMENSION_MAP dimension_map;
		r = base::MapGet<MULTI_DIMENSION_MAP,MULTI_DIMENSION_MAP::iterator,int64,DIMENSION_MAP>
		(multi_dimension_map,id,dimension_map);

		int i = 0;
		for(DIMENSION_MAP::iterator itr = dimension_map.begin();itr!=dimension_map.end();itr++){
			//获取是10个
			base_logic::MusicInfo info;
			//base_logic::MusicInfo info = itr->second;
			//music_list[info.id()] = info;
			info = dimension_map[0];
			music_list[info.id()] = info;
			i++;
			if(i>10)
				break;
		}

	}*/
	MULTI_DIMENSION_VEC  multi_dimension_vec;
	RADOMIN_MAP   dimension_radomin;
	std::list<int32>  radom_list;
	{
		base_logic::RLockGd lk(lock_);
		if(name=="chl"){
			multi_dimension_vec = music_cache_->channel_dimension_.dimension_vec_;
			dimension_radomin = music_cache_->channel_dimension_.dimension_radomin_;
		}
		else if(name=="ms"){
			multi_dimension_vec = music_cache_->scene_dimension_.dimension_vec_;
			dimension_radomin = music_cache_->scene_dimension_.dimension_radomin_;
		}
		else if(name=="mm"){
			multi_dimension_vec = music_cache_->mood_dimension_.dimension_vec_;
			dimension_radomin = music_cache_->mood_dimension_.dimension_radomin_;
		}
	}
	//获取随机数
	GetRadomin(dimension_radomin,id,num,radom_list);

	{

		base_logic::RLockGd lk(lock_);
		DIMENSION_VEC dimension_vec;
		r = base::MapGet<MULTI_DIMENSION_VEC,MULTI_DIMENSION_VEC::iterator,int64,DIMENSION_VEC>
		(multi_dimension_vec,id,dimension_vec);

		int i = 0;
		while(radom_list.size()>0){
			int32 radom = radom_list.front();
			radom_list.pop_front();
			base_logic::MusicInfo info;
			if(radom<=dimension_vec.size()){
				info = dimension_vec[radom];
				music_list[info.id()] = info;
			}
		}

	}
	GetMusicInfo(music_list);
}

void WholeManager::DelCollectSong(const int64 uid,const int64 songid){
	musicsvc_logic::MusicDicComm::DelCollect(uid,songid);
	bool r = false;
	MUSICINFO_MAP music_list;
	//删除缓存
	{
		base_logic::WLockGd lk(lock_);
			//写入个人红心歌单
		r = base::MapGet<MUSICINFONLIST_MAP,MUSICINFONLIST_MAP::iterator,
						int64,MUSICINFO_MAP >(music_cache_->collect_map_,uid,music_list);
		if(!r)
			return;
		r = base::MapDel<MUSICINFO_MAP,MUSICINFO_MAP::iterator,int64>(music_list,songid);
		if(!r)
			return;
		r = base::MapAdd<MUSICINFONLIST_MAP,int64,MUSICINFO_MAP>
		(music_cache_->collect_map_,uid,music_list);
	}
}


void WholeManager::SetMusicPreference(const int64 uid,base_logic::MusicInfo& music,MUSICINFONLIST_MAP& map,
		void (*redis_set)(const int64,const int64, const std::string&)){
	bool r = false;
	std::string str_json;
	MUSICINFO_MAP music_list;
	//写入redis
	music.JsonDeserialize(str_json,1);
	redis_set(uid,music.id(),str_json);
	{
		base_logic::WLockGd lk(lock_);
		//读取完整信息存入缓存
		r = base::MapGet<MUSICINFO_MAP,MUSICINFO_MAP::iterator,
				int64,base_logic::MusicInfo >(music_cache_->music_info_map_,music.id(),music);

		//写入个人红心歌单
		r = base::MapGet<MUSICINFONLIST_MAP,MUSICINFONLIST_MAP::iterator,
					int64,MUSICINFO_MAP >(map,uid,music_list);
		if(!r)
			return;
		r =base::MapAdd<MUSICINFO_MAP,int64,base_logic::MusicInfo>
		(music_list,music.id(),music);
		if(!r)
			return;
		r = base::MapAdd<MUSICINFONLIST_MAP,int64,MUSICINFO_MAP>
		(map,uid,music_list);
	}
}

void WholeManager::SetCollectSong(const int64 uid,base_logic::MusicInfo& music){
	SetMusicPreference(uid,music,music_cache_->collect_map_,musicsvc_logic::MusicDicComm::SetCollect);
	/*bool r = false;
	std::string str_json;
	//写入redis
	music.JsonDeserialize(str_json,1);
	MUSICINFO_MAP music_list;
	musicsvc_logic::MusicDicComm::SetCollect(uid,music.id(),str_json);
	{
		base_logic::WLockGd lk(lock_);
		//读取完整信息存入缓存
		r = base::MapGet<MUSICINFO_MAP,MUSICINFO_MAP::iterator,
				int64,base_logic::MusicInfo >(music_cache_->music_info_map_,music.id(),music);

		//写入个人红心歌单
		r = base::MapGet<MUSICINFONLIST_MAP,MUSICINFONLIST_MAP::iterator,
					int64,MUSICINFO_MAP >(music_cache_->collect_map_,uid,music_list);
		if(!r)
			return;
		r =base::MapAdd<MUSICINFO_MAP,int64,base_logic::MusicInfo>
		(music_list,music.id(),music);
		if(!r)
			return;
		r = base::MapAdd<MUSICINFONLIST_MAP,int64,MUSICINFO_MAP>
		(music_cache_->collect_map_,uid,music_list);
	}*/

}

void WholeManager::SetHatList(const int64 uid,base_logic::MusicInfo& music){
	SetMusicPreference(uid,music,music_cache_->hate_map_,musicsvc_logic::MusicDicComm::SetHate);
	/*bool r = false;
	std::string str_json;
	MUSICINFO_MAP music_list;
	music.JsonDeserialize(str_json,1);
	musicsvc_logic::MusicDicComm::SetHate(uid,music.id(),str_json);
	{
		base_logic::WLockGd lk(lock_);
		//读取完整信息存入缓存
		r = base::MapGet<MUSICINFO_MAP,MUSICINFO_MAP::iterator,
				int64,base_logic::MusicInfo >(music_cache_->music_info_map_,music.id(),music);

		//写入个人红心歌单
		r = base::MapGet<MUSICINFONLIST_MAP,MUSICINFONLIST_MAP::iterator,
					int64,MUSICINFO_MAP >(music_cache_->hate_map_,uid,music_list);
		if(!r)
			return;
		r =base::MapAdd<MUSICINFO_MAP,int64,base_logic::MusicInfo>
		(music_list,music.id(),music);
		if(!r)
			return;
		r = base::MapAdd<MUSICINFONLIST_MAP,int64,MUSICINFO_MAP>
		(music_cache_->hate_map_,uid,music_list);
	}*/
}

void WholeManager::CheckIsCollectSong(const int64 uid,std::list<base_logic::UserAndMusic>& infolist){
	MUSICINFO_MAP music_list;
	GetCollectList(uid,music_list);

	//遍历
	for(std::list<base_logic::UserAndMusic>:: iterator it = infolist.begin();
			it!=infolist.end();++it){
		base_logic::UserAndMusic info = (*it);
		base_logic::MusicInfo musicinfo;
		if(base::MapGet<MUSICINFO_MAP,MUSICINFO_MAP::iterator,
				int64,base_logic::MusicInfo >(music_list,info.musicinfo_.id(),musicinfo))
			info.musicinfo_.set_like(1);
	}
}


void WholeManager::GetCollectList(const int64 uid,MUSICINFO_MAP& music_list){
	GetMusicListT(uid,music_cache_->collect_map_,music_list,basic_logic::PubDicComm::GetColllectList);
	GetMusicInfo(music_list);
}

void WholeManager::GetMusicInfo(MUSICINFO_MAP& list){
//
	base_logic::RLockGd lk(lock_);
	bool r = false;
	MUSICINFO_MAP::iterator it  = list.begin();
	for(;it!=list.end();it++){
		base_logic::MusicInfo tmusicinfo = it->second;
		base_logic::MusicInfo rmusicinfo;
		//读取信息
		r = base::MapGet<MUSICINFO_MAP,MUSICINFO_MAP::iterator,
				int64,base_logic::MusicInfo >(music_cache_->music_info_map_,tmusicinfo.id(),rmusicinfo);
		if(r)
			tmusicinfo.BaseCopy(rmusicinfo);
	}
}

void WholeManager::GetMusicListT(const int64 uid,MUSICINFONLIST_MAP& container,
		MUSICINFO_MAP& music_list,void (*redis_get)(const int64,std::list<std::string>&)){
	bool r = false;
	std::list<std::string> list;
	{
		base_logic::RLockGd lk(lock_);
		//加锁
		r = base::MapGet<MUSICINFONLIST_MAP,MUSICINFONLIST_MAP::iterator,
				int64,MUSICINFO_MAP >
		(container,uid,music_list);
	}

	if(!r||music_list.size()==0)//读取
		redis_get(uid,list);
	if(list.size()<=0)//
			return ;
		//写入存储
	while(list.size()>0){//
		std::string str = list.front();
		list.pop_front();
		base_logic::MusicInfo info;
		info.JsonSeralize(str);
		music_list[info.id()] = info;
	}
	{
		base_logic::WLockGd lk(lock_);
		//加锁
		r = base::MapAdd<MUSICINFONLIST_MAP,int64,MUSICINFO_MAP >
		(container,uid,music_list);
	}
}

void WholeManager::GetRadomin(RADOMIN_MAP& dimension_radomin,const int32 id,
		int32 num,std::list<int32>& list){
	//
	base::MigRadomInV2* radomin;
	int i = 0;
	int32* rands = new int32[num];
	bool r = base::MapGet<RADOMIN_MAP,RADOMIN_MAP::iterator,
				int32,base::MigRadomInV2* >(dimension_radomin,id,radomin);
	if(!r)
		return;
	radomin->GetPrize(rands,num);
	while(i<=num){
		list.push_back(rands[i]);
		i++;
	}

	if (rands){
		delete[] rands;
		rands = NULL;
	}
}

void WholeManager::CreateRadomin(DimensionCache& dimension_cache){

	//遍历MAP
	MULTI_DIMENSION_MAP multl_dimension_map = dimension_cache.dimension_map_;
	for(MULTI_DIMENSION_MAP::iterator it = multl_dimension_map.begin();
			it!=multl_dimension_map.end();it++){
		DIMENSION_MAP dimenson_map = it->second;
		base::MigRadomInV2* radomV2 = new base::MigRadomInV2((dimenson_map.size()));
		dimension_cache.dimension_radomin_[it->first] = radomV2;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
void CacheManagerOp::FetchDicMusicBasic(){
//读取基础信息
}

void CacheManagerOp::FetchDBMusicURL(){
//读取所有URL信息
	basic_logic::PubDBComm::GetMusicURL(whole_mgr_->GetFindCache()->music_info_map_);
}


void CacheManagerOp::FetchAvailableMusicInfo(){
	basic_logic::PubDBComm::GetAvailableMusicInfo(whole_mgr_->GetFindCache()->music_info_map_);
}

void CacheManagerOp::FetchDimensionMusic(){
//读取维度信息
	std::list<base_logic::Dimension> list;
	musicsvc_logic::DBComm::GetAllDimension(list);
//读取每个维度的歌曲
	while(list.size()>0){
		base_logic::Dimension dimension = list.front();
		list.pop_front();
		FectchDimensionMusic(dimension);
	}

}



void CacheManagerOp::FectchDimensionMusic(base_logic::Dimension& dimension){
	// 获取所有歌曲
	std::list<std::string> list;
	std::stringstream os;
	os<<dimension.class_name()<<"_r"<<dimension.id();
	std::string key;
	key = os.str();
	DIMENSION_MAP  dimension_map;
	DIMENSION_VEC  dimension_vec;
	basic_logic::PubDicComm::GetDimensionMusicList(key,list);
	while(list.size()>0){
		std::string str = list.front();
		list.pop_front();
		base_logic::MusicInfo info;
		info.set_id(atoll(str.c_str()));
		info.set_class_name(dimension.class_name());
		info.set_class(dimension.id());
		//添加歌曲
		dimension_map[info.id()] = info;
		dimension_vec.push_back(info);
	}

	//根据维度添加缓存
	if(dimension.class_name()=="chl"){
		whole_mgr_->GetFindCache()->channel_dimension_.dimension_map_[dimension.id()] = dimension_map;
		whole_mgr_->GetFindCache()->channel_dimension_.dimension_vec_[dimension.id()] = dimension_vec;
	}
	else if(dimension.class_name()=="mm"){
		whole_mgr_->GetFindCache()->mood_dimension_.dimension_map_[dimension.id()] = dimension_map;
		whole_mgr_->GetFindCache()->mood_dimension_.dimension_vec_[dimension.id()] = dimension_vec;
	}
	else if(dimension.class_name()=="ms"){
		whole_mgr_->GetFindCache()->scene_dimension_.dimension_map_[dimension.id()] = dimension_map;
		whole_mgr_->GetFindCache()->scene_dimension_.dimension_vec_[dimension.id()] = dimension_vec;
	}

}

}




