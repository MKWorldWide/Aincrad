# Glasswater + Skyroot Hub - Unity Project

This is the Unity project for the VRChat world "Glasswater + Skyroot Hub".

## Project Structure

- **/Assets/Scenes** - Unity scene files
- **/Assets/Materials** - Shared materials and shaders
- **/Assets/Textures** - Texture assets
- **/Assets/Audio** - Sound effects and music
- **/Assets/Udon** - UdonSharp scripts
- **/Assets/Prefabs** - Reusable prefabs
- **/Assets/Shaders** - Custom shaders
- **/Assets/Animations** - Animation clips and controllers
- **/Assets/Resources** - Resources loaded at runtime

## Key Scripts

1. **GlassTeleport.cs**
   - Handles in-scene teleportation when interacting with glass windows
   - Attach to any object that should teleport the player

2. **SpawnPortal.cs**
   - Spawns a VRChat portal when interacted with
   - Can be configured with world ID and auto-destruction timer

## Setup Instructions

1. Import VRChat SDK3 and UdonSharp into the project
2. Create a new scene in the Scenes folder
3. Set up your environment with the following layers:
   - MirrorReflection
   - PlayerLocal
   - Player
   - Interactive
   - UI

## Performance Guidelines

- Keep textures under 2K resolution for Quest
- Use texture atlasing where possible
- Enable occlusion culling for complex scenes
- Use LOD groups for detailed models
- Keep draw calls under 100 for Quest

## Publishing

1. Test in VRChat SDK Play Mode
2. Build for Windows/Android as needed
3. Upload through VRChat SDK Control Panel
