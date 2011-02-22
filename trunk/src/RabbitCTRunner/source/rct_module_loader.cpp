// PROJECT INCLUDES
// 
#include "rct_module_loader.h" // ISA
#include "rabbitct.h" // USES


using namespace std;


// global variables
// 
TfncLoadAlgorithm      s_fncLoadAlgorithm;
TfncPrepareAlgorithm   s_fncPrepareAlgorithm;
TfncAlgorithmIteration s_fncAlgorithmIteration;
TfncFinishAlgorithm    s_fncFinishAlgorithm;
TfncUnloadAlgorithm    s_fncUnloadAlgorithm;


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

	if (s_mod == NULL) {
		cerr << "Failed to load algorithm module " << slpath << endl;
		return false;
	}

	s_fncLoadAlgorithm = (TfncLoadAlgorithm) GetProcAddress(s_mod, RCT_FNCN_LOADALGORITHM);
	if (!s_fncLoadAlgorithm) {
		cerr << "Failed to locate function " << RCT_FNCN_LOADALGORITHM << " in module." << endl;
		return false;
	}

	s_fncPrepareAlgorithm = (TfncPrepareAlgorithm) GetProcAddress(s_mod, RCT_FNCN_PREPAREALGORITHM);
	if (!s_fncPrepareAlgorithm) {
#ifdef _DEBUG
		cout << "Notice: Function " << RCT_FNCN_PREPAREALGORITHM << " not found in module. Continuing without it." << endl;
#endif // _DEBUG
	}

	s_fncAlgorithmIteration = (TfncAlgorithmIteration) GetProcAddress(s_mod, RCT_FNCN_ALGORITHMBACKPROJ);
	if (!s_fncAlgorithmIteration) {
		cerr << "Failed to locate function " << RCT_FNCN_ALGORITHMBACKPROJ << " in module." << endl;
		return false;
	}

	s_fncFinishAlgorithm = (TfncFinishAlgorithm) GetProcAddress(s_mod, RCT_FNCN_FINISHALGORITHM);
	if (!s_fncFinishAlgorithm) {
		cerr << "Failed to locate function " << RCT_FNCN_FINISHALGORITHM << " in module." << endl;
		return false;
	}

	s_fncUnloadAlgorithm = (TfncUnloadAlgorithm) GetProcAddress(s_mod, RCT_FNCN_UNLOADALGORITHM);
	if (!s_fncUnloadAlgorithm) {
		cerr << "Failed to locate function " << RCT_FNCN_UNLOADALGORITHM << " in module." << endl;
		return false;
	}

	return true;
}

void releaseSharedLibrary()
{
	if (s_mod) {
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

	if (!s_mod) {
		cerr << "Failed to load algorithm module " << slpath << endl;
		return false;
	}

	// reset errors
	dlerror();


	if (!(s_fncLoadAlgorithm = (TfncLoadAlgorithm) dlsym(s_mod, RCT_FNCN_LOADALGORITHM))) {
		cerr << "Failed to locate function " << RCT_FNCN_LOADALGORITHM << " in module." << endl;
		return false;
	}

	if (!(s_fncPrepareAlgorithm = (TfncPrepareAlgorithm) dlsym(s_mod, RCT_FNCN_PREPAREALGORITHM))) {
#ifdef _DEBUG
		cout << "Notice: Function " << RCT_FNCN_PREPAREALGORITHM << " not found in module. Continuing without it." << endl;
#endif // _DEBUG
	}

	if (!(s_fncAlgorithmIteration = (TfncAlgorithmIteration) dlsym(s_mod, RCT_FNCN_ALGORITHMBACKPROJ))) {
		cerr << "Failed to locate function " << RCT_FNCN_ALGORITHMBACKPROJ << " in module." << endl;
		return false;
	}

	if (!(s_fncFinishAlgorithm = (TfncFinishAlgorithm) dlsym(s_mod, RCT_FNCN_FINISHALGORITHM))) {
		cerr << "Failed to locate function " << RCT_FNCN_FINISHALGORITHM << " in module." << endl;
		return false;
	}

	if (!(s_fncUnloadAlgorithm = (TfncUnloadAlgorithm) dlsym(s_mod, RCT_FNCN_UNLOADALGORITHM))) {
		cerr << "Failed to locate function " << RCT_FNCN_UNLOADALGORITHM << " in module." << endl;
		return false;
	}

	return true;
}

void releaseSharedLibrary()
{
	if (s_mod) {
// Intel icpc does not unload dyn libs, if they have threads running
// This is the case for OpenMP!
// Since unloading the lib is the very last action of RabbitCTRunner it's fine to skip it.
//		dlclose(s_mod);
		s_mod = NULL;
	}
}
#endif // Linux specific code
