# MMORPG Project Documentation

## Project Structure
- **Characters**: Contains `PlayerCharacter.cpp` and `PlayerCharacter.h` for third-person movement, attack combos, lock-on, and stamina system.
- **Combat**: Contains `WeaponComponent.cpp` and `WeaponComponent.h` for weapon logic, attack chaining, and durability.
- **Skills**: Contains `SkillSystemComponent.cpp` and `SkillSystemComponent.h` for skill management and sample skills.
- **AI**: Contains `EnemyAI.cpp` and `EnemyAI.h` for enemy behavior and weak spot logic.
- **UI**: Contains `HUDSystem.cpp` and `HUDSystem.h` for health, stamina, and skill cooldown indicators.
- **World**: Contains `ZoneSystem.cpp` and `ZoneSystem.h` for test floor, environment, and boss arena trigger.
- **Networking**: Reserved for future multiplayer support.
- **Lore**: Reserved for world-building content.

## Dependencies
- Unreal Engine 5
- C++ and Blueprints

## Usage
- Attach `PlayerCharacter` to a Blueprint-derived character for full functionality.
- Attach `WeaponComponent` and `SkillSystemComponent` to player or enemy characters.
- Attach `EnemyAI` to enemy character Blueprints.
- Attach `HUDSystem` to player HUD Blueprint.
- Attach `ZoneSystem` to level Blueprint or as a world subsystem.

## Performance
- Optimized for networked play and minimal draw calls.

## Security
- Input validation and anti-cheat hooks recommended.

## Changelog
- Initial scaffold 

## Central Hub: Eden One City
Eden One City is the immersive, modular central hub of AINCRAD, inspired by Disney/Universal theming and advanced worldbuilding. It features water, glass, floating gardens, and starlit vistas, serving as the launchpad to all other ages and worlds.

- For a quantum-detailed narrative and design, see [World/Hub_System.md](./World/Hub_System.md).
- For code structure, see `World/CityZone.h` and `World/CityZoneRegistry.h`.
- For the full Eden One City project, visit the [Eden One City GitHub repository](https://github.com/eden-one-city/eden-one-city).

--- 