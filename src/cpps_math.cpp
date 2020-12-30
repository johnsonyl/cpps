#include "cpps/cpps.h"

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
	cpps_value cpps_math_acos(cpps_value x)
	{
		return acos(cpps_to_number(x));
	}
	cpps_value cpps_math_asin(cpps_value x)
	{
		return asin(cpps_to_number(x));
	}

	cpps_value cpps_math_atan(cpps_value x)
	{
		return atan(cpps_to_number(x));
	}
	cpps_value cpps_math_ceil(cpps_value x)
	{
		return ceil(cpps_to_number(x));
	}
	cpps_value cpps_math_cos(cpps_value x)
	{
		return cos(cpps_to_number(x));
	}
	cpps_value cpps_math_exp(cpps_value x)
	{
		return exp(cpps_to_number(x));
	}
	cpps_value cpps_math_floor(cpps_value x)
	{
		return floor(cpps_to_number(x));
	}
	cpps_value cpps_math_fmod(cpps_value x, cpps_value y)
	{
		return fmod(cpps_to_number(x),cpps_to_number(y));
	}
	cpps_value cpps_math_log(cpps_value x)
	{
		return log(cpps_to_number(x));
	}

	cpps_value cpps_math_sin(cpps_value x)
	{
		return sin(cpps_to_number(x));
	}
	cpps_value cpps_math_sqrt(cpps_value x)
	{
		return sqrt(cpps_to_number(x));
	}
	cpps_value cpps_math_tan(cpps_value x)
	{
		return tan(cpps_to_number(x));
	}
	cpps_number cpps_math_randf()
	{
		cpps_number ret = abs(rand_ex() % 10000000000 / 1000000001.0 - 1.0);
		ret = ret > 1.0 ? ret / 10.0 : ret;
		return ret;
	}
	void cpps_regmath(C *c)
	{
		cpps::_module(c,"math")[
			def("abs", cpps_math_abs64),
			def("srand", cpps_math_srand),
			def("randf", cpps_math_randf),
			def("rand", cpps_math_rand),
			def("random", cpps_math_random),
			def("acos", cpps_math_acos),
			def("asin", cpps_math_asin),
			def("atan", cpps_math_atan),
			def("ceil", cpps_math_ceil),
			def("cos", cpps_math_cos),
			def("exp", cpps_math_exp),
			def("floor", cpps_math_floor),
			def("fmod", cpps_math_fmod),
			defvar(c,"HUGE", HUGE_VALL),
			def("log", cpps_math_log),
			def("max", cpps_math_max),
			defvar(c, "maxinteger", MAXINT64),
			def("min", cpps_math_min),
			defvar(c, "mininteger", MININT64),
			defvar(c, "pi", M_PI),
			def("sin", cpps_math_sin),
			def("sqrt", cpps_math_sqrt),
			def("tan", cpps_math_tan)

		];
	}
}
