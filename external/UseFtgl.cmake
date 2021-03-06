set(EXTLIB_NAME ftgl)
set(EXTLIB_TGZ ${CMAKE_SOURCE_DIR}/external/${EXTLIB_NAME}.tar.gz)
set(EXTLIB_DIR ${CMAKE_BINARY_DIR}/${EXTLIB_NAME})

if(NOT EXISTS ${EXTLIB_DIR})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf
    ${EXTLIB_TGZ} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif(NOT EXISTS ${EXTLIB_DIR})

add_subdirectory(${EXTLIB_DIR} ${CMAKE_BINARY_DIR}/src/3rdparty/${EXTLIB_NAME})
