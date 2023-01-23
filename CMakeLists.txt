cmake_minimum_required(VERSION 3.5)

project(otchaenie VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(dlls)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)


include(FetchContent)
FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.2
)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        form.h
        form.cpp
        form.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(otchaenie
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(otchaenie SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(otchaenie
            ${PROJECT_SOURCES}
        )
    endif()
endif()



target_link_libraries(otchaenie PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(otchaenie PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(otchaenie PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_link_libraries(otchaenie PUBLIC nlohmann_json::nlohmann_json)


set_target_properties(otchaenie PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(otchaenie)
endif()

install(TARGETS otchaenie DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(TARGETS vk-parser RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/dlls)

install(FILES login.txt DESTINATION .)
install(DIRECTORY photo DESTINATION .)

install(TARGETS otchaenie DESTINATION .)
install(TARGETS vk-parser RUNTIME DESTINATION ./dlls)
