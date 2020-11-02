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
FILE(GLOB SRCS *.h *.C)

ADD_LIBRARY(dnlBC SHARED ${SRCS})
TARGET_LINK_LIBRARIES(dnlBC dnlMaths dnlKernel lapacke lapack blas)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Include of previous directories
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlKernel)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlMaths)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlElements)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlMaterials)

SOURCE_GROUP(base             REGULAR_EXPRESSION ".*\\.(C|i|h)")
SOURCE_GROUP(generated        REGULAR_EXPRESSION ".*\\.(cxx|py)")
