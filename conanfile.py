from conan import ConanFile, tools
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout, CMake
from conan.tools.system.package_manager import Apt
import os

class CppTeConan(ConanFile):
    name = "cpp-te"
    version = "1.0"
    license = ""
    author = "Olivia Quinet olivia.quinet@gmail.com"
    description = "CPP-TypeErasure with visitor"
    topics = ("cpp-te")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False], "ENABLE_COVERAGE": ["ON", "OFF"], "ENABLE_TESTING": ["ON", "OFF"]}
    default_options = {"shared": True, "fPIC": True, "ENABLE_COVERAGE": "OFF", "ENABLE_TESTING": "ON", "fmt/*:shared": False}
    generators = "VirtualBuildEnv", "VirtualRunEnv"
    build_policy = "missing"
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "tests/*", "conanfile.py", "cmake/*"
    package_type = "shared-library"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["ENABLE_COVERAGE"] = self.options.ENABLE_COVERAGE
        tc.cache_variables["ENABLE_TESTING"] = self.options.ENABLE_TESTING
        tc.extra_cflags.append('-DRCPP_NO_UNWIND_PROTECT')
        tc.absolute_paths = True
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def requirements(self):
        self.test_requires("catch2/3.4.0")
        self.requires("spdlog/1.9.2", visible=True, transitive_libs=True)
        self.requires("fmt/8.0.1")

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        cmake.configure(variables={'CMAKE_EXPORT_COMPILE_COMMANDS': 'ON'})
        cmake.build()
        cmake.test()

    def package(self):
        cmake = CMake(self)
        #cmake.configure()
        cmake.install()

    def package_info(self):
        # These are default values and doesn't need to be adjusted
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.libs = ["cpptest"]
        self.env_info.LD_LIBRARY_PATH.append(os.path.join(self.package_folder, "lib"))
