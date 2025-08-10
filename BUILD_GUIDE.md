# Glasswater Hub - Build Guide

This guide will walk you through building and deploying the Glasswater Hub VRChat world.

## Prerequisites

- Unity 2022.3 LTS or later
- VRChat Creator Companion (VCC)
- VRChat SDK3 - Worlds
- UdonSharp
- TextMeshPro (import via Unity Package Manager)

## Setup Instructions

1. **Clone the Repository**
   ```bash
   git clone https://github.com/yourusername/glasswater-hub.git
   cd glasswater-hub
   ```

2. **Open the Project in Unity**
   - Launch Unity Hub
   - Click "Add Project" and select the project folder
   - Make sure to use Unity 2022.3 LTS or later

3. **Import Required Packages**
   - Open VRChat Creator Companion (VCC)
   - Go to the Project tab
   - Click "Add" and add the following packages:
     - VRChat SDK3 - Worlds
     - UdonSharp
     - TextMeshPro

## Building the World

### Windows Build

1. Open the build menu: `Build/Build Windows (Production)`
2. The build will be saved to `Builds/Windows/GlasswaterHub/`
3. Test the build locally before uploading to VRChat

### Quest Build

1. Switch to Android platform: `File > Build Settings > Android`
2. Open the build menu: `Build/Android (Quest)`
3. The APK will be saved to `Builds/Android/`
4. Install on your Quest device for testing

## Performance Optimization

1. Open the Performance Optimizer: `Aincrad/Optimize Scene`
2. Select the optimizations you want to apply
3. Click "Optimize Current Scene"
4. Use the "Check Performance" button to verify optimizations

## Uploading to VRChat

1. Build the world for your target platform
2. Open the VRChat SDK Control Panel: `VRChat SDK > Show Control Panel`
3. Go to the "Builder" tab
4. Select your scene and click "Build & Publish for Windows" or "Build & Publish for Android"
5. Fill in the world details and upload

## Development Workflow

1. **Scene Structure**
   - `Main` scene contains the core world
   - Prefabs are organized in `Assets/Prefabs`
   - Scripts are in `Assets/Udon`
   - Materials and shaders are in `Assets/Materials` and `Assets/Shaders`

2. **Version Control**
   - Use Git LFS for large files
   - Follow the GitFlow branching model
   - Create feature branches for new features
   - Submit pull requests for code review

3. **Testing**
   - Test on both PC and Quest
   - Verify performance with the in-game debug menu
   - Check for any console errors or warnings

## Performance Guidelines

### Target Performance
- **PC**: 90 FPS on mid-range hardware
- **Quest 2**: 72 FPS (80 FPS for Quest Pro)
- **Draw Calls**: < 100 (Quest), < 500 (PC)
- **Triangles**: < 100k (Quest), < 500k (PC)
- **Textures**: < 50MB (Quest), < 200MB (PC)

### Optimization Tips
- Use LOD groups for complex models
- Combine meshes where possible
- Use texture atlases
- Enable GPU instancing
- Use occlusion culling
- Minimize real-time lights
- Use baked lighting where possible

## Troubleshooting

### Common Issues

**Build Fails**
- Make sure all required packages are imported
- Check for compilation errors in the console
- Verify the build target platform is set correctly

**Performance Issues**
- Use the Performance Optimizer tool
- Check the profiler for bottlenecks
- Reduce polygon count and texture sizes

**VR Issues**
- Test in VR frequently
- Ensure proper scale (1 unit = 1 meter)
- Check for any camera-related issues

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

- VRChat Team for the amazing platform
- UdonSharp developers
- The VRChat community for support and inspiration
