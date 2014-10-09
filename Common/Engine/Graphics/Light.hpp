#ifndef INCLUDED_LIGHT_HPP
#define INCLUDED_LIGHT_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "../Math/EngineMath.hpp"
#include "../Math/FloatVector3.hpp"
#include "../Math/FloatVector4.hpp"
#include "../Color.hpp"

//-----------------------------------------------------------------------------------------------
class Light
{
private:
	static const FloatVector3 NO_DIRECTION;

	static const float ALWAYS_MAXIMUM_INTENSITY;
	static const float NEVER_ZERO_INTENSITY;

	static const float APERTURE_360_DEGREES;
	static const float APERTURE_INVERSE_SIZE_360_DEGREES;

	static const float POSITION_GIVEN;
	static const float POSITION_NOT_GIVEN;

	friend class Material;
	friend struct MaterialComponent;

public:

	#pragma region Constructors
	Light()
		: m_position( FloatVector3( 0.f, 0.f, 0.f ) )
		, m_direction( NO_DIRECTION )
		, m_colorAndBrightness( 1.f, 1.f, 1.f, 1.f )
		, m_outerRadiusOfZeroIntensity( NEVER_ZERO_INTENSITY )
		, m_inverseSizeOfDistanceAttenuationZone( ALWAYS_MAXIMUM_INTENSITY )
		, m_outerApertureAngleAsDotProduct( APERTURE_360_DEGREES )
		, m_inverseSizeOfApertureAttenuationZone( APERTURE_INVERSE_SIZE_360_DEGREES )
		, m_percentOfLightIsAmbientZeroToOne( 0.f )
		, m_positionWasGiven( POSITION_NOT_GIVEN )
	{ }

	/* Infinite Point Light */
	Light( const FloatVector3& position, const Color& color, float brightness )
		: m_position( position )
		, m_direction( NO_DIRECTION )
		, m_colorAndBrightness( color )
		, m_outerRadiusOfZeroIntensity( NEVER_ZERO_INTENSITY )
		, m_inverseSizeOfDistanceAttenuationZone( ALWAYS_MAXIMUM_INTENSITY )
		, m_outerApertureAngleAsDotProduct( APERTURE_360_DEGREES )
		, m_inverseSizeOfApertureAttenuationZone( APERTURE_INVERSE_SIZE_360_DEGREES )
		, m_percentOfLightIsAmbientZeroToOne( 0.f )
		, m_positionWasGiven( POSITION_GIVEN )
	{
		m_colorAndBrightness.a = static_cast< unsigned char >( brightness * 255.f );
	}

	/* Point Light and Local Ambient Light*/
	Light( const FloatVector3& position, const Color& color, float brightness, 
			float innerRadiusOfMaximumIntensity, float outerRadiusOfZeroIntensity, bool isAmbient )
		: m_position( position )
		, m_direction( NO_DIRECTION )
		, m_colorAndBrightness( color )
		, m_outerRadiusOfZeroIntensity( outerRadiusOfZeroIntensity )
		, m_inverseSizeOfDistanceAttenuationZone( 1.f / ( outerRadiusOfZeroIntensity - innerRadiusOfMaximumIntensity ) )
		, m_outerApertureAngleAsDotProduct( APERTURE_360_DEGREES )
		, m_inverseSizeOfApertureAttenuationZone( APERTURE_INVERSE_SIZE_360_DEGREES )
		, m_positionWasGiven( POSITION_GIVEN )
	{
		m_colorAndBrightness.a = static_cast< unsigned char >( brightness * 255.f );
		if( isAmbient )
			m_percentOfLightIsAmbientZeroToOne = 1.f;
		else
			m_percentOfLightIsAmbientZeroToOne = 0.f;
	}

	/* Local Spotlight */
	Light( const FloatVector3& position, const Color& color, float brightness, 
		float innerRadiusOfMaximumIntensity, float outerRadiusOfZeroIntensity,
		float innerApertureAngleDegrees, float outerApertureAngleDegrees, const FloatVector3& direction )
		: m_position( position )
		, m_direction( direction )
		, m_colorAndBrightness( color )
		, m_outerRadiusOfZeroIntensity( outerRadiusOfZeroIntensity )
		, m_inverseSizeOfDistanceAttenuationZone( 1.f / ( outerRadiusOfZeroIntensity - innerRadiusOfMaximumIntensity ) )
		, m_outerApertureAngleAsDotProduct( cos( ConvertDegreesToRadians( outerApertureAngleDegrees ) ) )
		, m_inverseSizeOfApertureAttenuationZone( 1.f / ( m_outerApertureAngleAsDotProduct - cos( ConvertDegreesToRadians( innerApertureAngleDegrees ) ) ) )
		, m_percentOfLightIsAmbientZeroToOne( 0.f )
		, m_positionWasGiven( POSITION_GIVEN )
	{
		m_colorAndBrightness.a = static_cast< unsigned char >( brightness * 255.f );
		//FIX: How to get aperture again?
	}

	/* Global Spotlight */
	Light( const FloatVector3& position, const Color& color, float brightness, 
		float innerApertureAngleDegrees, float outerApertureAngleDegrees, const FloatVector3& direction )
		: m_position( position )
		, m_direction( direction )
		, m_colorAndBrightness( color )
		, m_outerRadiusOfZeroIntensity( NEVER_ZERO_INTENSITY )
		, m_inverseSizeOfDistanceAttenuationZone( ALWAYS_MAXIMUM_INTENSITY )
		, m_outerApertureAngleAsDotProduct( cos( ConvertDegreesToRadians( outerApertureAngleDegrees ) ) )
		, m_inverseSizeOfApertureAttenuationZone( 1.f / ( m_outerApertureAngleAsDotProduct - cos( ConvertDegreesToRadians( innerApertureAngleDegrees ) ) ) )
		, m_percentOfLightIsAmbientZeroToOne( 0.f )
		, m_positionWasGiven( POSITION_GIVEN )
	{
		m_colorAndBrightness.a = static_cast< unsigned char >( brightness * 255.f );
		//FIX: How to get aperture again?
	}

	/* Infinite Directional Light */
	Light( const Color& color, float brightness, const FloatVector3& direction )
		: m_position( FloatVector3( 0.f, 0.f, 0.f ) )
		, m_direction( direction )
		, m_colorAndBrightness( color )
		, m_outerRadiusOfZeroIntensity( NEVER_ZERO_INTENSITY )
		, m_inverseSizeOfDistanceAttenuationZone( ALWAYS_MAXIMUM_INTENSITY )
		, m_outerApertureAngleAsDotProduct( APERTURE_360_DEGREES )
		, m_inverseSizeOfApertureAttenuationZone( APERTURE_INVERSE_SIZE_360_DEGREES )
		, m_percentOfLightIsAmbientZeroToOne( 0.f )
		, m_positionWasGiven( POSITION_NOT_GIVEN )
	{
		m_colorAndBrightness.a = static_cast< unsigned char >( brightness * 255.f );
	}

	/* Global Ambient Light */
	Light( const Color& color, float brightness )
		: m_position( FloatVector3( 0.f, 0.f, 0.f ) )
		, m_direction( NO_DIRECTION )
		, m_colorAndBrightness( color )
		, m_outerRadiusOfZeroIntensity( NEVER_ZERO_INTENSITY )
		, m_inverseSizeOfDistanceAttenuationZone( ALWAYS_MAXIMUM_INTENSITY )
		, m_outerApertureAngleAsDotProduct( APERTURE_360_DEGREES )
		, m_inverseSizeOfApertureAttenuationZone( APERTURE_INVERSE_SIZE_360_DEGREES )
		, m_percentOfLightIsAmbientZeroToOne( 1.f )
		, m_positionWasGiven( POSITION_NOT_GIVEN )
	{
		m_colorAndBrightness.a = static_cast< unsigned char >( brightness * 255.f );
	}

	/* Full Control Light */
	Light( const FloatVector3& position, const Color& color, float brightness, 
		float innerRadiusOfMaximumIntensity, float outerRadiusOfZeroIntensity,
		float innerApertureAngleDegrees, float outerApertureAngleDegrees, const FloatVector3& direction, float percentOfLightIsAmbientZeroToOne )
		: m_position( position )
		, m_direction( direction )
		, m_colorAndBrightness( color )
		, m_outerRadiusOfZeroIntensity( outerRadiusOfZeroIntensity )
		, m_inverseSizeOfDistanceAttenuationZone( 1.f / ( outerRadiusOfZeroIntensity - innerRadiusOfMaximumIntensity ) )
		, m_outerApertureAngleAsDotProduct( cos( ConvertDegreesToRadians( outerApertureAngleDegrees ) ) )
		, m_inverseSizeOfApertureAttenuationZone( 1.f / ( m_outerApertureAngleAsDotProduct - cos( ConvertDegreesToRadians( innerApertureAngleDegrees ) ) ) )
		, m_percentOfLightIsAmbientZeroToOne( percentOfLightIsAmbientZeroToOne )
		, m_positionWasGiven( POSITION_GIVEN )
	{
		m_colorAndBrightness.a = static_cast< unsigned char >( brightness * 255.f );
	}
	#pragma endregion

	FloatVector3 GetPosition() const { return m_position; }
	void SetPosition( const FloatVector3& position );
	void Render() const;

private:
	FloatVector3 m_position;
	FloatVector3 m_direction;
	Color m_colorAndBrightness;
	float m_outerRadiusOfZeroIntensity;
	float m_inverseSizeOfDistanceAttenuationZone;
	float m_outerApertureAngleAsDotProduct;
	float m_inverseSizeOfApertureAttenuationZone;
	float m_percentOfLightIsAmbientZeroToOne;
	float m_positionWasGiven;
};

inline void Light::SetPosition( const FloatVector3& position )
{
	m_position = position;
	m_positionWasGiven = POSITION_GIVEN;
}

#endif //INCLUDED_LIGHT_HPP
