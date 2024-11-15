find_package(spdlog)

add_executable(demo
demo.cpp
animal.cpp
opengldrawer.cpp
)

target_link_libraries(demo
  PRIVATE
  cppte
  spdlog::spdlog
  )
