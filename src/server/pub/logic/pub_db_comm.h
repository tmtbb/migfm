/*
 * pub_db_comm.h
 *
 *  Created on: 2014年12月11日
 *      Author: kerry
 */

#ifndef PUB_DB_COMM_H_
#define PUB_DB_COMM_H_
#include "logic_infos.h"
#include "config/config.h"
#include <list>

namespace basic_logic{

class PubDBComm{
public:
	PubDBComm();
	virtual ~PubDBComm();
public:
	static void Init(std::list<base::ConnAddr>& addrlist);
	static void Dest();
public:
	static void GetDimensions(const std::string& type,
			base_logic::Dimensions& dimensions);
};

}


#endif /* PUB_DB_COMM_H_ */