#ifndef INCLUDED_FLOAT_4X4_MATRIX_HPP
#define INCLUDED_FLOAT_4X4_MATRIX_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "Matrix.hpp"

//-----------------------------------------------------------------------------------------------
typedef Matrix< float, 4, 4 > Float4x4Matrix;

//-----------------------------------------------------------------------------------------------
static const float F4X4_IDENTITY_ARRAY[ 16 ] = { 1.f, 0.f, 0.f, 0.f, 
												0.f, 1.f, 0.f, 0.f, 
												0.f, 0.f, 1.f, 0.f,
												0.f, 0.f, 0.f, 1.f };

static const Float4x4Matrix F4X4_IDENTITY_MATRIX = Float4x4Matrix( F4X4_IDENTITY_ARRAY );

//-----------------------------------------------------------------------------------------------
//This implementation courtesy of: http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
inline bool Invert( const Float4x4Matrix& matrix, Float4x4Matrix& out_invertedMatrix )
{
	Float4x4Matrix invertedMatrix = F4X4_IDENTITY_MATRIX;
	float determinant;
	int i;

	invertedMatrix[0] = matrix[5]  * matrix[10] * matrix[15] - 
		matrix[5]  * matrix[11] * matrix[14] - 
		matrix[9]  * matrix[6]  * matrix[15] + 
		matrix[9]  * matrix[7]  * matrix[14] +
		matrix[13] * matrix[6]  * matrix[11] - 
		matrix[13] * matrix[7]  * matrix[10];

	invertedMatrix[4] = -matrix[4]  * matrix[10] * matrix[15] + 
		matrix[4]  * matrix[11] * matrix[14] + 
		matrix[8]  * matrix[6]  * matrix[15] - 
		matrix[8]  * matrix[7]  * matrix[14] - 
		matrix[12] * matrix[6]  * matrix[11] + 
		matrix[12] * matrix[7]  * matrix[10];

	invertedMatrix[8] = matrix[4]  * matrix[9] * matrix[15] - 
		matrix[4]  * matrix[11] * matrix[13] - 
		matrix[8]  * matrix[5] * matrix[15] + 
		matrix[8]  * matrix[7] * matrix[13] + 
		matrix[12] * matrix[5] * matrix[11] - 
		matrix[12] * matrix[7] * matrix[9];

	invertedMatrix[12] = -matrix[4]  * matrix[9] * matrix[14] + 
		matrix[4]  * matrix[10] * matrix[13] +
		matrix[8]  * matrix[5] * matrix[14] - 
		matrix[8]  * matrix[6] * matrix[13] - 
		matrix[12] * matrix[5] * matrix[10] + 
		matrix[12] * matrix[6] * matrix[9];

	invertedMatrix[1] = -matrix[1]  * matrix[10] * matrix[15] + 
		matrix[1]  * matrix[11] * matrix[14] + 
		matrix[9]  * matrix[2] * matrix[15] - 
		matrix[9]  * matrix[3] * matrix[14] - 
		matrix[13] * matrix[2] * matrix[11] + 
		matrix[13] * matrix[3] * matrix[10];

	invertedMatrix[5] = matrix[0]  * matrix[10] * matrix[15] - 
		matrix[0]  * matrix[11] * matrix[14] - 
		matrix[8]  * matrix[2] * matrix[15] + 
		matrix[8]  * matrix[3] * matrix[14] + 
		matrix[12] * matrix[2] * matrix[11] - 
		matrix[12] * matrix[3] * matrix[10];

	invertedMatrix[9] = -matrix[0]  * matrix[9] * matrix[15] + 
		matrix[0]  * matrix[11] * matrix[13] + 
		matrix[8]  * matrix[1] * matrix[15] - 
		matrix[8]  * matrix[3] * matrix[13] - 
		matrix[12] * matrix[1] * matrix[11] + 
		matrix[12] * matrix[3] * matrix[9];

	invertedMatrix[13] = matrix[0]  * matrix[9] * matrix[14] - 
		matrix[0]  * matrix[10] * matrix[13] - 
		matrix[8]  * matrix[1] * matrix[14] + 
		matrix[8]  * matrix[2] * matrix[13] + 
		matrix[12] * matrix[1] * matrix[10] - 
		matrix[12] * matrix[2] * matrix[9];

	invertedMatrix[2] = matrix[1]  * matrix[6] * matrix[15] - 
		matrix[1]  * matrix[7] * matrix[14] - 
		matrix[5]  * matrix[2] * matrix[15] + 
		matrix[5]  * matrix[3] * matrix[14] + 
		matrix[13] * matrix[2] * matrix[7] - 
		matrix[13] * matrix[3] * matrix[6];

	invertedMatrix[6] = -matrix[0]  * matrix[6] * matrix[15] + 
		matrix[0]  * matrix[7] * matrix[14] + 
		matrix[4]  * matrix[2] * matrix[15] - 
		matrix[4]  * matrix[3] * matrix[14] - 
		matrix[12] * matrix[2] * matrix[7] + 
		matrix[12] * matrix[3] * matrix[6];

	invertedMatrix[10] = matrix[0]  * matrix[5] * matrix[15] - 
		matrix[0]  * matrix[7] * matrix[13] - 
		matrix[4]  * matrix[1] * matrix[15] + 
		matrix[4]  * matrix[3] * matrix[13] + 
		matrix[12] * matrix[1] * matrix[7] - 
		matrix[12] * matrix[3] * matrix[5];

	invertedMatrix[14] = -matrix[0]  * matrix[5] * matrix[14] + 
		matrix[0]  * matrix[6] * matrix[13] + 
		matrix[4]  * matrix[1] * matrix[14] - 
		matrix[4]  * matrix[2] * matrix[13] - 
		matrix[12] * matrix[1] * matrix[6] + 
		matrix[12] * matrix[2] * matrix[5];

	invertedMatrix[3] = -matrix[1] * matrix[6] * matrix[11] + 
		matrix[1] * matrix[7] * matrix[10] + 
		matrix[5] * matrix[2] * matrix[11] - 
		matrix[5] * matrix[3] * matrix[10] - 
		matrix[9] * matrix[2] * matrix[7] + 
		matrix[9] * matrix[3] * matrix[6];

	invertedMatrix[7] = matrix[0] * matrix[6] * matrix[11] - 
		matrix[0] * matrix[7] * matrix[10] - 
		matrix[4] * matrix[2] * matrix[11] + 
		matrix[4] * matrix[3] * matrix[10] + 
		matrix[8] * matrix[2] * matrix[7] - 
		matrix[8] * matrix[3] * matrix[6];

	invertedMatrix[11] = -matrix[0] * matrix[5] * matrix[11] + 
		matrix[0] * matrix[7] * matrix[9] + 
		matrix[4] * matrix[1] * matrix[11] - 
		matrix[4] * matrix[3] * matrix[9] - 
		matrix[8] * matrix[1] * matrix[7] + 
		matrix[8] * matrix[3] * matrix[5];

	invertedMatrix[15] = matrix[0] * matrix[5] * matrix[10] - 
		matrix[0] * matrix[6] * matrix[9] - 
		matrix[4] * matrix[1] * matrix[10] + 
		matrix[4] * matrix[2] * matrix[9] + 
		matrix[8] * matrix[1] * matrix[6] - 
		matrix[8] * matrix[2] * matrix[5];

	determinant = matrix[0] * invertedMatrix[0] + matrix[1] * invertedMatrix[4] + matrix[2] * invertedMatrix[8] + matrix[3] * invertedMatrix[12];

	if (determinant == 0)
		return false;

	determinant = 1.f / determinant;

	for (i = 0; i < 16; i++)
		out_invertedMatrix[ i ] = invertedMatrix[ i ] * determinant;

	return true;
}

//-----------------------------------------------------------------------------------------------
inline void UnitTestFloat4x4Vector()
{
	Float4x4Matrix matrix1 = Float4x4Matrix( F4X4_IDENTITY_MATRIX );
	matrix1( 1, 1 ) = 1;
	matrix1( 1, 2 ) = 2;
	matrix1( 1, 3 ) = 3;
	matrix1( 2, 1 ) = 3;
	matrix1( 2, 2 ) = 2;
	matrix1( 2, 3 ) = 1;
	matrix1( 3, 1 ) = 2;
	matrix1( 3, 2 ) = 1;
	matrix1( 3, 3 ) = 3;
	Float4x4Matrix matrix2;
	Invert( matrix1, matrix2 );
	Float4x4Matrix matrix3;
	Invert( matrix2, matrix3 );
}

#endif //INCLUDED_FLOAT_4X4_MATRIX_HPP
