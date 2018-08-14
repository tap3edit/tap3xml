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

