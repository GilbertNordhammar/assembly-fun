$SOLUTION_DIR = "AssemblyFun"

# Create the build directory if it doesn't already exist
if (!(Test-Path $SOLUTION_DIR)) {
    New-Item -ItemType Directory -Path $SOLUTION_DIR | Out-Null
}

# Generate the build files with CMake
& cmake -DCMAKE_GENERATOR_PLATFORM=x64 -S . -B $SOLUTION_DIR -G "Visual Studio 16 2019"
