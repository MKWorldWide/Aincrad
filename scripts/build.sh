#!/bin/bash

# Aincrad Build Script
# This script automates the build process for the Aincrad project

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
PROJECT_NAME="Aincrad"
VERSION="1.0.0"
BUILD_TYPE="Release"
BUILD_DIR="build"
INSTALL_DIR="install"
PARALLEL_JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Build options
ENABLE_VR=true
ENABLE_VISION_PRO=false
ENABLE_STEAM=true
ENABLE_TESTING=true
ENABLE_PROFILING=false
BUILD_TOOLS=true
BUILD_EXAMPLES=false

# Platform detection
PLATFORM=$(uname -s)
case "$PLATFORM" in
    Linux*)     PLATFORM_NAME="Linux";;
    Darwin*)    PLATFORM_NAME="macOS";;
    CYGWIN*|MINGW*|MSYS*) PLATFORM_NAME="Windows";;
    *)          PLATFORM_NAME="Unknown";;
esac

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check dependencies
check_dependencies() {
    print_status "Checking dependencies..."
    
    # Check CMake
    if ! command -v cmake &> /dev/null; then
        print_error "CMake is not installed. Please install CMake 3.14 or higher."
        exit 1
    fi
    
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
    print_status "Found CMake version: $CMAKE_VERSION"
    
    # Check compiler
    if command -v g++ &> /dev/null; then
        COMPILER="g++"
        COMPILER_VERSION=$(g++ --version | head -n1)
    elif command -v clang++ &> /dev/null; then
        COMPILER="clang++"
        COMPILER_VERSION=$(clang++ --version | head -n1)
    else
        print_error "No C++ compiler found. Please install GCC or Clang."
        exit 1
    fi
    
    print_status "Found compiler: $COMPILER_VERSION"
    
    # Platform-specific checks
    case "$PLATFORM_NAME" in
        "Linux")
            # Check for required packages
            if ! pkg-config --exists jsoncpp; then
                print_warning "JsonCpp not found. You may need to install libjsoncpp-dev"
            fi
            if ! pkg-config --exists gtest; then
                print_warning "Google Test not found. You may need to install libgtest-dev"
            fi
            ;;
        "macOS")
            # Check for Homebrew packages
            if ! brew list | grep -q jsoncpp; then
                print_warning "JsonCpp not found. You may need to run: brew install jsoncpp"
            fi
            if ! brew list | grep -q googletest; then
                print_warning "Google Test not found. You may need to run: brew install googletest"
            fi
            ;;
        "Windows")
            print_status "Windows build - dependencies should be handled by vcpkg or manual installation"
            ;;
    esac
}

# Function to parse command line arguments
parse_arguments() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            --debug)
                BUILD_TYPE="Debug"
                shift
                ;;
            --release)
                BUILD_TYPE="Release"
                shift
                ;;
            --no-vr)
                ENABLE_VR=false
                shift
                ;;
            --vision-pro)
                ENABLE_VISION_PRO=true
                shift
                ;;
            --no-steam)
                ENABLE_STEAM=false
                shift
                ;;
            --no-tests)
                ENABLE_TESTING=false
                shift
                ;;
            --profiling)
                ENABLE_PROFILING=true
                shift
                ;;
            --no-tools)
                BUILD_TOOLS=false
                shift
                ;;
            --examples)
                BUILD_EXAMPLES=true
                shift
                ;;
            --jobs|-j)
                PARALLEL_JOBS="$2"
                shift 2
                ;;
            --help|-h)
                show_help
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                show_help
                exit 1
                ;;
        esac
    done
}

# Function to show help
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Build the Aincrad project"
    echo ""
    echo "Options:"
    echo "  --debug              Build in Debug mode"
    echo "  --release            Build in Release mode (default)"
    echo "  --no-vr              Disable VR support"
    echo "  --vision-pro         Enable Vision Pro support"
    echo "  --no-steam           Disable Steam integration"
    echo "  --no-tests           Disable testing"
    echo "  --profiling          Enable profiling support"
    echo "  --no-tools           Disable building tools"
    echo "  --examples           Build example applications"
    echo "  --jobs, -j N         Number of parallel jobs (default: auto-detect)"
    echo "  --help, -h           Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                    # Build with default settings"
    echo "  $0 --debug            # Build in debug mode"
    echo "  $0 --no-vr --no-tests # Build without VR and tests"
    echo "  $0 --jobs 8           # Build with 8 parallel jobs"
}

# Function to clean build directory
clean_build() {
    print_status "Cleaning build directory..."
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        print_success "Build directory cleaned"
    fi
}

# Function to create build directory
create_build_dir() {
    print_status "Creating build directory..."
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
}

# Function to configure CMake
configure_cmake() {
    print_status "Configuring CMake..."
    
    CMAKE_ARGS=(
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
        -DENABLE_VR="$ENABLE_VR"
        -DENABLE_VISION_PRO="$ENABLE_VISION_PRO"
        -DENABLE_STEAM="$ENABLE_STEAM"
        -DENABLE_TESTING="$ENABLE_TESTING"
        -DENABLE_PROFILING="$ENABLE_PROFILING"
        -DBUILD_TOOLS="$BUILD_TOOLS"
        -DBUILD_EXAMPLES="$BUILD_EXAMPLES"
        -DCMAKE_INSTALL_PREFIX="../$INSTALL_DIR"
    )
    
    # Platform-specific options
    case "$PLATFORM_NAME" in
        "Linux")
            CMAKE_ARGS+=(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
            ;;
        "macOS")
            CMAKE_ARGS+=(-DCMAKE_OSX_DEPLOYMENT_TARGET=10.15)
            ;;
        "Windows")
            CMAKE_ARGS+=(-G "Visual Studio 16 2019" -A x64)
            ;;
    esac
    
    print_status "CMake arguments: ${CMAKE_ARGS[*]}"
    cmake .. "${CMAKE_ARGS[@]}"
    
    if [ $? -eq 0 ]; then
        print_success "CMake configuration completed"
    else
        print_error "CMake configuration failed"
        exit 1
    fi
}

# Function to build the project
build_project() {
    print_status "Building project with $PARALLEL_JOBS parallel jobs..."
    
    case "$PLATFORM_NAME" in
        "Windows")
            cmake --build . --config "$BUILD_TYPE" --parallel "$PARALLEL_JOBS"
            ;;
        *)
            cmake --build . --parallel "$PARALLEL_JOBS"
            ;;
    esac
    
    if [ $? -eq 0 ]; then
        print_success "Build completed successfully"
    else
        print_error "Build failed"
        exit 1
    fi
}

# Function to run tests
run_tests() {
    if [ "$ENABLE_TESTING" = true ]; then
        print_status "Running tests..."
        
        if command -v ctest &> /dev/null; then
            ctest --output-on-failure --verbose
            if [ $? -eq 0 ]; then
                print_success "All tests passed"
            else
                print_error "Some tests failed"
                exit 1
            fi
        else
            print_warning "CTest not found, skipping tests"
        fi
    else
        print_status "Testing disabled, skipping tests"
    fi
}

# Function to install
install_project() {
    print_status "Installing project..."
    
    cmake --install .
    if [ $? -eq 0 ]; then
        print_success "Installation completed"
        print_status "Installed to: $(pwd)/../$INSTALL_DIR"
    else
        print_error "Installation failed"
        exit 1
    fi
}

# Function to create package
create_package() {
    print_status "Creating package..."
    
    PACKAGE_NAME="aincrad-${VERSION}-${PLATFORM_NAME,,}-${BUILD_TYPE,,}"
    PACKAGE_DIR="../$PACKAGE_NAME"
    
    # Create package directory
    mkdir -p "$PACKAGE_DIR"
    
    # Copy binaries
    if [ -d "bin" ]; then
        cp -r bin/* "$PACKAGE_DIR/"
    fi
    
    # Copy libraries
    if [ -d "lib" ]; then
        cp -r lib/* "$PACKAGE_DIR/"
    fi
    
    # Copy documentation
    if [ -d "../@docs" ]; then
        cp -r ../@docs "$PACKAGE_DIR/"
    fi
    
    # Copy README and license
    cp ../README.md ../LICENSE ../CHANGELOG.md "$PACKAGE_DIR/"
    
    # Create archive
    cd ..
    tar -czf "${PACKAGE_NAME}.tar.gz" "$PACKAGE_NAME"
    
    print_success "Package created: ${PACKAGE_NAME}.tar.gz"
}

# Function to show build summary
show_summary() {
    print_status "Build Summary:"
    echo "  Project: $PROJECT_NAME"
    echo "  Version: $VERSION"
    echo "  Platform: $PLATFORM_NAME"
    echo "  Build Type: $BUILD_TYPE"
    echo "  VR Support: $ENABLE_VR"
    echo "  Vision Pro: $ENABLE_VISION_PRO"
    echo "  Steam Integration: $ENABLE_STEAM"
    echo "  Testing: $ENABLE_TESTING"
    echo "  Profiling: $ENABLE_PROFILING"
    echo "  Tools: $BUILD_TOOLS"
    echo "  Examples: $BUILD_EXAMPLES"
    echo "  Parallel Jobs: $PARALLEL_JOBS"
    echo "  Build Directory: $BUILD_DIR"
    echo "  Install Directory: $INSTALL_DIR"
}

# Main function
main() {
    print_status "Starting Aincrad build process..."
    print_status "Platform: $PLATFORM_NAME"
    
    # Parse arguments
    parse_arguments "$@"
    
    # Show build configuration
    show_summary
    
    # Check dependencies
    check_dependencies
    
    # Clean build directory
    clean_build
    
    # Create build directory
    create_build_dir
    
    # Configure CMake
    configure_cmake
    
    # Build project
    build_project
    
    # Run tests
    run_tests
    
    # Install project
    install_project
    
    # Create package
    create_package
    
    print_success "Build process completed successfully!"
    print_status "You can find the installed files in: $INSTALL_DIR"
    print_status "Package created: aincrad-${VERSION}-${PLATFORM_NAME,,}-${BUILD_TYPE,,}.tar.gz"
}

# Run main function with all arguments
main "$@" 