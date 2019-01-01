add_executable(librtttl_unittest
  ${LIBRTTTL_EXPORT_HEADER}
  ${LIBRTTTL_VERSION_HEADER}
  ${LIBRTTTL_CONFIG_HEADER}
  binary.h
  counter5bits.h
  generator.h
  gtesthelper.cpp
  gtesthelper.h
  imperialmarch.cpp
  imperialmarch.h
  main.cpp
  mario.cpp
  mario.h
  testLibRtttl.cpp
  testLibRtttl.h
  ${CMAKE_SOURCE_DIR}/src/common/utils.cpp
  ${CMAKE_SOURCE_DIR}/src/common/utils.h
  ${LIBRTTTL_THIRD_PARTIES_DIR}/BitReader/src/bitreader.cpp
  ${LIBRTTTL_THIRD_PARTIES_DIR}/BitReader/src/bitreader.h
)

# Unit test projects requires to link with pthread if also linking with gtest
if(NOT WIN32)
  set(PTHREAD_LIBRARIES -pthread)
endif()

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(librtttl_unittest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(librtttl_unittest 
  PRIVATE
    ${GTEST_INCLUDE_DIR}
)
add_dependencies(librtttl_unittest librtttl)
target_link_libraries(librtttl_unittest PRIVATE rapidassist librtttl win32arduino ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )

install(TARGETS librtttl_unittest
        EXPORT librtttl-targets
        ARCHIVE DESTINATION ${LIBRTTTL_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBRTTTL_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBRTTTL_INSTALL_BIN_DIR}
)
