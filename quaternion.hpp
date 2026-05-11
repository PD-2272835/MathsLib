#ifndef _QUATERNION_HPP_
#define _QUATERNION_HPP_
#include "angles.hpp"
#include "vec.hpp"

namespace mfg
{


	template<typename T>
	struct quat
	{
		T values[4];

		T& x() { return (*this)[0]; }
		T& x() const { return (*this)[0]; }

		T& y() { return (*this)[1]; }
		T& y() const { return (*this)[1]; }

		T& z() { return (*this)[2]; }
		T& z() const { return (*this)[2]; }

		T& w() { return (*this)[3]; }
		T& w() const { return (*this)[3]; }


		//default is unit quat
		quat()
		{
			values[0] = T(0);
			values[1] = T(0);
			values[2] = T(0);
			
			values[3] = T(1);
		}

		quat(const quat<T>& other)
		{
			if(&other != this) std::copy(other.values[0], other.values[3], values);
		}
		
		//create quat from angle-axis
		quat(const mfg::vec<3, T>& axis, T angle, angleUnit angleType = Radians)
		{
			if (angleType != Radians) angle = ToRadians(angle);

			angle = angle / 2; //use half angle
			T sinAngle = std::sin(angle);

			//slightly more efficient than loop for vector scalar mul
			values[0] = axis * sinAngle;
			values[1] = axis * sinAngle;
			values[2] = axis * sinAngle;

			values[3] = std::cos(angle);
		}


		T& operator[](std::size_t i)
		{
			if (i < 0) return values[0];
			return values[i % 4];
		}

		const T& operator[](std::size_t i) const
		{
			if (i < 0) return values[0];
			return values[i % 4];
		}



		quat& operator=(quat<T>& other)
		{
			if (&other != this) std::copy(other.values[0], other.values[3], values);
			return *this;
		}

		//inverse this quat
		quat& Inverse()
		{
			values[0] *= -1;
			values[1] *= -1;
			values[2] *= -1;
		}

		//return an inverse of this quat
		quat& Inverse() const
		{
			quat<T> r = *this;
			return r.Inverse();
		}

	};



	using quaternion = quat<float>;
}
#endif