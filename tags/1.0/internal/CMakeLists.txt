project(RabbitCT_internal)

##############################################################################
# Subdirectories
##############################################################################

subdirs(applications)


##############################################################################
# Enable some compiler optimizations
##############################################################################

# fp: floating point model
# arch: cpu architecture
set (MSVC_CXX_OPT_FLAGS "/fp:fast /arch:SSE2 /O2")

set (GCC_CXX_OPT_FLAGS  "-O3 -msse2 -march=nocona -finline-functions")

# -fast means: enable -xT -O3 -ipo -no-prec-div -static
set (ICPC_CXX_OPT_FLAGS "-xT -O3 -ipo -mtune=core2 -march=core2 -msse3")


if (MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSVC_CXX_OPT_FLAGS}")
endif (MSVC)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_OPT_FLAGS}")
endif (CMAKE_COMPILER_IS_GNUCXX)

if (${CMAKE_C_COMPILER} MATCHES "icc")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ICPC_CXX_OPT_FLAGS}")
endif (${CMAKE_C_COMPILER} MATCHES "icc")

if (${CMAKE_C_COMPILER} MATCHES "icpc")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ICPC_CXX_OPT_FLAGS}")
endif (${CMAKE_C_COMPILER} MATCHES "icpc")

