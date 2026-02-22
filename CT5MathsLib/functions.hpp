#pragma once

#include "constants.hpp"

namespace mfg
{
	//clamp between values (default 0 to 1)
	template<typename T>
	T Clamp(T &value, const T &min = T(0), const T &max = T(1))
	{
		if (value > max)
		{
			value = max;
		}
		if (value < min)
		{
			value = min;
		}
		return value;
	}


}