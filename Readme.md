# Voxel Renderer

Repository contains implementation of skeleton animation of voxel models.

## Building instruction

If you want to use application only with already converted models, build Voxel Renderer application.
You need conversion tools only if you want have your own models

### Building Voxel Renderer

Repository contains Visual Studio 2015 projects.

To build main application you need to install [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812):\.

Installation should define **DXSDK_DIR** environment variable, which points to SDK directory.

Note that DirectX SDK is deprecated and all libraries were moved to Windows SDK.
Maybe I will migrate code to new SDK in future versions.

#### Included dependencies

All other dependencies are included in repository:

- Autodesk [FBX SDK](http://usa.autodesk.com/adsk/servlet/pc/item?siteID=123112&id=10775847) for loading shell meshes
- Sleeping Wombat Libraries (included as git subtree). Libraries are used to display native window, handle user input and render.
  - [swCommonLib](https://github.com/nieznanysprawiciel/swCommonLib)
  - [swGraphicAPI](https://github.com/nieznanysprawiciel/swGraphicAPI)
  - [swInputLibrary](https://github.com/nieznanysprawiciel/swInputLibrary)
  - [swGUI](https://github.com/nieznanysprawiciel/swGUI)
- [RTTR](https://github.com/rttrorg/rttr) C++ Reflection library (Sleeping Wombat library depends on it)

### Building Conversion Tools

#### Dependencies:

- [Soil2](https://bitbucket.org/SpartanJ/soil2)

TODO:

## Application usage

TODO:

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