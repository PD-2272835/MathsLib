#pragma once

#include "vec.hpp"
#include "angles.hpp"

namespace mfg
{
	template<typename T, std::size_t columns, std::size_t rows>
	struct mat
	{
		T values[col * rows];

		constexpr std::size_t Columns() const { return columns; }
		constexpr std::size_t Rows() const { return rows; }

		//blank matrix
		mat()
		{
			for (std::size_t i = 0; i < (col * rows); ++i)
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

}