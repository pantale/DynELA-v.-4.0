#
# DynELA Finite Element Code CMakeList
#
#@!CODEFILE = DynELA-CMakeList
#
# -------------------------------------------------------------------------------
# Beginning of the private area for the Makefile
# Local modifications must be made in this area.
# -------------------------------------------------------------------------------
#@!BEGIN = PRIVATE

CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

PROJECT(DynELA C CXX)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# -- Search for Python
FIND_PACKAGE(PythonInterp 3.7 REQUIRED)
FIND_PACKAGE(PythonLibs 3.7 REQUIRED)

# -- Search for Swig
FIND_PACKAGE(SWIG REQUIRED)

# -- Search for Zlib
FIND_PACKAGE(ZLIB REQUIRED)

# Search for OpenMP
FIND_PACKAGE(OpenMP REQUIRED)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Non optimized version
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# Optimized version
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -ffast-math")

ADD_SUBDIRECTORY(dnlKernel)
ADD_SUBDIRECTORY(dnlMaths)
ADD_SUBDIRECTORY(dnlMaterials)
ADD_SUBDIRECTORY(dnlElements)
ADD_SUBDIRECTORY(dnlBC)
ADD_SUBDIRECTORY(dnlFEM)

ADD_SUBDIRECTORY(dnlPython)

ADD_SUBDIRECTORY(dnlSettings)

ADD_SUBDIRECTORY(dnlUtils)
