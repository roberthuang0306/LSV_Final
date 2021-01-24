CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(CNOT_syn)
SET(BIN_NAME CNOT_syn)

SET(CMAKE_CXX_FLAGS "-static -O3 -std=c++17 -DIL_STD -lm -pthread ")
SET(CMAKE_C_FLAGS "-static -O3  -c ")


SET(BASEPATH ${PROJECT_SOURCE_DIR})
SET(SRCPATH ${BASEPATH}/src)
SET(BINPATH ${BASEPATH}/bin)
SET(INCPATH ${BASEPATH}/include)
SET(LIBPATH ${BASEPATH}/lib)

IF (NOT EXISTS "${BINPATH}")
  FILE(MAKE_DIRECTORY ${BINPATH})
ENDIF()

IF (NOT EXISTS "${INCPATH}")
  FILE(MAKE_DIRECTORY ${INCPATH})
ENDIF()

find_package(Boost)
IF (NOT EXISTS "${LIBPATH}")
  FILE(MAKE_DIRECTORY ${LIBPATH})
ENDIF()

INCLUDE_DIRECTORIES(${SRCPATH} ${INCPATH})
find_package (Threads)
############################################################
# Install Boost                
############################################################
# Install Boost
SET(BOOST_VERSION 1_72_0)
FIND_PACKAGE(Boost 1.72)
IF(Boost_FOUND)
  SET(Boost_USE_STATIC_LIBS   ON)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ELSE()
  IF (NOT EXISTS "${INCPATH}/boost_${BOOST_VERSION}")
    execute_process (COMMAND bash -c "wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz -P ${INCPATH}")
    execute_process (COMMAND bash -c "tar zxvf ${INCPATH}/boost_${BOOST_VERSION}.tar.gz -C ${INCPATH}")
    execute_process (COMMAND bash -c "rm ${INCPATH}/boost_${BOOST_VERSION}.tar.gz")
  ENDIF()

  SET(BOOST_ROOT ${INCPATH}/boost_${BOOST_VERSION})
  SET(Boost_INCLUDE_DIR ${INCPATH}/boost_${BOOST_VERSION})
  SET(Boost_LIBRARY_DIR_RELEASE ${INCPATH}/boost_${BOOST_VERSION})
  SET(Boost_USE_STATIC_LIBS   ON)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()


############################################################
# library                         
############################################################
set(LIBRARY_OUTPUT_PATH ${LIBPATH})
# LIB
file(GLOB LIB ${LIBPATH}/*.a)

############################################################
# Create SRCLIB       
############################################################

file(GLOB SUBDIR RELATIVE ${SRCPATH} ${SRCPATH}/*)
foreach(dir ${SUBDIR})
    IF(IS_DIRECTORY ${SRCPATH}/${dir})
        file(GLOB FILE ${SRCPATH}/${dir}/*)
        set(SRC ${SRC} ${FILE})
    ENDIF()
endforeach()

add_library(srclib STATIC ${SRC})
target_link_libraries(srclib PRIVATE ${LIB} ${CMAKE_DL_LIBS})

ADD_EXECUTABLE(main ${SRCPATH}/main.cpp)

target_link_libraries(main PRIVATE ${LIB} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS} srclib)

set_target_properties(main PROPERTIES OUTPUT_NAME bin/${BIN_NAME})



