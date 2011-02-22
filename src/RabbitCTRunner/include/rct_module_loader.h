#ifndef __LME_RCT_MODULE_LOADER__H
#define __LME_RCT_MODULE_LOADER__H 1

// PROJECT INCLUDES
// 
#include "rabbitctrunner.h" // USES (for OS detection)

#ifdef __RCT_OS_WINDOWS
	#include <windows.h>
#endif // __RCT_OS_WINDOWS


extern TfncLoadAlgorithm      s_fncLoadAlgorithm;
extern TfncPrepareAlgorithm   s_fncPrepareAlgorithm;
extern TfncAlgorithmIteration s_fncAlgorithmIteration;
extern TfncFinishAlgorithm    s_fncFinishAlgorithm;
extern TfncUnloadAlgorithm    s_fncUnloadAlgorithm;


#ifdef __RCT_OS_WINDOWS
    extern HMODULE s_mod;
#endif // __RCT_OS_WINDOWS


#ifdef __RCT_OS_LINUX
    extern void* s_mod;
#endif // __RCT_OS_LINUX


bool loadSharedLibraryFunctions(const char * slpath);
void releaseSharedLibrary();


#endif // __LME_RCT_MODULE_LOADER__H
