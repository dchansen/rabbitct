// PROJECT INCLUDES
// 
#include "rct_module_loader.h" // ISA
#include "rabbitct.h" // USES


using namespace std;


// global variables
// 
TfncLoadAlgorithm	s_fncLoadAlgorithm;
TfncUnloadAlgorithm	s_fncUnloadAlgorithm;
TfncUnloadAlgorithm	s_fncFinishAlgorithm;
TfncAlgorithmIteration	s_fncAlgorithmIteration;


#ifdef __RCT_OS_WINDOWS
    HMODULE s_mod;
#endif // __RCT_OS_WINDOWS

#ifdef __RCT_OS_LINUX
    void* s_mod;
#endif // __RCT_OS_LINUX


#ifdef __RCT_OS_WINDOWS
// Windows specific code

bool loadSharedLibraryFunctions(const char * slpath)
{
	s_mod = LoadLibrary(slpath);

	if (s_mod == NULL)
	{
		cerr << "Failed to load algorithm module " << slpath << endl;
		return false;
	}

	if (!(s_fncLoadAlgorithm = (TfncLoadAlgorithm) GetProcAddress(s_mod, RCT_FNCN_LOADALGORITHM)))
	{
		cerr << "Failed to locate module function: " << RCT_FNCN_LOADALGORITHM << endl;
		return false;
	}

	if (!(s_fncUnloadAlgorithm = (TfncUnloadAlgorithm) GetProcAddress(s_mod, RCT_FNCN_UNLOADALGORITHM)))
	{
		cerr << "Failed to locate module function: " << RCT_FNCN_UNLOADALGORITHM << endl;
		return false;
	}

	if (!(s_fncFinishAlgorithm = (TfncFinishAlgorithm) GetProcAddress(s_mod, RCT_FNCN_FINISHALGORITHM)))
	{
		cerr << "Failed to locate module function: " << RCT_FNCN_FINISHALGORITHM << endl;
		return false;
	}

	if (!(s_fncAlgorithmIteration = (TfncAlgorithmIteration) GetProcAddress(s_mod, RCT_FNCN_ALGORITHMBACKPROJ)))
	{
		cerr << "Failed to locate module function: " << RCT_FNCN_ALGORITHMBACKPROJ << endl;
		return false;
	}

	return true;
}

void releaseSharedLibrary()
{
	if (s_mod)
	{
		FreeLibrary(s_mod);
		s_mod = NULL;
	}
}

#endif // Windows specific code



#ifdef __RCT_OS_LINUX
// Linux specific code

bool loadSharedLibraryFunctions(const char * slpath)
{
	s_mod = dlopen(slpath, RTLD_LAZY);

	if (!s_mod)
	{
		cerr << "Failed to load algorithm module " << slpath << endl;
		return false;
	}

	// reset errors
	dlerror();


	if (!(s_fncLoadAlgorithm = (TfncLoadAlgorithm) dlsym(s_mod, RCT_FNCN_LOADALGORITHM)))
	{
		cerr << "Failed to locate module function: " << RCT_FNCN_LOADALGORITHM << endl;
		return false;
	}

	if (!(s_fncUnloadAlgorithm = (TfncUnloadAlgorithm) dlsym(s_mod, RCT_FNCN_UNLOADALGORITHM)))
	{
		cerr << "Failed to locate module function: " << RCT_FNCN_UNLOADALGORITHM << endl;
		return false;
	}

	if (!(s_fncFinishAlgorithm = (TfncFinishAlgorithm) dlsym(s_mod, RCT_FNCN_FINISHALGORITHM)))
	{
		cerr << "Failed to locate module function: " << RCT_FNCN_FINISHALGORITHM << endl;
		return false;
	}

	if (!(s_fncAlgorithmIteration = (TfncAlgorithmIteration) dlsym(s_mod, RCT_FNCN_ALGORITHMBACKPROJ)))
	{
		cerr << "Failed to locate module function: " << RCT_FNCN_ALGORITHMBACKPROJ << endl;
		return false;
	}

	return true;
}

void releaseSharedLibrary()
{
	if (s_mod)
	{
// Intel icpc does not unload dyn libs, if they have threads running
// This is the case for OpenMP!
// Since unloading the lib is the very last action of RabbitCTRunner it's fine to skip it.
//		dlclose(s_mod);
		s_mod = NULL;
	}
}
#endif // Linux specific code
