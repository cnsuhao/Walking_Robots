# find GAMCS (Generalized Agent Model and Computer Simulation) includes and library
#
# GAMCS_INCLUDE_DIR - where the directory containing the GAMCS headers can be
#                   found
# GAMCS_LIBRARY     - full path to the GAMCS library
# GAMCS_FOUND       - TRUE if GAMCS was found

IF (NOT GAMCS_FOUND)

    FIND_PATH(GAMCS_INCLUDE_DIR gamcs/GIOM.h
        /usr/include
        /usr/local/include
        )
    FIND_LIBRARY(GAMCS_LIBRARY
        NAMES gamcs 
        PATHS
        /usr/lib
        /usr/local/lib
        )

    IF(GAMCS_INCLUDE_DIR)
        MESSAGE(STATUS "Found GAMCS include dir: ${GAMCS_INCLUDE_DIR}")
    ELSE(GAMCS_INCLUDE_DIR)
        MESSAGE(STATUS "Could NOT find GAMCS headers.")
    ENDIF(GAMCS_INCLUDE_DIR)

    IF(GAMCS_LIBRARY)
        MESSAGE(STATUS "Found GAMCS library: ${GAMCS_LIBRARY}")
    ELSE(GAMCS_LIBRARY)
        MESSAGE(STATUS "Could NOT find GAMCS library.")
    ENDIF(GAMCS_LIBRARY)

    IF(GAMCS_INCLUDE_DIR AND GAMCS_LIBRARY)
        SET(GAMCS_FOUND TRUE CACHE STRING "Whether GAMCS was found or not")
    ELSE(GAMCS_INCLUDE_DIR AND GAMCS_LIBRARY)
        SET(GAMCS_FOUND FALSE)
        IF(GAMCS_FIND_REQUIRED)
            MESSAGE(FATAL_ERROR "Could not find GAMCS. Please install GAMCS (http://www.andy87.com/gamcs)")
        ENDIF(GAMCS_FIND_REQUIRED)
    ENDIF(GAMCS_INCLUDE_DIR AND GAMCS_LIBRARY)
ENDIF (NOT GAMCS_FOUND)

