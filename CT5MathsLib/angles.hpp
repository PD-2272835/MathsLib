#pragma once
#include "constants.hpp"

namespace mfg
{
	enum angleUnit
	{
		Degrees,
		Radians
	};


	template<typename T>
	static T DegToRad(const T& degrees) //convert angle from degrees to radians
	{
		T angle = degrees * (PI<T> / 180);
		return angle;
	}

	template<typename T>
	static T RadToDeg(const T& radians) //convert angle from radians to degrees
	{
		T angle = radians * (180 / PI<T>);
		return angle;
	}

	//conversion between types of angle
	template<typename T>
	static T AngleConvert(const T &angle, const angleUnit convertTo = Radians)
	{	
		T result;
		if (convertTo == mfg::Radians)
		{
			result = mfg::DegToRad(angle);
		} else
		{
			result = mfg::RadToDeg(angle);
		}
		return result;
	}
}