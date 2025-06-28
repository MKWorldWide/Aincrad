@echo off
setlocal enabledelayedexpansion

REM Aincrad Build Script for Windows
REM This script automates the build process for the Aincrad project on Windows

REM Configuration
set PROJECT_NAME=Aincrad
set VERSION=1.0.0
set BUILD_TYPE=Release
set BUILD_DIR=build
set INSTALL_DIR=install
set PARALLEL_JOBS=4

REM Build options
set ENABLE_VR=true
set ENABLE_VISION_PRO=false
set ENABLE_STEAM=true
set ENABLE_TESTING=true
set ENABLE_PROFILING=false
set BUILD_TOOLS=true
set BUILD_EXAMPLES=false

REM Parse command line arguments
:parse_args
if "%1"=="" goto :main
if "%1"=="--debug" (
    set BUILD_TYPE=Debug
    shift
    goto :parse_args
)
if "%1"=="--release" (
    set BUILD_TYPE=Release
    shift
    goto :parse_args
)
if "%1"=="--no-vr" (
    set ENABLE_VR=false
    shift
    goto :parse_args
)
if "%1"=="--vision-pro" (
    set ENABLE_VISION_PRO=true
    shift
    goto :parse_args
)
if "%1"=="--no-steam" (
    set ENABLE_STEAM=false
    shift
    goto :parse_args
)
if "%1"=="--no-tests" (
    set ENABLE_TESTING=false
    shift
    goto :parse_args
)
if "%1"=="--profiling" (
    set ENABLE_PROFILING=true
    shift
    goto :parse_args
)
if "%1"=="--no-tools" (
    set BUILD_TOOLS=false
    shift
    goto :parse_args
)
if "%1"=="--examples" (
    set BUILD_EXAMPLES=true
    shift
    goto :parse_args
)
if "%1"=="--jobs" (
    set PARALLEL_JOBS=%2
    shift
    shift
    goto :parse_args
)
if "%1"=="-j" (
    set PARALLEL_JOBS=%2
    shift
    shift
    goto :parse_args
)
if "%1"=="--help" goto :show_help
if "%1"=="-h" goto :show_help
echo Unknown option: %1
goto :show_help

:show_help
echo Usage: %0 [OPTIONS]
echo.
echo Build the Aincrad project on Windows
echo.
echo Options:
echo   --debug              Build in Debug mode
echo   --release            Build in Release mode (default)
echo   --no-vr              Disable VR support
echo   --vision-pro         Enable Vision Pro support
echo   --no-steam           Disable Steam integration
echo   --no-tests           Disable testing
echo   --profiling          Enable profiling support
echo   --no-tools           Disable building tools
echo   --examples           Build example applications
echo   --jobs, -j N         Number of parallel jobs (default: 4)
echo   --help, -h           Show this help message
echo.
echo Examples:
echo   %0                    # Build with default settings
echo   %0 --debug            # Build in debug mode
echo   %0 --no-vr --no-tests # Build without VR and tests
echo   %0 --jobs 8           # Build with 8 parallel jobs
exit /b 1

:main
echo [INFO] Starting Aincrad build process...
echo [INFO] Platform: Windows

REM Show build configuration
call :show_summary

REM Check dependencies
call :check_dependencies

REM Clean build directory
call :clean_build

REM Create build directory
call :create_build_dir

REM Configure CMake
call :configure_cmake

REM Build project
call :build_project

REM Run tests
call :run_tests

REM Install project
call :install_project

REM Create package
call :create_package

echo [SUCCESS] Build process completed successfully!
echo [INFO] You can find the installed files in: %INSTALL_DIR%
echo [INFO] Package created: aincrad-%VERSION%-windows-%BUILD_TYPE%.zip
exit /b 0

:check_dependencies
echo [INFO] Checking dependencies...

REM Check CMake
cmake --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake is not installed. Please install CMake 3.14 or higher.
    exit /b 1
)

for /f "tokens=3" %%i in ('cmake --version 2^>nul ^| findstr /r "cmake version"') do set CMAKE_VERSION=%%i
echo [INFO] Found CMake version: %CMAKE_VERSION%

REM Check Visual Studio
where cl >nul 2>&1
if errorlevel 1 (
    echo [WARNING] Visual Studio compiler not found in PATH. Make sure to run from Developer Command Prompt.
)

echo [INFO] Dependencies check completed
goto :eof

:show_summary
echo [INFO] Build Summary:
echo   Project: %PROJECT_NAME%
echo   Version: %VERSION%
echo   Platform: Windows
echo   Build Type: %BUILD_TYPE%
echo   VR Support: %ENABLE_VR%
echo   Vision Pro: %ENABLE_VISION_PRO%
echo   Steam Integration: %ENABLE_STEAM%
echo   Testing: %ENABLE_TESTING%
echo   Profiling: %ENABLE_PROFILING%
echo   Tools: %BUILD_TOOLS%
echo   Examples: %BUILD_EXAMPLES%
echo   Parallel Jobs: %PARALLEL_JOBS%
echo   Build Directory: %BUILD_DIR%
echo   Install Directory: %INSTALL_DIR%
goto :eof

:clean_build
echo [INFO] Cleaning build directory...
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
    echo [SUCCESS] Build directory cleaned
)
goto :eof

:create_build_dir
echo [INFO] Creating build directory...
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"
goto :eof

:configure_cmake
echo [INFO] Configuring CMake...

set CMAKE_ARGS=-DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DENABLE_VR=%ENABLE_VR% -DENABLE_VISION_PRO=%ENABLE_VISION_PRO% -DENABLE_STEAM=%ENABLE_STEAM% -DENABLE_TESTING=%ENABLE_TESTING% -DENABLE_PROFILING=%ENABLE_PROFILING% -DBUILD_TOOLS=%BUILD_TOOLS% -DBUILD_EXAMPLES=%BUILD_EXAMPLES% -DCMAKE_INSTALL_PREFIX=../%INSTALL_DIR% -G "Visual Studio 16 2019" -A x64

echo [INFO] CMake arguments: %CMAKE_ARGS%
cmake .. %CMAKE_ARGS%

if errorlevel 1 (
    echo [ERROR] CMake configuration failed
    exit /b 1
)

echo [SUCCESS] CMake configuration completed
goto :eof

:build_project
echo [INFO] Building project with %PARALLEL_JOBS% parallel jobs...

cmake --build . --config %BUILD_TYPE% --parallel %PARALLEL_JOBS%

if errorlevel 1 (
    echo [ERROR] Build failed
    exit /b 1
)

echo [SUCCESS] Build completed successfully
goto :eof

:run_tests
if "%ENABLE_TESTING%"=="true" (
    echo [INFO] Running tests...
    
    ctest --output-on-failure --verbose
    
    if errorlevel 1 (
        echo [ERROR] Some tests failed
        exit /b 1
    )
    
    echo [SUCCESS] All tests passed
) else (
    echo [INFO] Testing disabled, skipping tests
)
goto :eof

:install_project
echo [INFO] Installing project...

cmake --install .

if errorlevel 1 (
    echo [ERROR] Installation failed
    exit /b 1
)

echo [SUCCESS] Installation completed
echo [INFO] Installed to: %CD%\..\%INSTALL_DIR%
goto :eof

:create_package
echo [INFO] Creating package...

set PACKAGE_NAME=aincrad-%VERSION%-windows-%BUILD_TYPE%
set PACKAGE_DIR=..\%PACKAGE_NAME%

REM Create package directory
mkdir "%PACKAGE_DIR%"

REM Copy binaries
if exist "bin" (
    xcopy /e /i "bin" "%PACKAGE_DIR%\"
)

REM Copy libraries
if exist "lib" (
    xcopy /e /i "lib" "%PACKAGE_DIR%\"
)

REM Copy documentation
if exist "..\@docs" (
    xcopy /e /i "..\@docs" "%PACKAGE_DIR%\docs\"
)

REM Copy README and license
copy "..\README.md" "%PACKAGE_DIR%\"
copy "..\LICENSE" "%PACKAGE_DIR%\"
copy "..\CHANGELOG.md" "%PACKAGE_DIR%\"

REM Create zip archive
cd ..
powershell -command "Compress-Archive -Path '%PACKAGE_NAME%' -DestinationPath '%PACKAGE_NAME%.zip' -Force"

echo [SUCCESS] Package created: %PACKAGE_NAME%.zip
goto :eof 