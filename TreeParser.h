/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: TreeParser.h,v 1.5 2007/06/05 22:44:56 javier Exp $
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
|* Module: TreeParser.h
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

#ifndef __TreeParser_h__
#define __TreeParser_h__


/* 1. Includes */

#include <iostream>
#include <string>

#include "TreeItem.h"
#include "Buffout.h"


/* 2. Defines and namespace */

using namespace std;


/* 3. Typedefs and structures */


/* 4. Prototypes */

class TreeParser
{
    private:
        Buffout*    _bfout;     /* Buffer to write to */
        treenode*   _tree;      /* Structure of the tap file */
        TreeItem*   _treeitem;  /* Asn and Xml are derivation from TreeItem */

        void _process(treenode *tree, int depth);

    public:

        TreeParser();

        void buffout(Buffout *bfout);
        void tree(treenode *tree);
        void handler(TreeItem *treeitem);
        void process();

        ~TreeParser();

};


#endif
