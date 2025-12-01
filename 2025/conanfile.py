from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout

require_conan_version = ">=2.0"

class CSTConan(ConanFile):
    name = "cst"
    settings = "os", "compiler", "build_type", "arch"
    version = "0.1"

    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }

    default_options = {
        "shared": False,
        "fPIC": True,
    }

    exports_sources = "cmake/*", "src/*", "CMakeLists.txt", "LICENSE"

    implements = ["auto_shared_fpic"]

    def requirements(self):
        self.requires("fmt/12.0.0")

    def build_requirements(self):
        self.tool_requires("cmake/[^3.27]")
        self.test_requires("catch2/3.11.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = "ConanPresets.json"
        tc.generate()

        cmake = CMakeDeps(self)
        cmake.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

