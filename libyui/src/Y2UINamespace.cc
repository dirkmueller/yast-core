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

   File:	Y2UINamespace.cc

   Authors:	Klaus Kaempf <kkaempf@suse.de>
		Stanislav Visnovsky <visnov@suse.cz>

   Maintainer:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <ycp/YCPBoolean.h>
#include <ycp/YCPInteger.h>
#include <ycp/YCPVoid.h>
#include <ycp/YCPString.h>
#include <y2/Y2Function.h>

#define y2log_component "ui"
#include <ycp/y2log.h>

#include "YUI.h"
#include "YUIComponent.h"
#include "Y2UINamespace.h"

#define VERBOSE_UI_CALLS	0


YCPValue
Y2UINamespace::SetLanguage( const YCPString & language )
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateSetLanguage( language );

    return YCPVoid();
}


YCPValue
Y2UINamespace::SetLanguage( const YCPString & language, const YCPString & encoding )
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateSetLanguage( language, encoding );

    return YCPVoid();
}


YCPValue
Y2UINamespace::GetProductName()
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateGetProductName();
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::SetProductName( const YCPString & name )
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateSetProductName( name );

    return YCPVoid();
}


YCPValue
Y2UINamespace::SetConsoleFont( const YCPString & console_magic,
		  const YCPString & font,
		  const YCPString & screen_map,
		  const YCPString & unicode_map,
		  const YCPString & encoding )
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateSetConsoleFont( console_magic,
						 font,
						 screen_map,
						 unicode_map,
						 encoding );
    return YCPVoid();
}


YCPValue
Y2UINamespace::SetKeyboard()
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateSetKeyboard();

    return YCPVoid();
}


YCPValue
Y2UINamespace::GetLanguage( const YCPBoolean & strip )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateGetLanguage( strip );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::RunInTerminal( const YCPString &module )
{
    if (YUIComponent::ui() )
	return YUIComponent::ui()->evaluateRunInTerminal( module );
    else
        return YCPVoid();
}

YCPValue
Y2UINamespace::UserInput()
{
    return m_comp->ui()->evaluateUserInput();
}


YCPValue
Y2UINamespace::PollInput()
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluatePollInput();
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::TimeoutUserInput( const YCPInteger& timeout )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateTimeoutUserInput( timeout );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::WaitForEvent()
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateWaitForEvent();
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::WaitForEvent( const YCPInteger & timeout )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateWaitForEvent( timeout );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::OpenDialog( const YCPTerm & opts, const YCPTerm & dialog_term )
{
    if ( YUIComponent::ui() )
	// Notice: Parameter order is switched!
	return YUIComponent::ui()->evaluateOpenDialog( dialog_term, opts );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::OpenDialog( const YCPTerm & dialog_term )
{
    return m_comp->ui()->evaluateOpenDialog( dialog_term );
}


YCPValue
Y2UINamespace::CloseDialog()
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateCloseDialog();
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::ChangeWidget( const YCPSymbol & widget_id, const YCPSymbol & property, const YCPValue & new_value )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateChangeWidget( widget_id, property, new_value );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::ChangeWidget( const YCPTerm & widget_id, const YCPSymbol & property, const YCPValue & new_value )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateChangeWidget( widget_id, property, new_value );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::ChangeWidget( const YCPTerm & widget_id, const YCPTerm & property, const YCPValue & new_value )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateChangeWidget( widget_id, property, new_value );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::QueryWidget( const YCPSymbol & widget_id, const YCPSymbol & property )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateQueryWidget( widget_id, property );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::QueryWidget( const YCPSymbol & widget_id, const YCPTerm & property )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateQueryWidget( widget_id, property );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::QueryWidget( const YCPTerm & widget_id, const YCPSymbol & property )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateQueryWidget( widget_id, property );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::QueryWidget( const YCPTerm & widget_id, const YCPTerm & property )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateQueryWidget( widget_id, property );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::ReplaceWidget( const YCPSymbol & widget_id, const YCPTerm & new_widget )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateReplaceWidget( widget_id, new_widget );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::ReplaceWidget( const YCPTerm & widget_id, const YCPTerm & new_widget )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateReplaceWidget( widget_id, new_widget );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::SetFocus( const YCPSymbol & widget_id )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateSetFocus( widget_id );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::SetFocus( const YCPTerm & widget_id )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateSetFocus( widget_id );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::BusyCursor()
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateBusyCursor();

    return YCPVoid();
}


YCPValue
Y2UINamespace::RedrawScreen()
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateRedrawScreen();

    return YCPVoid();
}


YCPValue
Y2UINamespace::NormalCursor()
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateNormalCursor();

    return YCPVoid();
}


YCPValue
Y2UINamespace::MakeScreenShot( const YCPString & filename )
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateMakeScreenShot( filename );

    return YCPVoid();
}


YCPValue
Y2UINamespace::MakeScreenShot()
{
    if ( YUIComponent::ui() )
	return MakeScreenShot( YCPNull() );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::DumpWidgetTree()
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateDumpWidgetTree();

    return YCPVoid();
}


YCPValue
Y2UINamespace::Beep()
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateBeep();

    return YCPVoid();
}

YCPValue
Y2UINamespace::RecordMacro( const YCPString & filename )
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateRecordMacro( filename );

    return YCPVoid();
}


YCPValue
Y2UINamespace::StopRecordMacro()
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateStopRecordMacro();

    return YCPVoid();
}


YCPValue
Y2UINamespace::PlayMacro( const YCPString & filename )
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluatePlayMacro( filename );

    return YCPVoid();
}

YCPValue
Y2UINamespace::FakeUserInput()
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateFakeUserInput( YCPVoid() );

    return YCPVoid();
}


YCPValue
Y2UINamespace::FakeUserInput( const YCPValue & next_input )
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateFakeUserInput( next_input );

    return YCPVoid();
}



YCPValue
Y2UINamespace::Glyph( const YCPSymbol & glyphSym  )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateGlyph( glyphSym );
    else
	return YCPString( "*" );
}


YCPValue
Y2UINamespace::GetDisplayInfo()
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateGetDisplayInfo();
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::RecalcLayout()
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateRecalcLayout();

    return YCPVoid();
}


YCPValue
Y2UINamespace::PostponeShortcutCheck()
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluatePostponeShortcutCheck();

    return YCPVoid();
}

YCPValue
Y2UINamespace::CheckShortcuts()
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateCheckShortcuts();

    return YCPVoid();
}


YCPValue
Y2UINamespace::WidgetExists( const YCPSymbol & widget_id )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateWidgetExists( widget_id );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::WidgetExists( const YCPTerm & widget_id )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateWidgetExists( widget_id );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::RunPkgSelection( const YCPValue & widget_id )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateRunPkgSelection( widget_id );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::AskForExistingDirectory( const YCPString & startDir, const YCPString & headline )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateAskForExistingDirectory( startDir, headline );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::AskForExistingFile( const YCPString & startWith, const YCPString & filter, const YCPString & headline  )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateAskForExistingFile( startWith, filter, headline );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::AskForSaveFileName( const YCPString & startWith, const YCPString & filter, const YCPString & headline )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateAskForSaveFileName( startWith, filter, headline );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::SetFunctionKeys( const YCPMap & new_fkeys )
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateSetFunctionKeys( new_fkeys );

    return YCPVoid();
}


YCPValue
Y2UINamespace::Recode( const YCPString & from, const YCPString & to, const YCPString & text )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateRecode( from, to, text );
    else
	return YCPVoid();
}


YCPValue
Y2UINamespace::SetModulename( const YCPString & name )
{
    if ( YUIComponent::ui() )
	YUIComponent::ui()->evaluateSetModulename( name );

    return YCPVoid();
}


YCPValue
Y2UINamespace::HasSpecialWidget( const YCPSymbol & widget )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateHasSpecialWidget( widget );
    else
	return YCPBoolean( false );
}


YCPValue
Y2UINamespace::WizardCommand( const YCPTerm & command )
{
    if ( YUIComponent::ui() )
	return YUIComponent::ui()->evaluateWizardCommand( command );
    else
	return YCPBoolean( false );
}


YCPValue
Y2UINamespace::CallHandler( void * ptr, int argc, YCPValue argv[] )
{
    if ( YUIComponent::uiComponent() )
    {
	return YUIComponent::uiComponent()->callBuiltin( ptr, argc, argv );
    }
    else
    {
	y2warning( "No UI instance available yet!" );
	return YCPVoid();
    }
}

// -------------

Y2UIFunction::Y2UIFunction (Y2UINamespace* instance, YUIComponent* comp, unsigned int pos, bool play_macro_blocks) :
m_position (pos)
, m_instance (instance)
, m_comp (comp)
, m_play_macro_blocks (play_macro_blocks)
, m_param1 ( YCPNull() )
, m_param2 ( YCPNull() )
, m_param3 ( YCPNull() )
, m_param4 ( YCPNull() )
, m_param5 ( YCPNull() )
{
};

bool Y2UIFunction::attachParameter (const YCPValue& arg, const int position)
{
    switch (position)
    {
    case 0: m_param1 = arg; break;
    case 1: m_param2 = arg; break;
    case 2: m_param3 = arg; break;
    case 3: m_param4 = arg; break;
    case 4: m_param5 = arg; break;
    default: return false;
    }

    return true;
}

constTypePtr Y2UIFunction::wantedParameterType() const
{
    // we do not help them with type conversion
    // hey but we could, at least for non-overloaded functions
    return Type::Any; // FIXME make this the default behavior in parent class
}

bool Y2UIFunction::appendParameter (const YCPValue& arg)
{
    if (m_param1.isNull())
    {
        m_param1 = arg;
        return true;
    } else if (m_param2.isNull())
    {
        m_param2 = arg;
        return true;
    } else if (m_param3.isNull())
    {
        m_param3 = arg;
        return true;
    } else if (m_param4.isNull())
    {
        m_param4 = arg;
        return true;
    } else if (m_param5.isNull())
    {
        m_param5 = arg;
        return true;
    }
    y2internal ("appendParameter > 5 not implemented");
    return false;
}

bool Y2UIFunction::finishParameters()
{
    // Aha, perl could give us any garbage.
    // Hmm, how do we cope with giving garbage to ycp modules?
    // - it heeds wantedParameterType
    // Funny, why is this function not called?

    y2internal ("FIXME finishParameters not implemented");
    return true;
}

YCPValue Y2UIFunction::evaluateCall()
{
    if ( ! m_comp->ui() )
    {
        y2debug( "Late creation of UI instance" );
        m_comp->createUI();
    }

    if ( ! m_comp->ui() )
	return YCPVoid();	// dummy UI component

    if ( m_play_macro_blocks && m_comp->ui()->playingMacro() )
    {
	m_comp->ui()->playNextMacroBlock();
    }

    YCPValue ret = YCPVoid();

    if ( m_comp->ui()-> runningWithThreads() )
    {
        m_comp->ui()->_builtinCallData.function = this;

        m_comp->ui()->signalUIThread();

        while ( ! m_comp->ui()->waitForUIThread() )
        {
            // NOP
        }

        ret = m_comp->ui()->_builtinCallData.result;
    }
    else
    {
        ret = evaluateCall_int();
    }

    return ret;
}

YCPValue Y2UIFunction::evaluateCall_int()
{
    switch (m_position) {
#include "UIBuiltinCalls.h"
    }

    return YCPNull();
}

bool Y2UIFunction::reset()
{
    m_param1 = YCPNull();
    m_param2 = YCPNull();
    m_param3 = YCPNull();
    m_param4 = YCPNull();
    m_param5 = YCPNull();

    return true;
}

string Y2UIFunction::name() const
{
    // FIXME. when is this called at all?
    return("Y2UI");
}

// error reporting helper
static
void no_match (const char * name, constTypePtr type,
	       vector<SymbolEntryPtr>::iterator	b,
	       vector<SymbolEntryPtr>::iterator e)
{
    y2error ("Function %s called with wrong type %s",
	     name, type->toString().c_str());
    y2error ("TODO: display the candidates"); // use b and e
}


Y2UIOverloadedFunction::Y2UIOverloadedFunction ( Y2UINamespace* instance, YUIComponent* comp,
						 unsigned pos_offset,
						 vector<SymbolEntryPtr>::iterator candidates_b,
						 vector<SymbolEntryPtr>::iterator candidates_e,
						 bool play_macro_blocks )
    : Y2UIFunction (instance, comp, pos_offset, play_macro_blocks)
    , m_candidates_b (candidates_b)
    , m_candidates_e (candidates_e)
{
    // still to do: m_position is just the first candidate
}


bool
Y2UIOverloadedFunction::finishParameters()
{
    FunctionTypePtr real_tp = new FunctionType (Type::Unspec); //return type
    if (!m_param1.isNull())
    {
	real_tp->concat (Type::vt2type (m_param1->valuetype()));
	if (!m_param2.isNull())
	{
	    real_tp->concat (Type::vt2type (m_param2->valuetype()));
	    if (!m_param3.isNull())
	    {
		real_tp->concat (Type::vt2type (m_param3->valuetype()));
		if (!m_param4.isNull())
		{
		    real_tp->concat (Type::vt2type (m_param4->valuetype()));

		    if (!m_param5.isNull())
		    {
			real_tp->concat (Type::vt2type (m_param5->valuetype()));
		    }
		}
	    }
	}
    }
    y2debug ("Actual type: %s", real_tp->toString().c_str());

    vector<SymbolEntryPtr>::iterator
	b = m_candidates_b,
	e = m_candidates_e,
	it;

    for (it = b; it != e; ++it)
    {
	SymbolEntryPtr se_p = *it;
	constFunctionTypePtr cand_type = se_p->type();
	int m = real_tp->match (cand_type);
	y2debug ("Candidate: %s MATCH: %d", se_p->toString().c_str(), m);

	if (m == 0)
	    break;
    }

    if (it == e)
    {
	// any candidate has a good name. report actual used signature.
	no_match ((*b)->name(), real_tp, b, e);
	return false;
    }

    // found
    m_position += (it - b);
    return true;
}


Y2UINamespace::Y2UINamespace (YUIComponent* comp)
{
    m_comp = comp;
    registerFunctions();
}


Y2UINamespace::~Y2UINamespace()
{
    // NOP
}


Y2Function* Y2UINamespace::createFunctionCall( const string name, constFunctionTypePtr type )
{
#if VERBOSE_UI_CALLS
    y2debug ("Creating function call for %s", name.c_str());
    y2debug ("whose type is %s", type? type->toString().c_str() : "unknown");
#endif

    // overloading: functions with same name must be adjacent in m_symbols

    symbols_t::iterator
	b = m_symbols.begin(),
	e  = m_symbols.end(),
	it = b;
    SymbolEntryPtr p;

    //
    // 1: Find start of range of overloaded functions
    //

    for ( ; it != e; it++ )
    {
	p = (*it);
	if ( p->name() == name )
	    break;
    }

    if ( it == e )
    {
        y2error ("No such function %s", name.c_str());
        return NULL;
    }

    bool play_macro_blocks = (name == "UserInput" || name == "TimeoutUserInput" || name == "WaitForEvent");

    symbols_t::iterator rb = it;
    // 2: find end of range of overloaded functions or the exact match

    for ( ; it != e; it++ )
    {
	p = (*it);
	if ( p->name() != name )
	    break;

	// type comparison, ugh, use match

	if ( type && p->type() == type )
	{
	    // found exact match
	    // BTW, p->position() may change as we go, don't get confused.
	    // What matters is our own counting.
#if VERBOSE_UI_CALLS
	    y2debug ("Found symbol '%s' @%d", p->toString().c_str(), p->position());
#endif
	    return new Y2UIFunction (this, m_comp, it - b, play_macro_blocks);
	}
    }

    symbols_t::iterator re = it;
    // [rb, re) have matching name

    if ( type )
    {
	// we have exhausted the candidates without finding a match
	y2error ("Incompatible yast2-core?");
	no_match (name.c_str(), type, rb, re);
	return NULL;
    }

    y2debug ("overloaded %s, %u@%u", name.c_str(), re - rb, rb - b);
    return new Y2UIOverloadedFunction (this, m_comp, rb - b, rb, re, play_macro_blocks);
}


void Y2UINamespace::registerFunctions()
{
#include "UIBuiltinTable.h"
}

