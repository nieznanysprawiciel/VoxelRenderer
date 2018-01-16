import subprocess
import os
import sys
import shutil


## Configuration
cleanTmpFiles = False
recopyConverters = True


def GetConvertersPath():
    scriptPath = sys.argv[ 0 ]
    return os.path.dirname( scriptPath )

def GetOOCBuildDir():
    convertersPath = GetConvertersPath()
    return os.path.join( convertersPath, "../../", "External/ooc_svo_builder/build/" )
    
def GetVoxelConverterBuildDir():
    convertersPath = GetConvertersPath()
    return os.path.join( convertersPath, "../../", "LibDir/VoxelConverter/Release_x64_v140/" )
    
def GetFbx2ObjConverterBuildDir():
    convertersPath = GetConvertersPath()
    return os.path.join( convertersPath, "../../", "LibDir/FbxToObjConverter/Release_x64_v140/" )
    
def GetMatlistFilePath( mainFilePath ):
    return mainFilePath + ".matlist"
    
def CopyExecutableIfNotExist( executableName, buildDir ):
    
    convertersDir = GetConvertersPath()
    converterPath = os.path.join( convertersDir, executableName )

    if not os.path.isfile( converterPath ):
        
        converterBuildFileName = buildDir + executableName
        shutil.copy2( converterBuildFileName, convertersDir )
    
        print executableName + " doesn't exist under path [" + converterPath + "]. Coping from [" + converterBuildFileName + "]."
        #return False
    
def CallTriConverter( modelFilePath ):
    
    convertersDir = GetConvertersPath()
    executableName = "tri_convert.exe"
    triConverterPath = os.path.join( convertersDir, executableName )
    
    CopyExecutableIfNotExist( executableName, GetOOCBuildDir() )
    
    callInfo = "Calling " + triConverterPath + " with arguments: " + "-f" + modelFilePath
    
    result = subprocess.call( [ triConverterPath, "-f", modelFilePath ] )
    
    if result > 0:
        raise NameError( callInfo + " failed" )
    
def ComputeTriFile( inputModelFile ):
    
    modelFileName, modelFileExtension = os.path.splitext( inputModelFile )
    return modelFileName + ".tri"
    
    
def CallSVOBuilder( triFilePath, gridSize ):
    
    convertersDir = GetConvertersPath()
    executableName = "svo_builder.exe"
    svoBuilderPath = os.path.join( convertersDir, executableName )
    
    CopyExecutableIfNotExist( executableName, GetOOCBuildDir() )
        
    arguments = [ svoBuilderPath, "-f", triFilePath ]
    
    arguments.extend( [ "-s", str( gridSize ) ] )
    arguments.extend( [ "-c", "model" ] )
    arguments.extend( [ "-v" ] )
        
    callInfo = "Calling " + svoBuilderPath + " with arguments: " + str( arguments )
       
    result = subprocess.call( arguments )

    if result > 0:
        raise NameError( callInfo + " failed" )
    
    
def CallVoxelConverter( octreeFilePath, outputPath, texturePath, filter ):

    convertersDir = GetConvertersPath()
    executableName = "VoxelConverter.exe"
    voxelConverterPath = os.path.join( convertersDir, executableName )
    
    CopyExecutableIfNotExist( executableName, GetVoxelConverterBuildDir() )

    arguments = [ voxelConverterPath ]
    
    arguments.extend( [ "-i", octreeFilePath ] )
    arguments.extend( [ "-o", outputPath ] )
    
    if texturePath is not None:
        
        ## Determine if parameter is single texture or texture list file.
        if os.path.splitext( texturePath )[ -1 ] == ".matlist":
            arguments.extend( [ "-m", texturePath ] )
        else:
            arguments.extend( [ "-t", texturePath ] )
        
        if filter is not None:
            arguments.extend( [ "-f", filter ] )
    
    callInfo = "Calling " + voxelConverterPath + " with arguments: " + str( arguments )
    
    result = subprocess.call( arguments )

    if result > 0:
        raise NameError( callInfo + " failed" )
    
    
def ComputeOctreeFile( inputModelFile, gridSize ):
    
    modelFileName, modelFileExtension = os.path.splitext( inputModelFile )
    
    # I don't know how to determine this number from svo_builder arguments yet.
    splitCount = "_8"
    
    return modelFileName + str( gridSize ) + splitCount + ".octree"
    
    
def ComputeDefaultOuputPath( inputModelFile ):

    modelFileName, modelFileExtension = os.path.splitext( inputModelFile )
    return modelFileName + ".voxtree"

    
def CleanTemporaries( triFile, octreeFile):
    
    os.remove( triFile )
    os.remove( triFile + "data" )
    
    os.remove( octreeFile )
    os.remove( octreeFile + "data" )
    os.remove( octreeFile + "nodes" )
    
    
def CallFbx2ObjConverter( inputFBX, outputObj, genMatfile ):
    
    convertersDir = GetConvertersPath()
    executableName = "FbxToObjConverter.exe"
    Fbx2ObjConverterPath = os.path.join( convertersDir, executableName )
    
    CopyExecutableIfNotExist( executableName, GetFbx2ObjConverterBuildDir() )
    CopyExecutableIfNotExist( "libfbxsdk.dll", GetFbx2ObjConverterBuildDir() )
    CopyExecutableIfNotExist( "Reflection.dll", GetFbx2ObjConverterBuildDir() )
    
    arguments = [ Fbx2ObjConverterPath ]
    
    arguments.extend( [ "-i", inputFBX ] )
    arguments.extend( [ "-o", outputObj ] )
    
    if genMatfile:
        arguments.extend( [ "--generateMatfile" ] )
    
    callInfo = "Calling " + Fbx2ObjConverterPath + " with arguments: " + str( arguments )
    
    result = subprocess.call( arguments )

    if result > 0:
        raise NameError( callInfo + " failed" )
    
def SafeRemove( filePath ):
    if os.path.exists( filePath ):
        os.remove( filePath )
    
def RemoveConverters():

    convertersDir = GetConvertersPath()
    SafeRemove( os.path.join( convertersDir, "FbxToObjConverter.exe" ) )
    SafeRemove( os.path.join( convertersDir, "libfbxsdk.dll" ) )
    SafeRemove( os.path.join( convertersDir, "Reflection.dll" ) )
    SafeRemove( os.path.join( convertersDir, "VoxelConverter.exe" ) )
    SafeRemove( os.path.join( convertersDir, "tri_convert.exe" ) )
    SafeRemove( os.path.join( convertersDir, "svo_builder.exe" ) )
    
def MakeConvertsion():
    
    if len( sys.argv ) < 2:
        print "We need at least 1 argument: input file"
        return
    
    modelToConvert = sys.argv[ 1 ]
    outputPath = ComputeDefaultOuputPath( modelToConvert )
    texturePath = None
    filterType = "Bilinear"
    gridSize = 2048
    
    
    if recopyConverters:
        RemoveConverters()
    
    if len( sys.argv ) > 2:
        outputPath = sys.argv[ 2 ]
    
    if len( sys.argv ) > 3:
        texturePath = sys.argv[ 3 ]
        
    modelFileWithoutExt, modelExt = os.path.splitext( modelToConvert )
        
    # If file is in FBX format, convert it to .obj
    if modelExt.lower() == ".fbx":
        
        generateMatfile = True
        if texturePath is not None:
            generateMatfile = False
        
        outputFile = modelFileWithoutExt + ".obj"
        CallFbx2ObjConverter( modelToConvert, outputFile, generateMatfile )
        
        matlistPath = GetMatlistFilePath( outputFile )
        
        # If .matlist file was generated we should use it unless user specified file explicite.
        if generateMatfile:
            texturePath = matlistPath
        
        modelToConvert = outputFile
        
    
    CallTriConverter( modelToConvert )
    CallSVOBuilder( ComputeTriFile( modelToConvert ), gridSize )
    CallVoxelConverter( ComputeOctreeFile( modelToConvert, gridSize ), outputPath, texturePath, filterType )
    
    if cleanTmpFiles:
        CleanTemporaries( ComputeTriFile( modelToConvert ), ComputeOctreeFile( modelToConvert, gridSize ) )
    
    
MakeConvertsion()
    
    
    


