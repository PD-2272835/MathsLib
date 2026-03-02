#pragma once

#include "vec.hpp"
#include "angles.hpp"

namespace mfg
{
	template<std::size_t columns, std::size_t rows, typename T>
	struct mat
	{
		T values[col * rows];

		constexpr std::size_t Columns() const { return columns; }
		constexpr std::size_t Rows() const { return rows; }

		//blank matrix
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
			for(std::size_t i = 0; i < columns; ++i)
			{
				for (std::size_t j = 0; j < rows; ++i)
				{
					if (i == j)
					{
						values[i + j] = val;
					}
					else {
						values[i + j] = T(0);
					}
				}
			}
		}

		//

	};

	template<std::size_t col, std::size_t row> using highp_mat = mat<col, row, long double>;
	template<std::size_t col, std::size_t row> using medp_mat = mat<col, row, double>;
	template<std::size_t col, std::size_t row> using lowp_mat = mat<col, row, float>;

	using mat4 = lowp_mat<4, 4>;
	using mat3 = lowp_mat<3, 3>;
	using mat2 = lowp_mat<2, 2>;
}