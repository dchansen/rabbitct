project(RabbitCT_Modules)

##############################################################################
# Enable some compiler optimizations
##############################################################################

### MSVC
########

# fp: floating point model
# arch: cpu architecture
# /O2 is equivalent to: /Og /Oi /Ot /Oy /Ob2 /Gs /GF /Gy
# /Ox is even more aggressive, http://msdn.microsoft.com/en-us/library/19z1t1wy.aspx
set (MSVC_CXX_OPT_FLAGS "/fp:fast /O2")
if (!CMAKE_CL_64)
# on x86 (not x64!) you should add /arch:SSE2
set (MSVC_CXX_OPT_FLAGS "${MSVC_CXX_OPT_FLAGS} /arch:SSE2")
endif(!CMAKE_CL_64)
# if supported (VS 2010, and Intel Sandy Bridge processor or AMD Bulldozer processor) you should add /arch:AVX
#set (MSVC_CXX_OPT_FLAGS "${MSVC_CXX_OPT_FLAGS} /arch:AVX")

### GCC
########
set (GCC_C_OPT_FLAGS   "-O3 -msse2 -march=nocona -finline-functions")
set (GCC_CXX_OPT_FLAGS "${GCC_C_OPT_FLAGS}")

### ICC
########
if (WIN32)
    message("Optimization flags for Windows/Intel are untested.")
    set (ICC_C_OPT_FLAGS    "/fast")
    set (ICPC_CXX_OPT_FLAGS "${ICC_C_OPT_FLAGS}")
endif(WIN32)
if (UNIX)
    # -fast means: enable -xT -O3 -ipo -no-prec-div -static
    #set (ICC_C_OPT_FLAGS    "-xT -O3 -ipo -mtune=core2 -march=core2 -msse3")
    set (ICC_C_OPT_FLAGS    "-fast")
    set (ICPC_CXX_OPT_FLAGS "${ICC_C_OPT_FLAGS}")
endif(UNIX)


# apply optimization flags

# MSVC
if (MSVC)
    set (CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   ${MSVC_CXX_OPT_FLAGS}")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_CXX_OPT_FLAGS}")
endif (MSVC)

# GCC
if (CMAKE_COMPILER_IS_GNUCC)
    set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${GCC_C_OPT_FLAGS}")
endif (CMAKE_COMPILER_IS_GNUCC)
if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${GCC_CXX_OPT_FLAGS}")
endif (CMAKE_COMPILER_IS_GNUCXX)

# Intel
if (${CMAKE_C_COMPILER} MATCHES "icc")
    set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${ICC_C_OPT_FLAGS}")
endif (${CMAKE_C_COMPILER} MATCHES "icc")
if (${CMAKE_CXX_COMPILER} MATCHES "icpc")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${ICPC_CXX_OPT_FLAGS}")
endif (${CMAKE_CXX_COMPILER} MATCHES "icpc")

# default to release build if no build type is specified
if (CMAKE_BUILD_TYPE STREQUAL "")
    set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${CMAKE_C_FLAGS_RELEASE}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
endif(CMAKE_BUILD_TYPE STREQUAL "")


##############################################################################
# Compiler flags
##############################################################################

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -shared")
endif (CMAKE_COMPILER_IS_GNUCXX)


##############################################################################
# Subdirectories
##############################################################################

subdirs(
	LolaBunny
	LolaOpenMP
	)

