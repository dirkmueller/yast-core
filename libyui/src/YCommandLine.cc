/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:		YCommandLine.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <stdlib.h>	// malloc()
#include <string.h>	// strdup()
#include <unistd.h>	// getpid()

#include <vector>
#include <sstream>
#include <fstream>

#include "YCommandLine.h"
#include "YUIException.h"

#define YUILogComponent "ui"
#include "YUILog.h"

using std::vector;
using std::ifstream;


struct YCommandLinePrivate
{
    vector<string> args;
};





YCommandLine::YCommandLine()
    : priv( new YCommandLinePrivate() )
{
    YUI_CHECK_NEW( priv );

    std::ostringstream fileName;
    fileName << "/proc/" << getpid() << "/cmdline";

    ifstream cmdline( fileName.str().c_str(), ifstream::in | ifstream::binary );

    while ( cmdline.good() )
    {
	string arg;
	getline( cmdline, arg, '\0' );

	if ( ! arg.empty() )
	{
	    yuiDebug() << "Arg #" << priv->args.size()
		       << ": \"" << arg << "\"" << endl;

	    priv->args.push_back( arg );
	}
    }
}


YCommandLine::~YCommandLine()
{

}


int
YCommandLine::argc() const
{
    return priv->args.size();
}


char **
YCommandLine::argv() const
{
    char ** argArray = (char **) ( malloc( argc() * sizeof( char * ) ) );

    if ( argArray )
    {
	for ( int i=0; i < argc(); i++ )
	{
	    argArray[ i ] = strdup( priv->args[i].c_str() );
	}
    }

    return argArray;
}


void
YCommandLine::add( const string & arg )
{
    priv->args.push_back( arg );
}


string
YCommandLine::arg( int index ) const
{
    YUI_CHECK_INDEX( index, 0, (int) priv->args.size() );

    return priv->args[ index ];
}


void
YCommandLine::remove( int index )
{
    YUI_CHECK_INDEX( index, 0, (int) priv->args.size() );

}


void
YCommandLine::replace( int index, const string & newArg )
{
    YUI_CHECK_INDEX( index, 0, (int) priv->args.size() );

    priv->args[ index ] = newArg;
}

