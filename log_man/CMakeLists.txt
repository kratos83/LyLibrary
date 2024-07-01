project(log_man)

cmake_minimum_required(VERSION 3.5)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules )

set(log_man "log_man")


include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Sql_INCLUDE_DIRS} )
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Sql REQUIRED)


set(log_man_includes
${CMAKE_BINARY_DIR}
)

include_directories(${log_man_includes})

set(log_man_uis 
	license.ui	
)


qt5_wrap_ui(log_man_ui_hdrs ${log_man_uis})


set(log_man_moc_hdrs 
	license.h
)

qt5_wrap_cpp(log_man_mocs ${log_man_moc_hdrs})


set(log_man_srcs 
	license.cpp
)

qt5_add_resources(RESOURCE_FILES license.qrc) 


IF(WIN32)
    LINK_LIBRARIES(-lws2_32 -lwtsapi32 -lnetapi32 -luserenv -ladvapi32 ${QT_LIBRARIES})
    SET(CMAKE_MODULE_LINKER_FLAGS -Wl,-export-all-symbols)
endif(WIN32)

IF(WIN64)
ADD_DEFINITIONS(-D_X64)
ENDIF(WIN64)

LINK_LIBRARIES(-lpthread Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Sql  )


ADD_LIBRARY( log_man SHARED ${log_man_srcs} ${log_man_ui_hdrs} ${log_man_mocs} ${RESOURCE_FILES})

IF(WIN32)
    INSTALL( TARGETS log_man RUNTIME DESTINATION .)
else()
    INSTALL( TARGETS log_man LIBRARY DESTINATION ${LIB_DIR})
endif()
