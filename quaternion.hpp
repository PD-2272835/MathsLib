#ifndef _QUATERNION_HPP_
#define _QUATERNION_HPP_
#include "angles.hpp"
#include "vec.hpp"

namespace mfg
{


	template<typename T>
	struct iquat
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

		vec3 v() { return vec3(values[0], values[1], values[2]); }


		//default is Identity quat
		iquat()
		{
			values[0] = T(0);
			values[1] = T(0);
			values[2] = T(0);
			
			values[3] = T(1);
		}

		//permit a explicit identity quat
		static iquat<T> Identity()
		{
			return iquat<T>();
		}

		iquat(const iquat<T>& other)
		{
			if (&other != this)
			{
				values[0] = other.values[0];
				values[1] = other.values[1];
				values[2] = other.values[2];
				values[3] = other.values[3];
			}
		}
		
		//create quat from angle-axis
		iquat(const mfg::vec<3, T>& axis, T angle, angleUnit angleType = Radians)
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


		//same as copy constructor
		iquat& operator=(iquat<T>& other)
		{
			if (&other != this)
			{
				values[0] = other.values[0];
				values[1] = other.values[1];
				values[2] = other.values[2];
				values[3] = other.values[3];
			}
			return *this;
		}

		//inverse this quat
		iquat& Inverse()
		{
			values[0] *= -1;
			values[1] *= -1;
			values[2] *= -1;
			return *this;
		}

		//return an inverse of this quat
		iquat Inverse() const
		{
			iquat<T> r = *this;
			return r.Inverse();
		}

		//quaternion-quaternion rotation
		iquat& operator*(iquat<T>& s)
		{
			values[3] = (s.values[3] * values[3]) - Dot(s.v(), (*this).v());
			vec<3, T> v(s.values[3] * (*this).v() + values[3] * s.v() + Cross((*this).v(), s.v()));
			
			values[0] = v.values[0];
			values[1] = v.values[1];
			values[2] = v.values[2];
			return *this;
		}


	};


	//convert internal quat to different precision definitions
	using highp_quat = iquat<long double>;
	using medp_quat = iquat<double>;
	using lowp_quat = iquat<float>;

	//define standard quat
	using quat = lowp_quat;
}
#endif