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

FOREACH(ATest ${AllTests})
    message(${ATest})
    FILE(GLOB SRCS ${TestDir}/${ATest}/*.c ${TestDir}/${ATest}/*.h)
    LIST(REMOVE_ITEM SRCS "*main.c")
    add_executable(ATest ${TestDir}/${ATest}/main.c ${SRCS})
    target_include_directories(ATest PRIVATE ./)
ENDFOREACH()