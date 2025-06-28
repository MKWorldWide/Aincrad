# VR Configuration Guide

## Unreal Engine VR Settings

### Base VR Configuration
```ini
[/Script/Engine.RendererSettings]
r.VR.Enable=1
r.VR.MirrorMode=1
r.VR.MirrorModeAspectRatio=1.777778
r.VR.MirrorModeTextureSize=2048
r.VR.MirrorModeTextureFormat=2
r.VR.MirrorModeTextureMipCount=1
r.VR.MirrorModeTextureFilter=1
r.VR.MirrorModeTextureAddressMode=1
r.VR.MirrorModeTextureBorderColor=0.000000,0.000000,0.000000,1.000000
r.VR.MirrorModeTextureBorderWidth=0
r.VR.MirrorModeTextureBorderHeight=0
r.VR.MirrorModeTextureBorderDepth=0
r.VR.MirrorModeTextureBorderArray=0
r.VR.MirrorModeTextureBorderCube=0
r.VR.MirrorModeTextureBorder3D=0
r.VR.MirrorModeTextureBorder2DArray=0
r.VR.MirrorModeTextureBorderCubeArray=0
r.VR.MirrorModeTextureBorderVolume=0
r.VR.MirrorModeTextureBorderBuffer=0
r.VR.MirrorModeTextureBorderTexture2D=0
r.VR.MirrorModeTextureBorderTexture2DArray=0
r.VR.MirrorModeTextureBorderTextureCube=0
r.VR.MirrorModeTextureBorderTextureCubeArray=0
r.VR.MirrorModeTextureBorderTexture3D=0
r.VR.MirrorModeTextureBorderTextureVolume=0
r.VR.MirrorModeTextureBorderTextureBuffer=0
r.VR.MirrorModeTextureBorderTextureRenderTarget2D=0
r.VR.MirrorModeTextureBorderTextureRenderTarget2DArray=0
r.VR.MirrorModeTextureBorderTextureRenderTargetCube=0
r.VR.MirrorModeTextureBorderTextureRenderTargetCubeArray=0
r.VR.MirrorModeTextureBorderTextureRenderTargetVolume=0
r.VR.MirrorModeTextureBorderTextureRenderTargetBuffer=0
```

### Performance Settings
```ini
[/Script/Engine.RendererSettings]
r.VR.PixelDensity=1.0
r.VR.PixelDensityMin=0.8
r.VR.PixelDensityMax=1.4
r.VR.PixelDensityAdaptive=1
r.VR.PixelDensityAdaptiveTarget=90
r.VR.PixelDensityAdaptiveMin=0.8
r.VR.PixelDensityAdaptiveMax=1.4
r.VR.PixelDensityAdaptiveStep=0.1
r.VR.PixelDensityAdaptiveDelay=0.5
r.VR.PixelDensityAdaptiveHistory=10
r.VR.PixelDensityAdaptiveThreshold=0.1
r.VR.PixelDensityAdaptiveScale=1.0
```

### Motion Controller Settings
```ini
[/Script/Engine.InputSettings]
+AxisConfig=(AxisKeyName="MotionController (R) Thumbstick X",AxisProperties=(DeadZone=0.250000,Sensitivity=1.000000,Exponent=1.000000,bInvert=False))
+AxisConfig=(AxisKeyName="MotionController (R) Thumbstick Y",AxisProperties=(DeadZone=0.250000,Sensitivity=1.000000,Exponent=1.000000,bInvert=False))
+AxisConfig=(AxisKeyName="MotionController (L) Thumbstick X",AxisProperties=(DeadZone=0.250000,Sensitivity=1.000000,Exponent=1.000000,bInvert=False))
+AxisConfig=(AxisKeyName="MotionController (L) Thumbstick Y",AxisProperties=(DeadZone=0.250000,Sensitivity=1.000000,Exponent=1.000000,bInvert=False))
```

## Oculus-Specific Settings

### Oculus SDK Configuration
```ini
[/Script/OculusHMD.OculusHMDSettings]
bSupportsDash=1
bCompositesDepth=1
bHQDistortion=1
bChromaAbCorrectionEnabled=1
bUpdateOnRT=1
bIsUsingDirectMultiview=1
bIsUsingMobileMultiView=0
bLowPersistenceMode=1
bUpdateOnAnyThread=1
bDoNotMonitorOnTaskBar=1
bDisplayAllMonitors=0
bShowAllMonitorsInDebug=0
bShowAllMonitorsInDebugGame=0
```

### Oculus Performance Settings
```ini
[/Script/OculusHMD.OculusHMDSettings]
PixelDensityMin=0.8
PixelDensityMax=1.4
PixelDensityTarget=1.0
bDynamicResolutionEnabled=1
bDynamicResolutionMax=1.4
bDynamicResolutionMin=0.8
bDynamicResolutionTarget=90
```

## Steam VR-Specific Settings

### Steam VR Configuration
```ini
[/Script/SteamVR.SteamVRSettings]
bEnableSteamVR=1
bEnableSteamVRHome=1
bEnableSteamVRDashboard=1
bEnableSteamVRInput=1
bEnableSteamVRAchievements=1
bEnableSteamVRLeaderboards=1
bEnableSteamVRFriends=1
bEnableSteamVRCloudSaves=1
```

### Steam VR Performance Settings
```ini
[/Script/SteamVR.SteamVRSettings]
bEnableMotionSmoothing=1
bEnableReprojection=1
bEnableAsyncReprojection=1
bEnableInterleavedReprojection=1
bEnableAlwaysOnReprojection=0
bEnableForceInterleavedReprojectionOn=0
bEnableForceInterleavedReprojectionOff=0
```

## VR Comfort Settings

### Movement Settings
```ini
[/Script/Aincrad.VRSettings]
bEnableTeleportation=1
bEnableSmoothLocomotion=1
bEnableRoomScale=1
bEnableSeatedPlay=1
bEnableSnapTurning=1
bEnableSmoothTurning=1
bEnableVignette=1
bEnableHeightAdjustment=1
bEnableIPDCalibration=1
```

### Comfort Features
```ini
[/Script/Aincrad.VRSettings]
VignetteStrength=0.5
SnapTurnAngle=45.0
SmoothTurnSpeed=60.0
HeightAdjustmentRange=0.2
IPDAdjustmentRange=0.1
```

## Build Configuration

### Development Build
```ini
[/Script/UnrealEd.ProjectPackagingSettings]
BuildConfiguration=PPBC_Development
BuildTarget=Win64
StagingDirectory=(Path="C:/Builds/Development")
FullRebuild=False
ForDistribution=False
IncludeDebugFiles=True
```

### Shipping Build
```ini
[/Script/UnrealEd.ProjectPackagingSettings]
BuildConfiguration=PPBC_Shipping
BuildTarget=Win64
StagingDirectory=(Path="C:/Builds/Shipping")
FullRebuild=True
ForDistribution=True
IncludeDebugFiles=False
```

## Testing Configuration

### Performance Testing
```ini
[/Script/Engine.Console]
stat fps
stat unit
stat game
stat physics
stat anim
stat scene
stat memory
stat streaming
```

### VR Testing
```ini
[/Script/Engine.Console]
vr.bShowDebugInfo=1
vr.bShowTracking=1
vr.bShowControllers=1
vr.bShowHMD=1
vr.bShowMotionController=1
vr.bShowMotionControllerDebug=1
``` 