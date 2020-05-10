# link to external library
find_library(CDK_LIB cdk)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(SQLITE REQUIRED sqlite3)
PKG_CHECK_MODULES(NCURSES REQUIRED
    ncursesw menuw panelw form formw libaria2)

#SHARED
add_library(
    util
    util/util.cpp
    util/util.hpp
    util/sqlite_util.cpp
    util/sqlite_util.hpp)
target_include_directories(util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/util")
target_link_libraries(util ${SQLITE_LIBRARIES})
target_include_directories(util PUBLIC ${SQLITE_INCLUDE_DIRS})
target_compile_options(util PUBLIC ${SQLITE_CFLAGS_OTHER})

add_library(
    launcher
    launcher/launcher.cpp
    launcher/launcher.hpp
    launcher/sw_active.cpp
    launcher/sw_active.hpp
    launcher/sw_about.cpp
    launcher/sw_about.hpp
    launcher/sw_new.cpp
    launcher/sw_new.hpp
    launcher/extern.hpp
    launcher/extern.cpp)
target_include_directories(launcher PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/launcher")
target_include_directories(launcher PUBLIC ${NCURSES_INCLUDE_DIRS})
target_compile_options(launcher PUBLIC ${NCURSES_CFLAGS_OTHER})
target_link_libraries(launcher ${NCURSES_LIBRARIES})
target_link_libraries(launcher ${launcher_LIBRARIES} pthread)
target_link_libraries(launcher ${CDK_LIB})
target_link_libraries(launcher util)

#target_include_directories(launcher PUBLIC ${CDK_INCLUDE_DIRS})
#target_compile_options(launcher PUBLIC ${CDK_CFLAGS_OTHER})

#INCLUDE_DIRECTORIES(/usr/local/include)
#LINK_DIRECTORIES(/usr/local/lib)
#
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/local/lib)
#find_library(CDK cdk PATHS /usr/local/lib)
