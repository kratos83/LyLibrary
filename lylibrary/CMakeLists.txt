project(lylibrary)

cmake_minimum_required(VERSION 3.5)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules )

if(WIN32)
SET( CMAKE_CXX_FLAGS  "-fexceptions -std=c++14 -mwindows" )
ELSE()
SET( CMAKE_CXX_FLAGS  "-fexceptions -std=c++11" )
ENDIF()

set(lylibrary "lylibrary")


include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} 
		    ${Qt5Core_INCLUDE_DIRS} ${Qt5Sql_INCLUDE_DIRS} 
		    ${Qt5Xml_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS}
		    ${Qt5Network_INCLUDE_DIRS} ${Qt5Svg_INCLUDE_DIRS}
		    ${Qt5Multimedia_INCLUDE_DIRS})
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Xml REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)
FIND_PACKAGE(Qt5Sql REQUIRED)
FIND_PACKAGE(Qt5Svg REQUIRED)
FIND_PACKAGE(Qt5PrintSupport REQUIRED)
FIND_PACKAGE(Qt5Multimedia REQUIRED)

set(lylibrary_moc_hdrs 
	about.h
	articoli.h
	azienda.h
	barcode.h
	categoria.h
	classe.h
	cod_fisc.h
	details_plugin.h
	ean13.h
	esci.h
	find_cap_italian.h
	impo_iva_toto.h
	itdelegato.h
	mainwindow.h
	pluginmanager.h
	pref.h
	prest_lib.h
	presto.h
	print.h
	progressbar.h
	settingsmanager.h
	sound.h
	splashscreen.h
	topheader.h
	user.h
	verify_codicefiscale.h
	verify_piva.h
	stampa/StampaUtenti.h
	stampa/StampaBiblioteca.h
	stampa/StampaPrestiti.h
	stampa/StampaScadenze.h
	Process.h
)


qt5_wrap_cpp(lylibrary_mocs ${lylibrary_moc_hdrs})


set(lylibrary_uis 
	ui/about.ui
	ui/articoli.ui
	ui/azienda.ui
	ui/barcode.ui
	ui/categoria.ui
	ui/cod_fisc.ui
	ui/details_plugin.ui
	ui/esci.ui
	ui/find_cap_italian.ui
	ui/mainwindow.ui
	ui/pluginmanager.ui
	ui/pref.ui
	ui/prest_lib.ui
	ui/presto.ui
	ui/user.ui
	ui/verify_codicefiscale.ui
	ui/verify_piva.ui
)

qt5_wrap_ui(lylibrary_ui_hdrs ${lylibrary_uis})


set(lylibrary_srcs 
	about.cpp
	articoli.cpp
	azienda.cpp
	barcode.cpp
	categoria.cpp
	classe.cpp
	cod_fisc.cpp
	details_plugin.cpp
	ean13.cpp
	esci.cpp
	find_cap_italian.cpp
	impo_iva_toto.cpp
	itdelegato.cpp
	main.cpp
	mainwindow.cpp
	pluginmanager.cpp
	pref.cpp
	prest_lib.cpp
	presto.cpp
	print.cpp
	progressbar.cpp
	settingsmanager.cpp
	sound.cpp
	splashscreen.cpp
	topheader.cpp
	user.cpp
	verify_codicefiscale.cpp
	verify_piva.cpp
	stampa/StampaUtenti.cpp
	stampa/StampaBiblioteca.cpp
	stampa/StampaPrestiti.cpp
	stampa/StampaScadenze.cpp
	Process.cpp
)

qt5_add_resources(RESOURCE_FILES biblio.qrc lang.qrc) 


IF(WIN32)
 	SET(WINRC "${CMAKE_CURRENT_BINARY_DIR}/lylibrary.obj")
 	ADD_DEFINITIONS(-D__USE_MINGW_ANSI_STDIO=1)
 	ADD_CUSTOM_COMMAND(OUTPUT ${WINRC}
 						COMMAND ${WINDRES}
 							-I${CMAKE_CURRENT_SOURCE_DIR}
 							-o${WINRC}
 							-i${CMAKE_CURRENT_BINARY_DIR}/winicon.rc
 						DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/winicon.rc)

ENDIF(WIN32)

if (APPLE)
    set(SOURCE_FILES ${SOURCE_FILES} images/logo4.icns)
    set_source_files_properties(images/logo4.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    ADD_EXECUTABLE(lylibrary MACOSX_BUNDLE ${lylibrary_srcs} ${lylibrary_mocs} ${lylibrary_ui_hdrs} ${RESOURCE_FILES} ${WINRC} ${SOURCE_FILES})
else()
ADD_EXECUTABLE(lylibrary ${lylibrary_srcs} ${lylibrary_mocs} ${lylibrary_ui_hdrs} ${RESOURCE_FILES} ${WINRC} ${SOURCE_FILES})
endif()

target_link_libraries( lylibrary
  anaclogclock 
  initdb 
  log_man
  xlsx_lylibrary
  Qt5::Widgets Qt5::Xml
  Qt5::Network Qt5::Gui 
  Qt5::Sql Qt5::Core 
  Qt5::Svg Qt5::PrintSupport 
  Qt5::Multimedia
)


IF(WIN32)
    INSTALL(TARGETS lylibrary RUNTIME DESTINATION .)
	INSTALL(FILES
		"${MINGW_PREFIX}/bin/Qt5Core.dll"
		"${MINGW_PREFIX}/bin/Qt5Gui.dll"
		"${MINGW_PREFIX}/bin/Qt5Svg.dll"
		"${MINGW_PREFIX}/bin/Qt5Xml.dll"
		"${MINGW_PREFIX}/bin/Qt5Network.dll"
		"${MINGW_PREFIX}/bin/Qt5Multimedia.dll"
		"${MINGW_PREFIX}/bin/Qt5MultimediaWidgets.dll"
		"${MINGW_PREFIX}/bin/Qt5Sql.dll"
		"${MINGW_PREFIX}/bin/Qt5Widgets.dll"
		"${MINGW_PREFIX}/bin/Qt5PrintSupport.dll"
		"${MINGW_PREFIX}/bin/Qt5OpenGL.dll"
		"${MINGW_PREFIX}/bin/Qt5Concurrent.dll"
		"${MINGW_PREFIX}/bin/libexpat-1.dll"
		"${MINGW_PREFIX}/bin/libpng16-16.dll"
		"${MINGW_PREFIX}/bin/libstdc++-6.dll"
		"${MINGW_PREFIX}/bin/libtiff-6.dll"
		"${MINGW_PREFIX}/bin/libtiffxx-6.dll"
		"${MINGW_PREFIX}/bin/libturbojpeg.dll"
		"${MINGW_PREFIX}/bin/zlib1.dll"
		"${MINGW_PREFIX}/bin/libwinpthread-1.dll"
		"${MINGW_PREFIX}/bin/libgcc_s_seh-1.dll"
		"${MINGW_PREFIX}/bin/libsqlite3-0.dll"
		"${MINGW_PREFIX}/bin/libfreetype-6.dll"
		"${MINGW_PREFIX}/bin/libglib-2.0-0.dll"
		"${MINGW_PREFIX}/bin/libharfbuzz-0.dll"
		"${MINGW_PREFIX}/bin/libpcre2-16-0.dll"
		"${MINGW_PREFIX}/bin/libbz2-1.dll"
		"${MINGW_PREFIX}/bin/libmariadb.dll"
		"${MINGW_PREFIX}/bin/libiconv-2.dll"
		"${MINGW_PREFIX}/bin/libssp-0.dll"
		"${MINGW_PREFIX}/bin/libzstd.dll"
		"${MINGW_PREFIX}/bin/libgraphite2.dll"
		"${MINGW_PREFIX}/bin/libintl-8.dll"
		"${MINGW_PREFIX}/bin/libbrotlidec.dll"
		"${MINGW_PREFIX}/bin/libbrotlicommon.dll"
		"${MINGW_PREFIX}/bin/libpcre2-8-0.dll"
		"${MINGW_PREFIX}/bin/libcrypto-3-x64.dll"
		"${MINGW_PREFIX}/bin/libssl-3-x64.dll"
		"${MINGW_PREFIX}/bin/libjpeg-8.dll"
		"images/sito.html"
		"../README.txt"
		DESTINATION .
		)
	INSTALL(FILES 
		"${MINGW_PREFIX}/lib/qt/plugins/sqldrivers/qsqlmysql.dll" DESTINATION ./sqldrivers
		)
	INSTALL(FILES 
		"${MINGW_PREFIX}/lib/qt/plugins/platforms/qminimal.dll" 
		"${MINGW_PREFIX}/lib/qt/plugins/platforms/qwindows.dll"
		DESTINATION ./platforms
		)
	INSTALL(FILES 
		"${MINGW_PREFIX}/lib/qt/plugins/audio/qtaudio_windows.dll" 
		DESTINATION ./audio
		)
	INSTALL(FILES
		"${MINGW_PREFIX}/lib/qt/plugins/imageformats/qgif.dll"
		"${MINGW_PREFIX}/lib/qt/plugins/imageformats/qico.dll"
		"${MINGW_PREFIX}/lib/qt/plugins/imageformats/qjpeg.dll"
		"${MINGW_PREFIX}/lib/qt/plugins/imageformats/qsvg.dll"
		DESTINATION ./imageformats
		)
	INSTALL(FILES 
		"${MINGW_PREFIX}/lib/qt/plugins/printsupport/windowsprintersupport.dll" 
		DESTINATION ./printsupport
		)
	INSTALL(FILES
                "${MINGW_PREFIX}/lib/qt/plugins/styles/qwindowsvistastyle.dll"
                DESTINATION ./styles
                )
	INSTALL(FILES
		"../windows/gunzip.exe"
		"../windows/gzip.exe"
		"../windows/unzip.exe" 
		DESTINATION ./windows )
	INSTALL(FILES
		"installdb/install_db.sql"
		DESTINATION ./installdb )
	INSTALL(FILES
		"updatedb/update_db.sql"
		DESTINATION ./updatedb )
ELSE()
    INSTALL( TARGETS lylibrary RUNTIME DESTINATION /opt/lylibrary)
    INSTALL( FILES "images/logo4.png" DESTINATION /opt/lylibrary)
    INSTALL(FILES
		"installdb/install_db.sql"
		DESTINATION /opt/lylibrary/installdb )
	INSTALL(FILES
		"updatedb/update_db.sql"
		DESTINATION /opt/lylibrary/updatedb )
    INSTALL( FILES "desktop/lylibrary.desktop" DESTINATION /usr/share/applications )
endif()
