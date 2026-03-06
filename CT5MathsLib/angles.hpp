#pragma once
#include "constants.hpp"

//NOTES: these static methods for angle conversion could probably be written better 
//they only allow for the precision of a float, when a double can be used as an input argument
//user (me) may desire higher precision when passing in a higher precision datatype

namespace mfg
{
	enum angleUnit
	{
		Degrees,
		Radians
	};


	template<typename T, typename = std::enable_if_t<std::is_convertible<T, float>::value>>
	static float ToRadians(const T& degrees) //convert angle from degrees to radians
	{
		float angle = float(degrees) * (PI<float> / 180);
		return angle;
	}

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, float>::value>>
	static float ToDegrees(const T& radians) //convert angle from radians to degrees
	{
		float angle = float(radians) * (180 / PI<float>);
		return angle;
	}

	//conversion between types of angle
	template<typename T, typename = std::enable_if_t<std::is_convertible<T, float>::value>>
	static float AngleConvert(const T &angle, const angleUnit convertTo = Radians)
	{	
		float result;
		if (convertTo == mfg::Radians)
		{
			result = mfg::ToRadians(angle);
		} else
		{
			result = mfg::ToDegrees(angle);
		}
		return result;
	}
}