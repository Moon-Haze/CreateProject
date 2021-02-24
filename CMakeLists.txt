cmake_minimum_required(VERSION 3.17)
project(CreateProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets Qml Quick)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Qml Qt5::Quick)

set(QRC_FILE "./source/qml.qrc" "./source/font.qrc" "./source/image.qrc" "./source/myapp.rc" ProjectConfig.cpp ProjectConfig.h)

aux_source_directory(. CPP_FILE)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QT_DIR}/$ENV{QT_VERSION}/gcc_64/lib/cmake)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)


add_executable(${PROJECT_NAME} WIN32 ${CPP_FILE} ${QRC_FILE})

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
