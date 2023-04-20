CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT("Photon")

# --- FILES --- #
# Main files
SET(MAIN_FILE
    "main.cpp"
)

# Source files
SET(SOURCE_FILES
    "Assembler/Assembler.cpp"
    "CommandLine/CommandLine.cpp"
    "ComponentTest/ComponentTest.cpp"
    "ComponentTest/ComponentTests.cpp"
    "Electronics/GPIO.cpp"
    "Files/Files.cpp"
    "SimulationTestBuilder/SimulationTestBuilder.cpp"
    "Util/Util.cpp" 
    "Util/Errors.cpp" 
    "Util/Timer.cpp" 
)

# Test files
SET(TEST_FILES
    "TestAssembler.cpp"
    "TestFiles.cpp"
    "TestTestBuilder.cpp"
)

#set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_CXX_STANDARD 17)

# Create library
ADD_LIBRARY(photon_library INTERFACE)
TARGET_COMPILE_OPTIONS(photon_library INTERFACE) # -fstandalone-debug
TARGET_LINK_LIBRARIES(photon_library INTERFACE)
target_compile_options(photon_library PRIVATE -lwiringPi)

# Subdirectories
ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(tests)