/**@file SerializationMain.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief XML serializer test.*/

#include <windows.h>
#include <fstream>
#include <numeric>
#undef max
#undef min

#include "swCommonLib/Serialization/Serializer.h"
#include "swCommonLib/Serialization/Deserializer.h"


#ifdef TEST_JSON
	std::string writeFileName = "SerializerTest/serialWrite.json";
	std::string readFileName = "SerializerTest/serialRead.json";
	std::ofstream outputFile( "SerializerTest/testResultJSON.txt" );
#elif TEST_XML
	std::string writeFileName = "SerializerTest/serialWrite.xml";
	std::string readFileName = "SerializerTest/serialRead.xml";
	std::ofstream outputFile( "SerializerTest/testResultXML.txt" );
#endif



int main()
{
	ISerializer serializer( std::make_unique< SerializationContext >() );

	serializer.SetAttribute( "FreeValue", "value" );	// Nie pojawi siê w XMLu.
	serializer.EnterObject( "FirstObject" );
		serializer.SetAttribute( "Map", "LightmapGen1" );
		serializer.SetAttribute( "Path", "/LightmapGen1.map" );
		serializer.SetAttribute( "Load", true );
		serializer.EnterObject( "Data" );
			serializer.SetAttribute( "NumberUnits", 1266643 );
			serializer.SetAttribute( "PositionOffset", 0.4124667623 );
			serializer.SetAttribute( "Visible", true );
			serializer.SetAttribute( "Key", 218588284382834538 );
		serializer.Exit();	// Data

		serializer.EnterArray( "Actors" );
			for( unsigned int i = 0; i < 12; ++i )
			{
				serializer.EnterObject( "Actor" );
					serializer.SetAttribute( "Name", std::string( "TIE Fighter " ) + std::to_string( i ) );
					serializer.EnterObject( "Position" );
						serializer.SetAttribute( "X", 12 );
						serializer.SetAttribute( "Y", 12 );
						serializer.SetAttribute( "Z", 12 );
					serializer.Exit();
				serializer.Exit();
			}
		serializer.Exit();	// Actors



		serializer.EnterArray( "Assets" );
			serializer.SetAttribute( "Asset", "TIE Fighter model" );
			serializer.SetAttribute( "Asset", "Nebulon B" );
			serializer.SetAttribute( "Asset", "Death Star" );
			serializer.SetAttribute( "Asset", "Calamarian Cruiser" );
			serializer.SetAttribute( "Asset", "Star Destroyer" );
		serializer.Exit();	// Assets

		// Nie wiem czy takie coœ ma sens
		serializer.EnterArray( "Passes" );
			serializer.EnterArray( "Lights" );
				serializer.SetAttribute( "light", "directional" );
				serializer.SetAttribute( "light", "point" );
			serializer.Exit();	//	Lights
		serializer.Exit();	// Passes

		serializer.EnterObject( "Data1" );
			serializer.SetAttribute( "Map", "LightmapGen1" );
			serializer.SetAttribute( "Path", "/LightmapGen1.map" );
			serializer.SetAttribute( "Load", true );
		serializer.Exit();	// Data

		serializer.EnterObject( "Data2" );
			serializer.SetAttribute( "Asset1", "TIE Fighter model" );
			serializer.SetAttribute( "Asset2", "Nebulon B" );
			serializer.SetAttribute( "Asset3", "Death Star" );
			serializer.SetAttribute( "Asset4", "Calamarian Cruiser" );
			serializer.SetAttribute( "Asset5", "Star Destroyer" );
		serializer.Exit();	// Data2

		serializer.EnterObject( "Data3" );
			serializer.SetAttribute( "NumberUnits", std::numeric_limits<uint32>::max() );
			serializer.SetAttribute( "UnitsModifier", std::numeric_limits<int32>::min() );
			serializer.SetAttribute( "PositionOffset", 0.412466762375758958574674674764488949497747648 );
			serializer.SetAttribute( "Visible", true );
			serializer.SetAttribute( "Key", std::numeric_limits<int64>::min() );
			serializer.SetAttribute( "KeyHash", std::numeric_limits<uint64>::max() );
		serializer.Exit();	// Data3

		serializer.EnterArray( "ActorEnemies" );
			for( unsigned int i = 0; i < 30; ++i )
			{
				serializer.EnterObject( "ActorEnemy" );
					serializer.SetAttribute( "Name", std::string( "X-wing " ) + std::to_string( i ) );
					serializer.EnterObject( "Position" );
						serializer.SetAttribute( "X", 15 );
						serializer.SetAttribute( "Y", 124 );
						serializer.SetAttribute( "Z", 97 );
					serializer.Exit();	// Position
				serializer.Exit();	// ActorEnemy
			}
		serializer.Exit();	// ActorEnemies

		// Iteracja po zwyk³ym obiekcie (zamiast tablicy)
		serializer.EnterObject( "ButtonContainer" );
			
			serializer.EnterObject( "PushButton" );
				serializer.SetAttribute( "ButtonName", "Exit" );
				serializer.SetAttribute( "Visible", true );
			serializer.Exit();	// PushButton

			serializer.EnterObject( "PushButton" );
				serializer.SetAttribute( "ButtonName", "Start" );
				serializer.SetAttribute( "Visible", true );
			serializer.Exit();	// PushButton

			serializer.EnterObject( "TriStateButton" );
				serializer.SetAttribute( "ButtonName", "GameMode" );
				serializer.SetAttribute( "Visible", false );
			serializer.Exit();	// TriStateButton

			serializer.EnterObject( "SwitchButton" );
				serializer.SetAttribute( "ButtonName", "GameMode2" );
				serializer.SetAttribute( "Visible", true );
			serializer.Exit();	// SwitchButton

			serializer.EnterObject( "SelectField" );
				serializer.SetAttribute( "ButtonName", "TankType" );
				serializer.SetAttribute( "Visible", true );
			serializer.Exit();	// SelectField

		serializer.Exit();	// ButtonContainer

	serializer.Exit();	// FirstObject


	if( !serializer.SaveFile( writeFileName, WritingMode::Readable ) )
#ifdef TEST_JSON
		OutputDebugString( L"Error: Saving \"SerializerTest/serialWrite.json\" failed!\n" );
#elif TEST_XML
		OutputDebugString( L"Error: Saving \"SerializerTest/serialWrite.xml\" failed!\n" );
#endif
	
	IDeserializer deser;


	if( deser.LoadFromFile( readFileName, ParsingMode::ParseInsitu ) )
	{
		outputFile << deser.GetAttribute( "FreeValue", "GetAttribute error" ) << std::endl;
		deser.EnterObject( "FirstObject" );
			outputFile << deser.GetAttribute( "Map", "GetAttribute error" ) << std::endl;
			outputFile << deser.GetAttribute( "Path", "GetAttribute error" ) << std::endl;
			outputFile << deser.GetAttribute( "Load", false ) << std::endl;
			deser.EnterObject( "Data" );
				outputFile << deser.GetAttribute( "NumberUnits", 0 ) << std::endl;
				outputFile << deser.GetAttribute( "PositionOffset", 0.0 ) << std::endl;
				outputFile << deser.GetAttribute( "Visible", false ) << std::endl;
				outputFile << deser.GetAttribute( "Key", 0 ) << std::endl;
			deser.Exit();	// Data
		if( deser.EnterArray( "Actors" ) )
		{
			if( deser.FirstElement() )
			{
				do
				{
					outputFile << deser.GetAttribute( "Name", "Wrong string" ) << std::endl;
					
					deser.EnterObject( "Position" );
						outputFile << "\t" << deser.GetAttribute( "X", 0 ) << std::endl;
						outputFile << "\t" << deser.GetAttribute( "Y", 0 ) << std::endl;
						outputFile << "\t" << deser.GetAttribute( "Z", 0 ) << std::endl;
					deser.Exit();

				} while( deser.NextElement() );
				//deser.Exit();		// Mo¿e to jest do wyrzucenia. Serializator móg³by sam wycyhodziæ.
			}

			deser.Exit();	// Actors
		}

		if( deser.EnterObject( "Data1" ) )
		{
			outputFile << deser.GetAttribute( "Map", "GetAttribute error" ) << std::endl;
			outputFile << deser.GetAttribute( "Path", "GetAttribute error" ) << std::endl;
			outputFile << deser.GetAttribute( "Load", false ) << std::endl;

			deser.Exit();	// Data1
		}

		if( deser.EnterObject( "Data2" ) )
		{
			outputFile << deser.GetAttribute( "Asset1", "GetAttribute error" ) << std::endl;
			outputFile << deser.GetAttribute( "Asset2", "GetAttribute error" ) << std::endl;
			outputFile << deser.GetAttribute( "Asset3", "GetAttribute error" ) << std::endl;
			outputFile << deser.GetAttribute( "Asset4", "GetAttribute error" ) << std::endl;
			outputFile << deser.GetAttribute( "Asset5", "GetAttribute error" ) << std::endl;

			deser.Exit();	// Data2
		}

		if( deser.EnterObject( "Data3" ) )
		{
			outputFile << deser.GetAttribute( "NumberUnits", (uint32)0 ) << std::endl;
			outputFile << deser.GetAttribute( "UnitsModifier", (int32)0 ) << std::endl;
			outputFile << deser.GetAttribute( "PositionOffset", 0.0 ) << std::endl;
			outputFile << deser.GetAttribute( "Visible", false ) << std::endl;
			outputFile << deser.GetAttribute( "Key", (int64)0 ) << std::endl;
			outputFile << deser.GetAttribute( "KeyHash", (uint64)0 ) << std::endl;
			
			deser.Exit();	// Data3
		}

		if( deser.EnterArray( "ActorEnemies" ) )
		{
			if( deser.LastElement() )
			{
				do
				{
					outputFile << deser.GetAttribute( "Name", "Wrong string" ) << std::endl;
					
					deser.EnterObject( "Position" );
						outputFile << "\t" << deser.GetAttribute( "X", 0 ) << std::endl;
						outputFile << "\t" << deser.GetAttribute( "Y", 0 ) << std::endl;
						outputFile << "\t" << deser.GetAttribute( "Z", 0 ) << std::endl;
					deser.Exit();

				} while( deser.PrevElement() );
				//deser.Exit();		// Mo¿e to jest do wyrzucenia. Serializator móg³by sam wycyhodziæ.
			}

			deser.Exit();	// ActorEnemies
		}

		if( deser.EnterObject( "ButtonContainer" ) )
		{
			if( deser.FirstElement() )
			{
				do
				{
					outputFile << "Name: " << deser.GetName() << std::endl;
					outputFile << "\tButtonName: " << deser.GetAttribute( "ButtonName", "Wrong string" ) << std::endl;
					outputFile << "\tVisible: " << deser.GetAttribute( "Visible", false ) << std::endl;

				} while( deser.NextElement() );
			}
			deser.Exit();	// ButtonContainer
		}

		deser.Exit();	// FirstObject

	}
	else
	{
		std::string errorMessage = deser.GetError();
		OutputDebugString( std::wstring( errorMessage.begin(), errorMessage.end() ).c_str() );
		OutputDebugString( L"\n" );
	}

	return 0;
}