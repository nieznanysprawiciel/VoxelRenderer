import subprocess
import os
import sys



def GetConvertersPath():
    scriptPath = sys.argv[ 0 ]
    return os.path.dirname( scriptPath )


def CallTriConverter( modelFilePath ):
    
    convertersDir = GetConvertersPath()
    triConverterPath = os.path.join( convertersDir, "tri_convert.exe" )
    
    if not os.path.isfile( triConverterPath ):
        print "tri_converter.exe doesn't exist under path [" + triConverterPath + "]"
        return False
    
    print "Calling " + triConverterPath + " with arguments: " + "-f" + modelFilePath
    
    result = subprocess.call( [ triConverterPath, "-f", modelFilePath ] )
    
    if result > 0:
        return False
    return True
    
def ComputeTriFile( inputModelFile ):
    
    modelFileName, modelFileExtension = os.path.splitext( inputModelFile )
    return modelFileName + ".tri"
    
    
def CallSVOBuilder( triFilePath, gridSize ):
    
    convertersDir = GetConvertersPath()
    svoBuilderPath = os.path.join( convertersDir, "svo_builder.exe" )
    
    if not os.path.isfile( svoBuilderPath ):
        print "svo_builder.exe doesn't exist under path [" + svoBuilderPath + "]"
        return False
        
    arguments = [ svoBuilderPath, "-f", triFilePath ]
    
    arguments.extend( [ "-s", str( gridSize ) ] )
    arguments.extend( [ "-c", "model" ] )
    arguments.extend( [ "-v" ] )
        
    print "Calling " + svoBuilderPath + " with arguments: " + str( arguments )
       
    result = subprocess.call( arguments )

    if result > 0:
        return False
    return True
    
    
def CallVoxelConverter( octreeFilePath, outputPath, texturePath, filter ):

    convertersDir = GetConvertersPath()
    voxelConverterPath = os.path.join( convertersDir, "VoxelConverter.exe" )
    
    if not os.path.isfile( voxelConverterPath ):
        print "svo_builder.exe doesn't exist under path [" + voxelConverterPath + "]"
        return False
    
    arguments = [ voxelConverterPath, "-i", octreeFilePath ]
    
    arguments.extend( [ "-o", outputPath ] )
    
    if texturePath is not None:
        arguments.extend( [ "-t", texturePath ] )
        
        if filter is not None:
            arguments.extend( [ "-f", filter ] )
    
    print "Calling " + voxelConverterPath + " with arguments: " + str( arguments )
    
    result = subprocess.call( arguments )

    if result > 0:
        return False
    return True
    
    
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
    
    
def CallFbx2ObjConverter( inputFBX, outputObj ):
    
    convertersDir = GetConvertersPath()
    Fbx2ObjConverterPath = os.path.join( convertersDir, "FbxToObjConverter.exe" )
    
    if not os.path.isfile( Fbx2ObjConverterPath ):
        print "FbxToObjConverter.exe doesn't exist under path [" + Fbx2ObjConverterPath + "]"
        return False
    
    arguments = [ Fbx2ObjConverterPath ]
    
    arguments.extend( [ "-i", inputFBX ] )
    arguments.extend( [ "-o", outputObj ] )
    
    print "Calling " + Fbx2ObjConverterPath + " with arguments: " + str( arguments )
    
    result = subprocess.call( arguments )

    if result > 0:
        return False
    return True
    
    
def MakeConvertsion():
    
    if len( sys.argv ) < 2:
        print "We need at least 1 argument: input file"
        return
    
    modelToConvert = sys.argv[ 1 ]
    outputPath = ComputeDefaultOuputPath( modelToConvert )
    texturePath = None
    filterType = "Bilinear"
    gridSize = 2048
    clean = True
    
    if len( sys.argv ) > 2:
        outputPath = sys.argv[ 2 ]
    
    if len( sys.argv ) > 3:
        texturePath = sys.argv[ 3 ]
        
    modelFileWithoutExt, modelExt = os.path.splitext( modelToConvert )
        
    # If file is in FBX format, convert it to .obj
    if modelExt.lower() == ".fbx":
        
        outputFile = modelFileWithoutExt + ".obj"
        CallFbx2ObjConverter( modelToConvert, outputFile )
        
        modelToConvert = outputFile
        
    
    CallTriConverter( modelToConvert )
    CallSVOBuilder( ComputeTriFile( modelToConvert ), gridSize )
    CallVoxelConverter( ComputeOctreeFile( modelToConvert, gridSize ), outputPath, texturePath, filterType )
    
    if clean:
        CleanTemporaries( ComputeTriFile( modelToConvert ), ComputeOctreeFile( modelToConvert, gridSize ) )
    
    
MakeConvertsion()
    
    
    


