// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Vector.h"

/// [url] https://en.cppreference.com/w/cpp/header/cstdlib [/url]
#include <stdlib.h>
/// [url] https://en.cppreference.com/w/cpp/header/cstdio [/url]
#include <stdio.h>
/// [url] https://en.cppreference.com/w/cpp/header/cassert [/url]
#include <assert.h>
/// [url] https://en.cppreference.com/w/cpp/header/ctgmath [/url]
#include <tgmath.h>

///====================================================================================================================================
/// <summary>   Creates INT vector. </summary>
///
/// <remarks>   MyLibh, 05.08.2019. </remarks>
///
/// <param name="cX">	Value for x-component. </param>
/// <param name="cY">	Value for y-component. </param>
///
/// <returns>	New vector or NULL. </returns>
///====================================================================================================================================
PVECTOR CreateIntVector(const int cX, const int cY)
{
	VECTOR* pVector = (VECTOR*)calloc(1, sizeof(VECTOR));
	if (!pVector)
		return NULL;

	pVector->type = VECTOR_INTEGRAL;

	pVector->pX = calloc(1, sizeof(int));
	if(pVector->pX)
		VEC_X(pVector, int) = cX;

	pVector->pY = calloc(1, sizeof(int));
	if (pVector->pY)
		VEC_Y(pVector, int) = cY;

	return pVector;
}

///====================================================================================================================================
/// <summary>   Creates REAL vector. </summary>
///
/// <remarks>   MyLibh, 05.08.2019. </remarks>
///
/// <param name="cX">	Value for x-component. </param>
/// <param name="cY">	Value for y-component. </param>
///
/// <returns>	New vector or NULL. </returns>
///====================================================================================================================================
PVECTOR CreateRealVector(const REAL cX, const REAL cY)
{
	VECTOR* pVector = (VECTOR*)calloc(1, sizeof(VECTOR));
	if (!pVector)
		return NULL;

	pVector->type = VECTOR_REAL;

	pVector->pX = calloc(1, sizeof(REAL));
	if (pVector->pX)
		VEC_X(pVector, REAL) = cX;

	pVector->pY = calloc(1, sizeof(REAL));
	if (pVector->pY)
		VEC_Y(pVector, REAL) = cY;

	return pVector;
}

///====================================================================================================================================
/// <summary>   Creates COMPLEX vector. </summary>
///
/// <remarks>   MyLibh, 05.08.2019. </remarks>
///
/// <param name="cpX">	Value for x-component. </param>
/// <param name="cpY">	Value for y-component. </param>
///
/// <returns>	New vector or NULL. </returns>
///====================================================================================================================================
PVECTOR CreateComplexVector(COMPLEX const* cpX, COMPLEX const* cpY)
{
	VECTOR* pVector = (VECTOR*)calloc(1, sizeof(VECTOR));
	if (!pVector)
		return NULL;

	pVector->type = VECTOR_COMPLEX;

	pVector->pX = calloc(1, sizeof(COMPLEX));
	if (pVector->pX)
		VEC_X(pVector, COMPLEX) = *cpX;

	pVector->pY = calloc(1, sizeof(COMPLEX));
	if (pVector->pY)
		VEC_Y(pVector, COMPLEX) = *cpY;

	return pVector;
}

PVECTOR CreateVector(int type, void const* cpX, void const* cpY)
{
	if (type == VECTOR_INTEGRAL)
		return CreateIntVector(*(INT const*)cpX, *(INT const*)cpY);
	else if (type == VECTOR_REAL)
		return CreateRealVector(*(REAL const*)cpX, *(REAL const*)cpY);
	else if (type == VECTOR_COMPLEX)
		return CreateComplexVector(cpX, cpY);
	else
		return NULL;
}

void DeleteVector(PVECTOR pVector)
{
	if (pVector)
	{
		if (pVector->pX)
		{
			free(pVector->pX);
			pVector->pX = NULL;
		}

		if (pVector->pY)
		{
			free(pVector->pY);
			pVector->pY = NULL;
		}

		free(pVector);
		pVector = NULL;
	}
}

PVECTOR Sum(const PVECTOR cpLHV, const PVECTOR cpRHV)
{
	assert(cpLHV->type == cpRHV->type);

	if (cpLHV->type == VECTOR_INTEGRAL)
		return CreateIntVector(VEC_X(cpLHV, INT) + VEC_X(cpRHV, INT), VEC_Y(cpLHV, INT) + VEC_Y(cpRHV, INT));
	else if (cpLHV->type == VECTOR_REAL)
		return CreateRealVector(VEC_X(cpLHV, REAL) + VEC_X(cpRHV, REAL), VEC_Y(cpLHV, REAL) + VEC_Y(cpRHV, REAL));
	else if (cpLHV->type == VECTOR_COMPLEX)
	{
		COMPLEX x = _LCOMPLEX_(creall(VEC_X(cpLHV, COMPLEX)) + creall(VEC_X(cpRHV, COMPLEX)), cimagl(VEC_X(cpLHV, COMPLEX)) + cimagl(VEC_X(cpRHV, COMPLEX)));
		COMPLEX y = _LCOMPLEX_(creall(VEC_Y(cpLHV, COMPLEX)) + creall(VEC_Y(cpRHV, COMPLEX)), cimagl(VEC_Y(cpLHV, COMPLEX)) + cimagl(VEC_Y(cpRHV, COMPLEX)));

		return CreateComplexVector(&x, &y);
	}
	else
		return NULL;
}

#pragma warning(push)
#pragma warning(disable:4715) // 'ScalarProduct': not all control paths return a value
REAL ScalarProduct(const PVECTOR cpLHV, const PVECTOR cpRHV)
{
	assert(cpLHV->type == cpRHV->type);

	if (cpLHV->type == VECTOR_INTEGRAL)
		return ((REAL)VEC_X(cpLHV, INT) * VEC_X(cpRHV, INT) + (REAL)VEC_Y(cpLHV, INT) * VEC_Y(cpRHV, INT));
	else if (cpLHV->type == VECTOR_REAL)
		return (VEC_X(cpLHV, REAL) * VEC_X(cpRHV, REAL) + VEC_Y(cpLHV, REAL) * VEC_Y(cpRHV, REAL));
	else if (cpLHV->type == VECTOR_COMPLEX)
		return (REAL)(creall(_LCmulcc(VEC_X(cpLHV, COMPLEX), conjl(VEC_X(cpRHV, COMPLEX)))) + creall(_LCmulcc(VEC_Y(cpLHV, COMPLEX), conjl(VEC_Y(cpRHV, COMPLEX)))));

	abort();
}
#pragma warning(pop)

void Dump(const PVECTOR cpVector)
{
	static const char* SCP_VECTOR_TYPE[] = { {"int"}, {"long double"}, {"complex"} };

	printf("[VECTOR DUMP]\n\tVECTOR (%s) [0x%p]\n\t{\n", SCP_VECTOR_TYPE[cpVector->type], cpVector);
	if (cpVector->type == VECTOR_INTEGRAL)
		printf("\t\tx [0x%p] = %d\n\t\ty [0x%p] = %d", cpVector->pX, VEC_X(cpVector, INT), cpVector->pY, VEC_Y(cpVector, INT));
	else if (cpVector->type == VECTOR_REAL)
		printf("\t\tx [0x%p] = %Lf\n\t\ty [0x%p] = %Lf", cpVector->pX, VEC_X(cpVector, REAL), cpVector->pY, VEC_Y(cpVector, REAL));
	else if (cpVector->type == VECTOR_COMPLEX)
		printf("\t\tx [0x%p] = %Lf%+Lfi\n\t\ty [0x%p] = %Lf%+Lfi", cpVector->pX, creall(VEC_X(cpVector, COMPLEX)), cimagl(VEC_X(cpVector, COMPLEX)),  cpVector->pY, creall(VEC_Y(cpVector, COMPLEX)), cimagl(VEC_Y(cpVector, COMPLEX)));

	printf("\n\t}\n");
}