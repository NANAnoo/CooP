cmake_minimum_required(VERSION 3.26)
project(OOPC C)

set(CMAKE_C_STANDARD 11)

add_executable(OOPC main.c
        CClass.h)

MACRO(SUB_DIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

set(TestDir ${PROJECT_SOURCE_DIR}/test)
SUB_DIRLIST(AllTests ${TestDir})

set(NANOSTD_INCLUDE "nanostd/include")
set(NANOSTD_SRC "nanostd/src")
file(GLOB nanostd_srcs ${NANOSTD_SRC}/*.c)
add_library(nanostd STATIC ${nanostd_srcs})
target_include_directories(nanostd PRIVATE ${NANOSTD_INCLUDE} ./)

FOREACH(ATest ${AllTests})
    FILE(GLOB SRCS ${TestDir}/${ATest}/*.c ${TestDir}/${ATest}/*.h)
    LIST(REMOVE_ITEM SRCS "*main.c")
    add_executable(${ATest} ${TestDir}/${ATest}/main.c ${SRCS})
    target_link_libraries(${ATest} PRIVATE nanostd)
    target_include_directories(${ATest} PRIVATE ${TestDir}/${ATest}/ ${NANOSTD_INCLUDE} ./)
ENDFOREACH()