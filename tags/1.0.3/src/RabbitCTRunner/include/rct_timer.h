#ifndef __LME_RCT_TIMER__H
#define __LME_RCT_TIMER__H 1

// PROJECT INCLUDES
// 
#include "rabbitctrunner.h" // USES (OS detection)


#ifdef __RCT_OS_WINDOWS
	#include <windows.h>

	#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
		#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
	#else
		#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
	#endif

	struct timezone {
		int  tz_minuteswest; /* minutes W of Greenwich */
		int  tz_dsttime;     /* type of dst correction */
	};

	int gettimeofday(struct timeval* tv, struct timezone* tz);
#endif // __RCT_OS_WINDOWS


#ifdef __RCT_OS_LINUX
	// empty
#endif // __RCT_OS_LINUX


#endif // __LME_RCT_TIMER__H
