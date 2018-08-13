/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: XmlItem.cpp,v 1.10 2012/04/15 11:49:46 javier Exp $
|*
|* tap3edit Tools (http://www.tap3edit.com)
|* 
|* Copyright (C) 2007  Javier Gutierrez. All rights reserved.
|* email address <jgutierrez@tap3edit.com>
|* 
|* This program is free software; you can redistribute it and/or
|* modify it under the terms of the GNU General Public License
|* as published by the Free Software Foundation; either version 2
|* of the License, or (at your option) any later version.
|* 
|* This program is distributed in the hope that it will be useful,
|* but WITHOUT ANY WARRANTY; without even the implied warranty of
|* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|* GNU General Public License for more details.
|* 
|* You should have received a copy of the GNU General Public License
|* along with this program; if not, write to the Free Software
|* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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

