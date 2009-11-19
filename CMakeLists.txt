PROJECT(kraft)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/ ${CMAKE_MODULE_PATH} )

find_package(KDE4 REQUIRED)
find_package (KdepimLibs REQUIRED)
find_package(Ctemplate REQUIRED)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions (-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)

include(KDE4Defaults)

include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} src)

add_subdirectory(cmake)
add_subdirectory(src)
add_subdirectory(database)
add_subdirectory(reports)
add_subdirectory(importfilter)
add_subdirectory(tools)
