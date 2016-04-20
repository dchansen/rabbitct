# Introduction #

Some tools, e.g. GPU profilers, might require you to statically link RabbitCTRunner with your CUDA/OpenCL/whatever module. Here are some easy steps to accomplish that.

# Details #

  * make clean
  * Build a static library from your module.<br />In your module's `CMakeLists.txt` replace SHARED with STATIC:
```
add_library(YourModuleName STATIC [source files]
```
  * Override the module loading code.<br />Manually declare RabbitCT module functions and assign function pointers in `rct_module_loader.cpp`:<br />At global scope:
```
#define RCT_STATIC_LINKING 1

#if RCT_STATIC_LINKING
bool RCTLoadAlgorithm(RabbitCtGlobalData * rcgd);
bool RCTAlgorithmBackprojection(RabbitCtGlobalData * rcgd);
bool RCTFinishAlgorithm(RabbitCtGlobalData * rcgd);
bool RCTUnloadAlgorithm(RabbitCtGlobalData * rcgd);
#endif
```
> and, at the beginning of `loadSharedLibraryFunctions()`:
```
#if RCT_STATIC_LINKING
    s_fncLoadAlgorithm      = &RCTLoadAlgorithm;
    s_fncAlgorithmIteration = &RCTAlgorithmBackprojection;
    s_fncFinishAlgorithm    = &RCTFinishAlgorithm;
    s_fncUnloadAlgorithm    = &RCTUnloadAlgorithm;
    return true;
#endif
```
  * Don't export your module's functions.<br />In the main source file of your module, e.g. YourModuleName.cpp, define `FNCSIGN` to an empty string (after all includes):
```
// RCT_STATIC_LINKING
#undef FNCSIGN
#define FNCSIGN
```
  * Link your module and the libaries it depends on (e.g. OpenCL or TBB) to RabbitCTRunner.<br />In RabbitCTRunner's `CMakeLists.txt` (don't forget to include Find`*` scripts where necessary):
```
target_link_libraries(RabbitCTRunner
    ${RabbitCTRunner_LINK_LIBRARIES}
    ../modules/YourModuleName/Release/YourModuleName
    opencl
)
```