#include "swCommonLib/External/Catch/catch.hpp"

#include "swCommonLib/ParameterAnimation/Timelines/TimelineBase.h"
#include "swCommonLib/ParameterAnimation/Timelines/RelativeTimeline.h"

#include "FakeTimeline.h"



const double epsilon = 0.000001;


TEST_CASE( "Timelines - RelativeTimeline - WrapModes", "[RelativeTimeline - WrapModes]" )
{
	FakeTimelinePtr global = MakePtr< FakeTimeline >();

	RelativeTimelinePtr timeline = MakePtr< RelativeTimeline >( global );

	SECTION( "WrapModes: Clamp, Clamp" )
	{
		timeline->SetDuration( 3.0 );
		timeline->SetPreWrap( WrapMode::Clamp );
		timeline->SetPostWrap( WrapMode::Clamp );

		REQUIRE( timeline->GetDuration() == 3.0 );
		REQUIRE( timeline->GetPostWrap() == WrapMode::Clamp );
		REQUIRE( timeline->GetPreWrap() == WrapMode::Clamp );

		TimeType globalStartT = 1.0;
		TimeType timeStep = 0.1;

		global->SetCurrentTIme( globalStartT );
		timeline->Start();

		// Check inside timeline duration.
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Time evaluation: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - time ) < epsilon );
		}

		// Check post clamp.
		globalStartT += 3.0;
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Post clamp Time: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - 3.0 ) < epsilon );
		}

		// Check pre clamp
		globalStartT = -2.0;
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Pre clamp Time: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - 0.0 ) < epsilon );
		}

	}

// ================================ //
//

	SECTION( "WrapModes: Clamp, Mirror" )
	{
		WrapMode preMode = WrapMode::Clamp;
		WrapMode postMode = WrapMode::Mirror;
		TimeType duration = 3.0;

		timeline->SetDuration( duration );
		timeline->SetPreWrap( preMode );
		timeline->SetPostWrap( postMode );

		REQUIRE( timeline->GetDuration() == duration );
		REQUIRE( timeline->GetPostWrap() == postMode );
		REQUIRE( timeline->GetPreWrap() == preMode );

		TimeType globalStartT = 1.0;
		TimeType timeStep = 0.1;

		global->SetCurrentTIme( globalStartT );
		timeline->Start();

		// Check inside timeline duration.
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Time evaluation: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - time ) < epsilon );
		}

		// Check post clamp.
		globalStartT += 3.0;
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Post clamp Time: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - ( 3.0 - time ) ) < epsilon );
		}

		// Check post clamp, after 2 * duration. (Pre clamp should be used)
		globalStartT += 3.0;
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Pre clamp Time: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - 0.0 ) < epsilon );
		}

		// Check inside timeline duration after first period.
		globalStartT += 3.0;
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Time evaluation: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - 0.0 ) < epsilon );
		}

		// Check pre clamp
		globalStartT = -2.0;
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Pre clamp Time: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - 0.0 ) < epsilon );
		}

	}


// ================================ //
//

	SECTION( "WrapModes: Clamp, Repeat" )
	{
		WrapMode preMode = WrapMode::Clamp;
		WrapMode postMode = WrapMode::Repeat;
		TimeType duration = 3.0;

		timeline->SetDuration( duration );
		timeline->SetPreWrap( preMode );
		timeline->SetPostWrap( postMode );

		REQUIRE( timeline->GetDuration() == duration );
		REQUIRE( timeline->GetPostWrap() == postMode );
		REQUIRE( timeline->GetPreWrap() == preMode );

		TimeType globalStartT = 1.0;
		TimeType timeStep = 0.1;

		global->SetCurrentTIme( globalStartT );
		timeline->Start();

		// Check inside timeline duration.
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Time evaluation: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - time ) < epsilon );
		}

		// Check post clamp.
		for( int reapeats = 0; reapeats < 10; reapeats++ )
		{
			globalStartT += 3.0;
			for( int i = 1; i < 30; ++i )
			{
				TimeType time = i * timeStep;
				global->SetCurrentTIme( globalStartT + time );
				timeline->Update();

				INFO( "Post clamp Time: " << globalStartT + time );
				CHECK( abs( timeline->GetTime() - time ) < epsilon );
			}
		}


		// Check pre clamp
		globalStartT = -2.0;
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Pre clamp Time: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - 0.0 ) < epsilon );
		}
	}


// ================================ //
//

	SECTION( "WrapModes: Mirror, Mirror" )
	{
		WrapMode preMode = WrapMode::Mirror;
		WrapMode postMode = WrapMode::Mirror;
		TimeType duration = 3.0;

		timeline->SetDuration( duration );
		timeline->SetPreWrap( preMode );
		timeline->SetPostWrap( postMode );

		REQUIRE( timeline->GetDuration() == duration );
		REQUIRE( timeline->GetPostWrap() == postMode );
		REQUIRE( timeline->GetPreWrap() == preMode );

		TimeType globalStartT = 1.0;
		TimeType timeStep = 0.1;

		global->SetCurrentTIme( globalStartT );
		timeline->Start();

		// Check inside timeline duration.
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Time evaluation: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - time ) < epsilon );
		}

		// Check post clamp.
		globalStartT += 3.0;
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Post clamp Time: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - ( 3.0 - time ) ) < epsilon );
		}

		// Check post clamp, after 2 * duration. (Pre clamp should be used)
		globalStartT += 3.0;
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Pre clamp Time: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - time ) < epsilon );
		}

		// Check inside timeline duration after first period.
		globalStartT += 3.0;
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Time evaluation: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - ( 3.0 - time ) ) < epsilon );
		}

		// Check pre clamp
		globalStartT = -2.0;
		for( int i = 0; i < 30; ++i )
		{
			TimeType time = i * timeStep;
			global->SetCurrentTIme( globalStartT + time );
			timeline->Update();

			INFO( "Pre clamp Time: " << globalStartT + time );
			CHECK( abs( timeline->GetTime() - ( 3.0 - time ) ) < epsilon );
		}

	}


}


TEST_CASE( "Timelines - RelativeTimeline - StartPauseStop", "[RelativeTimeline - StartPauseStop]" )
{
	FakeTimelinePtr global = MakePtr< FakeTimeline >();
	RelativeTimelinePtr timeline = MakePtr< RelativeTimeline >( global );

	SECTION( "Start - Pause - Start" )
	{
		TimeType globalStartT = 0.0;

		global->SetCurrentTIme( globalStartT );
		timeline->Start();

		global->SetCurrentTIme( 1.0 );
		timeline->Update();

		timeline->Pause();

		global->SetCurrentTIme( 2.0 );
		timeline->Update();

		REQUIRE( abs( timeline->GetTime() - 1.0 ) < epsilon );

		timeline->Start();

		global->SetCurrentTIme( 3.0 );
		timeline->Update();

		REQUIRE( abs( timeline->GetTime() - 2.0 ) < epsilon );
	}

	SECTION( "Start - Stop - Start" )
	{
		TimeType globalStartT = 0.0;

		global->SetCurrentTIme( globalStartT );
		timeline->Start();

		global->SetCurrentTIme( 1.0 );
		timeline->Update();

		timeline->Stop();

		global->SetCurrentTIme( 2.0 );
		timeline->Update();

		REQUIRE( abs( timeline->GetTime() - 0.0 ) < epsilon );

		timeline->Start();

		global->SetCurrentTIme( 3.0 );
		timeline->Update();

		REQUIRE( abs( timeline->GetTime() - 1.0 ) < epsilon );
	}
}

