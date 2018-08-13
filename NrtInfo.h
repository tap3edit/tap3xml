/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: NrtInfo.h,v 1.1 2007/12/23 11:27:19 javier Exp $
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
|* Module: NrtInfo.h
|*
|* Description: 
|*
|* Author: Javier Gutierrez (JG)
|*
|* Modifications:
|*
|* When         Who     Pos.    What
|* 20051026     JG              Initial version
|*
****************************************************************************/

#ifndef __NrtInfo_h__
#define __NrtInfo_h__


/* 1. Includes */

#pragma warning(disable: 4786) /* To clean compile under MSVC */

#include <iostream>
#include <string>
#include "Buffin.h"
#include "global.h"


/* 2. Defining namespace */

using namespace std;


/* 3. Typedefs and structures */


/* 4. Prototypes */

class NrtInfo
{
    private:
        Buffin*     _bfin;
        int         _version;
        int         _release;
        string      _type;

        void        _findtreeversion(treenode *tree, int depth);

    public:

        NrtInfo();

        void buffin(Buffin *bfin);
        void tree(treenode *tree);
        int version();
        int release();
        string type();

        ~NrtInfo();

};

#endif
