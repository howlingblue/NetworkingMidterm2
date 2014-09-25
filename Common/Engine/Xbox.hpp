#ifndef INCLUDED_XBOX_HPP
#define INCLUDED_XBOX_HPP
#pragma once

//----------------------------------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include "Vector2.hpp"

#pragma comment( lib, "xinput" ) // Link in the xinput.lib static library

//----------------------------------------------------------------------------------------------------
namespace Xbox
{
	//----------------------------------------------------------------------------------------------------
	static enum Button
	{
		NONE = 0,
		DPAD_UP = 0x1,
		DPAD_DOWN = 0x2,
		DPAD_LEFT = 0x4,
		DPAD_RIGHT = 0x8,
		START = 0x10,
		SELECT = 0x20,
		PUSH_LEFT_STICK = 0x40,
		PUSH_RIGHT_STICK = 0x80,
		LEFT_BUMPER = 0x100,
		RIGHT_BUMPER = 0x200,
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000
	};

	//----------------------------------------------------------------------------------------------------
	static enum Stick
	{
		LEFT_STICK = 0,
		RIGHT_STICK = 1
	};

	//----------------------------------------------------------------------------------------------------
	static enum Trigger
	{
		LEFT_TRIGGER = 0,
		RIGHT_TRIGGER = 1
	};

	//----------------------------------------------------------------------------------------------------
	class Controller
	{
	public:
		enum PadNumber
		{
			ONE = 0,
			TWO,
			THREE,
			FOUR
		};

	private:
		static const float STICK_DEAD_ZONE;
		static const float STICK_MAXIMUM;
		static const Vector2 STICK_NORMALIZING_VECTOR;
		static const float TRIGGER_DEAD_ZONE;
		static const float TRIGGER_NORMALIZER;
		static const unsigned char TRIGGER_MAXIMUM = 255;
		static const unsigned int VIBRATION_MAXIMUM = 65535;

		PadNumber m_padNumber;
		XINPUT_STATE m_controllerState;
		XINPUT_VIBRATION m_vibration;
		WORD m_currentButtonState, m_lastButtonState;
		Vector2 m_leftStick, m_rightStick;
		float m_currentLeftTrigger, m_currentRightTrigger;
		float m_lastLeftTrigger, m_lastRightTrigger;
		float m_vibrationTimeSeconds;

		float smoothInput( float magnitude, float deadZone );

	public:
		Controller( PadNumber padNumber );

		Vector2 getRawStickPosition( Stick stick );
		float getStickMagnitude( Stick stick );
		float getStickAngle( Stick stick );

		float getTriggerMagnitude( Trigger trigger );
		bool isTriggerPressed( Trigger trigger );
		bool isTriggerPressedOrHeld( Trigger trigger );

		inline bool isButtonPressedOrHeld( Button button ) 
		{ 
			int maskedState = m_currentButtonState & button;
			return ( maskedState != NONE ); 
		}

		inline bool isButtonPressed( Button button ) 
		{ 
			int maskedState = m_lastButtonState & button;
			bool notPreviouslyPressed = !( maskedState != NONE );
			return isButtonPressedOrHeld( button ) && notPreviouslyPressed; 
		}

		void SetControllerNumber( PadNumber number ) { m_padNumber = number; }

		void Update( float deltaSeconds );
		void Vibrate( float leftIntensityZeroToOne, float rightIntensityZeroToOne, float durationSeconds );
	};
}

#endif //INCLUDED_XBOX_HPP
