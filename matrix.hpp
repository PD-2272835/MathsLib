#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_
#include <iostream>
#include "angles.hpp"
#include "vec.hpp"


namespace mfg
{
	//forward declare vector
	template<std::size_t dim, typename T> struct vec;

	//Matrices are expected in Column-Major order
	template<std::size_t rows, std::size_t columns, typename T>
	struct mat
	{
		T values[rows*columns];

		constexpr std::size_t Columns() const { return columns; }
		constexpr std::size_t Rows() const { return rows; }

		//create blank matrix
		mat()
		{
			for (std::size_t i = 0; i < columns * rows; ++i)
			{
				values[i] = T(0);
			}
		}

		//initialize along identity INCLUDES HOMOGENOUS COORDINATE
		mat(T val)
		{
			for (std::size_t i = 0; i < rows; ++i)
			{
				for(std::size_t j = 0; j < columns; ++j)
				{
					(*this)(i, j) = (i == j) ? val : T(0);
				}
			}
		}

		//initialize entire matrix with a sequence of values
		template<typename... Args,
			typename = std::enable_if_t<(sizeof...(Args) == rows * columns)>>
		mat(Args... args) //argument should be provided in column-major order
		{
			T temp[rows*columns] = { static_cast<T>(args)... };
			for (std::size_t i = 0; i < rows*columns; ++i)
			{
				values[i] = temp[i];
			}
		}

		//allow this type to be read as continuous memory
		T& operator[](std::size_t index)
		{
			if (index < 0 || index > (rows * columns)-1)
			{
				return this->values[0];
			}
			return this->values[index];
		}

		//functor for accessing the matrix values by index (row then column)
		//column multiplied by rows - rows is the stride between each element in a column
		T& operator()(std::size_t row, std::size_t col) {
			
			if ((row < 0 || col < 0) || (row >= rows || col >= columns))
			{
				std::cerr << "Trying to access matrix element outside of range, returned first element\nindex is: (" << row << "," << col << ") max allowed is: (" << rows-1 << "," << columns-1 << ")\n";
				return this->values[0];
			}
			return this->values[(col*rows) + row];
		}



		

		//combining matrices through multiplication
		//iterative algorithm: https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm
		//this is a const method as matrix multiplication can produce a resulting matrix of a different size
		template<std::size_t C, typename type,
			typename = std::enable_if_t<std::is_convertible<type, T>::value>>
		mat<rows, C, T>& operator*(const mat<columns, C, type> &rhs)
		{
			mat<rows, C, T> result;
			
			for (std::size_t i = 0; i < rows; ++i)
			{
				for (std::size_t j = 0; j < C; ++j)
				{
					T sum = T(0);
					for (std::size_t k = 0; k < columns; ++k)
					{
						sum += (*this)(i, k) * rhs(k, j);
					}
					result(i, j) = sum;
				}
			}
			return result;
		}


		//Identity Matrix - Only use for matrix assignment/initialization
		static mat Identity()
		{
			return mat(1.f);
		}


	};

	template<std::size_t R, std::size_t C, typename T>
	static mat<R, C, T>& mul(mat<R, C, T> &lhs, mat<R, C, T> &rhs)
	{
		mat<R, C, T> result;

		for (std::size_t i = 0; i < R; ++i)
		{
			for (std::size_t j = 0; j < C; ++j)
			{
				T sum = T(0);
				for (std::size_t k = 0; k < C; ++k)
				{
					sum += T(lhs[(j * R) + i]) * T(rhs[(k * R) + i]);
				}
				result(i, j) = sum;
			}
		}
		return result;
	}

	//Pack a Translation Matrix
	template <typename T>
	mat<4, 4, T> Translate(const vec<3, T> &vector)
	{
		mat<4, 4, T> r(1.f);
		r[12] = vector[0];
		r[13] = vector[1];
		r[14] = vector[2];
		return r;
	}

	//Pack a Scale Matrix
	template<typename T>
	static mat<4, 4, T> Scale(const vec<3, T>& vector)
	{
		mat<4, 4, T> r(1.f);
		r[0] = vector[0];
		r[5] = vector[1];
		r[10] = vector[2];
		return r;
	}

	//Create an arbitrary Rotation Matrix from Axis-Angle
	//using arbitrary matrix shown in learnopengl.com https://learnopengl.com/Getting-started/Transformations
	template<typename T>
	static mat<4, 4, T> Rotate(T angle, vec<3, T> axis)
	{
		axis.Normalize();
		T c = std::cos(angle);
		T s = std::sin(angle);
		T oneMinus = 1 - c;
		mat<4, 4, T> r;

		//right
		r[0] = c + (axis[0] * axis[0]) * oneMinus;
		r[1] = axis[1] * axis[0] * oneMinus + (axis[2] * s);
		r[2] = axis[2] * axis[0] * oneMinus - (axis[1] * s);
		
		//up
		r[4] = axis[0] * axis[1] * oneMinus - (axis[2] * s);
		r[5] = c + (axis[1] * axis[1]) * oneMinus;
		r[6] = axis[2] * axis[1] * oneMinus + (axis[0] * s);
				
		//front (z)
		r[8] = axis[0] * axis[2] * oneMinus + (axis[1] * s);
		r[9] = axis[1] * axis[2] * oneMinus - (axis[0] * s);
		r[10] = c + (axis[2] * axis[2]) * oneMinus;

		r[15] = 1;

		return r;
	}

	//Create A View Matrix
	template<typename T>
	static mat<4, 4, T> View(const vec<3,T> &right, const vec<3,T> &up, const vec<3,T> &front, const vec<3,T> &position = {0.f, 0.f, 0.f})
	{
		mat<4, 4, T> r;

		r[0] = right[0];
		r[1] = right[1];
		r[2] = right[2];
		
		r[4] = up[0];
		r[5] = up[1];
		r[6] = up[2];
		
		r[8] = front[0];
		r[9] = front[1];
		r[10] = front[2];
		
		r[12] = position[0];
		r[13] = position[1];
		r[14] = position[2];
		
		r[15] = 1;
		return r;
	}


	//https://stackoverflow.com/questions/8115352/glmperspective-explanation
	//Create a Perspective Matrix
	template<typename T>
	static mat<4, 4, T> Perspective(T viewAngle, T aspectRatio, T nearClip, T farClip)
	{
		mat<4, 4, T> r;
		T tanHalfAngle = std::tan(viewAngle / 2);

		r[0] = 1 / (aspectRatio * tanHalfAngle);
		r[5] = 1 / tanHalfAngle;
		r[10] = -(farClip + nearClip) / (farClip - nearClip);
		r[11] = -(2 * farClip * nearClip) / (farClip - nearClip);
		r[14] = -1;
		return r;
	}



	template<std::size_t col, std::size_t row> using highp_mat = mat<col, row, long double>;
	template<std::size_t col, std::size_t row> using medp_mat = mat<col, row, double>;
	template<std::size_t col, std::size_t row> using lowp_mat = mat<col, row, float>;

	using mat4 = lowp_mat<4, 4>;
	using mat3 = lowp_mat<3, 3>;
	using mat2 = lowp_mat<2, 2>;
}
#endif