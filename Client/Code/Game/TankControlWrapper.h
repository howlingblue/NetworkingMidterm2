#ifndef INCLUDED_TANK_CONTROL_WRAPPER_HPP
#define INCLUDED_TANK_CONTROL_WRAPPER_HPP

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
struct TankControlWrapper
{
	float tankMovementMagnitude, tankMovementHeading;
	float turretMovementMagnitude, turretMovementHeading;
	bool isShooting;

	TankControlWrapper()
		: isShooting( false )
		, tankMovementMagnitude( 0.0f )
		, tankMovementHeading( 0.0f )
		, turretMovementMagnitude( 0.0f )
		, turretMovementHeading( 0.0f )
	{ }
};

#endif //INCLUDED_TANK_CONTROL_WRAPPER_HPP
