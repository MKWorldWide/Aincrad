# Aincrad Shared Asset Management System

A cross-platform asset management system for the Aincrad VR hub, designed to handle shared assets across multiple platforms (Windows, Mac, Linux, and VR).

## Features

- **Cross-Platform Support**: Optimized asset handling for Windows, Mac, Linux, and VR platforms
- **Asset Pipeline**: Comprehensive asset creation and processing pipeline
- **Asset Management**: Centralized asset database with metadata management
- **Asset Distribution**: Efficient asset distribution with delta updates and streaming
- **Memory Management**: Advanced memory management with platform-specific optimizations
- **Asset Validation**: Robust asset validation and quality control
- **Update System**: Flexible update system with version control

## Architecture

### Asset Pipeline
- High-poly to low-poly workflow
- PBR texture workflow
- Animation processing
- Platform-specific optimization

### Asset Management
- Centralized asset database
- Metadata management
- Dependency tracking
- Asset validation

### Asset Distribution
- Delta updates
- Asset streaming
- Platform-specific packages
- Version control

### Memory Management
- Pool allocation
- Dynamic allocation
- Platform-specific optimization
- Quality-based management

## Building

### Prerequisites
- CMake 3.14 or higher
- C++17 compatible compiler
- Google Test
- JsonCpp

### Build Steps
```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
ctest
```

## Usage

### Asset Creation
```cpp
// Create asset metadata
AssetMetadata metadata;
metadata.assetId = "my_asset";
metadata.assetType = "model";
metadata.platforms = {"windows", "mac", "linux"};
metadata.dependencies = {};
metadata.version = "1.0.0";
metadata.permissions = "public";
metadata.usage = "game";

// Add to database
assetManager->m_assetDatabase->addAssetMetadata(metadata);
```

### Asset Loading
```cpp
// Load asset
auto asset = assetManager->loadAsset("my_asset");
if (asset) {
    // Use asset
}
```

### Asset Unloading
```cpp
// Unload asset
assetManager->unloadAsset("my_asset");
```

### Asset Update
```cpp
// Update asset manager
assetManager->update();
```

## Platform-Specific Optimization

### Windows
- DirectX 12 optimization
- VR support
- Memory management

### Mac
- Metal optimization
- Apple Silicon support
- Battery optimization

### Linux
- Vulkan optimization
- OpenXR integration
- Performance tuning

### VR Platforms
- Oculus optimization
- SteamVR support
- Mobile optimization

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- VRChat for inspiration
- Modern VR games for best practices
- Open source community for tools and libraries 