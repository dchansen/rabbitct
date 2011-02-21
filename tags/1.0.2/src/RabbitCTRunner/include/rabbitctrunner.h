#ifndef __LME_RABBITCTRUNNER__H
#define __LME_RABBITCTRUNNER__H 1

#include "rabbitct.h"


// Defines
// 
#define _CRT_SECURE_NO_WARNINGS 1

#ifdef WIN32
  #define __RCT_OS_WINDOWS
#elif WIN64
  #define __RCT_OS_WINDOWS
#else
  #define __RCT_OS_LINUX
#endif

// System includes
// 
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>



#define  US_ROUND(x) (floorf((x)+0.5f))

#ifdef __RCT_OS_LINUX
  #include <sys/time.h>
  #include <sys/types.h>
  #include <dlfcn.h>
#endif
#ifdef __RCT_OS_WINDOWS
  #include <windows.h>
  #include <time.h>
#endif



// Typedefs
// 
typedef bool (*TfncLoadAlgorithm)(RabbitCtGlobalData *);
typedef bool (*TfncAlgorithmIteration)(RabbitCtGlobalData *);
typedef bool (*TfncUnloadAlgorithm)(RabbitCtGlobalData *);
typedef bool (*TfncFinishAlgorithm)(RabbitCtGlobalData *);

typedef unsigned long long int Ttime;


// Types
// 
struct RabbitCtHeader
{
	unsigned int img_sz[2];			// projection image dimension
	unsigned int glb_numImg;		// number of projection images
	float	     glb_HUScale[2];		// HU scaling factors
};





#endif // __LME_RABBITCTRUNNER__H
