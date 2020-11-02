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

# configuration.dnl Utility File
add_custom_target(copy-configuration.dnl ALL DEPENDS ${EXECUTABLE_OUTPUT_PATH}/configuration.dnl)
add_custom_command(
        OUTPUT ${EXECUTABLE_OUTPUT_PATH}/configuration.dnl
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dnlSettings/configuration.dnl ${EXECUTABLE_OUTPUT_PATH}/configuration.dnl
		DEPENDS ${PROJECT_SOURCE_DIR}/dnlSettings/configuration.dnl)

SOURCE_GROUP(base             REGULAR_EXPRESSION ".*\\.(C|i|h)")
SOURCE_GROUP(generated        REGULAR_EXPRESSION ".*\\.(cxx|py)")
