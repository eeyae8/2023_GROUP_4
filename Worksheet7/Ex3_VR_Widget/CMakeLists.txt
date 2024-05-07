cmake_minimum_required(VERSION 3.5)

project(FirstWidget VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(VTK REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
	TreeModel/ModelPart.cpp
	TreeModel/ModelPartList.cpp
	TreeModel/ModelPart.h
	TreeModel/ModelPartList.h
	VRRenderThread.h
	VRRenderThread.cpp
    app.rc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FirstWidget
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        #app.rc
        icons.qrc
        optiondialog.h optiondialog.cpp optiondialog.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FirstWidget APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(FirstWidget SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FirstWidget
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FirstWidget PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${VTK_LIBRARIES})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FirstWidget)
endif()
set_target_properties(FirstWidget PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS FirstWidget
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FirstWidget)
endif()

add_custom_target(vrbindings)

add_custom_command(TARGET vrbindings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/
)
#INSTALLER

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "FirstWidget ${CPACK_PACKAGE_VERSION}")
set(CPACK_SOURCE_GENERATOR "NSIS")

set(CPACK_PACKAGE_NAME "FirstWidget")
set(CPACK_PACKAGE_GROUP "University_Of_Nottingham")
set(CPACK_PACKAGE_VERSION "1.0.5")
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)



set(CPACK_PACKAGE_EXECUTABLES "FirstWidget" "FirstWidget")
set (CPACK_CREATE_DESKTOP_LINKS "FirstWidget")
set_property(INSTALL subdir/FirstWidget PROPERTY CPACK_START_MENU_SHORTCUTS "My executable")
set_property(INSTALL subdir/FirstWidget PROPERTY CPACK_DESKTOP_SHORTCUTS "My executable")

include(CPack)
install(TARGETS FirstWidget RUNTIME DESTINATION bin COMPONENT DEV)
install(DIRECTORY "C:/Program Files (x86)/VTK/bin" DESTINATION bin COMPONENT VTK FILES_MATCHING PATTERN "*.dll")
install(DIRECTORY "C:/Qt/6.6.2/msvc2019_64/bin" DESTINATION bin COMPONENT Qt FILES_MATCHING PATTERN "*.dll")
# Replace the existing install commands for OpenVR with the following
install(DIRECTORY "C:/OpenVR/bin/win64/" DESTINATION bin COMPONENT OpenVR FILES_MATCHING PATTERN "*.dll")

cpack_add_component(VTK
  DISPLAY_NAME "VTK"
  DESCRIPTION
   "VTK dll files"
  GROUP Development)
cpack_add_component(OpenVR
  DISPLAY_NAME "OpenVR"
  DESCRIPTION
   "OpenVR dll files"
  GROUP Development)
  cpack_add_component(Qt
  DISPLAY_NAME "QT"
  DESCRIPTION
   "QT dll files"
  GROUP Development)