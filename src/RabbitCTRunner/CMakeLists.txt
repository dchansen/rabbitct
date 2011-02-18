project(RabbitCTRunner)

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


##############################################################################
# Compile flags
##############################################################################

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl")
endif (CMAKE_COMPILER_IS_GNUCXX)


##############################################################################
# Sources
##############################################################################

set(RabbitCTRunner_SRCS_H
    ../include/rabbitct.h
    include/rabbitctrunner.h
    include/rabbit_progress.h
    include/rct_module_loader.h
    include/rct_timer.h
    include/CImg.h
)

set(RabbitCTRunner_SRCS_TPP
)

set(RabbitCTRunner_SRCS_CPP
    source/rabbitctrunner.cpp
    source/rct_module_loader.cpp
    source/rct_timer.cpp
)


##############################################################################
# Includes
##############################################################################

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${RabbitCTRunner_INCLUDES}
    ${RabbitCT_INCLUDE_DIRECTORIES}
    ../include/
)


##############################################################################
# Linking
##############################################################################

link_directories(
    ${RabbitCTRunner_LINK_DIRECTORIES}
    ${RabbitCT_LINK_DIRECTORIES}
)

#message("RabbitCTRunner link libraries: ${RabbitCTRunner_LINK_LIBRARIES}")


##############################################################################
# Targets
##############################################################################

add_executable(RabbitCTRunner
    ${RabbitCTRunner_SRCS_CPP} 
    ${RabbitCTRunner_SRCS_TPP} 
    ${RabbitCTRunner_SRCS_H}
)

target_link_libraries(RabbitCTRunner
    ${RabbitCTRunner_LINK_LIBRARIES}
)


##############################################################################
# Install
##############################################################################

#install(TARGETS RabbitCTRunner
#	RUNTIME DESTINATION $ENV{RTK_INSTALL_PATH}/bin
#	LIBRARY DESTINATION $ENV{RTK_INSTALL_PATH}/lib
#	ARCHIVE DESTINATION $ENV{RTK_INSTALL_PATH}/lib
#)
