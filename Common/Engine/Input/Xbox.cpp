#include <math.h>
#include "../Math/EngineMath.hpp"
#include "../Math/FloatVector2.hpp"
#include "Xbox.hpp"

//----------------------------------------------------------------------------------------------------
const float   Xbox::Controller::STICK_DEAD_ZONE = 0.3f;
const float	  Xbox::Controller::STICK_MAXIMUM = 32768.f;
const FloatVector2 Xbox::Controller::STICK_NORMALIZING_VECTOR = (1.f / FloatVector2( STICK_MAXIMUM, STICK_MAXIMUM ));
const float   Xbox::Controller::TRIGGER_DEAD_ZONE = 0.1f;
const float   Xbox::Controller::TRIGGER_NORMALIZER = 1.f / TRIGGER_MAXIMUM;

//----------------------------------------------------------------------------------------------------
float Xbox::Controller:: SmoothInput( float magnitude, float deadZone ) const
{
	return ( magnitude - deadZone ) / ( 1.f - deadZone );
}

//----------------------------------------------------------------------------------------------------
Xbox::Controller::Controller()
	: m_padNumber( Controller::ONE )
	, m_currentButtonState( 0 )
	, m_lastButtonState( m_currentButtonState )
	, m_currentLeftTrigger( 0 )
	, m_currentRightTrigger( 0 )
	, m_lastLeftTrigger( m_currentLeftTrigger )
	, m_lastRightTrigger( m_currentRightTrigger )
{
	memset( &m_controllerState, 0, sizeof( m_controllerState ) );
	memset( &m_vibration, 0, sizeof(XINPUT_VIBRATION) );
}

//----------------------------------------------------------------------------------------------------
Xbox::Controller::Controller( PadNumber padNumber )
	: m_padNumber( padNumber )
	, m_currentButtonState( 0 )
	, m_lastButtonState( m_currentButtonState )
	, m_currentLeftTrigger( 0 )
	, m_currentRightTrigger( 0 )
	, m_lastLeftTrigger( m_currentLeftTrigger )
	, m_lastRightTrigger( m_currentRightTrigger )
{
	memset( &m_controllerState, 0, sizeof( m_controllerState ) );
	memset( &m_vibration, 0, sizeof(XINPUT_VIBRATION) );
}

//----------------------------------------------------------------------------------------------------
FloatVector2 Xbox::Controller::GetRawStickPosition( Stick stick ) const
{
	switch( stick )
	{
	case LEFT_STICK:
		return m_leftStick;
	case RIGHT_STICK:
	default:
		return m_rightStick;
	}
}

//----------------------------------------------------------------------------------------------------
float Xbox::Controller::GetStickMagnitude( Stick stick ) const
{
	FloatVector2 stickPosition;
	switch( stick )
	{
	case LEFT_STICK:
		stickPosition = m_leftStick;
		break;
	case RIGHT_STICK:
	default:
		stickPosition = m_rightStick;
	}

	float stickRadiusLength = stickPosition.CalculateNorm();
	if(stickRadiusLength > STICK_DEAD_ZONE)
	{
		return SmoothInput( stickRadiusLength, STICK_DEAD_ZONE );
	}
	else return 0.0f;
}

//----------------------------------------------------------------------------------------------------
float Xbox::Controller::GetStickAngleDegrees( Stick stick ) const
{
	return ConvertRadiansToDegrees( GetStickAngleRadians( stick ) );
}

//----------------------------------------------------------------------------------------------------
float Xbox::Controller::GetStickAngleRadians( Stick stick ) const
{
	FloatVector2 stickPosition;
	switch( stick )
	{
	case LEFT_STICK:
		stickPosition = m_leftStick;
		break;
	case RIGHT_STICK:
	default:
		stickPosition = m_rightStick;
	}

	float stickRadiusLength = stickPosition.CalculateNorm();
	if(stickRadiusLength > STICK_DEAD_ZONE)
	{
		float stickAngleRadians = atan2( stickPosition.y, stickPosition.x );
		return stickAngleRadians;
	}
	else return 0.0f;
}

//----------------------------------------------------------------------------------------------------
float Xbox::Controller::GetTriggerMagnitude( Trigger trigger ) const
{
	if( !IsTriggerPressedOrHeld( trigger ) )
		return 0.0f;

	float triggerPosition;
	switch( trigger )
	{
	case LEFT_TRIGGER:
		triggerPosition = m_currentLeftTrigger;
		break;
	case RIGHT_TRIGGER:
	default:
		triggerPosition = m_currentRightTrigger;
	}

	return SmoothInput( triggerPosition, TRIGGER_DEAD_ZONE );
}

//----------------------------------------------------------------------------------------------------
bool Xbox::Controller::IsTriggerPressed( Trigger trigger ) const
{
	float previousTriggerPosition;
	switch( trigger )
	{
	case LEFT_TRIGGER:
		previousTriggerPosition = m_lastLeftTrigger;
		break;
	case RIGHT_TRIGGER:
	default:
		previousTriggerPosition = m_lastRightTrigger;
	}
	return ( IsTriggerPressedOrHeld( trigger ) && previousTriggerPosition < TRIGGER_DEAD_ZONE );
}

//----------------------------------------------------------------------------------------------------
bool Xbox::Controller::IsTriggerPressedOrHeld( Trigger trigger ) const
{
	float triggerPosition;
	switch( trigger )
	{
	case LEFT_TRIGGER:
		triggerPosition = m_currentLeftTrigger;
		break;
	case RIGHT_TRIGGER:
	default:
		triggerPosition = m_currentRightTrigger;
	}

	if( triggerPosition > TRIGGER_DEAD_ZONE )
		return true;

	return false;
}

//----------------------------------------------------------------------------------------------------
void Xbox::Controller::Update( float deltaSeconds )
{
	if( m_vibrationTimeSeconds > 0 )
	{
		m_vibrationTimeSeconds -= deltaSeconds;
	}
	else
	{
		m_vibrationTimeSeconds = 0.f;
		this->Vibrate( 0.f, 0.f, 0.f );
	}
	DWORD errorStatus = XInputGetState( m_padNumber, &m_controllerState );

	if( errorStatus != ERROR_SUCCESS )
		return;

	m_lastButtonState = m_currentButtonState;
	m_lastLeftTrigger = m_currentLeftTrigger;
	m_lastRightTrigger = m_currentRightTrigger;

	m_currentButtonState = m_controllerState.Gamepad.wButtons;
	m_leftStick = FloatVector2( m_controllerState.Gamepad.sThumbLX, m_controllerState.Gamepad.sThumbLY );
	m_leftStick = MultiplyComponents( m_leftStick, STICK_NORMALIZING_VECTOR );
	m_rightStick = FloatVector2( m_controllerState.Gamepad.sThumbRX, m_controllerState.Gamepad.sThumbRY );
	m_rightStick = MultiplyComponents( m_rightStick, STICK_NORMALIZING_VECTOR );
	m_currentLeftTrigger = m_controllerState.Gamepad.bLeftTrigger;
	m_currentLeftTrigger *= TRIGGER_NORMALIZER;
	m_currentRightTrigger = m_controllerState.Gamepad.bRightTrigger;
	m_currentRightTrigger *= TRIGGER_NORMALIZER;
}

//----------------------------------------------------------------------------------------------------
void Xbox::Controller::Vibrate( float leftIntensityZeroToOne, float rightIntensityZeroToOne, float durationSeconds )
{
	m_vibration.wLeftMotorSpeed = static_cast< WORD >( leftIntensityZeroToOne * VIBRATION_MAXIMUM ); 
	m_vibration.wRightMotorSpeed = static_cast< WORD >( rightIntensityZeroToOne * VIBRATION_MAXIMUM ); 
	m_vibrationTimeSeconds = durationSeconds;

	//Vibrate the controller 
	XInputSetState( m_padNumber, &m_vibration ); 
}