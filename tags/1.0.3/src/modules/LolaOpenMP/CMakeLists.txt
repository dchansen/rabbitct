project(LolaOpenMP)


##############################################################################
# Enable OpenMP
##############################################################################

### MSVC
#########
# Visual Studio compiler
if (MSVC)
    set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /openmp")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
endif (MSVC)

### Intel
#########
if (WIN32)
    message("Compiler flags for Windows/Intel are untested.")
    # Intel compiler for Windows
    if (CMAKE_C_COMPILER_ID STREQUAL Intel)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /openmp")
    endif (CMAKE_C_COMPILER_ID STREQUAL Intel)
    if (CMAKE_CXX_COMPILER_ID STREQUAL Intel)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
    endif (CMAKE_CXX_COMPILER_ID STREQUAL Intel)
endif (WIN32)
if (UNIX)
    # Intel compiler for Linux/Unix
    if (CMAKE_C_COMPILER_ID STREQUAL Intel)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -openmp")
    endif (CMAKE_C_COMPILER_ID STREQUAL Intel)
    if (CMAKE_CXX_COMPILER_ID STREQUAL Intel)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -openmp")
    endif (CMAKE_CXX_COMPILER_ID STREQUAL Intel)
endif (UNIX)

### GCC
#########
# Assuming that gcc/g++ have the same parameter format on all OSs
if (CMAKE_COMPILER_IS_GNUCC)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
endif (CMAKE_COMPILER_IS_GNUCC)
if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif (CMAKE_COMPILER_IS_GNUCXX)

	
##############################################################################
# Sources
##############################################################################

set(LolaOpenMP_SRCS_H
    ../../include/rabbitct.h
)

set(LolaOpenMP_SRCS_CPP
    LolaOpenMP.cpp
)


##############################################################################
# Includes
##############################################################################

include_directories(
    ${RabbitCT_INCLUDE_DIRECTORIES}
)


##############################################################################
# Linking
##############################################################################

link_directories(
    ${RabbitCT_LINK_DIRECTORIES}
)


##############################################################################
# Targets
##############################################################################

add_library(LolaOpenMP SHARED
    ${LolaOpenMP_SRCS_CPP}
    ${LolaOpenMP_SRCS_H}
)

#target_link_libraries(LolaOpenMP ${OMP_LIBRARIES})


##############################################################################
# Install
##############################################################################

#install(TARGETS LolaOpenMP
#	RUNTIME DESTINATION $ENV{RTK_INSTALL_PATH}/bin
#	LIBRARY DESTINATION $ENV{RTK_INSTALL_PATH}/lib
#	ARCHIVE DESTINATION $ENV{RTK_INSTALL_PATH}/lib
#)

