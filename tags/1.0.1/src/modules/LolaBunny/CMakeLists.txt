project(LolaBunny)

##############################################################################
# Sources
##############################################################################

set(LolaBunny_SRCS_H
    ../../include/rabbitct.h
)

set(LolaBunny_SRCS_CPP
    source/LolaBunny.cpp
)


##############################################################################
# Includes
##############################################################################

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${LolaBunny_INCLUDES}
    ${RabbitCT_INCLUDE_DIRECTORIES}
)


##############################################################################
# Linking
##############################################################################

link_directories(
    ${LolaBunny_LINK_DIRECTORIES}
    ${RabbitCT_LINK_DIRECTORIES}
)

#message("LolaBunny link libraries: ${LolaBunny_LINK_LIBRARIES}")


##############################################################################
# Targets
##############################################################################

add_library(LolaBunny SHARED
    ${LolaBunny_SRCS_CPP}
    ${LolaBunny_SRCS_H}
)

target_link_libraries(LolaBunny
    ${LolaBunny_LINK_LIBRARIES}
)


##############################################################################
# Install
##############################################################################

#install(TARGETS LolaBunny
#	RUNTIME DESTINATION $ENV{RTK_INSTALL_PATH}/bin
#	LIBRARY DESTINATION $ENV{RTK_INSTALL_PATH}/lib
#	ARCHIVE DESTINATION $ENV{RTK_INSTALL_PATH}/lib
#)
