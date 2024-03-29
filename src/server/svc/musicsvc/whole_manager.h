/*
 * music_cache_manager.h
 *
 *  Created on: 2015年2月1日
 *      Author: pro
 */

#ifndef MUSIC_CACHE_MANAGER_H_
#define MUSIC_CACHE_MANAGER_H_
#include "logic/base_cache_manager.h"
#include "logic/logic_unit.h"
#include "net/music_comm_head.h"
#include "logic/base_values.h"
#include <list>
#include <map>
namespace musicsvc_logic{

enum PFE{
	PFE_LIKE = 1,
	PFE_HATE = 0
};

class DimensionCache{
public:
	MULTI_DIMENSION_MAP    dimension_map_;
	MULTI_DIMENSION_VEC    dimension_vec_;
	RADOMIN_MAP            dimension_radomin_;
};


class MusicCache:public base_logic::BaseCache{
public:
	DimensionCache                channel_dimension_;
	DimensionCache                mood_dimension_;
	DimensionCache                scene_dimension_;

	std::map<std::string,base_logic::Dimensions>   dimensions_;
};

class WholeManager: public base_logic::BaseWholeManager{
public:
	WholeManager();
	~WholeManager();
public:
	void Init();
public:
	void CreateRadomin();
public:
	void GetCollectList(const int64 uid,MUSICINFO_MAP& music_list);

	void SetCollectSong(const int64 uid,base_logic::MusicInfo& music);

	void DelCollectSong(const int64 uid,const int64 songid);

	void SetHatList(const int64 uid,base_logic::MusicInfo& music);

	void CheckIsCollectSong(const int64 uid,std::list<base_logic::UserAndMusic>& infolist);

	void GetDimensionList(const std::string& name,const int64 id,MUSICINFO_MAP& music_list,
			const int64 num = 10);

	void GetDimensionInfo(netcomm_send::DimensionInfo* dimension);

	void GetDimensionInfos(const std::string& type,
			netcomm_send::DimensionInfo* net_dimension);

	void GetDimensionInfoUnit(const std::string& type,
			std::map<int64,base_logic::Dimension>& map,
			netcomm_send::DimensionInfo* net_dimension);

private:
	void GetMusicListT(const int64 uid,MUSICINFONLIST_MAP& container,
			MUSICINFO_MAP& music_list,void (*redis_get)(const int64,std::list<std::string>&));

	void CreateRadomin(DimensionCache& dimension_cache);

	void GetRadomin(RADOMIN_MAP& dimension_radomin,const int32 id,
			int32 num,std::list<int32>& list);

	void SetMusicPreference(const int64 uid,base_logic::MusicInfo& music,MUSICINFONLIST_MAP& map,
			void (*redis_set)(const int64,const int64, const std::string&));
public:
	void GetMusicInfo(MUSICINFO_MAP& list);

	void CreateDimensions(const int64 id,const std::string& type,const std::string& name);
public:
	MusicCache* GetFindCache(){return this->music_cache_;}
private:
	MusicCache*                   music_cache_;
};


//
class CacheManagerOp{
private:
	static WholeManager              *whole_mgr_;
	static CacheManagerOp            *cache_manager_op_;

	CacheManagerOp(){};
	virtual ~CacheManagerOp(){};
public:
	static WholeManager* GetWholeManager (){
		if (whole_mgr_ == NULL)
			whole_mgr_ = new WholeManager ();

		return whole_mgr_;
	}

	static CacheManagerOp* GetCacheManagerOp(){
		if(cache_manager_op_ == NULL){
			cache_manager_op_ = new CacheManagerOp();
		}
		return cache_manager_op_;
	}

public:
	static void FetchDBMusicURL();
	static void FetchDicMusicBasic();
	static void FetchAvailableMusicInfo();
	static void FetchDimensionMusic();
	//static void FetchDBMusicAbout();
private:
	static void FectchDimensionMusic(base_logic::Dimension& dimension);

};


}

#endif /* MUSIC_CACHE_MANAGER_H_ */
