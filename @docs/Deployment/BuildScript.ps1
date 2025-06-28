# Aincrad VR Build Script
# This script automates the build and deployment process for both Oculus and Steam VR platforms

# Configuration
$ProjectPath = "C:\Users\sunny\Saved Games\Aincrad\Aincrad\Aincrad.uproject"
$BuildPath = "C:\Builds"
$UE5Path = "C:\Program Files\Epic Games\UE_5.0\Engine\Binaries\Win64\UnrealEditor.exe"
$Version = "1.0.0"

# Create build directories
New-Item -ItemType Directory -Force -Path "$BuildPath\Development"
New-Item -ItemType Directory -Force -Path "$BuildPath\Shipping"
New-Item -ItemType Directory -Force -Path "$BuildPath\Oculus"
New-Item -ItemType Directory -Force -Path "$BuildPath\Steam"

# Function to run UE5 build command
function Invoke-UE5Build {
    param (
        [string]$Configuration,
        [string]$Target,
        [string]$OutputPath
    )
    
    Write-Host "Building $Configuration for $Target..."
    & $UE5Path $ProjectPath -build -configuration $Configuration -platform Win64 -target $Target -output $OutputPath
}

# Function to run tests
function Invoke-VRTests {
    param (
        [string]$BuildPath
    )
    
    Write-Host "Running VR tests..."
    # Add test commands here
    # Example: & "$BuildPath\Aincrad.exe" -test
}

# Function to package for distribution
function Invoke-PackageBuild {
    param (
        [string]$SourcePath,
        [string]$TargetPath,
        [string]$Platform
    )
    
    Write-Host "Packaging for $Platform..."
    # Add packaging commands here
    # Example: Compress-Archive -Path $SourcePath -DestinationPath $TargetPath
}

# Main build process
try {
    # Development build
    Invoke-UE5Build -Configuration "Development" -Target "Development" -OutputPath "$BuildPath\Development"
    Invoke-VRTests -BuildPath "$BuildPath\Development"
    
    # Shipping build
    Invoke-UE5Build -Configuration "Shipping" -Target "Shipping" -OutputPath "$BuildPath\Shipping"
    Invoke-VRTests -BuildPath "$BuildPath\Shipping"
    
    # Oculus build
    Invoke-UE5Build -Configuration "Shipping" -Target "Oculus" -OutputPath "$BuildPath\Oculus"
    Invoke-PackageBuild -SourcePath "$BuildPath\Oculus" -TargetPath "$BuildPath\Oculus\Package" -Platform "Oculus"
    
    # Steam build
    Invoke-UE5Build -Configuration "Shipping" -Target "Steam" -OutputPath "$BuildPath\Steam"
    Invoke-PackageBuild -SourcePath "$BuildPath\Steam" -TargetPath "$BuildPath\Steam\Package" -Platform "Steam"
    
    Write-Host "Build process completed successfully!"
} catch {
    Write-Error "Build process failed: $_"
    exit 1
}

# Create deployment packages
$DeploymentPath = "C:\Deployment"
New-Item -ItemType Directory -Force -Path $DeploymentPath

# Oculus package
$OculusPackage = @{
    "version" = $Version
    "platform" = "Oculus"
    "build_date" = Get-Date -Format "yyyy-MM-dd"
    "build_path" = "$BuildPath\Oculus\Package"
    "requirements" = @{
        "min_ram" = "8GB"
        "min_gpu" = "NVIDIA GTX 1060 6GB"
        "min_cpu" = "Intel i5-4590"
        "os" = "Windows 10 64-bit"
    }
}

# Steam package
$SteamPackage = @{
    "version" = $Version
    "platform" = "Steam"
    "build_date" = Get-Date -Format "yyyy-MM-dd"
    "build_path" = "$BuildPath\Steam\Package"
    "requirements" = @{
        "min_ram" = "8GB"
        "min_gpu" = "NVIDIA GTX 1060 6GB"
        "min_cpu" = "Intel i5-4590"
        "os" = "Windows 10 64-bit"
    }
}

# Save package information
$OculusPackage | ConvertTo-Json | Out-File "$DeploymentPath\oculus_package.json"
$SteamPackage | ConvertTo-Json | Out-File "$DeploymentPath\steam_package.json"

Write-Host "Deployment packages created successfully!" 