# Modular World System

This directory contains the modular components of the Glasswater Hub world, designed to work together while staying within VRChat's 100MB limit per world.

## Module Structure

```
Assets/
├── Modules/
│   ├── Core/               # Shared assets and systems
│   │   ├── Prefabs/       # Shared prefabs
│   │   ├── Scripts/       # Shared scripts
│   │   └── Materials/     # Shared materials
│   │
│   ├── Hub/               # Main hub world
│   │   ├── Scenes/       # Hub scene
│   │   ├── Prefabs/      # Hub-specific prefabs
│   │   └── Materials/    # Hub-specific materials
│   │
│   ├── UpperLevels/       # Skyroot District
│   │   ├── Scenes/       # Upper level scenes
│   │   ├── Prefabs/      # Upper level prefabs
│   │   └── Materials/    # Upper level materials
│   │
│   └── LowerLevels/       # Glasswater Data Sanctum
│       ├── Scenes/       # Lower level scenes
│       ├── Prefabs/      # Lower level prefabs
│       └── Materials/    # Lower level materials
```

## Module Dependencies

- All modules depend on the `Core` module for shared assets
- `Hub` can work independently but is designed to connect to other modules
- `UpperLevels` and `LowerLevels` are completely independent of each other

## Build Process

1. Build each module as a separate VRChat world
2. Use the `ModuleManager` to handle world transitions
3. Each module should stay under 100MB when built

## Adding New Modules

1. Create a new folder under `Assets/Modules/`
2. Add your module's scenes, prefabs, and assets
3. Update the `ModuleManager` to handle the new module
4. Test the module independently and with the hub

## Optimization Guidelines

- Keep shared assets in the `Core` module
- Use addressables for large assets
- Optimize textures and models for mobile
- Test each module's build size before integration
