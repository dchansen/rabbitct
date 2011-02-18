cmake_minimum_required(VERSION 2.4)
project(RabbitCT)

##############################################################################
# Subdirectories
##############################################################################

subdirs(RabbitCTRunner)
subdirs(modules)


##############################################################################
# Includes
##############################################################################

set(RabbitCT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    $ENV{INCLUDE}
)


##############################################################################
# Linking
##############################################################################

set(RabbitCT_LINK_DIRECTORIES
    $ENV{LIB}
)


##############################################################################
# Custom find modules
##############################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/FindTBB/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/FindCUDA/")

