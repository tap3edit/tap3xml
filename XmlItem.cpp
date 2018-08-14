/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* tap3edit Tools (https://github.com/tap3edit)
|* 
|* Copyright (C) 2005-2018  Javier Gutierrez. All rights reserved.
|* email address <https://github.com/tap3edit/tap3xml>
|* 
|* Permission to use, copy, modify, and/or distribute this software for any
|* purpose with or without fee is hereby granted, provided that the above
|* copyright notice and this permission notice appear in all copies.
|* 
|* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
|* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
|* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
|* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
|* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
|* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
|* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
|* 
|*
|* Module: XmlItem.cpp
|*
|* Description: 
|*
|* Author: Javier Gutierrez (JG)
|*
|* Modifications:
|*
|* When         Who     Pos.    What
|* 20050912     JG              Initial version
|* 20070604     JG      _001_   Bug on encoding an empty value
|*
****************************************************************************/

/* 1. Includes */

#include "XmlItem.h"


/* 2. Global Variables */


XmlItem::XmlItem() 
{
    _tree=NULL;
    _bfin=NULL;
    _bfout=NULL;

}

void XmlItem::decode()
{
    ;
}

string XmlItem::encode(int depth)
{
    ostringstream item;
    
    extern string apply_xml_rules(string str);

    if (_tree==NULL) {
        cerr << "treenode structure was not set with tree() method";
        err_trace(__FILE__, __LINE__);
    }


    /* 1. Preparing XML Item */

    if ( !_tree->pc )
    {
        item << string(depth*4, ' ') << "<" << _tree->tagname << ">";
        /* _001_ { */
        if (_tree->value == NULL)
            item << "";
        else
            item << apply_xml_rules(*_tree->value) ;
        /* _001_ } */
        item << "</" << _tree->tagname << ">\n";
    }
    else
    {
        item << string(depth*4, ' ') << "<" << _tree->tagname << ">\n";
    }


    /* 2. Return value */

    return  item.str();

}

string XmlItem::enc_close(int depth)
{
    ostringstream item;


    if (_tree==NULL) {
        cerr << "treenode structure was not set with tree() method";
        err_trace(__FILE__, __LINE__);
    }


    /* 1. Preparing XML closing Item */

    if ( _tree->pc )
    {
        item << string(depth*4, ' ') << "</" << _tree->tagname << ">\n";
    }


    /* 2. Return */

    return item.str();

}

void XmlItem::buffin(Buffin *bfin)
{
    _bfin=bfin;
}

void XmlItem::buffout(Buffout *bfout)
{
    _bfout=bfout;
}

void XmlItem::tree(treenode *tree)
{
    _tree=tree;
}


XmlItem::~XmlItem() {}

