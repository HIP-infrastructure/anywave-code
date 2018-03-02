SET(SDK_FOUND 0)

IF( "$ENV{AWSDK}" STREQUAL "" )
	MESSAGE(STATUS "AnyWave SDK not found. The AWSDK variable must be defined in your system.")
	MESSAGE(STATUS "On Unices, this can be done by adding the definition to the .bashrc file.")
	MESSAGE(STATUS "example: export AWSDK=/pathtosdk")
	RETURN()
ELSE()
	SET(SDK_FOUND 1)
	SET(SDK_ROOT $ENV{AWSDK})
ENDIF()

#Link using SDK lib folder
LINK_DIRECTORIES(${SDK_ROOT}/lib)
# Include SDK headers
INCLUDE_DIRECTORIES(${SDK_ROOT}/include)
# Include current source dir as header dir
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_DIR})