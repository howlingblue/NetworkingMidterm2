#include <algorithm>
#include <vector>
#include "../DialogInterface.hpp"
#include "XMLHelpers.hpp"

//-----------------------------------------------------------------------------------------------
std::string StringIsPartOfSetWithBadCase( const std::string& casedString, const std::vector< std::string >& stringSet )
{
	std::string lowercasedString, lowercasedStringInSet;
	ConvertStringToLowercase( casedString, lowercasedString );
	for( unsigned int i = 0; i < stringSet.size(); ++i )
	{
		ConvertStringToLowercase( stringSet[ i ], lowercasedStringInSet );
		if( lowercasedString.compare( lowercasedStringInSet ) == 0 )
		{
			return stringSet[ i ];
		}
	}
	return "";
}

//-----------------------------------------------------------------------------------------------
void UnitTestXMLHelpers()
{
	pugi::xml_document textXMLDocument;

	pugi::xml_parse_result xmlParseResult = textXMLDocument.load_file( "Data/XML/test.xml" );
	if( xmlParseResult != true )
	{
		exit( -14 );
	}

	std::string source;
	pugi::xml_node rootNode = textXMLDocument.child( "hud" );
	for ( pugi::xml_node image = rootNode.child( "img" ); image; image = image.next_sibling( "img" ) )
	{
		//Correct attribute set
		XML::VerifyAttributesAtNodeOrDie( image, "src", "name, position" );
		source = XML::GetAttributeAsString( image, "thing", "noSource" );

		//Fails: name is not a required or optional attribute
		//XML::VerifyAttributesAtNodeOrDie( image, "src", "position" );

		//Fails: required attribute "misc" not found
		//XML::VerifyAttributesAtNodeOrDie( image, "src,   misc   ", "position" );

		//Fails: name, is not a required or optional attribute, but Name is!
		//XML::VerifyAttributesAtNodeOrDie( image, "src", "Name,position   " );
	}
}

//-----------------------------------------------------------------------------------------------
void XML::BuildStringVectorFromCommaSeparatedString( std::vector< std::string >& stringVector, const std::string& commaSeparatedString )
{
	size_t currentCommaLocation = 0, nextCommaLocation = 0;

	if( commaSeparatedString == "" )
		return;

	do
	{
		nextCommaLocation = commaSeparatedString.find( ",", currentCommaLocation );

		std::string substring = commaSeparatedString.substr( currentCommaLocation, nextCommaLocation - currentCommaLocation );
		substring.erase( std::remove_if( substring.begin(), substring.end(), isspace ), substring.end() ); //One liner to strip white space from string
		stringVector.push_back( substring );

		currentCommaLocation = nextCommaLocation + 1; 
		//When nextCommaLocation = npos, currentCommaLocation = 0, since npos = INT_MAX
	} while( currentCommaLocation != 0 );
}

void XML::VerifyAttributesAtNodeOrDie( const pugi::xml_node& xmlNode, const std::string& commaSeparatedRequiredAttributes, const std::string& commaSeparatedOptionalAttributes )
{
	static const std::string errorMessageBoxTitle = "Vingine Error: Bad Attribute in XML Document";

	std::vector< std::string > requiredAttributes;
	std::vector< std::string > optionalAttributes;

	BuildStringVectorFromCommaSeparatedString( requiredAttributes, commaSeparatedRequiredAttributes );
	BuildStringVectorFromCommaSeparatedString( optionalAttributes, commaSeparatedRequiredAttributes + ',' + commaSeparatedOptionalAttributes );

	std::string attributeName;
	for ( pugi::xml_attribute_iterator attributeIterator = xmlNode.attributes_begin(); attributeIterator != xmlNode.attributes_end(); ++attributeIterator )
	{
		attributeName = attributeIterator->name();

		//Check Requireds
		bool attributeFound = false;
		for( unsigned int i = 0; i < requiredAttributes.size(); ++i )
		{
			if( attributeName == requiredAttributes[ i ] )
			{
				attributeFound = true;
				requiredAttributes.erase( requiredAttributes.begin() + i );
				break;
			}
		}

		//Check Optionals
		for( unsigned int i = 0; ( i < optionalAttributes.size() ) && !attributeFound; ++i )
		{
			if( attributeName == optionalAttributes[ i ] )
				attributeFound = true;
		}

		if( !attributeFound )
		{
			std::string lowercasedMatchToAttributeName = StringIsPartOfSetWithBadCase( attributeName, optionalAttributes );

			if( lowercasedMatchToAttributeName != "" )
			{
				std::string errorString = "ERROR: Bad XML Attribute in file!\n\n";
				errorString.append( "Attribute \"" + attributeName + "\" in element <" + xmlNode.name() + "> probably has bad case.\n\n" );
				errorString.append( "Did you mean the attribute \"" + lowercasedMatchToAttributeName + "\"?\n" );
				PopUpSystemDialog( errorMessageBoxTitle, errorString );
				exit( -1 );
			}
			else
			{
				std::string errorString = "ERROR: Bad XML Attribute in file!\n\n";
				errorString.append( "Attribute \"" + attributeName + "\" is not a valid XML attribute of element <" + xmlNode.name() + ">.\n\n" );
				errorString.append( "Valid Attributes: \n");
				for( unsigned int i = 0; i < optionalAttributes.size(); ++i )
				{
					errorString.append( "\t" + optionalAttributes[ i ] + "\n" );
				}
				PopUpSystemDialog( errorMessageBoxTitle, errorString );
				exit( -1 );
			}
		}
	}

	if( !requiredAttributes.empty() )
	{
		std::string errorString = "ERROR: Bad XML Attribute in file!\n\n";
		std::string elementName = xmlNode.name();
		errorString.append( "Required Attributes missing in element <" + elementName + ">.\n\n" );
		errorString.append( "Missing Required Attributes: \n");
		for( unsigned int i = 0; i < requiredAttributes.size(); ++i )
		{
			errorString.append( "\t" + requiredAttributes[ i ] + "\n" );
		}
		PopUpSystemDialog( errorMessageBoxTitle, errorString );
		exit( -1 );
	}
}

//-----------------------------------------------------------------------------------------------
void XML::VerifyChildrenAtNodeOrDie( const pugi::xml_node& xmlNode, const std::string& commaSeparatedRequiredChildren, const std::string& commaSeparatedOptionalChildren )
{
	static const std::string errorMessageBoxTitle = "Vingine Error: Bad Child Element in XML Document";
	std::string errorString = "ERROR: Bad XML Child Element in file!\n\n";

	std::vector< std::string > requiredChildren;
	std::vector< std::string > optionalChildren;

	BuildStringVectorFromCommaSeparatedString( requiredChildren, commaSeparatedRequiredChildren );
	BuildStringVectorFromCommaSeparatedString( optionalChildren, commaSeparatedRequiredChildren + ',' + commaSeparatedOptionalChildren );

	std::string childName;
	for ( pugi::xml_node_iterator childIterator = xmlNode.begin(); childIterator != xmlNode.end(); ++childIterator )
	{
		childName = childIterator->name();

		//Check Requireds
		bool childFound = false;
		for( unsigned int i = 0; i < requiredChildren.size(); ++i )
		{
			if( childName == requiredChildren[ i ] )
			{
				childFound = true;
				requiredChildren.erase( requiredChildren.begin() + i );
				break;
			}
		}

		//Check Optionals
		for( unsigned int i = 0; ( i < optionalChildren.size() ) && !childFound; ++i )
		{
			if( childName == optionalChildren[ i ] )
				childFound = true;
		}

		if( !childFound )
		{
			std::string lowercasedMatchToChildName = StringIsPartOfSetWithBadCase( childName, optionalChildren );

			if( lowercasedMatchToChildName != "" )
			{
				errorString.append( "Child element <" + childName + "> of element <" + xmlNode.name() + "> probably has bad case.\n\n" );
				errorString.append( "Did you mean the element <" + lowercasedMatchToChildName + ">?\n" );
				PopUpSystemDialog( errorMessageBoxTitle, errorString );
				exit( -1 );
			}
			else
			{
				errorString.append( "Child element <" + childName + "> is not a valid child of element <" + xmlNode.name() + ">.\n\n" );
				errorString.append( "Valid Children: \n");
				for( unsigned int i = 0; i < optionalChildren.size(); ++i )
				{
					errorString.append( "\t" + optionalChildren[ i ] + "\n" );
				}
				PopUpSystemDialog( errorMessageBoxTitle, errorString );
				exit( -1 );
			}
		}
	}

	if( !requiredChildren.empty() )
	{
		std::string elementName = xmlNode.name();
		errorString.append( "Required child element of element <" + elementName + "> not found.\n\n" );
		errorString.append( "Missing Required Children: \n");
		for( unsigned int i = 0; i < requiredChildren.size(); ++i )
		{
			errorString.append( "\t" + requiredChildren[ i ] + "\n" );
		}
		PopUpSystemDialog( errorMessageBoxTitle, errorString );
		exit( -1 );
	}
}
