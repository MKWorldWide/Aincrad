# Steam Deployment Guide

## Steam SDK Integration
- **SDK Location**: `C:\Users\sunny\Downloads\Steamsdk`
- **Required Components**:
  - Steamworks SDK
  - Steam API
  - Steam Client
  - Steam Cloud
  - Steam Achievements
  - Steam Leaderboards
  - Steam Friends
  - Steam Workshop

## Version Management
### Version Types
1. **Alpha Version**
   - Early access for testing
   - Limited features
   - Development builds
   - Test server access

2. **Beta Version**
   - Feature complete
   - Performance testing
   - Community feedback
   - Stable server access

3. **Release Version**
   - Full feature set
   - Optimized performance
   - Production servers
   - Public access

4. **VR Version**
   - VR-specific features
   - Motion controls
   - Room-scale support
   - VR comfort features

## Deployment Process
1. **Version Configuration**
   ```ini
   [Steam]
   AppID=1234567
   BetaName=alpha
   BetaPassword=test123
   BuildID=123456
   ```

2. **Build Types**
   ```ini
   [Builds]
   Alpha=development
   Beta=testing
   Release=shipping
   VR=vr_shipping
   ```

3. **Depot Configuration**
   ```ini
   [Depots]
   Alpha=1234567
   Beta=1234568
   Release=1234569
   VR=1234570
   ```

## Steam Features Integration
### Achievements
```cpp
// Achievement definitions
const Achievement_t g_rgAchievements[] = {
    {"ACH_FIRST_LOGIN", "First Login", "Log into Aincrad for the first time"},
    {"ACH_FIRST_FLOOR", "Floor 1 Clear", "Clear the first floor of Aincrad"},
    {"ACH_SWORD_SKILL", "Sword Master", "Master the basic sword skill"},
    // Add more achievements
};
```

### Leaderboards
```cpp
// Leaderboard definitions
const Leaderboard_t g_rgLeaderboards[] = {
    {"LB_FLOOR_CLEAR", "Floor Clear Time", k_ELeaderboardSortMethodAscending},
    {"LB_PVP_WINS", "PVP Victories", k_ELeaderboardSortMethodDescending},
    {"LB_SKILL_LEVEL", "Skill Mastery", k_ELeaderboardSortMethodDescending},
    // Add more leaderboards
};
```

### Workshop Support
```cpp
// Workshop item types
const WorkshopItem_t g_rgWorkshopItems[] = {
    {"WS_CUSTOM_SWORD", "Custom Sword", "Player-created sword designs"},
    {"WS_CUSTOM_ARMOR", "Custom Armor", "Player-created armor designs"},
    {"WS_CUSTOM_SKILL", "Custom Skill", "Player-created skill effects"},
    // Add more workshop items
};
```

## Version-Specific Features
### Alpha Version
- Basic game mechanics
- First floor only
- Limited skills
- Test server access
- Basic VR support

### Beta Version
- All floors
- Full skill system
- PVP arena
- Community features
- Enhanced VR support

### Release Version
- Complete game
- All features
- Production servers
- Full VR support
- Workshop integration

### VR Version
- Optimized VR controls
- Room-scale support
- Motion controls
- VR comfort features
- Cross-platform VR

## Deployment Script
```powershell
# Steam deployment script
$SteamSDK = "C:\Users\sunny\Downloads\Steamsdk"
$AppID = "1234567"
$BuildPath = "C:\Builds"

# Function to deploy version
function Deploy-SteamVersion {
    param (
        [string]$Version,
        [string]$BuildType,
        [string]$DepotID
    )
    
    # Build the game
    & "$BuildPath\BuildScript.ps1" -Configuration $BuildType
    
    # Upload to Steam
    & "$SteamSDK\tools\ContentBuilder\builder\steamcmd.exe" +login $SteamUser $SteamPass +app_build "C:\Deployment\steam_app_build.vdf" +run_app_build
}

# Deploy all versions
Deploy-SteamVersion -Version "Alpha" -BuildType "Development" -DepotID "1234567"
Deploy-SteamVersion -Version "Beta" -BuildType "Testing" -DepotID "1234568"
Deploy-SteamVersion -Version "Release" -BuildType "Shipping" -DepotID "1234569"
Deploy-SteamVersion -Version "VR" -BuildType "VR_Shipping" -DepotID "1234570"
```

## Testing Protocol
1. **Version Testing**
   - Feature verification
   - Performance testing
   - Compatibility testing
   - Server testing

2. **Steam Integration Testing**
   - Achievement testing
   - Leaderboard testing
   - Workshop testing
   - Cloud save testing

3. **VR Testing**
   - Motion control testing
   - Room-scale testing
   - Performance testing
   - Comfort feature testing

## Monitoring and Analytics
- **Steam Analytics**
  - Player statistics
  - Achievement rates
  - Leaderboard activity
  - Workshop activity

- **Performance Monitoring**
  - FPS tracking
  - Memory usage
  - Network performance
  - VR performance

## Update Process
1. **Version Updates**
   - Alpha: Daily updates
   - Beta: Weekly updates
   - Release: Monthly updates
   - VR: Bi-weekly updates

2. **Hotfix Process**
   - Critical bug fixes
   - Performance patches
   - Security updates
   - Server maintenance

## Community Management
- **Steam Community**
  - Discussion forums
  - Bug reporting
  - Feature requests
  - Community guides

- **Workshop Management**
  - Content moderation
  - Featured items
  - Community events
  - Creator support 