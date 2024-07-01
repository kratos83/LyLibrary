project(inventario)

cmake_minimum_required(VERSION 3.5)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules )

set(inventario "inventario")


include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} 
		    ${Qt5Core_INCLUDE_DIRS} ${Qt5Sql_INCLUDE_DIRS}
		    ${Qt5PrintSupport_INCLUDE_DIRS})
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Sql REQUIRED)
FIND_PACKAGE(Qt5PrintSupport REQUIRED)


set(inventario_includes
${CMAKE_BINARY_DIR}
)

include_directories(${inventario_includes})

set(inventario_uis 
	inventario.ui	
)

qt5_wrap_ui(inventario_ui_hdrs ${inventario_uis})


set(inventario_moc_hdrs 
	inventario.h
	inventario_plugin.h
	StampaBiblioteca.h
)


qt5_wrap_cpp(inventario_mocs ${inventario_moc_hdrs})


set(inventario_srcs 
	inventario.cpp
	inventario_plugin.cpp
	StampaBiblioteca.cpp
)

IF(WIN32)
    LINK_LIBRARIES(-lws2_32 -lwtsapi32 -lnetapi32 -luserenv -ladvapi32 ${QT_LIBRARIES})
    SET(CMAKE_MODULE_LINKER_FLAGS -Wl,-export-all-symbols)
endif(WIN32)

IF(WIN64)
ADD_DEFINITIONS(-D_X64)
ENDIF(WIN64)

LINK_LIBRARIES(-lpthread Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Sql Qt5::PrintSupport  )


ADD_LIBRARY( inventario SHARED ${inventario_srcs} ${inventario_ui_hdrs} ${inventario_mocs} ${RESOURCE_FILES})

IF(WIN32)
    INSTALL( TARGETS inventario RUNTIME DESTINATION ./plugin)
ELSE()
    INSTALL( TARGETS inventario LIBRARY DESTINATION /opt/lylibrary/plugin)
endif()
