set(LIBRTTTL_HEADER_FILES ""
  ${LIBRTTTL_INCLUDE_DIR}/beep/beep.h
  ${LIBRTTTL_INCLUDE_DIR}/beep/ITextGenerator.h
  ${LIBRTTTL_INCLUDE_DIR}/beep/EnglishTextGenerator.h
  ${LIBRTTTL_INCLUDE_DIR}/beep/FrenchTextGenerator.h
)

add_library(beep SHARED
  ${LIBRTTTL_HEADER_FILES}
  ${LIBRTTTL_EXPORT_HEADER}
  ${LIBRTTTL_VERSION_HEADER}
  ${LIBRTTTL_CONFIG_HEADER}
  beep_api.h
  beep.h
  beep.cpp
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(beep PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Define include directories for exported code.
target_include_directories(beep
  PUBLIC
    $<INSTALL_INTERFACE:${LIBRTTTL_INSTALL_INCLUDE_DIR}>  # for clients using the installed library.
  PRIVATE
    
    ${GTEST_INCLUDE_DIR}
)
target_link_libraries(beep PUBLIC rapidassist PRIVATE ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )

install(TARGETS beep
        EXPORT beep-targets
        ARCHIVE DESTINATION ${LIBRTTTL_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBRTTTL_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBRTTTL_INSTALL_BIN_DIR}
)
