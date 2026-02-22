#pragma once

namespace mfg
{
	//not exactly typesafe but the user shouldn't be trying to get pi<char>
	//(I'm the user)
	template<typename T> 
	constexpr T PI = T(3.1415926535897932385);


}