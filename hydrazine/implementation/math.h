/*!	\file math.h
*
*	\brief Header file for common math functions.
*
*	\author Gregory Diamos
*
*	\date : 9/27/2007
*	\date : 10/24/2007 : added exceptions
*	\date : 5/27/2010 : added extended precision multiply
*
*/

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include <hydrazine/interface/MetaProgramming.h>
#include <cassert>

/*! \brief a namespace for common classes and functions */
namespace hydrazine
{

	/*! \brief Check to see if an int is a power of two */
	inline bool isPowerOfTwo (int value);

	/*! \brief Check to see if an unsigned int is a power of two */
	inline bool isPowerOfTwo (unsigned int value);

	/*! \brief Mod a power of power of two */
	inline unsigned int modPowerOfTwo( unsigned int value1, 
		unsigned int value );

	/*! \brief Mod a power of power of two */
	inline int modPowerOfTwo( int value1, int value );
		
	/*! \brief Compute the next highest power of two */
	inline unsigned int powerOfTwo( unsigned int value );

	/*! \brief Reverse the bits in the operand */
	template< typename type >
	type brev( type value );

	/*! \brief Get a bit at a single position */
	template< typename type >
	type bitExtract( type value, unsigned int position );

	/*! \brief Set a bit at a single position */
	template< typename type >
	type bitInsert( type value, unsigned int position );

	/*! \brief Find the leading bit in the operand */
	template< typename type >
	unsigned int bfind( type value, bool shiftAmount );

	/*! \brief Compute the number of bits set in the operand */
	template< typename type >
	unsigned int popc( type value );

	/*! \brief Compute the number of leading zeros in the operand */
	template< typename type >
	unsigned int countLeadingZeros( type value );
	
	/*! \brief Perform extended precision multiply */
	template< typename type >
	void multiplyHiLo( type& hi, type& lo, type r0, type r1 );

	/*! \brief Perform extended precision add */
	template< typename type >
	void addHiLo( type& hi, type& lo, type r0 );
	
	////////////////////////////////////////////////////////////////////////////
	// Power of two checks
	inline bool isPowerOfTwo( int value )
	{
		return (value & (~value+1)) == value;
	}
	
	inline bool isPowerOfTwo( unsigned int value )
	{
		return (value & (~value+1)) == value;
	}
	
	inline unsigned int modPowerOfTwo( unsigned int value1, unsigned int value )
	{
		assert( value != 0 );
		return value1 & ( value - 1 );
	}

	inline int modPowerOfTwo( int value1, int value )
	{
		assert( value != 0 );
		return value1 & ( value - 1 );
	}
	
	inline unsigned int powerOfTwo( unsigned int value )
	{	
		value--;		
		value |= value >> 1;		
		value |= value >> 2;		
		value |= value >> 4;		
		value |= value >> 8;		
		value |= value >> 16;		
		value++;		
		return value;		
	}
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Bit Manipulation
	template< typename type >
	unsigned int countLeadingZeros( type value )
	{
		unsigned int count = 0;
		unsigned int max = 8 * sizeof( type );
		type mask = ( ( type ) 1 ) << ( max - 1 );
		
		while( count < max && ( ( value & mask ) == 0 ) )
		{
			++count;
			value <<= 1;
		}
		
		return count;	
	}

	template< typename type >
	unsigned int popc( type value )
	{
		unsigned int count = 0;
		
		while( value != 0 )
		{
			if( value & 1 ) ++count;
			value >>= 1;
		}
		
		return count;	
	}

	template< typename type >
	unsigned int bfind( type value, bool shiftAmount )
	{
		unsigned int d = -1;
		long long int msb = 8 * sizeof( type ) - 1;
		
		for( long long int i = msb; i >= 0; --i )
		{
			if( value & ( 1 << i ) )
			{
				d = i;
				break;
			}
		}
		
		if( shiftAmount )
		{
			if( d != ( unsigned int ) - 1 )
			{
				d = msb - d;
			}
		}

		return d;
	}

	template< typename type >
	type bitExtract( type value, unsigned int position )
	{
		value >>= position;
		value &= 1;
		value <<= position;
		return value;
	}

	template< typename type >
	type bitInsert( type value, type bit, unsigned int position )
	{
		bit &= 1;
		bit <<= position;
		type mask = ~bit;
		value &= mask;
		value |= bit;
		return value;
	}

	template< typename type >
	type brev( type value )
	{
		type msb = sizeof( type ) * 8 - 1;
		
		type result = 0;
		for( unsigned int i = 0; i <= msb; ++i )
		{
			result = bitInsert( result, bitExtract( value, msb - i ), i );
		}
		
		return result;
	}
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// Extended precision
	template< typename type >
	void multiplyHiLo( type& hi, type& lo, type r0, type r1 )
	{
		bool r0Signed = isNegative( r0 );
		bool r1Signed = isNegative( r1 );

		r0 = ( r0Signed ) ? -r0 : r0;
		r1 = ( r1Signed ) ? -r1 : r1;
	
		typedef typename SignedToUnsigned< type >::type utype;

		bool negative = ( r0Signed && !r1Signed ) || ( r1Signed && !r0Signed );

		unsigned int bits = sizeof( type ) * 4;
		utype mask = ( ( type ) 1 << bits ) - 1;
	
		// A B
		// C D
		//
		//    DA DB
		// CA CB

		utype a = ( utype ) r0 >> bits;
		utype b = ( utype ) r0 & mask;
		utype c = ( utype ) r1 >> bits;
		utype d = ( utype ) r1 & mask;

		// 

		utype da = d * a;
		utype db = d * b;
		utype ca = c * a;
		utype cb = c * b;

		utype totalBits = ( sizeof( type ) * 8 - 1);
		utype upperMask = ( ( type ) 1 << totalBits ) - 1;
		utype daUpper = da >> totalBits;
		utype cbUpper = cb >> totalBits;
		utype xCarryIn = ( ( da & upperMask ) 
			+ ( cb & upperMask ) ) >> totalBits;
		utype xCarryOut = ( daUpper + cbUpper + xCarryIn ) >> 1;

		utype x = da + cb;

		utype xUpper = x >> totalBits;
		utype yCarryIn = ( ( x & upperMask ) + ( db >> bits ) ) >> totalBits;
		utype yCarryOut = ( yCarryIn + xUpper ) >> 1;

		utype y = ( db >> bits ) + x;

		lo = ( db & mask ) | ( ( y & mask ) << bits );
		hi = ( y >> bits ) + ca + ( ( yCarryOut + xCarryOut ) << bits );
	
		utype loUpperBit = ( utype )( ~( utype ) lo ) >> totalBits;
		utype signCarryIn = ( ( ( utype ) ( ~( utype ) lo ) 
			& upperMask ) + 1 ) >> totalBits;
		utype signCarryOut = ( loUpperBit + signCarryIn ) >> 1;

		lo = ( negative ) ? -lo : lo;
		hi = ( negative ) ? ( utype )( ~( utype ) hi ) + signCarryOut : hi;
	}
	
	template< typename type >
	void addHiLo( type& hi, type& lo, type r0 )
	{
		typedef typename SignedToUnsigned< type >::type utype;
	
		utype uHi = hi;
		utype uLo = lo;
		utype uR0 = r0;
		
		utype loResult = uR0 + uLo;
		utype carry = (loResult < uR0 || loResult < uLo) ? 1 : 0;
		utype hiResult = uHi + carry;
		
		hi = hiResult;
		lo = loResult;
	}

	////////////////////////////////////////////////////////////////////////////

}

#endif

