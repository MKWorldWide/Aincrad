# Memories

// This file is auto-initialized by Cursor for session continuity.
// Use this to track important project context, decisions, and reminders.

## Project Structure
- Content/: Contains game content and assets
- World/: Contains `ZoneSystem.cpp` and `ZoneSystem.h` for test floor, environment, and boss arena trigger
- Characters/: Contains `PlayerCharacter.cpp` and `PlayerCharacter.h` for third-person movement, attack combos, lock-on, and stamina system
- AincradTerminal/: Terminal interface implementation
- @docs/: Project documentation
- UI/: Contains `HUDSystem.cpp` and `HUDSystem.h` for health, stamina, and skill cooldown indicators
- AI/: Contains `EnemyAI.cpp` and `EnemyAI.h` for enemy behavior and weak spot logic
- Skills/: Contains `SkillSystemComponent.cpp` and `SkillSystemComponent.h` for skill management and sample skills
- Combat/: Contains `WeaponComponent.cpp` and `WeaponComponent.h` for weapon logic, attack chaining, and durability

## Technical Stack
- Unreal Engine 5
- C++ and Blueprints
- Networking support planned for future
- Custom physics engine for time dilation
- AR framework integration
- Oculus SDK integration
- Steam VR SDK integration
- Automated build system
- Automated testing framework
- Steam SDK integration

## Project Context
- SAO-inspired game development project
- Focus on creating an immersive virtual world experience
- Multiple interconnected systems (Combat, Skills, AI, etc.)
- Terminal-based interface component
- Third-person action RPG mechanics
- Advanced physics and time manipulation
- AR integration for real-world interaction
- VR support for Oculus Rift and Steam VR
- Automated deployment pipeline
- Multiple version deployment (Alpha, Beta, Release, VR)

## Recent Activities
- Initialized project structure
- Set up documentation framework
- Created core system directories
- Implemented basic character and combat systems
- Set up zone and environment systems
- Designed first 5 floors of Aincrad
- Implemented advanced physics systems
- Integrated AR features
- Configured VR support for Oculus and Steam
- Set up VR-specific input and movement systems
- Created automated build scripts
- Implemented automated testing framework
- Integrated Steam SDK
- Set up multiple version deployment

## Important Decisions
- Using a modular architecture for system components
- Maintaining separate documentation in @docs/ directory
- Following SAO-inspired game mechanics and systems
- Implementing third-person combat with combo system
- Using Unreal Engine 5 for development
- Planning for future multiplayer support
- Implementing 1:1000 time dilation
- Integrating AR features for real-world interaction
- Supporting both Oculus Rift and Steam VR platforms
- Implementing VR-specific comfort features
- Using automated build and test pipelines
- Implementing comprehensive performance monitoring
- Supporting multiple game versions on Steam
- Implementing Steam Workshop support

## Component Dependencies
- PlayerCharacter requires WeaponComponent and SkillSystemComponent
- Enemy characters need EnemyAI component
- HUD requires HUDSystem component
- ZoneSystem can be attached to level Blueprint or as world subsystem
- Physics system requires custom solver and time dilation controller
- AR system needs real-world object recognition and environmental mapping
- VR system requires motion controller support and comfort features
- Build system requires Unreal Engine 5 and platform SDKs
- Test system requires performance monitoring tools
- Steam integration requires Steam SDK and API

## World Design
- Floor 1: Town of Beginnings (Medieval European town)
- Floor 2: Forest of Wandering (Magical forest)
- Floor 3: Desert of Illusions (Mystical desert)
- Floor 4: Floating Islands (Sky islands)
- Floor 5: Crystal Caverns (Underground crystal realm)
- Each floor features unique physics and environmental interactions
- AR integration for real-world quests and events
- VR-specific optimizations for each floor

## VR Features
- Motion controller support for combat
- Gesture-based skill activation
- Room-scale and seated play support
- Comfort features (vignette, snap turning)
- Performance optimizations
- Cross-platform compatibility
- Social features integration

## Build System
- Automated build scripts for both platforms
- Development and shipping configurations
- Platform-specific optimizations
- Automated testing integration
- Performance monitoring
- Deployment packaging
- Version control integration

## Test System
- Automated VR testing framework
- Performance benchmarking
- Comfort feature testing
- Controller mapping verification
- Cross-platform compatibility testing
- Automated test reporting
- Continuous integration support

## Steam Integration
- Multiple version support (Alpha, Beta, Release, VR)
- Steam Workshop integration
- Achievement system
- Leaderboard system
- Cloud save support
- Community features
- Analytics tracking
- Update management 