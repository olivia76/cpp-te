import os

from conan import ConanFile, tools
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout, CMake
from conan.tools.build import can_run

class CppConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain", "VirtualRunEnv"
    build_policy = "missing"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    requires = (
        f"{open('../package.txt').read()}/{open('../version.txt').read()}",
        #"cppte/1.0.0",
        "spdlog/1.9.2",
        )

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            self.run(os.path.sep.join([self.cpp.build.bindir, "example"]), env="conanrun")
