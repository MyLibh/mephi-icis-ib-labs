#pragma once

///====================================================================================================================================
/// File:				Vector.h
/// Author:				MyLibh
/// Created:			05.08.2019
///
/// Last modified by:	MyLibh
/// Last modified on:	05.08.2019
///====================================================================================================================================
/// Copyright(c) MyLibh. All rights reserved.
///====================================================================================================================================

#ifndef __VECTOR_H_INCLUDED__
#define __VECTOR_H_INCLUDED__

/// [url] https://docs.microsoft.com/en-us/cpp/c-runtime-library/complex-math-support?view=vs-2019 [/url]
#include <complex.h>

typedef int         INT;
typedef long double REAL;
typedef _Lcomplex   COMPLEX;

#define VECTOR_INTEGRAL	0x0	
#define VECTOR_REAL     0x1
#define VECTOR_COMPLEX  0x2

///====================================================================================================================================
/// <summary>   A two-dimensional vector. </summary>
///
/// <remarks>	MyLibh, 05.08.2019. </remarks>
///====================================================================================================================================

typedef struct _Vector
{
	int   type : 2;
	void* pX;
	void* pY;
} VECTOR, *PVECTOR;

///====================================================================================================================================
/// <summary>   Macro for x-component. </summary>
///
/// <remarks>	MyLibh, 05.08.2019. </remarks>
///====================================================================================================================================
#define VEC_X(vector, type) (*(type*)(vector->pX))

///====================================================================================================================================
/// <summary>   Macro for y-component. </summary>
///
/// <remarks>	MyLibh, 05.08.2019. </remarks>
///====================================================================================================================================
#define VEC_Y(vector, type) (*(type*)(vector->pY))

///====================================================================================================================================
/// <summary>   Creates vector. </summary>
///
/// <remarks>   MyLibh, 05.08.2019. </remarks>
///
/// <param name="type"> Type of vector components. </param>
/// <param name="cpX">	Value for x-component. </param>
/// <param name="cpY">  Value for y-component. </param>
///
/// <returns>	New vector or NULL. </returns>
///====================================================================================================================================
PVECTOR CreateVector(int type, void const* cpX, void const* cpY);

///====================================================================================================================================
/// <summary>   Deletes vector. </summary>
///
/// <remarks>   MyLibh, 05.08.2019. </remarks>
///
/// <param name="cpVector"> Vector for deleting. </param>
///====================================================================================================================================
void DeleteVector(PVECTOR cpVector);

///====================================================================================================================================
/// <summary>   Calculates sum of two vectors. </summary>
///
/// <remarks>   MyLibh, 05.08.2019. </remarks>
///
/// <param name="cpLHV"> Vector. </param>
/// <param name="cpRHV"> Vector. </param>
///
/// <returns>	Result of the sum of vectors. </returns>
///====================================================================================================================================
PVECTOR Sum(const PVECTOR cpLHV, const PVECTOR cpRHV);

///====================================================================================================================================
/// <summary>   Calculates scalar product. </summary>
///
/// <remarks>   MyLibh, 05.08.2019. </remarks>
///
/// <param name="cpLHV"> Vector. </param>
/// <param name="cpRHV"> Vector. </param>
///
/// <returns>	Scalar product. </returns>
///====================================================================================================================================
REAL ScalarProduct(const PVECTOR cpLHV, const PVECTOR cpRHV);

///====================================================================================================================================
/// <summary>   Dumps vector info into the STDOUT. </summary>
///
/// <remarks>   MyLibh, 05.08.2019. </remarks>
///
/// <param name="cpVector"> Vector for dumping. </param>
///====================================================================================================================================
void Dump(const PVECTOR cpVector);

#endif /* __VECTOR_H_INCLUDED__ */