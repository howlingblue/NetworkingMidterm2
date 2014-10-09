#ifndef INCLUDED_XML_HELPERS_HPP
#define INCLUDED_XML_HELPERS_HPP

#include "../Math/FloatVector2.hpp"
#include "../Math/FloatVector3.hpp"
#include "../Math/FloatVector4.hpp"
#include "../Math/IntVector2.hpp"
#include "../Math/IntVector3.hpp"
#include "../Color.hpp"
#include "../StringConversion.hpp"
#include "pugixml.hpp"

void UnitTestXMLHelpers();

namespace XML
{
	#pragma region Attribute Helpers
	//Built-in
	bool GetAttributeAsBool( const pugi::xml_node& xmlNode, const std::string& attributeName, bool defaultValue );
	float GetAttributeAsFloat( const pugi::xml_node& xmlNode, const std::string& attributeName, float defaultValue );
	int GetAttributeAsInteger( const pugi::xml_node& xmlNode, const std::string& attributeName, int defaultValue );
	unsigned int GetAttributeAsUnsignedInt( const pugi::xml_node& xmlNode, const std::string& attributeName, unsigned int defaultValue );
	std::string GetAttributeAsString( const pugi::xml_node& xmlNode, const std::string& attributeName, const std::string& defaultValue );

	//Custom
	Color GetAttributeAsColor( const pugi::xml_node& xmlNode, const std::string& attributeName, const Color& defaultValue );
	FloatVector2 GetAttributeAsFloatVector2( const pugi::xml_node& xmlNode, const std::string& attributeName, const FloatVector2& defaultValue );
	FloatVector3 GetAttributeAsFloatVector3( const pugi::xml_node& xmlNode, const std::string& attributeName, const FloatVector3& defaultValue );
	FloatVector4 GetAttributeAsFloatVector4( const pugi::xml_node& xmlNode, const std::string& attributeName, const FloatVector4& defaultValue );
	IntVector2 GetAttributeAsIntVector2( const pugi::xml_node& xmlNode, const std::string& attributeName, const IntVector2& defaultValue );
	IntVector3 GetAttributeAsIntVector3( const pugi::xml_node& xmlNode, const std::string& attributeName, const IntVector3& defaultValue );
	#pragma endregion

	void BuildStringVectorFromCommaSeparatedString( std::vector< std::string >& stringVector, const std::string& commaSeparatedString );
	void VerifyAttributesAtNodeOrDie( const pugi::xml_node& xmlNode, const std::string& commaSeparatedRequiredAttributes, const std::string& commaSeparatedOptionalAttributes );
	void VerifyChildrenAtNodeOrDie( const pugi::xml_node& xmlNode, const std::string& commaSeparatedRequiredChildren, const std::string& commaSeparatedOptionalChildren );
}

#pragma region Attribute Helpers
//-----------------------------------------------------------------------------------------------
inline bool XML::GetAttributeAsBool( const pugi::xml_node& xmlNode, const std::string& attributeName, bool defaultValue )
{
	std::string attributeNameFromXML( xmlNode.attribute( attributeName.c_str() ).name() );
	if( attributeNameFromXML.compare( "" ) == 0 )
		return defaultValue;
	else
		return ConvertStringToBool( xmlNode.attribute( attributeName.c_str() ).value() );
}

//-----------------------------------------------------------------------------------------------
inline float XML::GetAttributeAsFloat( const pugi::xml_node& xmlNode, const std::string& attributeName, float defaultValue )
{
	std::string attributeNameFromXML( xmlNode.attribute( attributeName.c_str() ).name() );
	if( attributeNameFromXML.compare( "" ) == 0 )
		return defaultValue;
	else
		return ConvertStringToFloat( xmlNode.attribute( attributeName.c_str() ).value() );
}

//-----------------------------------------------------------------------------------------------
inline int XML::GetAttributeAsInteger( const pugi::xml_node& xmlNode, const std::string& attributeName, int defaultValue )
{
	std::string attributeNameFromXML( xmlNode.attribute( attributeName.c_str() ).name() );
	if( attributeNameFromXML.compare( "" ) == 0 )
		return defaultValue;
	else
		return ConvertStringToInteger( xmlNode.attribute( attributeName.c_str() ).value() );
}

//-----------------------------------------------------------------------------------------------
inline unsigned int GetAttributeAsUnsignedInt( const pugi::xml_node& xmlNode, const std::string& attributeName, unsigned int defaultValue )
{
	std::string attributeNameFromXML( xmlNode.attribute( attributeName.c_str() ).name() );
	if( attributeNameFromXML.compare( "" ) == 0 )
		return defaultValue;
	else
		return ConvertStringToUnsignedInt( xmlNode.attribute( attributeName.c_str() ).value() );
}

//-----------------------------------------------------------------------------------------------
inline std::string XML::GetAttributeAsString( const pugi::xml_node& xmlNode, const std::string& attributeName, const std::string& defaultValue )
{
	std::string attributeNameFromXML( xmlNode.attribute( attributeName.c_str() ).name() );
	if( attributeNameFromXML.compare( "" ) == 0 )
		return defaultValue;
	else
		return xmlNode.attribute( attributeName.c_str() ).value();
}

//-----------------------------------------------------------------------------------------------
inline Color XML::GetAttributeAsColor( const pugi::xml_node& xmlNode, const std::string& attributeName, const Color& defaultValue )
{
	std::string attributeNameFromXML( xmlNode.attribute( attributeName.c_str() ).name() );
	if( attributeNameFromXML.compare( "" ) == 0 )
		return defaultValue;
	else
		return ConvertStringToColorOrDie( xmlNode.attribute( attributeName.c_str() ).value() );
}

//-----------------------------------------------------------------------------------------------
inline FloatVector2 XML::GetAttributeAsFloatVector2( const pugi::xml_node& xmlNode, const std::string& attributeName, const FloatVector2& defaultValue )
{
	std::string attributeNameFromXML( xmlNode.attribute( attributeName.c_str() ).name() );
	if( attributeNameFromXML.compare( "" ) == 0 )
		return defaultValue;
	else
		return ConvertStringToFloatVector2OrDie( xmlNode.attribute( attributeName.c_str() ).value() );
}

//-----------------------------------------------------------------------------------------------
inline FloatVector3 XML::GetAttributeAsFloatVector3( const pugi::xml_node& xmlNode, const std::string& attributeName, const FloatVector3& defaultValue )
{
	std::string attributeNameFromXML( xmlNode.attribute( attributeName.c_str() ).name() );
	if( attributeNameFromXML.compare( "" ) == 0 )
		return defaultValue;
	else
		return ConvertStringToFloatVector3OrDie( xmlNode.attribute( attributeName.c_str() ).value() );
}

//-----------------------------------------------------------------------------------------------
inline FloatVector4 XML::GetAttributeAsFloatVector4( const pugi::xml_node& xmlNode, const std::string& attributeName, const FloatVector4& defaultValue )
{
	std::string attributeNameFromXML( xmlNode.attribute( attributeName.c_str() ).name() );
	if( attributeNameFromXML.compare( "" ) == 0 )
		return defaultValue;
	else
		return ConvertStringToFloatVector4OrDie( xmlNode.attribute( attributeName.c_str() ).value() );
}

//-----------------------------------------------------------------------------------------------
inline IntVector2 XML::GetAttributeAsIntVector2( const pugi::xml_node& xmlNode, const std::string& attributeName, const IntVector2& defaultValue )
{
	std::string attributeNameFromXML( xmlNode.attribute( attributeName.c_str() ).name() );
	if( attributeNameFromXML.compare( "" ) == 0 )
		return defaultValue;
	else
		return ConvertStringToIntVector2OrDie( xmlNode.attribute( attributeName.c_str() ).value() );
}

//-----------------------------------------------------------------------------------------------
inline IntVector3 XML::GetAttributeAsIntVector3( const pugi::xml_node& xmlNode, const std::string& attributeName, const IntVector3& defaultValue )
{
	std::string attributeNameFromXML( xmlNode.attribute( attributeName.c_str() ).name() );
	if( attributeNameFromXML.compare( "" ) == 0 )
		return defaultValue;
	else
		return ConvertStringToIntVector3OrDie( xmlNode.attribute( attributeName.c_str() ).value() );
}
#pragma endregion

#endif //INCLUDED_XML_HELPERS_HPP
