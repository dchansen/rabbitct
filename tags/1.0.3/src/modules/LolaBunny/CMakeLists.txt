project(LolaBunny)

##############################################################################
# Sources
##############################################################################

set(LolaBunny_SRCS_H
    ../../include/rabbitct.h
)

set(LolaBunny_SRCS_CPP
    LolaBunny.cpp
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

add_library(LolaBunny SHARED
    ${LolaBunny_SRCS_CPP}
    ${LolaBunny_SRCS_H}
)


##############################################################################
# Install
##############################################################################

#install(TARGETS LolaBunny
#	RUNTIME DESTINATION $ENV{RTK_INSTALL_PATH}/bin
#	LIBRARY DESTINATION $ENV{RTK_INSTALL_PATH}/lib
#	ARCHIVE DESTINATION $ENV{RTK_INSTALL_PATH}/lib
#)
