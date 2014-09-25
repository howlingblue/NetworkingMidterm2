#pragma once
#ifndef INCLUDED_COLOR_HPP
#define INCLUDED_COLOR_HPP

//-----------------------------------------------------------------------------------------------
struct Color
{
private:
	static const unsigned int INT_COLOR_RED_MASK   = 0xFF000000;
	static const unsigned int INT_COLOR_GREEN_MASK = 0x00FF0000;
	static const unsigned int INT_COLOR_BLUE_MASK  = 0x0000FF00;
	static const unsigned int INT_COLOR_ALPHA_MASK = 0x000000FF;

	static const unsigned int SHIFT_INT_RED_COMPONENT_TO_CHAR   = 24;
	static const unsigned int SHIFT_INT_GREEN_COMPONENT_TO_CHAR = 16;
	static const unsigned int SHIFT_INT_BLUE_COMPONENT_TO_CHAR  = 8;
	static const unsigned int SHIFT_ONE_BYTE = 8;

	static const float CHAR_COLOR_TO_FLOAT_COLOR;
	static const float FLOAT_COLOR_TO_CHAR_COLOR;

public:
	Color();
	Color( unsigned int rgbaComponentsAsBytesInInteger );
	Color( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255 );
	Color( float normalizedRed, float normalizedGreen, float normalizedBlue, float normalizedAlpha = 1.f );

	//Operators
	bool operator==( const Color& rhs ) const;
	bool operator<( const Color& rhs ) const;

	//Gets/Sets
	unsigned int GetColorAsInteger() const;

	void SetColor( unsigned int rgbaComponentsAsBytesInInteger );
	void SetColor( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255 );
	void SetColor( float normalizedRed, float normalizedGreen, float normalizedBlue, float normalizedAlpha = 1.f );

	//Data Members
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};



//-----------------------------------------------------------------------------------------------
inline Color::Color()
	: r( 0 )
	, g( 0 )
	, b( 0 )
	, a( 0 )
{ }

//-----------------------------------------------------------------------------------------------
inline Color::Color( unsigned int rgbaComponentsAsBytesInInteger )
	: r( (unsigned char)( ( rgbaComponentsAsBytesInInteger & INT_COLOR_RED_MASK   ) >> SHIFT_INT_RED_COMPONENT_TO_CHAR ) )
	, g( (unsigned char)( ( rgbaComponentsAsBytesInInteger & INT_COLOR_GREEN_MASK ) >> SHIFT_INT_GREEN_COMPONENT_TO_CHAR ) )
	, b( (unsigned char)( ( rgbaComponentsAsBytesInInteger & INT_COLOR_BLUE_MASK  ) >> SHIFT_INT_BLUE_COMPONENT_TO_CHAR ) )
	, a( (unsigned char)( ( rgbaComponentsAsBytesInInteger & INT_COLOR_ALPHA_MASK ) ) )
{ }

//-----------------------------------------------------------------------------------------------
inline Color::Color( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
	: r( red )
	, g( green )
	, b( blue )
	, a( alpha )
{ }

//-----------------------------------------------------------------------------------------------
inline Color::Color( float normalizedRed, float normalizedGreen, float normalizedBlue, float normalizedAlpha )
	: r( (unsigned char)( normalizedRed * FLOAT_COLOR_TO_CHAR_COLOR   ) )
	, g( (unsigned char)( normalizedGreen * FLOAT_COLOR_TO_CHAR_COLOR ) )
	, b( (unsigned char)( normalizedBlue * FLOAT_COLOR_TO_CHAR_COLOR  ) )
	, a( (unsigned char)( normalizedAlpha * FLOAT_COLOR_TO_CHAR_COLOR ) )
{ }

//-----------------------------------------------------------------------------------------------
inline bool Color::operator==( const Color& rhs ) const
{
	if( ( this->r == rhs.r ) &&
		( this->g == rhs.g ) &&
		( this->b == rhs.b ) &&
		( this->a == rhs.a ) )
		return true;

	return false;
}

//-----------------------------------------------------------------------------------------------
inline bool Color::operator<( const Color& rhs ) const
{
	if( this->r < rhs.r )
		return true;
	else if( this->r == rhs.r )
	{
		if( this->g < rhs.g )
			return true;
		else if( this->g == rhs.g )
		{
			if( this->b < rhs.b )
				return true;
			else if( this->b == rhs.b )
			{
				if( this->a < rhs.a )
					return true;
				else
					return false;
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------------------------------------
inline unsigned int Color::GetColorAsInteger() const
{
	unsigned int colorAsInteger = 0;

	colorAsInteger |= r;
	colorAsInteger <<= SHIFT_ONE_BYTE;
	colorAsInteger |= g;
	colorAsInteger <<= SHIFT_ONE_BYTE;
	colorAsInteger |= b;
	colorAsInteger <<= SHIFT_ONE_BYTE;
	colorAsInteger |= a;

	return colorAsInteger;
}

//-----------------------------------------------------------------------------------------------
inline void Color::SetColor( unsigned int rgbaComponentsAsBytesInInteger )
{
	r = (unsigned char)( ( rgbaComponentsAsBytesInInteger & INT_COLOR_RED_MASK   ) >> SHIFT_INT_RED_COMPONENT_TO_CHAR );
	g = (unsigned char)( ( rgbaComponentsAsBytesInInteger & INT_COLOR_GREEN_MASK ) >> SHIFT_INT_GREEN_COMPONENT_TO_CHAR );
	b = (unsigned char)( ( rgbaComponentsAsBytesInInteger & INT_COLOR_BLUE_MASK  ) >> SHIFT_INT_BLUE_COMPONENT_TO_CHAR );
	a = (unsigned char)( ( rgbaComponentsAsBytesInInteger & INT_COLOR_ALPHA_MASK ) );
}

//-----------------------------------------------------------------------------------------------
inline void Color::SetColor( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

//-----------------------------------------------------------------------------------------------
inline void Color::SetColor( float normalizedRed, float normalizedGreen, float normalizedBlue, float normalizedAlpha )
{
	r = (unsigned char)( normalizedRed * FLOAT_COLOR_TO_CHAR_COLOR   );
	g = (unsigned char)( normalizedGreen * FLOAT_COLOR_TO_CHAR_COLOR );
	b = (unsigned char)( normalizedBlue * FLOAT_COLOR_TO_CHAR_COLOR  );
	a = (unsigned char)( normalizedAlpha * FLOAT_COLOR_TO_CHAR_COLOR );
}

#endif //INCLUDED_COLOR_HPP