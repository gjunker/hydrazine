/*! \file string.cpp
	\date Friday February 13, 2009
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief Function sources for common C string manipulations
*/

#ifndef STRING_CPP_INCLUDED
#define STRING_CPP_INCLUDED

#include <hydrazine/implementation/string.h>
#include <hydrazine/implementation/debug.h>

namespace hydrazine
{

	void strlcpy( char* dest, const char* src, unsigned int length )
	{
		const char* end = src + ( length - 1 );
		for( ; src != end; ++src, ++dest )
		{
			*dest = *src;	
			if( *src == '\0' )
			{
				return;
			}
		}
		*dest = '\0';
	}
	
	std::string format( const std::string& input, 
		const std::string& firstPrefix, const std::string& prefix, 
		unsigned int width )
	{
		std::string word;
		std::string result = firstPrefix;
		unsigned int currentIndex = firstPrefix.size();
	
		for( std::string::const_iterator fi = input.begin(); 
			fi != input.end(); ++fi )
		{
			if( *fi == ' ' || *fi == '\t' || *fi == '\n' 
				|| *fi == '\r' || *fi == '\f' )
			{
				if( currentIndex + word.size() > width )
				{
					currentIndex = prefix.size();
					result += "\n";
					result += prefix;
				}
				
				if( !word.empty() )
				{
					result += word + " ";
					++currentIndex;
					word.clear();
				}
			}
			else
			{
				word.push_back( *fi );
				++currentIndex;
			}
		}

		if( currentIndex + word.size() > width )
		{
			result += "\n";
		}
		
		result += word + "\n";
		return result;
	}

	long long unsigned int binaryToUint( const std::string& string )
	{
		long long unsigned int result = 0;
		assert( string.size() > 2 );
	
		std::string::const_iterator ci = string.begin();
		assert( *ci == '0' );
		++ci;
		assert( *ci == 'b' );
		++ci;
	
		long long unsigned int mask = 1;
	
		for( ; ci != string.end(); ++ci )
		{
			assert( *ci == '0' || *ci == '1' );
	
			result |= mask & ( *ci == '1' );
			mask <<= 1;
		}
	
		return result;
	}

	std::string toGraphVizParsableLabel( const std::string& string )
	{
		std::string result;
		for( std::string::const_iterator fi = string.begin();
			fi != string.end(); ++fi ) 
		{
			if( *fi == '{' ) 
			{
				result.push_back( '[' );
			}
			else if( *fi == '}' ) 
			{
				result.push_back( ']' );
			}
			else 
			{
				result.push_back( *fi );
			}	
		}
		return std::move(result);
	}
	
	std::string addLineNumbers( const std::string& string )
	{
		unsigned int line = 1;
		std::stringstream result;
		
		result << line++ << " ";
		
		for( std::string::const_iterator s = string.begin(); 
			s != string.end(); ++s )
		{
			if( *s == '\n' )
			{
				result << "\n" << line++ << " ";
			}
			else
			{
				result << *s;
			}
		}
		return std::move( result.str() );
	}

}

#endif

