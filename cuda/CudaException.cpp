/*!
	\file CudaException.cpp
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\date Friday May 8, 2009 
	\brief The sources for the CUDA Exception class.
*/

#ifndef CUDA_EXCEPTION_CPP_INCLUDED
#define CUDA_EXCEPTION_CPP_INCLUDED

#include "CudaException.h"
#include <implementation/string.h>
#include <cuda.h>

namespace cuda
{
	CudaException::CudaException( ) : code( cudaErrorUnknown )
	{
	}

	CudaException::CudaException( cudaError_t c ) : code( c )
	{
	}
	
	CudaException::~CudaException() throw()
	{
	}

	const char* CudaException::what() const throw()
	{
		return cudaGetErrorString( code );
	}
}

void cudaCheck( cudaError_t code )
{
	if( code == cudaSuccess )
	{
		return;
	}
	
	throw cuda::CudaException( code );
}

#endif

