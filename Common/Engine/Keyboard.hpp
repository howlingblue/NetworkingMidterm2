#ifndef INCLUDED_KEYBOARD_HPP
#define INCLUDED_KEYBOARD_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <queue>
#include "Bitmasking.hpp"

//-----------------------------------------------------------------------------------------------
class Keyboard
{
	static const unsigned int NUMBER_OF_KEYS = 256;

public:
	static enum Key
	{
		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D',
		E = 'E',
		F = 'F',
		G = 'G',
		H = 'H',
		I = 'I',
		J = 'J',
		K = 'K',
		L = 'L',
		M = 'M',
		N = 'N',
		O = 'O',
		P = 'P',
		Q = 'Q',
		R = 'R',
		S = 'S',
		T = 'T',
		U = 'U',
		V = 'V',
		W = 'W',
		X = 'X',
		Y = 'Y',
		Z = 'Z',

		GRAVE = 0xC0, // VK_OEM_3 in Windows
		TILDE = 0xC0, // VK_OEM_3 in Windows
		NUMBER_1 = '1',
		NUMBER_2 = '2',
		NUMBER_3 = '3',
		NUMBER_4 = '4',
		NUMBER_5 = '5',
		NUMBER_6 = '6',
		NUMBER_7 = '7',
		NUMBER_8 = '8',
		NUMBER_9 = '9',
		NUMBER_0 = '0',

		FUNCTION_1  = 0x70, //VK_F1  in Windows
		FUNCTION_2  = 0x71, //VK_F2  in Windows
		FUNCTION_3  = 0x72, //VK_F3  in Windows
		FUNCTION_4  = 0x73, //VK_F4  in Windows
		FUNCTION_5  = 0x74, //VK_F5  in Windows
		FUNCTION_6  = 0x75, //VK_F6  in Windows
		FUNCTION_7  = 0x76, //VK_F7  in Windows
		FUNCTION_8  = 0x77, //VK_F8  in Windows
		FUNCTION_9  = 0x78, //VK_F9  in Windows
		FUNCTION_10 = 0x79, //VK_F10 in Windows
		FUNCTION_11 = 0x7A, //VK_F11 in Windows
		FUNCTION_12 = 0x7B, //VK_F12 in Windows

		BACKSPACE	= 0x08, //VK_BACK in Windows
		SPACEBAR	= 0x20, //VK_SPACE in Windows
		ENTER		= 0x0D, //VK_RETURN in Windows
		ESCAPE		= 0x1B, //VK_ESCAPE in Windows
		TAB			= 0x09, //VK_TAB in Windows
		CONTROL		= 0x11, //VK_CONTROL in Windows
		ALT			= 0x12, //VK_MENU in Windows
		SHIFT		= 0x10, //VK_SHIFT in Windows

		INSERT		= 0x2D, //VK_INSERT in Windows
		DELETE_KEY	= 0x2E, //VK_DELETE in Windows
		HOME		= 0x24, //VK_HOME in Windows
		END			= 0x23, //VK_END in Windows
		PAGE_UP		= 0x21, //VK_PRIOR in Windows
		PAGE_DOWN	= 0x22, //VK_NEXT in Windows

		ARROW_UP	= 0x26, // VK_UP in Windows
		ARROW_DOWN	= 0x28,	// VK_DOWN in Windows
		ARROW_LEFT	= 0x25,	// VK_LEFT in Windows
		ARROW_RIGHT = 0x27,	// VK_RIGHT in Windows

		NUMPAD_0		= 0x60, // VK_NUMPAD0 in Windows
		NUMPAD_1		= 0x61, // VK_NUMPAD1 in Windows
		NUMPAD_2		= 0x62, // VK_NUMPAD2 in Windows
		NUMPAD_3		= 0x63, // VK_NUMPAD3 in Windows
		NUMPAD_4		= 0x64, // VK_NUMPAD4 in Windows
		NUMPAD_5		= 0x65, // VK_NUMPAD5 in Windows
		NUMPAD_6		= 0x66, // VK_NUMPAD6 in Windows
		NUMPAD_7		= 0x67, // VK_NUMPAD7 in Windows
		NUMPAD_8		= 0x68, // VK_NUMPAD8 in Windows
		NUMPAD_9		= 0x69, // VK_NUMPAD9 in Windows
		NUMPAD_PERIOD	= 0x6E, // VK_DECIMAL in Windows
		NUMPAD_PLUS		= 0x6B, // VK_ADD in Windows
		NUMPAD_MINUS	= 0x6D, // VK_SUBTRACT in Windows
		NUMPAD_TIMES	= 0x6A, // VK_MULTIPLY in Windows
		NUMPAD_DIVIDE	= 0x6F, // VK_DIVIDE in Windows
	};

	//Key Modifiers
	typedef unsigned int KeyModifier;
	static const KeyModifier MODIFIER_NONE			= 0;
	static const KeyModifier MODIFIER_CONTROL		= 1;
	static const KeyModifier MODIFIER_SHIFT			= 2;
	static const KeyModifier MODIFIER_CTRL_SHIFT	= 3;
	static const KeyModifier MODIFIER_ALT			= 4;
	static const KeyModifier MODIFIER_CTRL_ALT		= 5;
	static const KeyModifier MODIFIER_ALT_SHIFT		= 6;
	static const KeyModifier MODIFIER_CTRL_ALT_SHIFT= 7;

private:
	std::queue< unsigned char > m_characterInput;
	KeyModifier				m_keyModifiers;

	bool m_wasKeyDown[ NUMBER_OF_KEYS ];
	bool m_isKeyDown [ NUMBER_OF_KEYS ];

public:
	Keyboard()
	{
		for( unsigned int i = 0; i < NUMBER_OF_KEYS; ++i )
		{
			m_isKeyDown[ i ] = false;
			m_wasKeyDown[ i ] = false;
		}
		m_keyModifiers = MODIFIER_NONE;
	}

	void AddCharacterToInputQueue( unsigned char key ) { m_characterInput.push( key ); }
	void ClearInputQueue();
	bool InputQueueIsEmpty() const { return m_characterInput.empty(); }
	unsigned char GetCharacterFromInputQueue();

	bool KeyIsPressedOrHeld( Key key ) const	{ return m_isKeyDown[ key ];  }
	bool KeyIsNotPressedOrHeld( Key key ) const	{ return !KeyIsPressedOrHeld( key ); }

	bool KeyIsPressed( Key key ) const			{ return ( m_isKeyDown[ key ] && !m_wasKeyDown[ key ] ); }
	bool KeyIsNotPressed( Key key ) const		{ return !KeyIsPressed( key ); }

	bool KeyCombinationIsPressed( KeyModifier modifier, Key key ) const;
	bool KeyCombinationIsNotPressed( KeyModifier modifier, Key key ) const { return !KeyCombinationIsPressed( modifier, key ); }

	void SetKeyDown( Key key );
	void SetKeyDown( unsigned char key ) { SetKeyDown( Key( key ) ); };
	void SetKeyUp( unsigned char key ) { SetKeyUp( Key( key ) ); };
	void SetKeyUp( Key key );

	void Update();
};







//-----------------------------------------------------------------------------------------------
inline void Keyboard::ClearInputQueue() 
{ 
	while ( !m_characterInput.empty() )
	{
		m_characterInput.pop();
	}
}

//-----------------------------------------------------------------------------------------------
inline bool Keyboard::KeyCombinationIsPressed( KeyModifier modifier, Key key ) const
{
	if( modifier != m_keyModifiers )
	{
		if( !IsBitSetInMask( static_cast< unsigned int >( modifier ), static_cast< unsigned int >( m_keyModifiers ) ) )
			return false;
	}

	if( KeyIsNotPressed( key ) )
		return false;
	return true;
}

//-----------------------------------------------------------------------------------------------
inline unsigned char Keyboard::GetCharacterFromInputQueue()
{ 
	unsigned char frontChar = m_characterInput.front();
	m_characterInput.pop(); 
	return frontChar;
}

//-----------------------------------------------------------------------------------------------
inline void Keyboard::SetKeyDown( Key key )
{
	switch( key )
	{
	case CONTROL:
		SetBitInMask( MODIFIER_CONTROL, m_keyModifiers );
		break;

	case ALT:
		SetBitInMask( MODIFIER_ALT, m_keyModifiers );
		break;

	case SHIFT:
		SetBitInMask( MODIFIER_SHIFT, m_keyModifiers );
		break;

	default:
		break;
	}

	m_isKeyDown[ key ] = true;
}

//-----------------------------------------------------------------------------------------------
inline void Keyboard::SetKeyUp( Key key )
{
	switch( key )
	{
	case CONTROL:
		UnsetBitInMask( MODIFIER_CONTROL, m_keyModifiers );
		break;

	case ALT:
		UnsetBitInMask( MODIFIER_ALT, m_keyModifiers );
		break;

	case SHIFT:
		UnsetBitInMask( MODIFIER_SHIFT, m_keyModifiers );
		break;

	default:
		break;
	}

	m_isKeyDown[ key ] = false;
	m_wasKeyDown[ key ] = false;
}

//-----------------------------------------------------------------------------------------------
inline void Keyboard::Update()
{
	for( unsigned int key = 0; key < NUMBER_OF_KEYS; ++key )
	{
		if( m_isKeyDown[ key ] )
		{
			m_wasKeyDown[ key ] = true;
		}
	}
	ClearInputQueue();
}

#endif //INCLUDED_KEYBOARD_HPP
