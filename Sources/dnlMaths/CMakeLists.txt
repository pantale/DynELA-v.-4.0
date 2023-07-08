#
# CMakefile for DynELA
#
#  DynELA Finite Element Code v.4.0
#  by Olivier PANTALE
#  Olivier.Pantale@enit.fr
#
#@!CODEFILE = DynELA-CMakeList
#
# -------------------------------------------------------------------------------
# Beginning of the private area for the Makefile
# Local modifications must be made in this area.
# -------------------------------------------------------------------------------
#@!BEGIN = PRIVATE

FILE(GLOB SRCS *.h *.C)

ADD_LIBRARY(dnlMaths SHARED ${SRCS})
LINK_DIRECTORIES(${LAPACK_BIN_DIR} ${BLAS_BIN_DIR})
TARGET_LINK_LIBRARIES(dnlMaths dnlKernel lapacke lapack blas zlib)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES("${LAPACK_SRC_DIR}/LAPACKE/include")
INCLUDE_DIRECTORIES("${LAPACK_SRC_DIR}/CBLAS/include")

# Include of previous directories
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlKernel)

SOURCE_GROUP(base             REGULAR_EXPRESSION ".*\\.(C|i|h)")
SOURCE_GROUP(generated        REGULAR_EXPRESSION ".*\\.(cxx|py)")
