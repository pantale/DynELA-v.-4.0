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

ADD_LIBRARY(dnlKernel SHARED ${SRCS})
TARGET_LINK_LIBRARIES(dnlKernel)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SOURCE_GROUP(base             REGULAR_EXPRESSION ".*\\.(C|i|h)")
SOURCE_GROUP(generated        REGULAR_EXPRESSION ".*\\.(cxx|py)")
