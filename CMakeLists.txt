cmake_minimum_required(VERSION 3.16)
project(sylar)

include(cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)

# 指定编译选项
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11 -O0 -ggdb -Wall -Werror")

# -rdynamic: 将所有符号都加入到符号表中，便于使用dlopen或者backtrace追踪到符号
# -fPIC: 生成位置无关的代码，便于动态链接
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -fPIC")

# -Wno-unused-function: 不要警告未使用函数
# -Wno-builtin-macro-redefined: 不要警告内置宏重定义，用于重定义内置的__FILE__宏
# -Wno-deprecated: 不要警告过时的特性
# -Wno-deprecated-declarations: 不要警告使用带deprecated属性的变量，类型，函数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated -Wno-deprecated-declarations")

include_directories(.)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(LIB_SRC
    src/thread.cc
    src/config.cc
    src/env.cc
    src/log.cc
    src/mutex.cc
    src/util.cc
    src/fiber.cc
)

add_library(sylar SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(sylar)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#测试========================================
option(BUILD_TEST "ON for compile test" ON)

set(LIBS sylar pthread yaml-cpp)

if(BUILD_TEST)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/test)
    sylar_add_executable(test_log "test/test_log.cpp" sylar "${LIBS}") 
    sylar_add_executable(test_env "test/test_env.cpp" sylar "${LIBS}")
    sylar_add_executable(test_config "test/test_config.cpp" sylar "${LIBS}")
    sylar_add_executable(test_thread "test/test_thread.cpp" sylar "${LIBS}")
    sylar_add_executable(test_util "test/test_util.cpp" sylar "${LIBS}")
    sylar_add_executable(test_fiber "test/test_fiber.cpp" sylar "${LIBS}")
endif()
