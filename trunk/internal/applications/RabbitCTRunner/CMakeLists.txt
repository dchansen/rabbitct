project(RabbitCTRunner)

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
    ../../../pub/include/rabbitct.h
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
    ../../../pub/include/
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
