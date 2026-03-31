#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_
#include <iostream>
#include "angles.hpp"
#include "vec.hpp"


namespace mfg
{
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

		//initialize along identity
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


		//functor for accessing the matrix values by index (row then column)
		//column multiplied by rows - rows is the stride between each element in a column
		T& operator()(std::size_t row, std::size_t col) {
			
			if ((row < 0 || col < 0) || (row >= rows || col >= columns))
			{
				std::cerr << "Trying to access matrix element outside of range, returned first element\nindex is: (" << row << "," << col << ") max allowed is: (" << rows-1 << "," << columns-1 << ")\n";
				return this->values[0];
			}
			return this->values[(col * rows) + row];
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

		

		//combining matrices through multiplication
		//iterative algorithm: https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm
		//this is a const method as matrix multiplication can produce a resulting matrix of a different size
		template<std::size_t R, std::size_t C, typename type,
			typename = std::enable_if_t<(columns == R) && std::is_convertible<type, T>::value>>
		mat<rows, C, T>& operator*(mat<R, C, type> &rhs) const
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



	};

	template<std::size_t col, std::size_t row> using highp_mat = mat<col, row, long double>;
	template<std::size_t col, std::size_t row> using medp_mat = mat<col, row, double>;
	template<std::size_t col, std::size_t row> using lowp_mat = mat<col, row, float>;

	using mat4 = lowp_mat<4, 4>;
	using mat3 = lowp_mat<3, 3>;
	using mat2 = lowp_mat<2, 2>;
}
#endif