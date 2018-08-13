/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: TreeItem.h,v 1.6 2012/04/08 18:47:25 javier Exp $
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
|* Module: TreeItem.h
|*
|* Description: 
|*
|* Author: Javier Gutierrez (JG)
|*
|* Modifications:
|*
|* When         Who     Pos.    What
|* 20050909     JG              Initial version
|*
****************************************************************************/

#ifndef __TreeItem_h__
#define __TreeItem_h__


/* 1. Includes */

#include <iostream>
#include <string>
#include "global.h"
#include "Buffout.h"


/* 2. Defining namespace */

using namespace std;


/* 3. Typedefs and structures */


/* 4. Prototypes */

class TreeItem
{
    protected:
        treenode    *_tree;
        vector<int> _taglist;
        vector<int>::iterator _taglist_ii;

    public:
        TreeItem();

        virtual void buffout(Buffout *bfout);
        virtual void tree(treenode *tree);
        virtual string encode(int depth);
        virtual string enc_close(int depth);

        virtual ~TreeItem();

};

#endif
