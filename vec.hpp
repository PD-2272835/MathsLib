#ifndef _VEC_HPP_
#define _VEC_HPP_
#include "angles.hpp"
#include "constants.hpp"
#include "matrix.hpp"
#include <cstddef> //allows use of std::size_t


//mfg should probably be renamed to something else (three letter abreviation) but for this module it should be fine
namespace mfg
{
	template<std::size_t dim, typename T>
	struct vec {

		T values[dim];

		constexpr std::size_t Dimension() const { return dim; }
		
		//slightly better xyzw accessors
		template <typename = typename std::enable_if<(dim > 0)>>
		T& x() { return (*this)[0]; }
		template <typename = typename std::enable_if<(dim > 0)>>
		const T& x() const { return (*this)[0]; }

		template <typename = typename std::enable_if<(dim > 1)>>
		T& y() { return (*this)[1]; }
		template <typename = typename std::enable_if<(dim > 1)>>
		const T& y() const { return (*this)[1]; }

		template <typename = typename std::enable_if<(dim > 2)>>
		T& z() { return (*this)[2]; }
		template <typename = typename  std::enable_if<(dim > 2)>>
		const T& z() const { return (*this)[2]; }

		template <typename = typename std::enable_if<(dim > 3)>>
		T& w() { return (*this)[3]; }
		template <typename = typename std::enable_if<(dim > 3)>>
		const T& w() const { return (*this)[3]; }



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
		template<typename... Args,
			typename = std::enable_if_t<(sizeof...(Args) == dim)>>
		vec(Args... args)
		{
			T temp[dim] = { static_cast<T>(args)... };
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = temp[i];
			}
		}


		//allow this type to be read as continuous memory
		T& operator[](std::size_t i)
		{
			return values[i];
		}

		const T& operator[](std::size_t i) const
		{
			return values[i];
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


		//copy assignment operator for vectors of a different dimension/type
		template<std::size_t D, typename type,
			typename = std::enable_if_t<std::is_convertible<type, T>::value>>
		vec& operator=(const vec<D, type> &other)
		{
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = T(other.values[i]);
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
		template<typename type, 
			typename = std::enable_if_t<std::is_convertible<type, T>::value>>
		vec<dim, T>& operator+=(const type &rhs)
		{
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = values[i] + T(rhs);
			}

			return *this;
		}

		//vector-vector per element addition (permits vectors of larger dimensions)
		template<std::size_t D, typename type, 
			typename = std::enable_if_t<std::is_convertible<type, T>::value && (D >= dim)>>
		vec& operator+=(const vec<D, type> &rhs)
		{
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = values[i] + T(rhs.values[i]);
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
		template<typename type,
			typename = std::enable_if_t<std::is_convertible<type, T>::value>>
		vec& operator-=(const type &rhs)
		{
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = values[i] - T(rhs);
			}
			return *this;
		}

		//Vector-Vector per element subtraction
		template<std::size_t D, typename type,
			typename = std::enable_if_t<std::is_convertible<type, T>::value>>
		vec& operator-=(const vec<D, type> &rhs)
		{
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = values[i] - T(rhs.values[i]);
			}
			return *this;
		}




		//allow compound multiplication/assignment
		template<typename type1>
		friend vec operator*(vec lhs, const type1 &rhs)
		{
			lhs *= rhs;
			return lhs;
		}

		//Vector Scaling (by scalar)
		template<typename type,
			typename = std::enable_if_t<std::is_convertible<type, T>::value &&
			std::is_arithmetic_v<type>>>
		vec& operator*=(const type &rhs)
		{
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] = values[i] * T(rhs);
			}
			return *this;
		}

		//Non-linear Vector Scaling
		template<typename type,
			typename = std::enable_if_t<std::is_convertible<type, T>::value>>
		vec& operator*=(const vec<dim, type>&rhs)
		{
			for (std::size_t i = 0; i < dim; ++i)
			{
				values[i] *= T(rhs.values[i]);
			}
			return *this;
		}

		//vector by matrix multiplication 
		//iterative algorithm: https://en.wikipedia.org/wiki/Matrix_multiplication
		//matrix is the lefthand symbol to preserve the order of matrix multiplication
		template<typename type, std::size_t C,
			typename = std::enable_if_t<std::is_convertible<type, T>::value>>
		friend vec operator*( mat<dim, C, type> &lhs, const vec<dim, T> &rhs)
		{
			vec<dim, T> result;
			for (std::size_t i = 0; i < dim; ++i)
			{
				T sum = T(0);
				for (std::size_t j = 0; j < dim; ++j)
				{
					sum += lhs(i, j) * rhs.values[j];
				}
				result.values[i] = sum;
			}
			return result;
		}



		//generic division
		template<typename type1, typename type2>
		friend vec operator/(type1 lhs, const type2 &rhs)
		{
			lhs /= rhs;
			return lhs;
		}

		//scalar division
		template<typename type,
			typename = std::enable_if_t<std::is_convertible<type, T>::value>>
		vec& operator/=(const type &rhs)
		{
			if (rhs != T(0))
			{
				for (std::size_t i = 0; i < dim; ++i)
				{
					values[i] = values[i] / T(rhs);
				}
			}
			return *this;
		}




		//get the magnitude of this vector
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

		//get the square magnitude of this vector
		T SqrMagnitude() const
		{
			T result = T();
			for (std::size_t i = 0; i < dim; ++i)
			{
				result += values[i] * values[i];
			}
			return result;
		}


		//Normalize this vector
		vec& Normalize()
		{
			T mag = this->Magnitude();
			*this = *this / mag;
			return *this;
		}


		//get the sqr distance from this vector to another vector
		T SqrDistance(const vec<dim, T>& other)
		{
			vec<dim, T> res = *this - other;
			return res.SqrMagnitude();
		}

		//get the distance from this vector to another vector
		T Distance(const vec<dim, T>& other)
		{
			return std::sqrt(*this->SqrDistance());
		}

	};

	//static wrapper function for magnitude
	template<std::size_t dim, typename T>
	static T Magnitude(const vec<dim, T> &vector)
	{
		return vector.Magnitude();
	}

	//normalize provided vector to unit vector of same dimension and type
	template<std::size_t dim, typename T>
	static vec<dim, T> Normalize(const vec<dim, T> &vector)
	{
		T mag = mfg::Magnitude(vector);
		vec<dim, T> result = vector / mag;
		return result;
	}

	//Dot Product between two vectors of the same size & type
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
		result.x() = (a.y() * b.z()) - (a.z() * b.y());
		result.y() = (a.z() * b.x()) - (a.x() * b.z());
		result.z() = (a.x() * b.y()) - (a.y() * b.x());
		return result;
	}

	

	template<std::size_t dim, typename T>
	static T Distance(vec<dim, T>& a, vec<dim, T>& b)
	{
		T result = mfg::Magnitude(a - b);
		return result;
	}


	//angle between two vectors of the same type + dimension
	template<std::size_t dim, typename T>
	static T AngleBetween(const vec<dim, T>& a, const vec<dim, T>& b)
	{
		T angle = std::acos(mfg::Dot(a, b) / (mfg::Magnitude(a) * mfg::Magnitude(b))); //standard arccos - maybe replace?
		return angle;
	}

	template<typename T = float>
	static vec<2, T> Vec2FromAngle(T angle, mfg::angleUnit angleType = mfg::Radians)
	{
		vec<2, T> result;
		if (angleType == mfg::Degrees)
		{
			angle = mfg::ToRadians(angle);
		}
		result[0] = std::cos(angle);
		result[1] = std::sin(angle);
		return result;
	}

	template<std::size_t dim> using highp_vec = vec<dim, long double>;
	template<std::size_t dim> using medp_vec = vec<dim, double>;
	template<std::size_t dim> using lowp_vec = vec<dim, float>;

	using vec2 = lowp_vec<2>;
	using vec3 = lowp_vec<3>;
	using vec4 = lowp_vec<4>;
}
#endif