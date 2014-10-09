#ifndef INCLUDED_XBOX_HPP
#define INCLUDED_XBOX_HPP
#pragma once

//----------------------------------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include "../Math/FloatVector2.hpp"

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
		LEFT_TRIGGER  = 1,
		RIGHT_TRIGGER = 2
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
		static const float			STICK_DEAD_ZONE;
		static const float			STICK_MAXIMUM;
		static const FloatVector2		STICK_NORMALIZING_VECTOR;
		static const float			TRIGGER_DEAD_ZONE;
		static const float			TRIGGER_NORMALIZER;
		static const unsigned char	TRIGGER_MAXIMUM = 255;
		static const unsigned int	VIBRATION_MAXIMUM = 65535;

		PadNumber			m_padNumber;
		XINPUT_STATE		m_controllerState;
		XINPUT_VIBRATION	m_vibration;
		float				m_vibrationTimeSeconds;
		WORD m_currentButtonState, m_lastButtonState;
		FloatVector2 m_leftStick, m_rightStick;
		float m_currentLeftTrigger, m_currentRightTrigger;
		float m_lastLeftTrigger, m_lastRightTrigger;

		float SmoothInput( float magnitude, float deadZone ) const;

	public:
		Controller();
		Controller( PadNumber padNumber );

		bool	IsButtonPressed( Button button ) const;
		bool	IsButtonPressedOrHeld( Button button ) const;

		FloatVector2 GetRawStickPosition( Stick stick ) const;
		float	GetStickMagnitude( Stick stick ) const;
		float	GetStickAngleDegrees( Stick stick ) const;
		float	GetStickAngleRadians( Stick stick ) const;

		float	GetTriggerMagnitude( Trigger trigger ) const;
		bool	IsTriggerPressed( Trigger trigger ) const;
		bool	IsTriggerPressedOrHeld( Trigger trigger ) const;

		void	Update( float deltaSeconds );
		void	Vibrate( float leftIntensityZeroToOne, float rightIntensityZeroToOne, float durationSeconds );
	};

	inline bool Controller::IsButtonPressed( Button button ) const
	{ 
		int maskedState = m_lastButtonState & button;
		bool notPreviouslyPressed = !( maskedState != NONE );
		return IsButtonPressedOrHeld( button ) && notPreviouslyPressed; 
	}

	inline bool Controller::IsButtonPressedOrHeld( Button button ) const
	{ 
		int maskedState = m_currentButtonState & button;
		return ( maskedState != NONE ); 
	}
}
#endif //INCLUDED_XBOX_HPP
