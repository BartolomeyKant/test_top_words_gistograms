cmake_minimum_required(VERSION 3.21)

project(top_words LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(
	top_words
	src/main.cpp
	src/top_words.cpp
	src/top_words_list.cpp
	src/res.qrc
)

target_compile_definitions(top_words
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QMLLDebug>)
target_link_libraries(top_words
  PRIVATE Qt5::Core Qt5::Quick pthread)
