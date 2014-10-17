#pragma once
#ifndef INCLUDED_HASH_FUNCTIONS_HPP
#define INCLUDED_HASH_FUNCTIONS_HPP

//-----------------------------------------------------------------------------------------------
//Hash functions in this file have been taken from many sources:
//	(DJB2 and sdbm) http://www.cse.yorku.ca/~oz/hash.html
//	(Eiserloh)		Given from Squirrel Eiserloh directly in Spring 2014
//	(Hsieh)			http://www.azillionmonkeys.com/qed/hash.html
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
typedef unsigned int Hash;


// DJB2
Hash HashWithDJB2( unsigned char* string );
Hash HashWithDJB2( unsigned char* buffer, unsigned int bufferSize );



// Eiserloh
Hash HashWithEiserloh( unsigned char* string );
Hash HashWithEiserloh( unsigned char* buffer, unsigned int bufferSize );



// Hsieh
Hash HashWithHsieh( unsigned char* string );
Hash HashWithHsieh( unsigned char* buffer, unsigned int bufferSize );



// SDBM
Hash HashWithSDBM( unsigned char* string );
Hash HashWithSDBM( unsigned char* buffer, unsigned int bufferSize );

#endif //INCLUDED_HASH_FUNCTIONS_HPP
