find_package(spdlog)

add_library(cppte ${LIBRARY_TYPE}
te.cpp
te2.cpp
)

target_include_directories(cppte PUBLIC $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_include_directories(cppte PUBLIC ${PROJECT_BINARY_DIR} )

target_link_libraries(cppte
  INTERFACE
  spdlog::spdlog
  PUBLIC
  spdlog::spdlog
  PRIVATE
  project_options
  project_warnings
  )

include(GNUInstallDirs)

install(TARGETS cppte
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  COMPONENT Devel
  FILES_MATCHING PATTERN "*.hpp"
)
