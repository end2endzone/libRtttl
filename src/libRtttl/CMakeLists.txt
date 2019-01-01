set(LIBRTTTL_HEADER_FILES ""
  ${LIBRTTTL_INCLUDE_DIR}/librtttl/rtttl.h
  ${LIBRTTTL_INCLUDE_DIR}/librtttl/stream.h
  ${LIBRTTTL_INCLUDE_DIR}/librtttl/tone.h
)

add_library(librtttl
  ${LIBRTTTL_HEADER_FILES}
  ${LIBRTTTL_EXPORT_HEADER}
  ${LIBRTTTL_VERSION_HEADER}
  ${LIBRTTTL_CONFIG_HEADER}
  notedelayarray.cpp
  notedelayarray.h
  notes.cpp
  notes.h
  rtttl.cpp
  stream.cpp
  TODO.txt
  tone.cpp
  ${CMAKE_SOURCE_DIR}/src/common/utils.cpp
  ${CMAKE_SOURCE_DIR}/src/common/utils.h
  ${LIBRTTTL_THIRD_PARTIES_DIR}/AnyRtttl/src/binrtttl.cpp
  ${LIBRTTTL_THIRD_PARTIES_DIR}/AnyRtttl/src/binrtttl.h
 )

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(librtttl PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Define include directories for exported code.
target_include_directories(librtttl
  PUBLIC
    $<INSTALL_INTERFACE:${LIBRTTTL_INSTALL_INCLUDE_DIR}>  # for clients using the installed library.
)
target_link_libraries(librtttl PUBLIC rapidassist PRIVATE win32arduino)

install(TARGETS librtttl
        EXPORT librtttl-targets
        ARCHIVE DESTINATION ${LIBRTTTL_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBRTTTL_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBRTTTL_INSTALL_BIN_DIR}
)
