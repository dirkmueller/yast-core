/*---------------------------------------------------------------------\
|                                                                      |  
|                      __   __    ____ _____ ____                      |  
|                      \ \ / /_ _/ ___|_   _|___ \                     |  
|                       \ V / _` \___ \ | |   __) |                    |  
|                        | | (_| |___) || |  / __/                     |  
|                        |_|\__,_|____/ |_| |_____|                    |  
|                                                                      |  
|                               core system                            | 
|                                                        (C) SuSE GmbH |  
\----------------------------------------------------------------------/ 

   File:       YCPSymbol.cc

   Author:	Mathias Kettner <kettner@suse.de>
   Maintainer:	Klaus Kaempf <kkaempf@suse.de>

/-*/

#include "ycp/y2log.h"
#include "ycp/YCPSymbol.h"
#include "ycp/Bytecode.h"


// YCPSymbolRep

YCPSymbolRep::YCPSymbolRep(const char *s)
    : v(Ustring (SymbolEntry::_nameHash, s))
{
}


YCPSymbolRep::YCPSymbolRep(string s)
    : v(Ustring (SymbolEntry::_nameHash, s))
{
}


string
YCPSymbolRep::symbol() const
{
    return v.asString();
}


const char *
YCPSymbolRep::symbol_cstr() const
{
    return v.asString().c_str();
}


YCPOrder
YCPSymbolRep::compare(const YCPSymbol& s) const
{
    if (v == s->v) return YO_EQUAL;
    else return v < s->v ? YO_LESS : YO_GREATER;
}


string
YCPSymbolRep::toString() const
{
    return string("`") + v.asString();
}


YCPValueType
YCPSymbolRep::valuetype() const
{
    return YT_SYMBOL;
}

/**
 * Output value as bytecode to stream
 */

std::ostream &
YCPSymbolRep::toStream (std::ostream & str) const
{
    return Bytecode::writeUstring (str, v);
}


// --------------------------------------------------------

static string
fromStream (std::istream & str)
{
    string s;
    Bytecode::readString (str, s);
    return s;
}


YCPSymbol::YCPSymbol (std::istream & str)
    : YCPValue (new YCPSymbolRep (fromStream (str)))
{
}
