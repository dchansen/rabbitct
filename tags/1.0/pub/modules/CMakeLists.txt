project(RabbitCT_pub_Modules)

##############################################################################
# Subdirectories
##############################################################################

subdirs(LolaBunny)


##############################################################################
# Compiler flags
##############################################################################

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -shared")
endif (CMAKE_COMPILER_IS_GNUCXX)
