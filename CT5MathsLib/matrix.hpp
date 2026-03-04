#pragma once

#include "vec.hpp"
#include "angles.hpp"

namespace mfg
{
	template<std::size_t rows, std::size_t columns, typename T>
	struct mat
	{
		T values[columns * rows];

		constexpr std::size_t Columns() const { return columns; }
		constexpr std::size_t Rows() const { return rows; }

		//create blank matrix
		mat()
		{
			for (std::size_t i = 0; i < col * rows; ++i)
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
					*this(i, j) = (i == j) ? val : T(0);
				}
			}
		}


		//accessing the matrix values by index (row then column)
		//column is multiplied by rows as rows is the size of the stride between each element in a column
		T& operator()(std::size_t row, std::size_t col) {
			static_assert(row <= Rows && col <= Columns, "Trying to access an element out of this matrix's bounds");
			return values[col * Rows + row];
		}

		const T& operator()(std::size_t row, std::size_t col) const {
			static_assert(row <= Rows && col <= Columns, "Trying to access an element out of this matrix's bounds");
			return values[col * Rows + row];
		}

		
		//combining (mulitplying) matrices/vectors
		//iterative algorithm: https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm
		template<std::size_t row, std::size_t col, typename type,
			typename = std::enable_if_t<(Columns == row) && std::is_convertible<type, T>::value>>
		mat& operator*(const mat<row, col, type> &rhs) const
		{
			mat<row, col, T> result;
			
			for (std::size_t i = 0; i < Rows; ++i)
			{
				for (std::size_t j = 0; j < col; ++j)
				{
					T sum = T(0);
					for (std::size_t k = 0; k < Columns; ++k)
					{
						sum += (*this)(i, k) * T(rhs(k, j));
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