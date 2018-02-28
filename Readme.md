# Voxel Renderer

Repository contains implementation of skeleton animation of voxel models.

## Building instruction

If you want to use application only with already converted models, build Voxel Renderer application.
You need conversion tools only if you want have your own models

### Building Voxel Renderer

Repository contains Visual Studio 2015 projects.

To build main application you need to install [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812).

Installation should define **DXSDK_DIR** environment variable, which points to SDK directory.

Note that DirectX SDK is deprecated and all libraries were moved to Windows SDK.
Maybe I will migrate code to new SDK in future versions.

#### Included dependencies

All other dependencies are included in repository:

- Autodesk [FBX SDK](http://usa.autodesk.com/adsk/servlet/pc/item?siteID=123112&id=10775847) for loading shell meshes.
- Sleeping Wombat Libraries (included as git subtree). Libraries are used to display native window, handle user input and render.
  - [swCommonLib](https://github.com/nieznanysprawiciel/swCommonLib)
  - [swGraphicAPI](https://github.com/nieznanysprawiciel/swGraphicAPI)
  - [swInputLibrary](https://github.com/nieznanysprawiciel/swInputLibrary)
  - [swGUI](https://github.com/nieznanysprawiciel/swGUI)
- [RTTR](https://github.com/rttrorg/rttr) C++ Reflection library (Sleeping Wombat library depends on it)

### Building Conversion Tools

To build conversion tools, you must download and compile some libraries:

- [liboctree](https://github.com/Forceflow/liboctree)
- My fork of [ooc_svo_builder](https://github.com/nieznanysprawiciel/ooc_svo_builder) library. Original version [here](https://github.com/Forceflow/ooc_svo_builder).
- My fork of [trimesh2](https://github.com/nieznanysprawiciel/trimesh2) library. Original version [here](http://gfx.cs.princeton.edu/proj/trimesh2/).

All libraries should be cloned to folder **/External**.

#### Included Dependencies:

- [Soil2](https://bitbucket.org/SpartanJ/soil2) for loading textures.
- [Templatized C++ Command Line Parser Library](http://tclap.sourceforge.net/)
- Autodesk [FBX SDK](http://usa.autodesk.com/adsk/servlet/pc/item?siteID=123112&id=10775847) for loading shell meshes.
- [swCommonLib](https://github.com/nieznanysprawiciel/swCommonLib)
- [RTTR](https://github.com/rttrorg/rttr) C++ Reflection library (Sleeping Wombat library depends on it)

## Application usage (VoxelRenderer)

Application can be configured using **WorkingDir/StartConfig.config** file.
For details, check example included in this repository.

### Controlling camera

To move camera use standard WASD keys.
To turn camera, move mouse with right button pressed.


Moreover you can use these keys to control camera movement:

- Q - step left
- E - step right
- R - step up
- F - step down

### Raycasters

There're multiple raycasters implemented which can be configured from StartConfig.config file, for example:

```
	<Raycaster Type="AnimationRaycasterGPU" />
```


Most of raycasters are for debugging purposes only. To test application I propose to start with:
- **AnimationRaycasterGPU** implements animation algorithms.
- **GPU Raycaster** can display static SVO models.


#### GPU Raycaster

GPU Raycaster can display static SVOs.

You can choose one of following shaders:

- Key 1 - RaycasterGPU/SimpleColor.hlsl - Displays voxels with colours defined in SVO.
- Key 2 - RaycasterGPU/Normals.hlsl - Displays SVO normal vectors.
- Key 3 - RaycasterGPU/PhongColor.hlsl - Displays SVO with Phong shading. Voxels colours are used as diffuse coefficients.

You can configure light parameters in config:

```
	<Lights>
		<DirectionalLight Intensity="1.0" >
			<Color X="1.00000" Y="1.00000" Z="1.000"/>
			<Direction X="0.0" Y="0.00000" Z="1.0000"/>			
		</DirectionalLight>
	</Lights>
```

#### AnimationRaycasterGPU


## Offline Tools

TODO:

### Conversion script

TODO:

### FbxToObjConverter

TODO:

### VoxelConverter

TODO:

## Copyright

This software contains Autodesk® FBX® code developed by Autodesk, Inc. Copyright 2014 Autodesk, Inc.
All rights, reserved. Such code is provided “as is” and Autodesk, Inc. disclaims any and all warranties,
whether express or implied, including without limitation the implied warranties of merchantability,
fitness for a particular purpose or non-infringement of third party rights. In no event shall Autodesk,
Inc. be liable for any direct, indirect, incidental, special, exemplary, or consequential damages
(including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits;
or business interruption) however caused and on any theory of liability, whether in contract, strict
liability, or tort (including negligence or otherwise) arising in any way out of such code. 