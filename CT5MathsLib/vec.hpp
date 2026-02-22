#pragma once
#include "cmathematics.hpp"
#include "angles.hpp"
#include "constants.hpp"
#include <cmath>
#include <cstddef>



//mfg should probably be renamed to something else (three letter abreviation) but for this module it should be fine
namespace mfg
{

	template<std::size_t dim, typename T>
	struct vec {

		T values[dim];

		constexpr std::size_t Dimension() const { return dim; }

		T& x = values[0];
		T& y = values[1];
		T& z = values[2];
		T& w = values[3];


		vec()
		{
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = T(0);
			}
		}


		//initialize all vector axies to one value
		vec(T initial)
		{
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = T(initial);
			}
		}

		//construct vector with preset values that exactly match the dimension size
		template<typename... Args, typename = std::enable_if_t<(sizeof...(Args) == dim)>>
		vec(Args... args)
		{
			T temp[dim] = { static_cast<T>(args)... };
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = temp[i];
			}
		}

		vec& operator=(const vec<dim, T> &other)
		{
			if (&other != this)
			{
				for (std::size_t i = 0; i < dim; ++i)
				{
					values[i] = other.values[i];
				}
			}
			return *this;
		}

		//copy assignment operator for vectors of a different dimension
		template<std::size_t D, typename type>
		vec& operator=(const vec<D, type> &other)
		{
			static_assert(std::is_same<type, T>::value, "Vector types are not the same, please use suitable conversion");

			if (&other != this)
			{
				T temp[dim];
				temp = { static_cast<T>(other.values) };

				for (std::size_t i = 0; i < dim; ++i)
				{
					values[i] = temp[i];
				}
			}

			return *this;
		}




		//generic addition
		template<typename type1, typename type2>
		friend vec operator+(type1 lhs, const type2 &rhs)
		{
			lhs += rhs;
			return lhs;
		}

		//Scalar addition
		template<typename type>
		vec<dim, T>& operator+=(const type &rhs)
		{
			static_assert(std::is_same<type, T>::value, "Cannot perform arithmetic on incompatible types");

			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = values[i] + rhs;
			}

			return *this;
		}

		//vector addition
		template<std::size_t D, typename type>
		vec& operator+=(const vec<D, type> &rhs)
		{
			static_assert((rhs.Dimension() == dim) && (std::is_same<type, T>::value), "cannot perform arithmetic on incompatible vectors");

			for (std::size_t i = 0; i < Dimension(); ++i)
			{
				values[i] = values[i] + rhs.values[i];
			}
			return *this;
		}




		//generic subtraction
		template<typename type1, typename type2>
		friend vec operator-(const type1 lhs, const type2 &rhs)
		{
			lhs -= rhs;
			return lhs;
		}

		//Scalar subtraction
		template<typename type>
		vec& operator-=(const type &rhs)
		{
			static_assert(std::is_same<type, T>::value, "Cannot perform arithmetic on incompatible types");

			for (std::size_t i = 0; i < Dimension(); ++i)
			{
				values[i] = values[i] - rhs;
			}
			return *this;
		}

		//Vector Substraction
		template<std::size_t D, typename type>
		vec& operator-=(const vec<D, type> &rhs)
		{
			static_assert(std::is_same<type, T>::value, "Cannot perform arithmetic on incompatible vectors");

			for (std::size_t i = 0; i < Dimension(); ++i)
			{
				values[i] = values[i] - rhs.values[i];
			}
			return *this;
		}




		//generic multiplication
		template<typename type1, typename type2>
		friend vec operator*(type1 lhs, const type2 &rhs)
		{
			lhs *= rhs;
			return lhs;
		}

		//Vector Scaling (by scalar)
		template<typename type>
		vec& operator*=(const type &rhs)
		{
			static_assert(std::is_same<type, T>::value, "Cannot perform arithmetic on incompatible types");

			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = values[i] * rhs;
			}
			return *this;
		}

		//Non-linear Vector Scaling
		template<vec<dim, T> V>
		vec& operator*=(const V& rhs)
		{
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] *= rhs.Values[i];
			}
			return *this;
		}




		//generic division
		template<typename type1, typename type2>
		friend vec operator/(type1 lhs, const type2& rhs)
		{
			lhs /= rhs;
			return lhs;
		}

		//scalar division
		template<typename type>
		vec& operator/=(const type& rhs)
		{
			static_assert(std::is_same<type, T>::value, "Cannot perform arithmetic on incompatible types");

			if (rhs != 0)
			{
				for (std::size_t i = 0; i < Dimension(); ++i)
				{
					values[i] = values[i] / rhs;
				}
			}
			return *this;
		}





		T Magnitude() const
		{
			T result = T();
			for (std::size_t i = 0; i < dim; ++i)
			{
				result += values[i] * values[i];
			}
			result = std::sqrt(result); //could replace this with a custom sqrt function...perchance...?
			return result;
		}

		void Normalize() const
		{
			T mag = this->Magnitude();
			this = this / mag;
		}

	};


	template<std::size_t dim, typename T>
	static T Magnitude(const vec<dim, T> &vector)
	{
		return vector.Magnitude();
	}


	template<std::size_t dim, typename T>
	static vec<dim, T> Normalize(const vec<dim, T> &vector)
	{
		T mag = mfg::Magnitude(vector);
		vec<dim, T> result = vector / mag;
		return result;
	}

	//Dot Product
	template<std::size_t dim, typename T>
	static T Dot(const vec<dim, T> &lhs, const vec<dim, T> &rhs)
	{
		T result = T();
		for (std::size_t i = 0; i < dim; ++i)
		{
			result += lhs.values[i] * rhs.values[i];
		}
		return result;
	}

	template<std::size_t dim, typename T, typename std::enable_if_t<dim == 3, int> = 0>
	static vec<dim, T> Cross(const vec<dim, T> &a, const vec<dim, T> &b)
	{
		vec<dim, T> result;
		result.x = (a.y * b.z) - (a.z * b.y);
		result.y = (a.z * b.x) - (a.x * b.z); 
		result.z = (a.x * b.y) - (a.y * b.x);
		return result;
	}

	

	template<std::size_t dim, typename T>
	static T Distance(vec<dim, T>& a, vec<dim, T>& b)
	{
		T result = mfg::Magnitude(a - b);
		return result;
	}


	//angle between two vectors
	template<std::size_t dim, typename T>
	static T Angle(const vec<dim, T>& a, const vec<dim, T>& b)
	{
		T angle = std::acos(mfg::Dot(a, b) / (mfg::Magnitude(a) * mfg::Magnitude(b))); //standard arccos - maybe replace?
		return angle;
	}

	template<typename T>
	static vec<2, T> Vec2FromAngle(T angle, mfg::angleUnit angleType = mfg::Radians)
	{
		vec<dim, T> result;
		if (angleType == mfg::Degrees)
		{
			angle = mfg::DegToRad(angle);
		}
		result.x = std::cos(angle);
		result.y = std::sin(angle);
		return result;
	}


	template<std::size_t dim> using highp_vec = vec<dim, long double>;
	template<std::size_t dim> using medp_vec = vec<dim, double>;
	template<std::size_t dim> using lowp_vec = vec<dim, float>;

	using vec2 = lowp_vec<2>;
	using vec3 = lowp_vec<3>;
	using vec4 = lowp_vec<4>;
}
