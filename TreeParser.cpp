/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: TreeParser.cpp,v 1.9 2012/04/15 11:49:46 javier Exp $
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
|* Module: TreeParser.cpp
|*
|* Description: 
|*
|* Author: Javier Gutierrez (JG)
|*
|* Modifications:
|*
|* When         Who     Pos.    What
|* 20050912     JG              Initial version
|*
****************************************************************************/

/* 1. Includes */

#include "TreeParser.h"


/* 2. Global Variables */


TreeParser::TreeParser() :
    _bfout(NULL), 
    _tree(NULL),
    _treeitem(NULL)
{}

void TreeParser::handler(TreeItem *treeitem)
{
    _treeitem=treeitem;

    if (_tree)
        _treeitem->tree(_tree);
}

void TreeParser::tree(treenode *tree)
{
    _tree=tree;

    if (_treeitem)
        _treeitem->tree(_tree);
}

void TreeParser::buffout(Buffout *bfout)
{
    _bfout=bfout;

    if (_tree && _treeitem)
        _treeitem->tree(_tree);
}

void TreeParser::process()
{
    
    this->_process(_tree, 0);

}

void TreeParser::_process(treenode *tree, int depth)
{

    vector<treenode *>::iterator   vii;

    _treeitem->tree(tree);
    _bfout->write(_treeitem->encode(depth));

    if (tree->children != NULL)
    {

        for(vii=tree->children->begin();vii!=tree->children->end();vii++)
        {
            this->_process((*vii), depth+1);
        }

    }

    _treeitem->tree(tree);
    _bfout->write(_treeitem->enc_close(depth));

}

TreeParser::~TreeParser() {}

