# Aincrad Portal System

A comprehensive portal system for VRChat worlds, supporting multiple portal types and optimized for performance.

## Features

- **Multiple Portal Types**:
  - **World Portals**: Connect to other VRChat worlds
  - **Render Texture Portals**: Display live camera feeds within the same world
  - **Video Portals**: Play video content on portal surfaces

- **Performance Optimizations**:
  - Configurable maximum active portals
  - Automatic cleanup of unused resources
  - Performance modes for different hardware

- **Easy Configuration**:
  - Centralized configuration asset
  - Per-portal settings
  - Visual feedback for interactions

## Setup Instructions

### 1. Basic Setup

1. Attach the `PortalSystem` script to an empty GameObject in your scene
2. Configure the portal settings in the Inspector
3. Add your portals to the `portals` array

### 2. Configuring a World Portal

1. Create a new PortalData entry
2. Set Type to `WorldPortal`
3. Enter the target world ID
4. Assign the portal's transform

### 3. Configuring a Render Texture Portal

1. Create a new PortalData entry
2. Set Type to `RenderTexture`
3. Assign a camera that will render to the portal
4. Create and assign a Render Texture asset
5. Assign a material that will display the render texture

### 4. Configuring a Video Portal

1. Create a new PortalData entry
2. Set Type to `VideoPanel`
3. Assign a VideoPlayer component
4. Create and assign a Render Texture asset
5. Assign a material that will display the video

## Performance Tips

- Keep the number of active portals low (2-3 max for Quest)
- Use lower resolution render textures for better performance
- Enable Performance Mode in the PortalConfig for mobile platforms
- Use the `autoCloseDelay` to automatically close portals after use

## Example Scenes

Example scenes demonstrating different portal configurations can be found in `Assets/Examples/Portals`.

## Troubleshooting

### Portals not rendering
- Ensure the camera's Culling Mask includes all layers you want to render
- Check that the Render Texture is properly assigned to both the camera and material

### Video not playing
- Make sure the video file is compatible with Unity's VideoPlayer
- Check that the video path is correct and accessible

### Performance issues
- Reduce the number of active portals
- Lower the render texture resolution
- Enable Performance Mode in PortalConfig

## API Reference

### PortalSystem

#### Properties
- `PortalData[] portals` - Array of all configured portals
- `int maxActivePortals` - Maximum number of portals that can be active at once
- `float interactionDistance` - Maximum interaction distance

#### Methods
- `void ActivatePortal(PortalData portal)` - Activates the specified portal
- `void ClosePortal(PortalData portal)` - Closes the specified portal
- `void ToggleRenderTexturePortal(PortalData portal)` - Toggles a render texture portal
- `void ToggleVideoPortal(PortalData portal)` - Toggles a video portal

### PortalData

#### Properties
- `string portalName` - Display name of the portal
- `PortalType portalType` - Type of portal (WorldPortal, RenderTexture, VideoPanel)
- `Transform portalTransform` - The transform of the portal object
- `string worldId` - Target world ID (for WorldPortal)
- `Camera renderCamera` - Camera that renders to the portal (for RenderTexture)
- `VideoPlayer videoPlayer` - Video player component (for VideoPanel)
- `RenderTexture renderTexture` - Render texture used by the portal
- `Material screenMaterial` - Material that displays the portal
- `float autoCloseDelay` - Time in seconds before auto-closing (0 = never)
- `AudioClip activationSound` - Sound played when portal is activated
- `ParticleSystem activationEffect` - Particle effect played when portal is activated
