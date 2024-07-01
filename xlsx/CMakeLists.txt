project(xlsx)

cmake_minimum_required(VERSION 3.5)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules )

if(WIN32)
SET( CMAKE_CXX_FLAGS  "-DUNICODE -DQT_NO_MTDEV -DQT_NO_LIBUDEV -DQT_NO_EVDEV -DQT_NO_TSLIB -DQT_NO_LIBINPUT -DQT_BUILD_XLSX_LIB -DQT_BUILDING_QT -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES -DQT_NO_CAST_TO_ASCII -DQT_ASCII_CAST_WARNINGS -DQT_MOC_COMPAT -DQT_USE_QSTRINGBUILDER -DQT_DEPRECATED_WARNINGS -DQT_DISABLE_DEPRECATED_BEFORE=0x040800 -DQT_NO_EXCEPTIONS -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_OPENGL_ES_2 -DQT_OPENGL_ES_2_ANGLE -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 --param=ssp-buffer-size=4 -fno-keep-inline-dllexport -msse2 -mstackrealign -mfpmath=sse -std=c++0x -fno-exceptions -frtti -Wall -Wextra" )
ELSE()
SET( CMAKE_CXX_FLAGS  "-fexceptions -std=c++11" )
ENDIF()
    
IF(WIN32)
set(Qt5Gui_PRIVATE_INCLUDE_DIRS
        "${MINGW_PREFIX}/include/qt/QtGui/5.15.8"
        "${MINGW_PREFIX}/include/qt/QtGui/5.15.8/QtGui"
    )
set(Qt5Core_PRIVATE_INCLUDE_DIRS
        "${MINGW_PREFIX}/include/qt/QtCore/5.15.8"
        "${MINGW_PREFIX}/include/qt/QtCore/5.15.8/QtCore"
    )
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} 
		    ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_PRIVATE_INCLUDE_DIRS} ${Qt5Core_PRIVATE_INCLUDE_DIRS})

ELSE()
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} 
		    ${Qt5Gui_PRIVATE_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Core_PRIVATE_INCLUDE_DIRS})
ENDIF()
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)


set(xlsx "xlsx")

set(xlsx_includes
${CMAKE_BINARY_DIR}/xlsx
)

include_directories(${xlsx_includes})

set(xlsx_moc_hdrs 
	xlsxabstractooxmlfile_p.h 
    xlsxabstractooxmlfile.h 
    xlsxabstractsheet_p.h 
    xlsxabstractsheet.h 
    xlsxcell_p.h 
    xlsxcell.h 
    xlsxcellformula_p.h 
    xlsxcellformula.h 
    xlsxcelllocation.h 
    xlsxcellrange.h 
    xlsxcellreference.h 
    xlsxchart_p.h 
    xlsxchart.h 
    xlsxchartsheet_p.h 
    xlsxchartsheet.h 
    xlsxcolor_p.h 
    xlsxconditionalformatting_p.h 
    xlsxconditionalformatting.h 
    xlsxcontenttypes_p.h 
    xlsxdatavalidation_p.h 
    xlsxdatavalidation.h 
    xlsxdatetype.h 
    xlsxdocpropsapp_p.h 
    xlsxdocpropscore_p.h 
    xlsxdocument_p.h 
    xlsxdocument.h 
    xlsxdrawing_p.h 
    xlsxdrawinganchor_p.h 
    xlsxformat_p.h 
    xlsxformat.h 
    xlsxglobal.h 
    xlsxmediafile_p.h 
    xlsxnumformatparser_p.h 
    xlsxrelationships_p.h 
    xlsxrichstring_p.h 
    xlsxrichstring.h 
    xlsxsharedstrings_p.h 
    xlsxsimpleooxmlfile_p.h 
    xlsxstyles_p.h 
    xlsxtheme_p.h 
    xlsxutility_p.h 
    xlsxworkbook_p.h 
    xlsxworkbook.h 
    xlsxworksheet_p.h 
    xlsxworksheet.h 
    xlsxzipreader_p.h 
    xlsxzipwriter_p.h
)

qt5_wrap_cpp(xlsx_mocs ${xlsx_moc_hdrs})


set(xlsx_srcs 
	xlsxabstractooxmlfile.cpp 
    xlsxabstractsheet.cpp 
    xlsxcell.cpp 
    xlsxcellformula.cpp 
    xlsxcelllocation.cpp 
    xlsxcellrange.cpp 
    xlsxcellreference.cpp 
    xlsxchart.cpp 
    xlsxchartsheet.cpp 
    xlsxcolor.cpp 
    xlsxconditionalformatting.cpp 
    xlsxcontenttypes.cpp 
    xlsxdatavalidation.cpp 
    xlsxdatetype.cpp 
    xlsxdocpropsapp.cpp 
    xlsxdocpropscore.cpp 
    xlsxdocument.cpp 
    xlsxdrawing.cpp 
    xlsxdrawinganchor.cpp 
    xlsxformat.cpp 
    xlsxmediafile.cpp 
    xlsxnumformatparser.cpp 
    xlsxrelationships.cpp 
    xlsxrichstring.cpp 
    xlsxsharedstrings.cpp 
    xlsxsimpleooxmlfile.cpp 
    xlsxstyles.cpp 
    xlsxtheme.cpp 
    xlsxutility.cpp 
    xlsxworkbook.cpp 
    xlsxworksheet.cpp 
    xlsxzipreader.cpp 
    xlsxzipwriter.cpp
)

IF(WIN32)
    LINK_LIBRARIES(-lws2_32 -lwtsapi32 -lnetapi32 -luserenv -ladvapi32 ${QT_LIBRARIES})
    SET(CMAKE_MODULE_LINKER_FLAGS -Wl,-export-all-symbols)
endif(WIN32)

IF(WIN64)
ADD_DEFINITIONS(-D_X64)
ENDIF(WIN64)

LINK_LIBRARIES(-lpthread Qt5::Widgets Qt5::Gui Qt5::Core  )

ADD_LIBRARY(xlsx_lylibrary SHARED ${xlsx_srcs} ${xlsx_mocs})


IF(WIN32)
    INSTALL( TARGETS xlsx_lylibrary RUNTIME DESTINATION .)
else()
    INSTALL( TARGETS xlsx_lylibrary LIBRARY DESTINATION ${LIB_DIR})
endif()
