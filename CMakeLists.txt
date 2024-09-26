cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(sqlBuilder)
add_executable(sqlBuilderExe main.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

target_include_directories(sql_query_builder PRIVATE ${PQXX_INCLUDE_DIRS})
target_link_directories(sql_query_builder PRIVATE ${PQXX_LIBRARY_DIRS})
target_link_libraries(sql_query_builder PRIVATE ${PQXX_LIBRARIES})


target_compile_options(sql_query_builder PRIVATE ${PQXX_CFLAGS_OTHER})