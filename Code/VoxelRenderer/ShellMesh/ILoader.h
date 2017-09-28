#pragma once
/**@file ILoader.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Zawiera deklaracjê interfejsu dla loaderów plików z modelami.
*/

#include "swCommonLib/Common/Nullable.h"
#include "swCommonLib/System/Path.h"
#include "swGraphicAPI/Resources/MeshResources.h"
#include "EngineCore/ModelsManager/AssetsManager.h"
#include "EngineCore/ModelsManager/Assets/Meshes/MeshAssetInitData.h"


namespace sw
{



/**@defgroup MakingLoaders Pisanie loaderów
@brief Instrukcje jak napisaæ loader do wczytywania w³asnego formatu plików z modelami 3D.
@ingroup ResourcesManagment

@todo Napisaæ dokumentacjê.*/

/**@brief Wartoœci, mo¿liwe do zwrócenia przez klasê Loader
*/
enum LoaderResult
{
	MESH_LOADING_OK,				///<Poprawne wczytywanie mesha
	MESH_LOADING_WRONG,				///<Niepoprawne wczytywanie mesha
	MESH_LOADING_OUT_OF_MEMORY,		///<Brakuje pamiêæ
	MESH_LOADING_WRONG_FILE_FORMAT	///<Niepoprawny format pliku
};

/**
@ingroup MakingLoaders
@brief Interfejs klasy do wczytywania plików z modelami.
Aby stworzyæ klasê do wczytywania w³asnego formatu plików nale¿y odziedziczyæ po tym interfejsie
i zaimplementowaæ metody wirtualne.

Dok³adniejsze informacje o tworzeniu w³asnych loaderów znajduj¹ siê w paragrafie @ref MakingLoaders.
@note Po wywo³aniu konstruktora klasa ma byæ w pe³ni gotowa do dzia³ania. Podobnie po ka¿dym wywo³aniu funkcji ³aduj¹cej
plik, klasa tak¿e ma byæ gotowa do wczytania kolejnego.
*/
class ILoader
{
protected:
	AssetsManager*		models_manager;
public:
	ILoader( AssetsManager* models_manager ) : models_manager( models_manager ) {};
	virtual ~ILoader() {};


	virtual Nullable< MeshInitData >		LoadMesh	( const filesystem::Path& fileName ) { return "Function should be overriden in derived class."; }

	/**@brief Checks if laoder can load file.
	This function should chack fileName's extension and determine if it can load file.*/
	virtual bool							CanLoad		( const filesystem::Path& fileName ) { return false; }
};


}	// sw

