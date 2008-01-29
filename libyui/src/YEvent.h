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

  File:		YEvent.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YEvent_h
#define YEvent_h


#include <string>

using std::string;
class YWidget;
class YItem;


/**
 * Abstract base class for events to be returned upon UI::UserInput()
 * and related functions.
 **/
class YEvent
{
public:

    enum EventType
    {
	NoEvent = 0,
	UnknownEvent,
	WidgetEvent,
	MenuEvent,
	KeyEvent,
	CancelEvent,
	TimeoutEvent,
	DebugEvent
    };


    enum EventReason
    {
	UnknownReason = 0,
	Activated,
	SelectionChanged,
	ValueChanged
    };


    /**
     * Constructor.
     **/
    YEvent( EventType eventType = UnknownEvent );

    /**
     * Virtual desctructor to force a polymorph object
     * so dynamic_cast<> can be used.
     **/
    virtual ~YEvent();

    /**
     * Returns the event type.
     **/
    EventType eventType() const { return _eventType; }

    /**
     * Returns the unique serial no. of this event.
     * This is mainly useful for debugging.
     **/
    unsigned long serial() const { return _serial; }

    /**
     * Returns the character representation of an event type.
     **/
    static const char * toString( EventType eventType );

    /**
     * Returns the character representation of an event reason.
     **/
    static const char * toString( EventReason reason );


protected:

    EventType 			_eventType;
    unsigned long		_serial;

    static unsigned long	_nextSerial;
    static int			_activeEvents;
};



class YWidgetEvent: public YEvent
{
public:

    /**
     * Constructor.
     **/
    YWidgetEvent( YWidget *	widget		= 0,
		  EventReason	reason		= Activated,
		  EventType 	eventType	= WidgetEvent );

    /**
     * Returns the widget that caused this event. This might as well be 0 if
     * this is not a widget event.
     **/
    YWidget * widget() const { return _widget; }

    /**
     * Returns the reason for this event. This very much like an event sub-type.
     **/
    EventReason reason() const { return _reason; }

protected:

    YWidget * 	_widget;
    EventReason	_reason;
};


class YKeyEvent: public YEvent
{
public:

    /**
     * Constructor.
     *
     * Create a key event with a specified key symbol (a text describing the
     * key, such as "CursorLeft", "F1", etc.) and optionally the widget that
     * currently has the keyboard focus.
     **/
    YKeyEvent( const string &	keySymbol,
	       YWidget *	focusWidget = 0 );

    /**
     * Returns the key symbol - a text describing the
     * key, such as "CursorLeft", "F1", "a", "A", etc.
     **/
    string keySymbol() const { return _keySymbol; }

    /**
     * Returns the widget that currently has the keyboard focus.
     *
     * This might be 0 if no widget has the focus or if the creator of
     * this event could not obtain that information.
     **/
    YWidget * focusWidget() const { return _focusWidget; }

protected:

    string	_keySymbol;
    YWidget * 	_focusWidget;
};


/**
 * Event to be returned upon menu selection.
 **/
class YMenuEvent: public YEvent
{
public:

    YMenuEvent( YItem * item )
	: YEvent( MenuEvent )
	, _item( item )
	{}

    YMenuEvent( const char *     id )	: YEvent( MenuEvent ), _item(0), _id( id ) {}
    YMenuEvent( const string & 	 id )	: YEvent( MenuEvent ), _item(0), _id( id ) {}

    /**
     * Return the YItem that corresponds to this event or 0 if the event was
     * constructed with a string ID.
     **/
    YItem * item() const { return _item; }

    /**
     * Return the string ID of this event. This will be an empty string if the
     * event was constructed with a YItem.
     **/
    string id() const { return _id; }

protected:

    YItem * _item;
    string  _id;
};


/**
 * Event to be returned upon closing a dialog with the window manager close
 * button (or Alt-F4)
 **/
class YCancelEvent: public YEvent
{
public:

    YCancelEvent() : YEvent( CancelEvent ) {}
};


/**
 * Event to be returned upon closing a dialog with the window manager close
 * button (or Alt-F4)
 **/
class YDebugEvent: public YEvent
{
public:

    YDebugEvent() : YEvent( DebugEvent ) {}
};


/**
 * Event to be returned upon timeout
 * (i.e. no event available in the specified timeout)
 **/
class YTimeoutEvent: public YEvent
{
public:

    YTimeoutEvent() : YEvent( TimeoutEvent ) {}
};


#endif // YEvent_h
