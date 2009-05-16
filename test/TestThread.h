#ifndef TEST_THREAD_H_INCLUDED
#define TEST_THREAD_H_INCLUDED

#include <interface/Thread.h>
#include <implementation/ArgumentParser.h>
#include <interface/Test.h>
#include <iostream>
#include <map>

namespace test
{
	typedef std::map< unsigned int, unsigned int > Map;
		
	class RingThread : public hydrazine::Thread
	{
		protected:
			void execute();
			
		public:
			Id source;
			Id destination;
			unsigned int loops;
	};
	
	RingThread* startRing( unsigned int threads, unsigned int loops );
	
	class TestThread : public Test
	{
		private:
			bool testMessage( );
			bool doTest( );
			
		public:
			TestThread();			
			unsigned int threads;
			unsigned int loops;
	
	};

}

int main( int argc, char* argv[] );

#endif

