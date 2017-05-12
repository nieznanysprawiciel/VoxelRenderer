#pragma once
/**
@file InputDeviceEvent.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"
#include "KeyState.h"




namespace sw {
namespace input
{


typedef uint16 Timestamp;


struct Keyboard
{

	/**@brief Physical keys numbers.
	Numbers are the same as in DirectInput.

	@ingroup Input*/
	enum PhysicalKeys : uint8
	{
		KEY_NONE			  = 0x00
		, KEY_ESCAPE          = 0x01
		, KEY_1               = 0x02
		, KEY_2               = 0x03
		, KEY_3               = 0x04
		, KEY_4               = 0x05
		, KEY_5               = 0x06
		, KEY_6               = 0x07
		, KEY_7               = 0x08
		, KEY_8               = 0x09
		, KEY_9               = 0x0A
		, KEY_0               = 0x0B
		, KEY_MINUS           = 0x0C    /* - on main keyboard */
		, KEY_EQUALS          = 0x0D
		, KEY_BACK            = 0x0E    /* backspace */
		, KEY_TAB             = 0x0F
		, KEY_Q               = 0x10
		, KEY_W               = 0x11
		, KEY_E               = 0x12
		, KEY_R               = 0x13
		, KEY_T               = 0x14
		, KEY_Y               = 0x15
		, KEY_U               = 0x16
		, KEY_I               = 0x17
		, KEY_O               = 0x18
		, KEY_P               = 0x19
		, KEY_LBRACKET        = 0x1A
		, KEY_RBRACKET        = 0x1B
		, KEY_RETURN          = 0x1C    /* Enter on main keyboard */
		, KEY_LCONTROL        = 0x1D
		, KEY_A               = 0x1E
		, KEY_S               = 0x1F
		, KEY_D               = 0x20
		, KEY_F               = 0x21
		, KEY_G               = 0x22
		, KEY_H               = 0x23
		, KEY_J               = 0x24
		, KEY_K               = 0x25
		, KEY_L               = 0x26
		, KEY_SEMICOLON       = 0x27
		, KEY_APOSTROPHE      = 0x28
		, KEY_GRAVE           = 0x29    /* accent grave */
		, KEY_LSHIFT          = 0x2A
		, KEY_BACKSLASH       = 0x2B
		, KEY_Z               = 0x2C
		, KEY_X               = 0x2D
		, KEY_C               = 0x2E
		, KEY_V               = 0x2F
		, KEY_B               = 0x30
		, KEY_N               = 0x31
		, KEY_M               = 0x32
		, KEY_COMMA           = 0x33
		, KEY_PERIOD          = 0x34    /* . on main keyboard */
		, KEY_SLASH           = 0x35    /* / on main keyboard */
		, KEY_RSHIFT          = 0x36
		, KEY_MULTIPLY        = 0x37    /* * on numeric keypad */
		, KEY_LMENU           = 0x38    /* left Alt */
		, KEY_SPACE           = 0x39
		, KEY_CAPITAL         = 0x3A
		, KEY_F1              = 0x3B
		, KEY_F2              = 0x3C
		, KEY_F3              = 0x3D
		, KEY_F4              = 0x3E
		, KEY_F5              = 0x3F
		, KEY_F6              = 0x40
		, KEY_F7              = 0x41
		, KEY_F8              = 0x42
		, KEY_F9              = 0x43
		, KEY_F10             = 0x44
		, KEY_NUMLOCK         = 0x45
		, KEY_SCROLL          = 0x46    /* Scroll Lock */
		, KEY_NUMPAD7         = 0x47
		, KEY_NUMPAD8         = 0x48
		, KEY_NUMPAD9         = 0x49
		, KEY_SUBTRACT        = 0x4A    /* - on numeric keypad */
		, KEY_NUMPAD4         = 0x4B
		, KEY_NUMPAD5         = 0x4C
		, KEY_NUMPAD6         = 0x4D
		, KEY_ADD             = 0x4E    /* + on numeric keypad */
		, KEY_NUMPAD1         = 0x4F
		, KEY_NUMPAD2         = 0x50
		, KEY_NUMPAD3         = 0x51
		, KEY_NUMPAD0         = 0x52
		, KEY_DECIMAL         = 0x53    /* . on numeric keypad */
		, KEY_OEM_102         = 0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
		, KEY_F11             = 0x57
		, KEY_F12             = 0x58
		, KEY_F13             = 0x64    /*                     (NEC PC98) */
		, KEY_F14             = 0x65    /*                     (NEC PC98) */
		, KEY_F15             = 0x66    /*                     (NEC PC98) */
		, KEY_KANA            = 0x70    /* (Japanese keyboard)            */
		, KEY_ABNT_C1         = 0x73    /* /? on Brazilian keyboard */
		, KEY_CONVERT         = 0x79    /* (Japanese keyboard)            */
		, KEY_NOCONVERT       = 0x7B    /* (Japanese keyboard)            */
		, KEY_YEN             = 0x7D    /* (Japanese keyboard)            */
		, KEY_ABNT_C2         = 0x7E    /* Numpad . on Brazilian keyboard */
		, KEY_NUMPADEQUALS    = 0x8D    /* = on numeric keypad (NEC PC98) */
		, KEY_PREVTRACK       = 0x90    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
		, KEY_AT              = 0x91    /*                     (NEC PC98) */
		, KEY_COLON           = 0x92    /*                     (NEC PC98) */
		, KEY_UNDERLINE       = 0x93    /*                     (NEC PC98) */
		, KEY_KANJI           = 0x94    /* (Japanese keyboard)            */
		, KEY_STOP            = 0x95    /*                     (NEC PC98) */
		, KEY_AX              = 0x96    /*                     (Japan AX) */
		, KEY_UNLABELED       = 0x97    /*                        (J3100) */
		, KEY_NEXTTRACK       = 0x99    /* Next Track */
		, KEY_NUMPADENTER     = 0x9C    /* Enter on numeric keypad */
		, KEY_RCONTROL        = 0x9D
		, KEY_MUTE            = 0xA0    /* Mute */
		, KEY_CALCULATOR      = 0xA1    /* Calculator */
		, KEY_PLAYPAUSE       = 0xA2    /* Play / Pause */
		, KEY_MEDIASTOP       = 0xA4    /* Media Stop */
		, KEY_VOLUMEDOWN      = 0xAE    /* Volume - */
		, KEY_VOLUMEUP        = 0xB0    /* Volume + */
		, KEY_WEBHOME         = 0xB2    /* Web home */
		, KEY_NUMPADCOMMA     = 0xB3    /* , on numeric keypad (NEC PC98) */
		, KEY_DIVIDE          = 0xB5    /* / on numeric keypad */
		, KEY_SYSRQ           = 0xB7
		, KEY_RMENU           = 0xB8    /* right Alt */
		, KEY_PAUSE           = 0xC5    /* Pause */
		, KEY_HOME            = 0xC7    /* Home on arrow keypad */
		, KEY_UP              = 0xC8    /* UpArrow on arrow keypad */
		, KEY_PRIOR           = 0xC9    /* PgUp on arrow keypad */
		, KEY_LEFT            = 0xCB    /* LeftArrow on arrow keypad */
		, KEY_RIGHT           = 0xCD    /* RightArrow on arrow keypad */
		, KEY_END             = 0xCF    /* End on arrow keypad */
		, KEY_DOWN            = 0xD0    /* DownArrow on arrow keypad */
		, KEY_NEXT            = 0xD1    /* PgDn on arrow keypad */
		, KEY_INSERT          = 0xD2    /* Insert on arrow keypad */
		, KEY_DELETE          = 0xD3    /* Delete on arrow keypad */
		, KEY_LWIN            = 0xDB    /* Left Windows key */
		, KEY_RWIN            = 0xDC    /* Right Windows key */
		, KEY_APPS            = 0xDD    /* AppMenu key */
		, KEY_POWER           = 0xDE    /* System Power */
		, KEY_SLEEP           = 0xDF    /* System Sleep */
		, KEY_WAKE            = 0xE3    /* System Wake */
		, KEY_WEBSEARCH       = 0xE5    /* Web Search */
		, KEY_WEBFAVORITES    = 0xE6    /* Web Favorites */
		, KEY_WEBREFRESH      = 0xE7    /* Web Refresh */
		, KEY_WEBSTOP         = 0xE8    /* Web Stop */
		, KEY_WEBFORWARD      = 0xE9    /* Web Forward */
		, KEY_WEBBACK         = 0xEA    /* Web Back */
		, KEY_MYCOMPUTER      = 0xEB    /* My Computer */
		, KEY_MAIL            = 0xEC    /* Mail */
		, KEY_MEDIASELECT     = 0xED    /* Media Select */

		/*
			*  Alternate names for keys, to facilitate transition from DOS.
			*/
		, KEY_BACKSPACE       = KEY_BACK            /* backspace */
		, KEY_NUMPADSTAR      = KEY_MULTIPLY        /* * on numeric keypad */
		, KEY_LALT            = KEY_LMENU           /* left Alt */
		, KEY_CAPSLOCK        = KEY_CAPITAL         /* CapsLock */
		, KEY_NUMPADMINUS     = KEY_SUBTRACT        /* - on numeric keypad */
		, KEY_NUMPADPLUS      = KEY_ADD             /* + on numeric keypad */
		, KEY_NUMPADPERIOD    = KEY_DECIMAL         /* . on numeric keypad */
		, KEY_NUMPADSLASH     = KEY_DIVIDE          /* / on numeric keypad */
		, KEY_RALT            = KEY_RMENU           /* right Alt */
		, KEY_UPARROW         = KEY_UP              /* UpArrow on arrow keypad */
		, KEY_PGUP            = KEY_PRIOR           /* PgUp on arrow keypad */
		, KEY_LEFTARROW       = KEY_LEFT            /* LeftArrow on arrow keypad */
		, KEY_RIGHTARROW      = KEY_RIGHT           /* RightArrow on arrow keypad */
		, KEY_DOWNARROW       = KEY_DOWN            /* DownArrow on arrow keypad */
		, KEY_PGDN            = KEY_NEXT            /* PgDn on arrow keypad */

		, KEY_PAGEUP		  = KEY_PGUP
		, KEY_PAGEDOWN		  = KEY_PGDN
		, KEY_PRINTSCREEN	  = KEY_SYSRQ
	};

};


struct Mouse
{

/**@brief Physical buttons.

Enums form BUTTON0 to BUTTON2 are buttons right, left and modle.
There'ra aliases for them: LEFT_BUTTON, RIGHT_BUTTON, MIDDLE_BUTTON, XBUTTON1, XBUTTON2.

Next numbers means rest of buttons which can exist on mouse device.

@ingroup Input*/
	enum PhysicalButtons : int8
	{
		BUTTON0 = 0,
		BUTTON1 = 1,
		BUTTON2 = 2,
		BUTTON3 = 3,
		BUTTON4 = 4,
		BUTTON5 = 5,
		BUTTON6 = 6,
		BUTTON7 = 7,

		LEFT_BUTTON = BUTTON0,
		RIGHT_BUTTON = BUTTON1,
		MIDDLE_BUTTON = BUTTON2,
		XBUTTON1 = BUTTON3,
		XBUTTON2 = BUTTON4,

		NONE = 8,
	};

	/**@brief Physical axes of mouse.
	@ingroup Input*/
	enum PhysicalAxes : int8
	{
		X_AXIS = 0,
		Y_AXIS = 1,
		Z_AXIS = 2,
		W_AXIS = 3,
		WHEEL = Z_AXIS
	};

};


/**@brief Input device event types.
@ingroup Input*/
enum class DeviceEventType : uint8
{
	KeyboardEvent,
	CharacterEvent,
	ButtonEvent,
	AxisEvent,
	CursorEvent,

	InvalidEvent
};

/**@brief Input device types.
@ingroup Input*/
enum class DeviceType : uint8
{
	Keyboard,
	Mouse,
	Joystick
};

	
/**@brief Keyboard key change event.
@ingroup Input*/
struct KeyEvent
{
	KeyState					State;			///< Only up or down state.
	Keyboard::PhysicalKeys		Key;
};

/**@brief Keyboard event after translation to character.
@ingroup Input*/
struct CharacterEvent
{
	wchar_t			Character;
};

/**@brief Mouse button Change event.
@ingroup Input*/
struct ButtonEvent
{
	KeyState					State;			///< Only up or down state.
	Mouse::PhysicalButtons		Button;
};

/**@brief Mouse or joystick axis value changed event.
@ingroup Input*/
struct AxisEvent
{
	float						Delta;			///< Axis delta.
	Mouse::PhysicalAxes			Axis;
};

/**@brief Cursor position changed event.
@ingroup Input*/
struct CursorEvent
{
	short				OffsetX;
	short				OffsetY;
};

/**@brief KeyStates changed events.
@ingroup Input*/
struct DeviceEvent
{
	union
	{
		KeyEvent		Key;
		CharacterEvent	Character;
		ButtonEvent		Button;
		AxisEvent		Axis;
		CursorEvent		Cursor;
	};

	DeviceEventType		Type;
	Timestamp			LogicalTime;	///< You can compare this counter with counters in other devices, to compare events order.
										///< This doesn't work between frames.


// ================================ //
//
	DeviceEvent()
	{
		Type = DeviceEventType::InvalidEvent;
		LogicalTime = std::numeric_limits< Timestamp >::max();
	}

	DeviceEvent( KeyEvent evt, Timestamp timestamp )
	{
		Type = DeviceEventType::KeyboardEvent;
		Key = evt;
		LogicalTime = timestamp;
	}

	DeviceEvent( CharacterEvent evt, Timestamp timestamp )
	{
		Type = DeviceEventType::CharacterEvent;
		Character = evt;
		LogicalTime = timestamp;
	}

	DeviceEvent( ButtonEvent evt, Timestamp timestamp )
	{
		Type = DeviceEventType::ButtonEvent;
		Button = evt;
		LogicalTime = timestamp;
	}

	DeviceEvent( AxisEvent evt, Timestamp timestamp )
	{
		Type = DeviceEventType::AxisEvent;
		Axis = evt;
		LogicalTime = timestamp;
	}

	DeviceEvent( CursorEvent evt, Timestamp timestamp )
	{
		Type = DeviceEventType::CursorEvent;
		Cursor = evt;
		LogicalTime = timestamp;
	}

	DeviceEvent( const DeviceEvent& second )
	{
		memcpy( this, &second, sizeof( DeviceEvent ) );
	}

	void operator=( const DeviceEvent& second )
	{
		memcpy( this, &second, sizeof( DeviceEvent ) );
	}
};







}	// input
}	// sw


