// PROJECT INCLUDES
// 
#include "rct_timer.h" // ISA

#ifdef __RCT_OS_WINDOWS
// Windows specific code
	#include <iostream>
    #include <time.h>

int gettimeofday(struct timeval* tv, struct timezone* tz)
{
    FILETIME ft;
    unsigned __int64 tmpres = 0;
    static int tzflag;

    if (NULL != tv) {
		GetSystemTimeAsFileTime(&ft);

		tmpres |= ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |= ft.dwLowDateTime;

		/*converting file time to unix epoch*/
		tmpres /= 10;  /*convert into microseconds*/
		tmpres -= DELTA_EPOCH_IN_MICROSECS; 
		tv->tv_sec  = (long)(tmpres / 1000000UL);
		tv->tv_usec = (long)(tmpres % 1000000UL);
    }

    if (NULL != tz) {
		std::cerr << "gettimeofday: Error, timezone argument not supported. Must be NULL." << std::endl;
#if 0
		if (!tzflag) {
			_tzset();
			++tzflag;
		}
		tz->tz_minuteswest = _timezone / 60;
		tz->tz_dsttime     = _daylight;
#endif
    }

    return 0;
}

#endif // __RCT_OS_WINDOWS
