#ifndef __LME_RCT_MODULE_LOADER__H
#define __LME_RCT_MODULE_LOADER__H 1

// PROJECT INCLUDES
// 
#include "rabbitctrunner.h" // USES (for OS detection)


extern TfncLoadAlgorithm	s_fncLoadAlgorithm;
extern TfncUnloadAlgorithm	s_fncUnloadAlgorithm;
extern TfncUnloadAlgorithm	s_fncFinishAlgorithm;
extern TfncAlgorithmIteration	s_fncAlgorithmIteration;


#ifdef __RCT_OS_WINDOWS
    extern HMODULE s_mod;
#endif // __RCT_OS_WINDOWS


#ifdef __RCT_OS_LINUX
    extern void* s_mod;
#endif // __RCT_OS_LINUX


bool loadSharedLibraryFunctions(const char * slpath);
void releaseSharedLibrary();


#endif // __LME_RCT_MODULE_LOADER__H
