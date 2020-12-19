#include "cpps/cpps.h"


namespace cpps
{
	
	
	cpps_integer	cpps_time_gettickcount()
	{
#ifdef _WIN32
		LARGE_INTEGER TicksPerSecond = { 0 };
		LARGE_INTEGER Tick;
		if (!TicksPerSecond.QuadPart)
			QueryPerformanceFrequency(&TicksPerSecond);  
		QueryPerformanceCounter(&Tick);  
		__int64 Seconds = Tick.QuadPart / TicksPerSecond.QuadPart;  
		__int64 LeftPart = Tick.QuadPart - (TicksPerSecond.QuadPart*Seconds);  
		__int64 MillSeconds = LeftPart * 1000 / TicksPerSecond.QuadPart;
		__int64 Ret = Seconds * 1000 + MillSeconds;
		return Ret;
#else
		timespec t;
		::clock_gettime(CLOCK_MONOTONIC, &t);
		return (cpps_integer)t.tv_sec * 1000 + t.tv_nsec / 1000000;
#endif
	}
	std::string time2str(tm& t)
	{
		char szTime[64] = { 0 };
#ifdef _WIN32
		sprintf_s(szTime,64, "%04d-%02d-%02d %02d:%02d:%02d",
				t.tm_year + 1900,
				t.tm_mon + 1,
				t.tm_mday,
				t.tm_hour,
				t.tm_min,
				t.tm_sec
				);
#else
		sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d",
				  t.tm_year + 1900,
				  t.tm_mon + 1,
				  t.tm_mday,
				  t.tm_hour,
				  t.tm_min,
				  t.tm_sec
				  );
#endif
		return szTime;
	}
	tm time2tm(time_t t)
	{
#ifdef _WIN32
		struct tm tt = { 0 };
		localtime_s(&tt, &t);
		return tt;
#else
		struct tm tt = { 0 };
		localtime_r(&t, &tt);
		return tt;
#endif
	}
	std::string cpps_time_time2str(cpps_integer nt)
	{
		struct tm t = time2tm(nt);
		return time2str(t);
	}
	cpps_integer tm2time(tm* t)
	{
		return mktime(t);
	}
	void cpps_time_string_replace(std::string& strBig, const std::string & strsrc, const std::string &strdst)
	{
		std::string::size_type pos = 0;

		while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
		{
			strBig.replace(pos, strsrc.length(), strdst);
			pos += strdst.length();
		}
	}
	void	cpps_time_split_string(std::vector<int32> &o, const char *i, const char *sp)
	{
		if (strlen(i) == 0) return;

		const char *a = i; const char *b;
		while (true)
		{
			b = strstr(a, sp);

			if (!b)	b = a + strlen(a);

			std::string s;
			s.append(a, strlen(a) - strlen(b));
			o.push_back(atoi(s.c_str()));

			//如果到了结尾那就出去吧。
			if (strlen(b) == 0 || strlen(b) == 1) break;
			else a = b + 1;
		}
	}
	cpps_integer cpps_time_str2time(std::string str)
	{
		
		cpps_time_string_replace(str, ":", "-");
		cpps_time_string_replace(str, " ", "-");
		cpps_time_string_replace(str, "/", "-");

		struct tm tt = { 0 };
		std::vector<int32> temp;
		cpps_time_split_string(temp,str.c_str(), "-");

		if (temp.size() == 6)
		{
			tt.tm_year = temp[0] - 1900;
			tt.tm_mon = temp[1] - 1;
			tt.tm_mday = temp[2];
			tt.tm_hour = temp[3];
			tt.tm_min = temp[4];
			tt.tm_sec = temp[5];
		}
		return tm2time(&tt);
	}
	cpps_integer	cpps_time_gettime()
	{
		time_t result = 0;
		::time(&result);
		return result;
	}
	std::string	cpps_time_gettimestr()
	{
		return cpps_time_time2str(cpps_time_gettime());
	}
	bool		cpps_time_issomeday(cpps_integer t1, cpps_integer t2)
	{
		struct tm tm1 = time2tm(t1);
		struct tm tm2 = time2tm(t2);
		return tm1.tm_year == tm2.tm_year && tm1.tm_mon == tm2.tm_mon &&tm1.tm_mday == tm2.tm_mday;
	}


	cpps_integer cpps_time_getyear(cpps_integer t1)
	{
		struct tm t = time2tm(t1);
		return (cpps_integer)t.tm_year + 1900;
	}
	cpps_integer cpps_time_getmon(cpps_integer t1)
	{
		struct tm t = time2tm(t1);
		return (cpps_integer)t.tm_mon + 1;
	}
	cpps_integer cpps_time_getday(cpps_integer t1)
	{
		struct tm t = time2tm(t1);
		return t.tm_mday;
	}
	cpps_integer cpps_time_gethour(cpps_integer t1)
	{
		struct tm t = time2tm(t1);
		return t.tm_hour;
	}
	cpps_integer cpps_time_getmin(cpps_integer t1)
	{
		struct tm t = time2tm(t1);
		return t.tm_min;
	}
	cpps_integer cpps_time_getsec(cpps_integer t1)
	{
		struct tm t = time2tm(t1);
		return t.tm_sec;
	}
	cpps_integer cpps_time_getyday(cpps_integer t1)
	{
		struct tm t = time2tm(t1);
		return t.tm_yday;
	}
	cpps_integer cpps_time_getwday(cpps_integer t1)
	{
		struct tm t = time2tm(t1);
		return t.tm_wday;
	}
	cpps_integer cpps_time_transtime(cpps_integer y, cpps_integer m, cpps_integer d, cpps_integer h, cpps_integer mi, cpps_integer s)
	{
		struct tm tt = { 0 };

		tt.tm_year = static_cast<int>(y - 1900);
		tt.tm_mon = static_cast<int>(m - 1);
		tt.tm_mday = static_cast<int>(d);
		tt.tm_hour = static_cast<int>(h);
		tt.tm_min = static_cast<int>(mi);
		tt.tm_sec = static_cast<int>(s);

		return tm2time(&tt);
	}
	cpps_integer cpps_time_adddays(cpps_integer t, cpps_integer i)
	{
		t += i * (60 * 60 * 24);
		return t;
	}
	int cpps_time_month12[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	cpps_integer cpps_time_addmonths(cpps_integer t,cpps_integer i)
	{
		cpps_integer takeday = 0;

		for (int j = 0; j < abs(i); j++)
		{

			cpps_integer mon_day = cpps_time_month12[cpps_time_getmon(t) - 1];
			if (cpps_time_leap(cpps_time_getyear(t)) && cpps_time_getmon(t) == 2)
			{
				mon_day++;
			}
			if (abs(takeday) > mon_day)
			{
				cpps_integer a = i < 0 ? -mon_day : mon_day;
				t = cpps_time_adddays(t, a);
				takeday = takeday - a;
			}
			else
			{
				t = cpps_time_adddays(t, takeday);
				takeday = 0;
			}


			takeday += i < 0 ? -(mon_day - (cpps_time_getday(t)-1)) : cpps_time_getday(t)-1;

			cpps_integer addday = i < 0 ? -(cpps_time_getday(t)-1) : mon_day - (cpps_time_getday(t)-1);
			
			t = cpps_time_adddays(t, addday);

		}


		t = cpps_time_adddays(t, takeday);

		
		return t;
	}
	cpps_integer cpps_time_addyears(cpps_integer t, cpps_integer i)
	{
		for (int j = 0; j < abs(i); j++)
		{
			cpps_integer a = cpps_time_leap(cpps_time_getyear(t)) ? 366 * (60 * 60 * 24) :  365 * 60 * 60 * 24;
			i < 0 ? t -= a : t += a;
		}
		return t;
	}
	cpps_integer cpps_time_addweeks(cpps_integer t, cpps_integer i)
	{
		return cpps_time_adddays(t, i * 7);
	}
	cpps_integer cpps_time_addhours(cpps_integer t, cpps_integer i)
	{
		t += i * (60 * 60);
		return t;
	}
	cpps_integer cpps_time_addminutes(cpps_integer t, cpps_integer i)
	{
		t += i * 60;
		return t;
	}
	cpps_integer cpps_time_addseconds(cpps_integer t, cpps_integer i)
	{
		t += i;
		return t;
	}
	std::string cpps_time_strftime(std::string fmt, cpps_integer t)
	{
		struct tm tt = time2tm(t);
		char *str = new char[fmt.size() + 256];
		strftime(str, fmt.size() + 256, fmt.c_str(), &tt);
		std::string ret = str;
		delete [] str;
		return ret;
	}

	void cpps_regtime(C *c)
	{
		cpps::_module(c)[
			def("now", cpps_time_gettime)
		];
		cpps::_module(c, "time")[
			def("gettime", cpps_time_gettime),
			def("gettimestr", cpps_time_gettimestr),
			def("maketime", cpps_time_transtime),
			def("gettickcount", cpps_time_gettickcount),
			def("time2str", cpps_time_time2str),
			def("str2time", cpps_time_str2time),
			def("issomeday",cpps_time_issomeday),
			def("getyear", cpps_time_getyear),
			def("getmon", cpps_time_getmon),
			def("getday", cpps_time_getday),
			def("gethour", cpps_time_gethour),
			def("getmin", cpps_time_getmin),
			def("getsec", cpps_time_getsec),
			def("getyday", cpps_time_getyday),
			def("getwday", cpps_time_getwday),
			def("addmonths", cpps_time_addmonths),
			def("adddays", cpps_time_adddays),
			def("addyears", cpps_time_addyears),
			def("addweeks", cpps_time_addweeks),
			def("addhours", cpps_time_addhours),
			def("addminutes", cpps_time_addminutes),
			def("addseconds", cpps_time_addseconds),
			def("strftime", cpps_time_strftime)
		];
	}
}