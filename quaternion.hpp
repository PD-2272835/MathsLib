#ifndef _QUATERNION_HPP_
#define _QUATERNION_HPP_
#include "angles.hpp"
#include "vec.hpp"

namespace mfg
{
	//last component of values is the angle
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

		constexpr vec3 v() const { return vec3(values[0], values[1], values[2]); } //get the vector components of this quaternion


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

			mfg::vec3 nAxis = mfg::Normalize(axis);

			angle = angle / 2; //use half angle
			T sinAngle = std::sin(angle);

			//slightly more efficient than loop for vector scalar mul
			values[1] = nAxis.values[1] * sinAngle;
			values[2] = nAxis.values[2] * sinAngle;
			values[0] = nAxis.values[0] * sinAngle;

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
		iquat& operator=(const iquat<T>& other)
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
			vec<3, T> v(s.values[3] * (*this).v() + values[3] * s.v() + Cross((*this).v(), s.v()));
			values[3] = (s.values[3] * values[3]) - Dot(s.v(), (*this).v());
			
			values[0] = v.values[0];
			values[1] = v.values[1];
			values[2] = v.values[2];
			return *this;
		}


		//rotate vector by quaternion
		vec3 operator*(const vec3& vec)
		{
			iquat<T> k(vec, 0);

			vec3 pPrime = ((*this) * k * (*this).Inverse()).v();
			return pPrime;
		}


		T GetAngle()
		{
			return std::acos((*this).w()) * 2;
		}

		vec3 GetAxis()
		{
			return ((*this).v() / std::sin(std::acos((*this).w())));
		}

		T GetAngle() const
		{
			return std::acos((*this).w()) * 2;
		}

		vec3 GetAxis() const
		{
			mfg::vec3 res((*this).v());
			res /= std::sin(std::acos((*this).w()));
			return res;
		}

		//return rotation matrix from axis angle using arbitrary rotation matrix
		mat4 ToMat()
		{
			return mfg::Rotate((*this).GetAngle(), (*this).GetAxis());
		}
		
		//attempt at converting a quaternion into a rotation matrix by encoding qpq-1 into matrix
		//https://automaticaddison.com/how-to-convert-a-quaternion-to-a-rotation-matrix/
		//https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
		mat4 ToMatrix() const
		{
			mat4 res = mat4();
			T q0 = values[0];
			T q1 = values[1];
			T q2 = values[2];
			T q3 = values[3];

			res.values[0] = 1 - (2*(q0 * q0 + q1 * q1));
			res.values[1] = 2 * (q1 * q2 + q0 * q3);
			res.values[2] = 2 * (q1 * q3 - q0 * q2);

			res.values[4] = 2 * (q1 * q2 - q0 * q3);
			res.values[5] = 1 - (2 * (q0 * q0 + q2 * q2));
			res.values[6] = 2 * (q2 * q3 + q0 * q1);

			res.values[8] = 2 * (q1 * q3 + q0 * q2);
			res.values[9] = 2 * (q2 * q3 - q0 * q1);
			res.values[10] = 1 - (2 * (q0 * q0 + q3 * q3));
			
			res.values[15] = T(1);
			return res;
		}


		T Magnitude() const
		{
			T sum(0); 
			sum += values[0] * values[0];
			sum += values[1] * values[1];
			sum += values[2] * values[2];
			sum += values[3] * values[3];

			return std::sqrt(sum);
		}

	};


	//convert internal quat to different precision definitions
	using highp_quat = iquat<long double>;
	using medp_quat = iquat<double>;
	using lowp_quat = iquat<float>;

	//define standard quat
	using quat = lowp_quat;
};

#endif