project(initdb)

cmake_minimum_required(VERSION 3.5)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules )

set(initdb "initdb")


include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Sql_INCLUDE_DIRS} )
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Sql REQUIRED)


set(initdb_includes
${CMAKE_BINARY_DIR}
)

include_directories(${initdb_includes})

set(initdb_uis 
	connessione.ui
	update_db.ui
)


qt5_wrap_ui(initdb_ui_hdrs ${initdb_uis})


set(initdb_moc_hdrs 
	connessione.h
	update_db.h
	../lylibrary/Process.h
)


qt5_wrap_cpp(initdb_mocs ${initdb_moc_hdrs})


set(initdb_srcs 
	connessione.cpp
	update_db.cpp
	../lylibrary/Process.cpp
)

qt5_add_resources(RESOURCE_FILES images.qrc) 


IF(WIN32)
    LINK_LIBRARIES(-lws2_32 -lwtsapi32 -lnetapi32 -luserenv -ladvapi32)
    SET(CMAKE_MODULE_LINKER_FLAGS -Wl,-export-all-symbols)
endif(WIN32)

IF(WIN64)
ADD_DEFINITIONS(-D_X64)
ENDIF(WIN64)


LINK_LIBRARIES(-lpthread Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Sql  )


ADD_LIBRARY( initdb SHARED ${initdb_srcs} ${initdb_ui_hdrs} ${initdb_mocs} ${RESOURCE_FILES})

IF(WIN32)
    INSTALL( TARGETS initdb RUNTIME DESTINATION .)
else()
    INSTALL( TARGETS initdb LIBRARY DESTINATION ${LIB_DIR})
endif()
