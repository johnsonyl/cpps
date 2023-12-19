#include "cpps/cpps.h"
namespace cpps {
	cpps_random::cpps_random()
	{
		std::random_device rd;
		generator.seed(rd());
	}

	cpps_random::~cpps_random()
	{
	}


	cpps_integer cpps_random::rand(object min, object max)
	{
		cpps_integer _min = 0;
		cpps_integer _max = MAXINT64;
		
		if (max.isint()) {
			_min = min.toint();
			_max = max.toint();
		}
		else if (min.isint()) {
			_max = min.toint();
		}
		std::uniform_int_distribution<cpps_integer> dist(_min, _max);
		return dist(generator);
	}

	cpps_number cpps_random::randf(object min, object max)
	{
#ifndef DBL_MAX
#define DBL_MAX 1.7976931348623158e+308
#endif
		cpps_number _min = 0;
		cpps_number _max = DBL_MAX;

		if (!max.isnull()) {
			_min = min.tonumber();
			_max = max.tonumber();
		}
		else if (!min.isnull()) {
			_max = min.tonumber();
		}
		std::uniform_real_distribution<cpps_number>  dist(_min, _max);
		return dist(generator);
	}

	void cpps_random::seed(object _seed)
	{
		if (_seed.isvector()) {
			std::vector<cpps_integer> _v;
			for (object vv : object::vector(_seed)) {
				_v.emplace_back(vv.toint());
			}
			std::seed_seq sseq(_v.data(), _v.data() + _v.size());
			generator.seed(sseq);
		}
		else if (_seed.isint()) {
			generator.seed(_seed.touint());
		}
	}
	void cpps_random::generate(object _v)
	{
		std::vector<cpps_uinteger> nums(object::vector(_v).size());
		std::independent_bits_engine< std::default_random_engine, sizeof(cpps_uinteger), cpps_uinteger> e;
		std::generate(nums.begin(), nums.end(), std::bind(e));

		if (_v.isvector()) {
			auto _vct = object::vector(_v);
			for (auto& v : nums) {
				_vct.realvector().emplace_back(v);
			}
		}
	}
	void cpps_regrandom(C* c)
	{
		cpps::_module(c)[
			_class<cpps_random>("Random")
				.def("rand", &cpps_random::rand)
				.def("randf", &cpps_random::randf)
				.def("seed", &cpps_random::seed)
				.def("generate", &cpps_random::generate)
		];
	}
}