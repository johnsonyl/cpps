#include "cpps.h"

namespace cpps
{
	cpps_integer	cpps_time_gettickcount();
	static cpps_integer _holdrand = cpps_time_gettickcount();

	cpps_integer  rand_ex()
	{
		return(((_holdrand = _holdrand * 214072853
			+ 259186743) >> 32) & 0x7fffffff);
	}
	void srandex(cpps_integer seed)
	{

#ifdef _WIN32
#ifdef _M_X64
		unsigned __int64 Flag = 0;
		Flag = seed;
#else
		usint32 Flag = 0;
		__asm{
			rdtsc
			mov Flag, eax
		}
#endif

#else
		cpps_integer Flag = 0;
		Flag = seed;
#endif
		_holdrand = seed;
		srand((unsigned int)Flag);
	}

	cpps_integer  cpps_math_abs64(cpps_value value)
	{
		cpps_integer num = cpps_to_integer(value);
		return (num >= 0 ? num : -num);
	}
	void cpps_math_srand(cpps_integer seed)
	{
		srandex(seed);
	}
	cpps_integer cpps_math_rand()
	{
		return rand_ex();
	}
	cpps_value	cpps_math_min(cpps_value _a, cpps_value _b)
	{
		return (((_a) < (_b)) ? (_a) : (_b));
	}
	cpps_value	cpps_math_max(cpps_value _a, cpps_value _b)
	{
		return (((_a) > (_b)) ? (_a) : (_b));
	}
	cpps_value cpps_math_random(cpps_value _min, cpps_value _max)
	{
		if (_min.tt == CPPS_TNUMBER || _max.tt == CPPS_TNUMBER)
		{
			cpps_integer tempMin = cpps_number2integer(cpps_to_number(cpps_math_min(_min, _max)) * 100.0f);
			cpps_integer tempMax = cpps_number2integer(cpps_to_number(cpps_math_max(_min, _max)) * 100.0f);

			cpps_integer r = cpps_math_rand() % ((tempMax + 1) - tempMin) + tempMin;

			return cpps_to_number(r) / 100.0f;
		}

		cpps_integer tempMin = cpps_to_integer(cpps_math_min(_min, _max));
		cpps_integer tempMax = cpps_to_integer(cpps_math_max(_min, _max));

		cpps_integer r = cpps_math_rand() % ((tempMax + 1) - tempMin) + tempMin;

		return r;
	}
	void cpps_regmath(C *c)
	{
		module(c,"math")[
			def("abs", cpps_math_abs64),
			def("srand", cpps_math_srand),
			def("rand", cpps_math_rand),
			def("random", cpps_math_random)
		];
	}
}
