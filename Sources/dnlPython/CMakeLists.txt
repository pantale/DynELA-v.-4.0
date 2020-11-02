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

INCLUDE(${SWIG_USE_FILE})

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

FILE(GLOB SRCS *.h *.C)
FILE(GLOB ISRCS *.i)

SET(CMAKE_SWIG_FLAGS "-Wall")

SET(SWINCFLAGS 
-I${PROJECT_SOURCE_DIR}/dnlKernel
-I${PROJECT_SOURCE_DIR}/dnlMaths
-I${PROJECT_SOURCE_DIR}/dnlMaterials
-I${PROJECT_SOURCE_DIR}/dnlElements
-I${PROJECT_SOURCE_DIR}/dnlBC
-I${PROJECT_SOURCE_DIR}/dnlFEM
)

SET_SOURCE_FILES_PROPERTIES(${ISRCS} PROPERTIES SWIG_FLAGS "${SWINCFLAGS}")
SET_SOURCE_FILES_PROPERTIES(${ISRCS} PROPERTIES CPLUSPLUS ON)

SWIG_ADD_LIBRARY(dnlPython LANGUAGE python SOURCES ${ISRCS} ${SRCS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlKernel)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlMaths)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlMaterials)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlElements)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlBC)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlFEM)

SWIG_LINK_LIBRARIES(dnlPython dnlFEM dnlBC dnlElements dnlMaterials dnlMaths dnlKernel ${PYTHON_LIBRARIES} lapacke lapack blas ${ZLIB_LIBRARIES}) 

ADD_CUSTOM_COMMAND(TARGET _dnlPython POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${CMAKE_BINARY_DIR}/dnlPython/dnlPython.py
                   ${EXECUTABLE_OUTPUT_PATH})

SOURCE_GROUP(base             REGULAR_EXPRESSION ".*\\.(C|i|h)")
SOURCE_GROUP(generated        REGULAR_EXPRESSION ".*\\.(cxx|py)")
