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

  File:	      YDialog.cc

  Author:     Mathias Kettner <kettner@suse.de>
  Maintainer: Stefan Hundhammer <sh@suse.de>

/-*/


#define y2log_component "ui"
#include <ycp/y2log.h>
#include <ycp/YCPSymbol.h>
#include "YDialog.h"
#include "YShortcutManager.h"


YDialog::YDialog( const YWidgetOpt & opt )
    : YContainerWidget( opt )
{
    _hasDefaultSize.setValue( opt.hasDefaultSize.value() );
    _hasWarnColor.setValue( opt.hasWarnColor.value() );
    _hasInfoColor.setValue( opt.hasInfoColor.value() );
    _isDecorated.setValue( opt.isDecorated.value() );
    _isCentered.setValue( opt.isCentered.value() );
    _hasSmallDecorations.setValue( opt.hasSmallDecorations.value() );
    _shortcutCheckPostponed = false;
}


YDialog::~YDialog()
{
}


bool YDialog::isDialog() const
{
    return true;
}


void YDialog::setInitialSize()
{
    setSize( nicesize( YD_HORIZ ), nicesize( YD_VERT ) );
}


void YDialog::checkShortcuts( bool force )
{
    if ( _shortcutCheckPostponed && ! force )
    {
	y2debug( "shortcut check postponed" );
	return;
    }

    YShortcutManager shortcutManager( this );
    shortcutManager.checkShortcuts();
	
    _shortcutCheckPostponed	= false;
}


YCPValue YDialog::queryWidget( const YCPSymbol & property )
{
    string symbol = property->symbol();
    
    if ( symbol == YUIProperty_DebugLabel ||
	 symbol == YUIProperty_DialogDebugLabel ||
	 symbol == YUIProperty_DialogDebugLabel1 )	return YCPString( dialogDebugLabel() );
    if ( symbol == YUIProperty_DialogDebugLabel2 )	return YCPString( dialogDebugLabel2() );
    else
    {
	return YWidget::queryWidget( property );
    }
}


YWidgetList YDialog::widgets() const
{
    YWidgetList widgetList;
    fillWidgetList( widgetList, this );

    return widgetList;
}


void YDialog::fillWidgetList( YWidgetList &		widgetList,
			      const YContainerWidget *	parent 		) const
{
    if ( ! parent )
	return;
    
    for ( int i = 0; i < parent->numChildren(); i++ )
    {
	YWidget * child = parent->child(i);

	if ( ! child->isDialog() )
	{
	    widgetList.push_back( child );
	    YContainerWidget * container = dynamic_cast<YContainerWidget *> ( child );

	    if ( container )
		fillWidgetList( widgetList, container );
	}
	else
	{
	    y2milestone( "Found dialog: %s", child->widgetClass() );
	}
    }
}


std::string YDialog::dialogDebugLabel()
{
    return string( "<TO DO>" );
}


std::string YDialog::dialogDebugLabel2()
{
    return string( "<TO DO>" );
}
