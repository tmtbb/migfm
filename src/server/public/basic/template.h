#ifndef _MIG_FM_PUBLIC_BASIC_TEMPLATE_H__
#define _MIG_FM_PUBLIC_BASIC_TEMPLATE_H__
#include <map>
#include <list>
#include <vector>
#include "basic/basictypes.h"
namespace base{

	template <typename MapType, typename MapTypeIT, typename ValType>
	static bool MapGet (MapType &map, int idx, ValType &val)
	{
		MapTypeIT it = map.find (idx);
		if (it == map.end ())
			return false;
		val = it->second;
		return true;
	}

	template <typename MapType, typename ValType>
	static bool MapAdd (MapType &map, int64 idx, const ValType &val)
	{
		map[idx] = val;
		return true;
	}

	template <typename MapType, typename MapTypeIT>
	static bool MapDel (MapType &map, int idx)
	{
		MapTypeIT it = map.find (idx);
		if (it == map.end ())
			return false;
		map.erase (it);
		return true;
	}

	template <typename MapType>
	static bool MapCount (MapType &map, int &count)
	{
		count = (int) map.size ();
		return true;
	}

	template <typename MapType>
	static bool MapClr (MapType &map)
	{
		map.clear ();
		return true;
	}

}
#endif 