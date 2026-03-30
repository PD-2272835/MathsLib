#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

namespace mfg
{
	//not exactly typesafe but we will assume the user won't be trying to get pi<char>
	template<typename T> 
	constexpr T PI = T(3.1415926535897932385);

}
#endif