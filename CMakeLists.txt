project(RabbitCT)

##############################################################################
# Subdirectories
##############################################################################

subdirs(internal)
subdirs(pub)


##############################################################################
# Includes
##############################################################################

set(RabbitCT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/pub/include
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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/pub/CMake/FindTBB/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/pub/CMake/FindCUDA/")

