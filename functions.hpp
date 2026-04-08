#ifndef _FUNCTIONS_HPP_
#define _FUNCTIONS_HPP_

#include "constants.hpp"
#include "vec.hpp"
#include <string>

namespace mfg
{
	//clamp between values (default 0 to 1)
	template<typename T>
	static T Clamp(T &value, const T &min = T(0), const T &max = T(1))
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

	template<std::size_t dim, typename T>
	static std::string VecToString(const mfg::vec<dim, T> &vector)
	{
		std::string res;
		for (std::size_t i = 0; i < dim; ++i)
		{
			res += std::to_string(i) + " " + std::to_string(vector.values[i]) + " ";
		}
		return res;
	}
}
#endif