find_package(Catch2 3 REQUIRED)
include(CTest)
include(Catch)

find_package(spdlog)

add_executable(tests
tests_tev.cpp
tests_tev2.cpp
tests_vv.cpp
)

target_link_libraries(tests
  PRIVATE
  Catch2::Catch2WithMain
  cppte
  spdlog::spdlog
  )

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/reports)
catch_discover_tests(
  tests
  OUTPUT_DIR ${PROJECT_SOURCE_DIR}/reports
  REPORTER junit
  OUTPUT_PREFIX "unittests."
  OUTPUT_SUFFIX .xml
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests
  EXTRA_ARGS --skip-benchmarks
  )
