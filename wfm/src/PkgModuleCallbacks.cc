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

   File:	PkgModuleCallbacks.cc

   Author:	Klaus Kaempf <kkaempf@suse.de>
   Maintainer:  Klaus Kaempf <kkaempf@suse.de>

   Purpose:	Implement callbacks from Y2PM to UI/WFM.

/-*/

#include <iostream>

#include <y2util/Y2SLog.h>
#include <y2util/stringutil.h>

#include <PkgModuleFunctions.h>
#include <PkgModuleCallbacks.h>
#include <PkgModuleCallbacks.YCP.h> // PkgModuleFunctions::CallbackHandler::YCPCallbacks

#include <y2pm/Y2PMCallbacks.h>

#include <y2pm/PMPackage.h>
#include <y2pm/PMYouPatchManager.h>
#include <y2pm/InstYou.h>
#include <y2pm/InstTarget.h>

///////////////////////////////////////////////////////////////////
namespace Y2PMRecipients {
///////////////////////////////////////////////////////////////////

  typedef PkgModuleFunctions::CallbackHandler::YCPCallbacks YCPCallbacks;

  ///////////////////////////////////////////////////////////////////
  // Data excange. Shared between Recipients, inherited by Y2PMReceive.
  ///////////////////////////////////////////////////////////////////
  struct RecipientCtl {
    const YCPCallbacks & _ycpcb;
    PkgModuleFunctions::CallbackHandler::ReferencesNeeded _referencesNeeded;
    bool _inCommitInstall;
    bool _inCommitRemove;
    public:
      RecipientCtl( const YCPCallbacks & ycpcb_r,
		    PkgModuleFunctions::CallbackHandler::ReferencesNeeded & args_r )
	: _ycpcb( ycpcb_r )
	, _referencesNeeded( args_r )
	, _inCommitInstall( false )
	, _inCommitRemove( false )
      {}
      virtual ~RecipientCtl() {}
  };

  ///////////////////////////////////////////////////////////////////
  // Base class common to Recipients. Provides RecipientCtl and inherits
  // YCPCallbacks::Send(see comment in PkgModuleCallbacks.YCP.h).
  ///////////////////////////////////////////////////////////////////
  struct Recipient : public YCPCallbacks::Send {
    RecipientCtl & _control; // shared beween Recipients.
    public:
      Recipient( RecipientCtl & control_r )
        : Send( control_r._ycpcb )
        , _control( control_r )
      {}
      virtual ~Recipient() {}
  };

  ///////////////////////////////////////////////////////////////////
  // RpmDbCallbacks::ConvertDbCallback
  ///////////////////////////////////////////////////////////////////
  struct ConvertDbReceive : public Recipient, public RpmDbCallbacks::ConvertDbCallback
  {
    ConvertDbReceive( RecipientCtl & construct_r ) : Recipient( construct_r ) {}

    ProgressCounter _pc;
    unsigned _lastFailed;
    unsigned _lastIgnored;
    unsigned _lastAlreadyInV4;

    virtual void reportbegin() {
      _pc.reset();
      _lastFailed = _lastIgnored = _lastAlreadyInV4 = 0;
    }
    virtual void reportend()   {
    }
    virtual void start( const Pathname & v3db ) {
      CB callback( ycpcb( YCPCallbacks::CB_StartConvertDb ) );
      if ( callback._set ) {
	callback.addStr( v3db );
	callback.evaluate();
      }
    }
    virtual void progress( const ProgressData & prg,
			   unsigned failed, unsigned ignored, unsigned alreadyInV4 ) {
      CB callback( ycpcb( YCPCallbacks::CB_ProgressConvertDb ) );
      if ( callback._set ) {
	_pc = prg;
	if ( _pc.updateIfNewPercent( 5 )
	     || failed != _lastFailed
	     || ignored != _lastIgnored
	     || alreadyInV4 != _lastAlreadyInV4 ) {
	  _lastFailed      = failed;
	  _lastIgnored     = ignored;
	  _lastAlreadyInV4 = alreadyInV4;
	  // report changed values
	  callback.addInt( _pc.percent() );
	  callback.addInt( _pc.max() );
	  callback.addInt( failed );
	  callback.addInt( ignored );
	  callback.addInt( alreadyInV4 );
	  callback.evaluate();
	}
      }
    }
  virtual void dbInV4( const std::string & pkg ) {
    static string type( "Nindb" );
    CB callback( ycpcb( YCPCallbacks::CB_NotifyConvertDb ) );
    if ( callback._set ) {
      callback.addStr( type );
      callback.addInt( -1 );
      callback.addStr( pkg );
      callback.evaluate();
    }
  }
  virtual CBSuggest dbReadError( int offset ) {
    static string type( "Eread" );
    CB callback( ycpcb( YCPCallbacks::CB_NotifyConvertDb ) );
    if ( callback._set ) {
      callback.addStr( type );
      callback.addInt( offset );
      callback.addStr( string() );
      return CBSuggest( callback.evaluateStr() );
    }
    return ConvertDbCallback::dbReadError( offset );
  }
  virtual void dbWriteError( const std::string & pkg ) {
    static string type( "Ewrite" );
    CB callback( ycpcb( YCPCallbacks::CB_NotifyConvertDb ) );
    if ( callback._set ) {
      callback.addStr( type );
      callback.addInt( -1 );
      callback.addStr( pkg );
      callback.evaluate();
    }
  }
    virtual void stop( PMError error ) {
      CB callback( ycpcb( YCPCallbacks::CB_StopConvertDb ) );
      if ( callback._set ) {
	callback.addInt( error );
	callback.addStr( error.errstr() );
	callback.evaluate();
      }
    }
  };

  ///////////////////////////////////////////////////////////////////
  // RpmDbCallbacks::RebuildDbCallback
  ///////////////////////////////////////////////////////////////////
  struct RebuildDbReceive : public Recipient, public RpmDbCallbacks::RebuildDbCallback
  {
    RebuildDbReceive( RecipientCtl & construct_r ) : Recipient( construct_r ) {}

    ProgressCounter _pc;

    virtual void reportbegin() {
      _pc.reset();
    }
    virtual void reportend()   {
    }
    virtual void start() {
    }
    virtual void progress( const ProgressData & prg ) {
      CB callback( ycpcb( YCPCallbacks::CB_ProgressRebuildDB ) );
      if ( callback._set ) {
	_pc = prg;
	if ( _pc.updateIfNewPercent( 5 ) ) {
	  // report changed values
	  callback.addInt( _pc.percent() );
	  callback.evaluate();
	}
      }
    }
    virtual void stop( PMError error ) {
    }
  };

  ///////////////////////////////////////////////////////////////////
  // RpmDbCallbacks::InstallPkgCallback
  ///////////////////////////////////////////////////////////////////
  struct InstallPkgReceive : public Recipient, public RpmDbCallbacks::InstallPkgCallback
  {
    InstallPkgReceive( RecipientCtl & construct_r ) : Recipient( construct_r ) {}

    ProgressCounter _pc;

    virtual void reportbegin() {
      _pc.reset();
    }
    virtual void reportend()   {
    }
    virtual void start( const Pathname & filename ) {
      if ( _control._inCommitInstall )
	return; // CommitInstall sends CB_StartPackage
      CB callback( ycpcb( YCPCallbacks::CB_StartPackage ) );
      if ( callback._set ) {
	callback.addStr( filename );
	callback.addStr( string() );
	callback.addInt( -1 );
	callback.addBool( /*is_delete*/false );
	callback.evaluateBool(); // return value ignored by RpmDb
      }
    }
    virtual void progress( const ProgressData & prg ) {
      CB callback( ycpcb( YCPCallbacks::CB_ProgressPackage ) );
      if ( callback._set ) {
	_pc = prg;
	if ( _pc.updateIfNewPercent( 5 ) ) {
	  // report changed values
	  callback.addInt( _pc.percent() );
	  callback.evaluate();
	}
      }
    }
    virtual void stop( PMError error ) {
      if ( _control._inCommitInstall )
	return; // CommitInstall sends CB_StartPackage
      CB callback( ycpcb( YCPCallbacks::CB_DonePackage ) );
      if ( callback._set ) {
	callback.addInt( error );
	callback.addStr( error.errstr() );
	callback.evaluateStr(); // return value ignored by RpmDb
      }
    }
  };

  ///////////////////////////////////////////////////////////////////
  // RpmDbCallbacks::RemovePkgCallback
  ///////////////////////////////////////////////////////////////////
  struct RemovePkgReceive : public Recipient, public RpmDbCallbacks::RemovePkgCallback
  {
    RemovePkgReceive( RecipientCtl & construct_r ) : Recipient( construct_r ) {}

    ProgressCounter _pc;

    virtual void reportbegin() {
      _pc.reset();
    }
    virtual void reportend()   {
    }
    virtual void start( const std::string & label ) {
      if ( _control._inCommitRemove )
	return; // CommitRemove sends CB_StartPackage
      CB callback( ycpcb( YCPCallbacks::CB_StartPackage ) );
      if ( callback._set ) {
	callback.addStr( label );
	callback.addStr( string() );
	callback.addInt( -1 );
	callback.addBool( /*is_delete*/true );
	callback.evaluateBool(); // return value ignored by RpmDb
      }
    }
    virtual void progress( const ProgressData & prg ) {
      CB callback( ycpcb( YCPCallbacks::CB_ProgressPackage ) );
      if ( callback._set ) {
	_pc = prg;
	if ( _pc.updateIfNewPercent( 5 ) ) {
	  // report changed values
	  callback.addInt( _pc.percent() );
	  callback.evaluate();
	}
      }
    }
    virtual void stop( PMError error ) {
      if ( _control._inCommitRemove )
	return; // CommitRemove sends CB_StartPackage
      CB callback( ycpcb( YCPCallbacks::CB_DonePackage ) );
      if ( callback._set ) {
	callback.addInt( error );
	callback.addStr( error.errstr() );
	callback.evaluateStr(); // return value ignored by RpmDb
      }
    }
  };

  ///////////////////////////////////////////////////////////////////
  // InstTargetCallbacks::ScriptExecCallback
  ///////////////////////////////////////////////////////////////////
  //
#warning InstTargetCallbacks::ScriptExecCallback is actually YOU specific
  // Actually a YouScriptProgress and the behaviour of precentage
  // report is strange. Space for improvement (e.g. error reporting).
  // Maybe provide a common YCP callback for ScriptExec and let
  // YOU redirect it to YouScriptProgress, if this kind of report
  // is desired.
  //
  struct ScriptExecReceive : public Recipient, public InstTargetCallbacks::ScriptExecCallback
  {
    ScriptExecReceive( RecipientCtl & construct_r ) : Recipient( construct_r ) {}

    virtual void reportbegin() {
    }
    virtual void reportend()   {
    }
    virtual void start( const Pathname & pkpath ) {
      CB callback( ycpcb( YCPCallbacks::CB_YouScriptProgress ) );
      if ( callback._set ) {
	callback.addInt( 0 );
	callback.evaluate();
      }
    }
    /**
     * Execution time is unpredictable. ProgressData range will be set to
     * [0:0]. Aprox. every half second progress is reported with incrementing
     * counter value. If <CODE>false</CODE> is returned, execution is canceled.
     **/
    virtual bool progress( const ProgressData & prg ) {
      CB callback( ycpcb( YCPCallbacks::CB_YouScriptProgress ) );
      if ( callback._set ) {
	callback.addInt( -1 );
	return callback.evaluateBool();
      }
      return ScriptExecCallback::progress( prg ); // return default implementation
    }
    virtual void stop( PMError error ) {
      CB callback( ycpcb( YCPCallbacks::CB_YouScriptProgress ) );
      if ( callback._set ) {
	callback.addInt( 100 );
	callback.evaluate();
      }
    }
  };

  ///////////////////////////////////////////////////////////////////
  // Y2PMCallbacks::CommitCallback
  ///////////////////////////////////////////////////////////////////
  struct CommitReceive : public Recipient, public Y2PMCallbacks::CommitCallback
  {
    CommitReceive( RecipientCtl & construct_r ) : Recipient( construct_r ) {}

    virtual void reportbegin() {
    }
    virtual void reportend()   {
    }
    virtual void advanceToMedia( constInstSrcPtr srcptr, unsigned mediaNr ) {
      CB callback( ycpcb( YCPCallbacks::CB_SourceChange ) );
      if ( callback._set ) {

	// Translate the (internal) InstSrcPtr to an (external) vector index

	const vector<InstSrcManager::ISrcId> & sources( _control._referencesNeeded._sources );
	const InstSrcManager::ISrcIdList &     inst_order( _control._referencesNeeded._inst_order );

	int sourceid = -1; // search vector index for the source pointer

	if ( inst_order.empty() ) {
	  for ( unsigned i = 0; i < sources.size(); ++i ) {
	    if ( sources[i] == srcptr ) {
	      sourceid = i;
	      break;
	    }
	  }
	} else { // we have an install order, use this
	  sourceid = 0;
	  for ( InstSrcManager::ISrcIdList::const_iterator it = inst_order.begin();
		it != inst_order.end(); ++it ) {
	    if ( *it == srcptr )
	      break;
	    ++sourceid;
	  }
	}

	callback.addInt( sourceid );
	callback.addInt( mediaNr );
	callback.evaluate();
      }
    }
  };

  ///////////////////////////////////////////////////////////////////
  // Y2PMCallbacks::CommitProvideCallback
  ///////////////////////////////////////////////////////////////////
  struct CommitProvideReceive : public Recipient, public Y2PMCallbacks::CommitProvideCallback
  {
    CommitProvideReceive( RecipientCtl & construct_r ) : Recipient( construct_r ) {}

    string _name;
    FSize  _size;
    bool   _isRemote;

    virtual void reportbegin() {
    }
    virtual void reportend()   {
    }
    virtual void start( constPMPackagePtr pkg, bool sourcepkg ) {
      // remember values to send on attempt
      _isRemote = pkg->isRemote();
      if ( sourcepkg ) {
	_name = pkg->nameEd() + ".src";
	_size = pkg->sourcesize(); // download size
      } else {
	_name = pkg->nameEdArch();
	_size = pkg->archivesize(); // download size
      }
    }
    virtual CBSuggest attempt( unsigned cnt ) {
      if ( _isRemote ) {
	CB callback( ycpcb( YCPCallbacks::CB_StartProvide ) );
	if ( callback._set ) {
	  callback.addStr( _name );
	  callback.addInt( _size );
	  callback.addBool( _isRemote );
	  callback.evaluate(); // CB_StartProvide is void
	}
      }
      return CommitProvideCallback::attempt( cnt ); // return default implementation
    }

#warning CB_ProgressProvide seems to be untriggered
    // Depends on what YCP expects. We could redirect a media download callback
    // if YCP does not alredy use it.

    virtual CBSuggest result( PMError error, const Pathname & localpath ) {
      if ( error || _isRemote ) {
	CB callback( ycpcb( YCPCallbacks::CB_DoneProvide ) );
	if ( callback._set ) {
	  callback.addInt( error );
	  callback.addStr( error.errstr() );
	  callback.addStr( localpath );
	  return CBSuggest( callback.evaluateStr() );
	}
      }
      return CommitProvideCallback::result( error, localpath ); // return default implementation
    }
    virtual void stop( PMError error, const Pathname & localpath ) {
    }
  };

  ///////////////////////////////////////////////////////////////////
  // Y2PMCallbacks::CommitInstallCallback
  ///////////////////////////////////////////////////////////////////
  struct CommitInstallReceive : public Recipient, public Y2PMCallbacks::CommitInstallCallback
  {
    CommitInstallReceive( RecipientCtl & construct_r ) : Recipient( construct_r ) {}

    string   _name;
    FSize    _size;
    string   _summary;

    virtual void reportbegin() {
      // let InstallPkgCallback send ProgressPackage only. Start/Done is sent here.
      _control._inCommitInstall = true;
    }
    virtual void reportend()   {
      _control._inCommitInstall = false;
    }
    virtual void start( constPMPackagePtr pkg, bool sourcepkg, const Pathname & path ) {
      // remember values to send on attempt
      if ( sourcepkg ) {
	_name = pkg->nameEd() + ".src";
      } else {
	_name = pkg->nameEdArch();
      }
      _size = pkg->size(); // content size
      _summary = pkg->summary();
    }
    virtual CBSuggest attempt( unsigned cnt ) {
      CB callback( ycpcb( YCPCallbacks::CB_StartPackage ) );
      if ( callback._set ) {
	callback.addStr( _name );
	callback.addStr( _summary );
	callback.addInt( _size );
	callback.addBool( /*is_delete*/false );
	bool res = callback.evaluateBool(); // CB_StartPackage is "true" to continue, "false" to cancel
	return CBSuggest( res ? CBSuggest::PROCEED : CBSuggest::CANCEL );
      }
      return CommitInstallCallback::attempt( cnt ); // return default implementation
    }
    virtual CBSuggest result( PMError error ) {
      CB callback( ycpcb( YCPCallbacks::CB_DonePackage ) );
      if ( callback._set ) {
	callback.addInt( error );
	callback.addStr( error.errstr() );
	return CBSuggest( callback.evaluateStr() );
      }
      return CommitInstallCallback::result( error ); // return default implementation
    }
    virtual void stop( PMError error ) {
    }
  };

  ///////////////////////////////////////////////////////////////////
  // Y2PMCallbacks::CommitRemoveCallback
  ///////////////////////////////////////////////////////////////////
  struct CommitRemoveReceive : public Recipient, public Y2PMCallbacks::CommitRemoveCallback
  {
    CommitRemoveReceive( RecipientCtl & construct_r ) : Recipient( construct_r ) {}

    string   _name;
    FSize    _size;
    string   _summary;

    virtual void reportbegin() {
      // let RemovePkgCallback send ProgressPackage only. Start/Done is sent here.
      _control._inCommitRemove = true;
    }
    virtual void reportend()   {
      _control._inCommitRemove = false;
    }
    virtual void start( constPMPackagePtr pkg ) {
      // remember values to send on attempt
      _name = pkg->nameEdArch();
      _size = pkg->size(); // content size
      _summary = pkg->summary();
    }
    virtual CBSuggest attempt( unsigned cnt ) {
      CB callback( ycpcb( YCPCallbacks::CB_StartPackage ) );
      if ( callback._set ) {
	callback.addStr( _name );
	callback.addStr( _summary );
	callback.addInt( _size );
	callback.addBool( /*is_delete*/true );
	bool res = callback.evaluateBool(); // CB_StartPackage is "true" to continue, "false" to cancel
	return CBSuggest( res ? CBSuggest::PROCEED : CBSuggest::CANCEL );
      }
      return CommitRemoveCallback::attempt( cnt ); // return default implementation
    }
    virtual CBSuggest result( PMError error ) {
      CB callback( ycpcb( YCPCallbacks::CB_DonePackage ) );
      if ( callback._set ) {
	callback.addInt( error );
	callback.addStr( error.errstr() );
	return CBSuggest( callback.evaluateStr() );
      }
      return CommitRemoveCallback::result( error ); // return default implementation
    }
    virtual void stop( PMError error ) {
    }
  };

  ///////////////////////////////////////////////////////////////////
  // InstSrcManagerCallbacks::MediaChangeCallback
  ///////////////////////////////////////////////////////////////////
  struct MediaChangeReceive : public Recipient, public InstSrcManagerCallbacks::MediaChangeCallback
  {
    MediaChangeReceive( RecipientCtl & construct_r ) : Recipient( construct_r ) {}

    /**
     * Whether a recipient is set.
     **/
    virtual bool isSet() {
      return _control._ycpcb.isSet( YCPCallbacks::CB_MediaChange );
    }
    /** media change callback
     * - product name (i.e "SuSE Linux Professional 8.1")
     * - product error
     * - media type (0=CD, 1=DVD, ...)
     * - expected media number
     * - found media number (0 == none)
     * return "": retry, "S": skip, "C" cancel, "I" ignore, "E" eject, else new url
     **/
    virtual string changeMedia( const string & error,
				const string & url,
				const string & product,
				int expected,
				int current ) {
      CB callback( ycpcb( YCPCallbacks::CB_MediaChange ) );
      if ( callback._set ) {
	callback.addStr( error );
	callback.addStr( url );
	callback.addStr( product );
	callback.addInt( expected );
	callback.addInt( current );
	return callback.evaluateStr();
      }
      return MediaChangeCallback::changeMedia( error, url, product, expected, current );
    }
  };

  ///////////////////////////////////////////////////////////////////
  // InstYou::Callbacks
  ///////////////////////////////////////////////////////////////////
  //
  // YOU is still special. Does not Trigger via Report.
  //
  struct YouReceive : public Recipient, public InstYou::Callbacks
  {
    YouReceive( RecipientCtl & construct_r ) : Recipient( construct_r ) {}

    virtual bool progress( int percent ) {
      D__ << "you progress: " << percent << endl;
      CB callback( ycpcb( YCPCallbacks::CB_YouProgress ) );
      if ( callback._set ) {
	callback.addInt( percent );
	return callback.evaluateBool();
      }
      return false;
    }
    virtual bool patchProgress( int percent, const string & pkg ) {
      D__ << "you patch progress: " << percent << endl;
      CB callback( ycpcb( YCPCallbacks::CB_YouPatchProgress ) );
      if ( callback._set ) {
	callback.addInt( percent );
	callback.addStr( pkg );
	return callback.evaluateBool();
      }
      return false;
    }
    virtual bool executeYcpScript( const string & script ) {
      D__ << "you execute YCP script" << endl;
      CB callback( ycpcb( YCPCallbacks::CB_YouExecuteYcpScript ) );
      if ( callback._set ) {
	callback.addStr( script );
	return callback.evaluateBool();
      }
      return false;
    }
  };
  ///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
}; // namespace Y2PMRecipients
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : PkgModuleFunctions::CallbackHandler::Y2PMReceive
/**
 * @short Manages the Y2PMCallbacks we receive.
 *
 **/
class PkgModuleFunctions::CallbackHandler::Y2PMReceive : public Y2PMRecipients::RecipientCtl {

  private:

    // RpmDbCallbacks
    Y2PMRecipients::ConvertDbReceive  _convertDbReceive;
    Y2PMRecipients::RebuildDbReceive  _rebuildDbReceive;
    Y2PMRecipients::InstallPkgReceive _installPkgReceive;
    Y2PMRecipients::RemovePkgReceive  _removePkgReceive;

    // InstTargetCallbacks
    Y2PMRecipients::ScriptExecReceive _scriptExecReceive;

    // Y2PMCallbacks
    Y2PMRecipients::CommitReceive        _commitReceive;
    Y2PMRecipients::CommitProvideReceive _commitProvideReceive;
    Y2PMRecipients::CommitInstallReceive _commitInstallReceive;
    Y2PMRecipients::CommitRemoveReceive  _commitRemoveReceive;

    // InstSrcManagerCallbacks
    Y2PMRecipients::MediaChangeReceive _mediaChangeReceive;

    // YouCallbacks
    Y2PMRecipients::YouReceive _youReceive;

  protected:

    // overloaded RecipientCtl calls

  public:

    Y2PMReceive( const YCPCallbacks & ycpcb_r, ReferencesNeeded & args_r )
      : RecipientCtl( ycpcb_r, args_r )
      , _convertDbReceive( *this )
      , _rebuildDbReceive( *this )
      , _installPkgReceive( *this )
      , _removePkgReceive( *this )
      , _scriptExecReceive( *this )
      , _commitReceive( *this )
      , _commitProvideReceive( *this )
      , _commitInstallReceive( *this )
      , _commitRemoveReceive( *this )
      , _mediaChangeReceive( *this )
      , _youReceive( *this )
    {
      RpmDbCallbacks::convertDbReport.redirectTo( _convertDbReceive );
      RpmDbCallbacks::rebuildDbReport.redirectTo( _rebuildDbReceive );
      RpmDbCallbacks::installPkgReport.redirectTo( _installPkgReceive );
      RpmDbCallbacks::removePkgReport.redirectTo( _removePkgReceive );

      InstTargetCallbacks::scriptExecReport.redirectTo( _scriptExecReceive );

      Y2PMCallbacks::commitReport.redirectTo( _commitReceive );
      Y2PMCallbacks::commitProvideReport.redirectTo( _commitProvideReceive );
      Y2PMCallbacks::commitInstallReport.redirectTo( _commitInstallReceive );
      Y2PMCallbacks::commitRemoveReport.redirectTo( _commitRemoveReceive );

      InstSrcManagerCallbacks::mediaChangeReport.redirectTo( _mediaChangeReceive );

      // YOU is still special. Does not trigger via Report.
      InstYou::setCallbacks( &_youReceive );
    }

    virtual ~Y2PMReceive()
    {
      RpmDbCallbacks::convertDbReport.redirectTo( 0 );
      RpmDbCallbacks::rebuildDbReport.redirectTo( 0 );
      RpmDbCallbacks::installPkgReport.redirectTo( 0 );
      RpmDbCallbacks::removePkgReport.redirectTo( 0 );

      InstTargetCallbacks::scriptExecReport.redirectTo( 0 );

      Y2PMCallbacks::commitReport.redirectTo( 0 );
      Y2PMCallbacks::commitProvideReport.redirectTo( 0 );
      Y2PMCallbacks::commitInstallReport.redirectTo( 0 );
      Y2PMCallbacks::commitRemoveReport.redirectTo( 0 );

      InstSrcManagerCallbacks::mediaChangeReport.redirectTo( 0 );

      // YOU is still special. Does not Trigger via Report.
      InstYou::setCallbacks( 0 );
    }
  public:

};

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : PkgModuleFunctions::CallbackHandler
//
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : PkgModuleFunctions::CallbackHandler::CallbackHandler
//	METHOD TYPE : Constructor
//
PkgModuleFunctions::CallbackHandler::CallbackHandler( YCPInterpreter *const wfm_r,
						      ReferencesNeeded args_r )
    : _ycpCallbacks( *new YCPCallbacks( wfm_r ) )
    , _y2pmReceive( *new Y2PMReceive( _ycpCallbacks, args_r ) )
{
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : PkgModuleFunctions::CallbackHandler::~CallbackHandler
//	METHOD TYPE : Destructor
//
PkgModuleFunctions::CallbackHandler::~CallbackHandler()
{
  delete &_y2pmReceive;
  delete &_ycpCallbacks;
}

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : PkgModuleFunctions
//
//      Set YCPCallbacks.  _ycpCallbacks
//
///////////////////////////////////////////////////////////////////

#define SET_YCP_CB(E,A) _callbackHandler._ycpCallbacks.setYCPCallback( CallbackHandler::YCPCallbacks::E, A );

YCPValue PkgModuleFunctions::CallbackStartProvide( YCPList args ) {
  return SET_YCP_CB( CB_StartProvide, args );
}
YCPValue PkgModuleFunctions::CallbackProgressProvide( YCPList args ) {
  return SET_YCP_CB( CB_ProgressProvide, args );
}
YCPValue PkgModuleFunctions::CallbackDoneProvide( YCPList args ) {
  return SET_YCP_CB( CB_DoneProvide, args );
}

YCPValue PkgModuleFunctions::CallbackStartPackage( YCPList args ) {
  return SET_YCP_CB( CB_StartPackage, args );
}
YCPValue PkgModuleFunctions::CallbackProgressPackage( YCPList args ) {
  return SET_YCP_CB( CB_ProgressPackage, args );
}
YCPValue PkgModuleFunctions::CallbackDonePackage( YCPList args ) {
  return SET_YCP_CB( CB_DonePackage, args );
}

YCPValue PkgModuleFunctions::CallbackMediaChange( YCPList args ) {
  string name;
  // Allow omission of 'src' argument in 'src, name'. Since we can
  // handle one callback function at most, passing a src argument
  // implies a per-source callback which isn't implemented anyway.
  if ( args->size() == 1 && args->value(0)->isString() ) {
    name = args->value(0)->asString()->value();
  } else if ( args->size() == 2
	      && args->value(0)->isInteger()
	      && args->value(1)->isString() ) {
    name = args->value(1)->asString()->value();
  } else {
    return YCPError( "Bad args to Pkg::CallbackMediaChange" );
  }
  _callbackHandler._ycpCallbacks.setCallback( CallbackHandler::YCPCallbacks::CB_MediaChange, name );
  return YCPVoid();
}

YCPValue PkgModuleFunctions::CallbackProgressRebuildDB( YCPList args ) {
  return SET_YCP_CB( CB_ProgressRebuildDB, args );
}

YCPValue PkgModuleFunctions::CallbackSourceChange( YCPList args ) {
  return SET_YCP_CB( CB_SourceChange, args );
}

YCPValue PkgModuleFunctions::CallbackYouProgress( YCPList args ) {
  return SET_YCP_CB( CB_YouProgress, args );
}

YCPValue PkgModuleFunctions::CallbackYouPatchProgress( YCPList args ) {
  return SET_YCP_CB( CB_YouPatchProgress, args );
}
YCPValue PkgModuleFunctions::CallbackYouExecuteYcpScript( YCPList args ) {
  return SET_YCP_CB( CB_YouExecuteYcpScript, args );
}
YCPValue PkgModuleFunctions::CallbackYouScriptProgress( YCPList args ) {
  return SET_YCP_CB( CB_YouScriptProgress, args );
}

YCPValue PkgModuleFunctions::CallbackStartConvertDb( YCPList args ) {
  return SET_YCP_CB( CB_StartConvertDb, args );
}
YCPValue PkgModuleFunctions::CallbackProgressConvertDb( YCPList args ) {
  return SET_YCP_CB( CB_ProgressConvertDb, args );
}
YCPValue PkgModuleFunctions::CallbackNotifyConvertDb( YCPList args ) {
  return SET_YCP_CB( CB_NotifyConvertDb, args );
}
YCPValue PkgModuleFunctions::CallbackStopConvertDb( YCPList args ) {
  return SET_YCP_CB( CB_StopConvertDb, args );
}

#undef SET_YCP_CB
