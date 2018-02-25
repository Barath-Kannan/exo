################################################
# Project Settings
################################################
project(exo CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)

################################################
# Dependencies
################################################
find_package(Boost REQUIRED COMPONENTS unit_test_framework log)

################################################
# Files
################################################
set(H inc/beak/exo/detail)
set(S src/detail)
set(LIB_DETAIL_FILES
    ${H}/signal_base.h
    ${H}/signal_impl.h
)

set(H inc/beak/exo)
set(S src)
set(LIB_FILES
    ${LIB_DETAIL_FILES}
    ${H}/signal.h               ${S}/signal.cpp
)

set(T test)
set(TEST_FILES
    ${T}/test.cpp
)

################################################
# Targets
################################################
add_library(beak_exo
    ${LIB_FILES}
)

target_include_directories(beak_exo
    PUBLIC inc
)

target_link_libraries(beak_exo
    Boost::log
)

if (NOT Boost_USE_STATIC_LIBS)
    target_compile_definitions(beak_exo PUBLIC -DBOOST_LOG_DYN_LINK)
endif()

add_executable(beak_exo_test
    ${TEST_FILES}
)
target_link_libraries(beak_exo_test
    beak_exo
    Boost::unit_test_framework
)

add_test(NAME beak_exo_test COMMAND beak_exo_test)


