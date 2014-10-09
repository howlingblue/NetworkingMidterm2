#include <limits>
//#include "../DebugDrawingSystem.hpp"
#include "Light.hpp"

STATIC const FloatVector3 Light::NO_DIRECTION = FloatVector3( 0.f, 0.f, 0.f );

//These values are chosen to make sure that the light should always work at all rational distances.
STATIC const float Light::NEVER_ZERO_INTENSITY = 10000000.f;
STATIC const float Light::ALWAYS_MAXIMUM_INTENSITY = 1.f / NEVER_ZERO_INTENSITY;

//This value is chosen as default because 2 - any dot product >= 1, so we will always get maximum light..
STATIC const float Light::APERTURE_360_DEGREES = 2.f;
//This value is chosen as default so that the light isn't attenuated at all.
STATIC const float Light::APERTURE_INVERSE_SIZE_360_DEGREES = 1.f;

//These are used in the fourth component of position. We will multiply this with the positionToLightVector in the shader.
STATIC const float Light::POSITION_GIVEN = 1.f;
STATIC const float Light::POSITION_NOT_GIVEN = 0.f;


void Light::Render() const
{
	//DebugDrawingSystem::DrawPoint( m_position, 0.5f, Color( m_colorAndBrightness.r, m_colorAndBrightness.g, m_colorAndBrightness.b, 1.f ), Shape::DRAWN_Always );
}
