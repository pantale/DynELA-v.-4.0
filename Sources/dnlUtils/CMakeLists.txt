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

# dnlCurves.py Utility File
add_custom_target(copy-dnlCurves.py ALL DEPENDS ${EXECUTABLE_OUTPUT_PATH}/dnlCurves.py)
add_custom_command(
        OUTPUT ${EXECUTABLE_OUTPUT_PATH}/dnlCurves.py
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dnlUtils/dnlCurves.py ${EXECUTABLE_OUTPUT_PATH}/dnlCurves.py
		DEPENDS ${PROJECT_SOURCE_DIR}/dnlUtils/dnlCurves.py)

# dnlBench.py Utility File
add_custom_target(copy-dnlBench.py ALL DEPENDS ${EXECUTABLE_OUTPUT_PATH}/dnlBench.py)
add_custom_command(
        OUTPUT ${EXECUTABLE_OUTPUT_PATH}/dnlBench.py
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dnlUtils/dnlBench.py ${EXECUTABLE_OUTPUT_PATH}/dnlBench.py
		DEPENDS ${PROJECT_SOURCE_DIR}/dnlUtils/dnlBench.py)

# AbaqusExtract.py Utility File
add_custom_target(copy-AbaqusExtract.py ALL DEPENDS ${EXECUTABLE_OUTPUT_PATH}/AbaqusExtract.py)
add_custom_command(
        OUTPUT ${EXECUTABLE_OUTPUT_PATH}/AbaqusExtract.py
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dnlUtils/AbaqusExtract.py ${EXECUTABLE_OUTPUT_PATH}/AbaqusExtract.py
		DEPENDS ${PROJECT_SOURCE_DIR}/dnlUtils/AbaqusExtract.py)

SOURCE_GROUP(base             REGULAR_EXPRESSION ".*\\.(C|i|h)")
SOURCE_GROUP(generated        REGULAR_EXPRESSION ".*\\.(cxx|py)")
