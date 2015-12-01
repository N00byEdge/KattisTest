#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <cmath>
#include <map>

#ifdef WINDOWS

#include <windows.h>

#else

#include <unistd.h>
#include <limits.h>

#endif

using namespace std;

stringstream ss;
string id;
string kattisDir;

#ifdef WINDOWS

string pathSeparator = "\\";

#else

string pathSeparator = "/";

#endif

void exec( const char* s ){

	#ifdef WINDOWS

	WinExec( s, SW_HIDE );

	#else

	system ( s );

	#endif
	
}

#ifndef WINDOWS
string getSelfpath ( ) {

	char buff [ PATH_MAX ];
	ssize_t len = ::readlink( "/proc/self/exe", buff, sizeof ( buff ) - 1 );
	if ( len != -1 ) {
		buff [ len ] = '\0';
		return string ( buff );
	}
}
#endif

bool setKattisDir ( ) {

	#ifdef WINDOWS

	kattisDir = getenv ( "kattis" );
	if ( kattisDir == "" ) return false;
	return true;

	#else

	//kattisDir = getSelfpath ( );
	kattisDir = ".";
	return true;

	#endif
}

void compile () {

	#ifdef DEBUGGING

	cout << "I'll go ahead and compile that for you.\n";

	#endif

	ss
	<< "g++ "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< id
	<< ".cpp -o "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< id

	#ifdef WINDOWS

	<< ".exe"

	#else

	<< ".out"

	#endif

	<< " -O2 -std=c++11 -Wall";

	#ifdef DEBUGGING

	cout << "ss constructed.\n";
	
	#endif

	system ( ss.str().c_str() );
	ss.str(""); 

	#ifdef DEBUGGING

	cout << "Sent compilation command.\n";

	#endif

}

bool compilationSucceeded ( ){

	ss
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< id

	#ifdef WINDOWS

	<< ".exe";

	#else

	<< ".out";

	#endif

	ifstream i ( ss.str( ) );
	ss.str ("");

	if ( !i.is_open() ){
		return false;
	}

	i.close();
	return true;

}

void testWithSampleData ( ) {

	bool running = true;
	int nTests = 1, nSucceededTests = 0;

	while ( running ) {

		vector < string > ofile (0);
		vector < string > tofile (0);

		ss
		<< kattisDir
		<< pathSeparator
		<< id
		<< pathSeparator
		<< id
		<< nTests
		<< ".o";

		ifstream o ( ss.str() );
		string outFile = ss.str ( );
		string inFile = ss.str ( );
		ss.str("");
		inFile [ inFile.size ( ) - 1 ] = 'i';

		ss
		<< kattisDir
		<< pathSeparator
		<< id
		<< pathSeparator
		<< "output";

		string tempOutputFile = ss.str();
		ss.str("");

		if ( o.is_open() ){

			#ifdef DEBUGGING

			cout << "Found output file #" << nTests << "." << endl;

			#endif

			ss
			<< kattisDir
			<< pathSeparator
			<< id
			<< pathSeparator
			<< id

			#ifdef WINDOWS

			<< ".exe"

			#else

			<< ".out"

			#endif

			<< " < "
			<< inFile
			<< " > "
			<< tempOutputFile;

			system ( ss.str().c_str() );
			ss.str ("");

			string line;

			while ( getline ( o, line ) ) {
				ofile.push_back ( line );
			}	

			o.close();

			ifstream o2 ( tempOutputFile );
			if( o2.is_open ( ) ) {
				while ( getline ( o2, line ) ) {
					tofile.push_back ( line );
				}

				o2.close ( );

			} else
				cout << "Unable to open temporary output file #" << nTests << ".\n";

			bool discrepancy = false;
			if ( ofile.size ( ) == tofile.size ( ) ) {
				for ( int i = 0; i < ofile.size(); i++ ) {
					if( ofile [ i ] != tofile [ i ] ) {
						discrepancy = true;
						break;
					}
				}
			} else {
				discrepancy = true;
			}

			if ( discrepancy ) {
				cout << "\n---------------\n\n";
				cout << "Your program failed the sample test #" << nTests << ".\n";
				cout << "Your program output the following (" << tofile.size( ) << " lines):\n";
				cout << endl;
				for ( int i = 0; i < tofile.size ( ); i ++ )
					cout << "\t" << tofile [ i ] << endl;
				cout << "While it should have output the following (" << ofile.size( ) << " lines):\n";
				cout << endl;
				for ( int i = 0; i < ofile.size ( ); i ++ )
					cout << "\t" << ofile [ i ] << endl;
				nTests ++;
			} else {
				cout << "Test #" << nTests << " succeeded!\n";
				nTests ++;
				nSucceededTests ++;
			}

			ss

			#ifdef WINDOWS

			<< "del "

			#else

			<< "rm "

			#endif

			<< kattisDir
			<< pathSeparator
			<< id
			<< pathSeparator
			<< "output";

			system ( ss.str().c_str() );
			ss.str("");


		} else {
			nTests --;
			cout << "\n--- SUMMARY ---\n\n";
			if ( nTests == nSucceededTests && nTests != 0 ) {
				cout << "Your program succeeded in all " << nTests << " tests.\n";
				running = false;
			} else {
				cout << "Your program succeeded in " << nSucceededTests << " out of " << nTests << " tests.\n";
				running = false;
			}
			cout << "\n---------------\n";
		}
	}
}

void clean ( ) {

	ss

	#ifdef WINDOWS

	<< "del "

	#else

	<< "rm "

	#endif

	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< id

	#ifdef WINDOWS

	<< ".exe";

	#else

	<< ".out";

	#endif

	system ( ss.str().c_str() );
	ss.str(""); 

}

int main ( int argc, const char * argv [ 	] ){

	vector <string> args ( argv, argv + argc );

	if ( args.size() < 2){
		cout << "Please supply a problem id.\n";
		return 0;
	}

	id = args[1];

	if ( id [ id.size ( ) - 1 ] == '/' )
		id.erase ( id.size ( ) - 1 );

	setKattisDir ( );

	cout << "\nCompiling your code...\n";

	compile ( );

	if ( compilationSucceeded ( ) )
		cout << "Compilation finished. Running tests...\n";
	else {
		cout << "Compilation failed. Aborting.\n";
		return 1;
	}

	testWithSampleData ( );

	clean ( );

	return 0;
}