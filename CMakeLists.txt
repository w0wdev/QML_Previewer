cmake_minimum_required(VERSION 3.16)

project(qml-ide VERSION 0.1 LANGUAGES CXX)

# ---------------------------------------------------------
# Qt automatic rules
# ---------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Qml Quick QuickWidgets)

set(QML_IMPORT_PATH "${Qt6_DIR}/../../../qml")
set(QML2_IMPORT_PATH "${Qt6_DIR}/../../../qml")

# ---------------------------------------------------------
# Collect ALL source files recursively
# ---------------------------------------------------------
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
)

list(FILTER PROJECT_SOURCES EXCLUDE REGEX "/(build|out|CMakeFiles)/")

# ---------------------------------------------------------
# Collect ALL qrc files (ABSOLUTE paths)
# ---------------------------------------------------------
file(GLOB_RECURSE QRC_FILES_ABS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc"
)

list(FILTER QRC_FILES_ABS EXCLUDE REGEX "/(build|out|CMakeFiles)/")

# Convert to RELATIVE paths (Qt6 REQUIRES this)
set(QRC_FILES "")
foreach(qrc ${QRC_FILES_ABS})
    file(RELATIVE_PATH rel "${CMAKE_CURRENT_SOURCE_DIR}" "${qrc}")
    list(APPEND QRC_FILES "${rel}")
endforeach()

# ---------------------------------------------------------
# Main executable
# ---------------------------------------------------------
qt_add_executable(qml-ide
    ${PROJECT_SOURCES}
    app_icon.rc
)

# ---------------------------------------------------------
# Add resources safely (only RELATIVE paths)
# ---------------------------------------------------------
qt_add_resources(qml-ide embedded_resources
    FILES ${QRC_FILES}
)

# ---------------------------------------------------------
# Include all subdirectories for headers
# ---------------------------------------------------------
macro(add_all_include_dirs target dir)
    file(GLOB children RELATIVE ${dir} ${dir}/*)
    foreach(child ${children})
        if(IS_DIRECTORY "${dir}/${child}")
            if(NOT child MATCHES "^(build|out|CMakeFiles|\\.git|\\.vs)$")
                target_include_directories(${target} PRIVATE "${dir}/${child}")
                add_all_include_dirs(${target} "${dir}/${child}")
            endif()
        endif()
    endforeach()
endmacro()

target_include_directories(qml-ide PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_all_include_dirs(qml-ide ${CMAKE_CURRENT_SOURCE_DIR})

# ---------------------------------------------------------
# Link Qt
# ---------------------------------------------------------
target_link_libraries(qml-ide PRIVATE
    Qt6::Widgets
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickWidgets
)

# ---------------------------------------------------------
# Windows exe icon / macOS bundle
# ---------------------------------------------------------
set_target_properties(qml-ide PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ---------------------------------------------------------
# Install rules
# ---------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS qml-ide
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---------------------------------------------------------
# Finalize Qt build
# ---------------------------------------------------------
qt_finalize_executable(qml-ide)

# Debug Print
message(STATUS "✅ QRC FILES (RELATIVE):")
foreach(q ${QRC_FILES})
    message(STATUS "  ${q}")
endforeach()
