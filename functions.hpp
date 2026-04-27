#ifndef _FUNCTIONS_HPP_
#define _FUNCTIONS_HPP_

#include "constants.hpp"
#include "vec.hpp"
#include "matrix.hpp"
#include <string>

namespace mfg
{
	//clamp between values (default 0 to 1)
	template<typename T>
	static T Clamp(T& value, const T& min = T(0), const T& max = T(1))
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

	//convert a vector to a formatted string
	template<std::size_t dim, typename T>
	static std::string VecToString(const mfg::vec<dim, T>& vector)
	{
		std::string res = "(";
		for (std::size_t i = 0; i < dim; ++i)
		{
			res += std::to_string(vector.values[i]) + ", ";
		}

		res = res.substr(0, res.size() - 2); //remove ", "  from end
		res += ")"; //replace with close bracket

		return res;
	}


	//convert a matrix to a formatted string
	template<std::size_t R, std::size_t C, typename T>
	static std::string MatToString(const mfg::mat<R, C, T>& matrix)
	{
		std::string res;
		for (std::size_t i = 0; i < R; ++i)
		{
			std::string line = "( ";
			for (std::size_t j = 0; j < C; ++j)
			{
				line += std::to_string(matrix.values[(j * R) + i]) + ", ";
			}
			line += ")\n";
			res += line;
		}
		return res;
	}


	//return lowest value from a or b
	//NOTE: returns b if values are the same
	template<typename T,
		std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
	T Min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	}

	//return highest value from a or b
	//NOTE: returns b if values are the same
	template<typename T,
		std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
	T Max(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	}

}

#endif