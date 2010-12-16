project(RabbitCT_pub)

##############################################################################
# Subdirectories
##############################################################################

subdirs(modules)


##############################################################################
# Includes
##############################################################################

set(RabbitCT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


##############################################################################
# Custom find modules
##############################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/FindTBB/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/FindCUDA/")