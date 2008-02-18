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

  File:		YUIComponent.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <string.h>
#include <stdio.h>
#define y2log_component "ui"
#include <ycp/y2log.h>
#include <ycp/YCPValue.h>
#include <ycp/YCPVoid.h>

#include "YUIComponent.h"
#include "YUINamespace.h"
#include "YUI.h"
#include "YUILoader.h"

#include "YMacro.h"
#include "YCPMacroRecorder.h"
#include "YCPMacroPlayer.h"

#define YUILogComponent "ui"
#include "YUILog.h"


static void
yui_y2logger( YUILogLevel_t	logLevel,
	      const char *	logComponent,
	      const char *	sourceFileName,
	      int 	 	sourceLineNo,
	      const char * 	sourceFunctionName,
	      const char *	message );




// Most class variables are static so they can be accessed from static methods.

YUI *		YUIComponent::_ui		= 0;
YUIComponent *	YUIComponent::_uiComponent	= 0;


YUIComponent::YUIComponent( const string & name )
    : _requestedUIName( name )
    , _withThreads( false )
    , _macroFile( 0 )
    , _haveServerOptions( false )
    , _namespace( 0 )
    , _callbackComponent( 0 )
{
    if ( _uiComponent )
    {
	y2error( "UI component already created" );
	return;
    }

    _uiComponent = this;
}


YUIComponent::~YUIComponent()
{
    if ( _ui )
    {
	_ui->shutdownThreads(); // cannot be called in the UI's destructor
	delete _ui;
    }
}


void
YUIComponent::setRequestedUIName( const string & name )
{
    if ( ! name.empty() )
    {
	y2debug( "Setting requested UI name to \"%s\"", name.c_str() );
	_requestedUIName = name;
    }
}


YUIComponent *
YUIComponent::uiComponent()
{
    return _uiComponent;
}


Y2Namespace *
YUIComponent::import( const char* name )
{
    y2debug ("%s trying to import %s", this->name().c_str(), name);
    
    if ( strcmp (name, "UI") == 0)
    {
        if (_namespace == NULL)
        {
            _namespace = new YUINamespace(this);
	    y2debug ("Namespace created %p", _namespace);
        }

        return _namespace;
    }

    return NULL;
}


void
YUIComponent::createUI()
{
    if ( ! _haveServerOptions )
    {
	y2error( "createUI() called before setServerOptions() !" );
	return;
    }

    if ( _ui )
    {
	y2error( "Can't create multiple UIs!" );
	return;
    }

    y2debug( "Creating UI" );
    
    YUILog::setLoggerFunction( yui_y2logger );

    if ( _requestedUIName.empty() )
	YUILoader::loadUI( _withThreads );
    else
	YUILoader::loadPlugin( _requestedUIName, _withThreads );

    _ui = YUI::ui();
    
    YMacro::setRecorder( new YCPMacroRecorder() );
    YMacro::setPlayer  ( new YCPMacroPlayer()   );
}


YCPValue
YUIComponent::callBuiltin( void * function, int fn_argc, YCPValue fn_argv[] )
{
    if ( ! _ui )
    {
	y2debug( "Late creation of UI instance" );
	createUI();
    }

    if ( _ui )
    {
#if 0
	// obsolete? The corresponding UI function always returns YCPull()
	return _ui->callBuiltin( function, fn_argc, fn_argv );
#else
	return YCPNull();
#endif
    }
    else
	return YCPVoid();
}


void
YUIComponent::setServerOptions( int argc, char **argv )
{
    // Evaluate some command line arguments
	
    _withThreads = true;
    _macroFile	  = 0;

    for ( int i=0; i < argc; i++ )
    {
	if ( strcmp( argv[i], "--nothreads" ) == 0 )
	{
	    _withThreads = false;
	}
	else if ( strcmp( argv[i], "--macro" ) == 0 )
	{
	    if ( i+1 >= argc )
	    {
		y2error( "Missing arg for '--macro'" );
		fprintf( stderr, "y2base: Missing argument for --macro\n" );
		exit( 1 );
	    }
	    else
	    {
		_macroFile = argv[++i];
		y2milestone( "Playing macro '%s' from command line",
			     _macroFile ? _macroFile : "<NULL>" );
	    }
	}
    }

    _haveServerOptions = true;

    // For now, the values are only saved. The UI gets instantiated upon the
    // first call to YUIComponent::ui() which will usually happen when the
    // first UI builtin is due to be executed via the call handler (see
    // YUI_bindings.cc).
}


void
YUIComponent::result( const YCPValue & /*result*/ )
{
    if ( _ui )
    {
	_ui->shutdownThreads(); // cannot be called in the UI's destructor
	delete _ui;
	_ui = 0;
    }
}




static void
yui_y2logger( YUILogLevel_t	logLevel,
	      const char *	logComponent,
	      const char *	sourceFileName,
	      int 	 	sourceLineNo,
	      const char * 	sourceFunctionName,
	      const char *	message )
{
    loglevel_t y2logLevel = LOG_DEBUG;
    
    switch ( logLevel )
    {
	case YUI_LOG_DEBUG:	y2logLevel = LOG_DEBUG;		break;
	case YUI_LOG_MILESTONE:	y2logLevel = LOG_MILESTONE;	break;
	case YUI_LOG_WARNING:	y2logLevel = LOG_WARNING;	break;
	case YUI_LOG_ERROR:	y2logLevel = LOG_ERROR;		break;
    }

    if ( ! logComponent )
	logComponent = "??";

    if ( ! sourceFileName )
	sourceFileName = "??";

    if ( ! sourceFunctionName )
	sourceFunctionName = "??";

    if ( ! message )
	message = "";

    y2_logger( y2logLevel, logComponent,
	       sourceFileName, sourceLineNo, sourceFunctionName,
	       "%s", message );
}



// EOF
