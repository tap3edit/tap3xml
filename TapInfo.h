/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: TapInfo.h,v 1.5 2007/06/05 22:44:56 javier Exp $
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
|* Module: TapInfo.h
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

#ifndef __TapInfo_h__
#define __TapInfo_h__


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

class TapInfo
{
    private:
        Buffin*     _bfin;
        int         _version;
        int         _suplversion;
        int         _release;
        int         _suplrelease;
        string      _type;

        void        _findtreeversion(treenode *tree, int depth);

    public:

        TapInfo();

        void buffin(Buffin *bfin);
        void tree(treenode *tree);
        int version();
        int release();
        int suplversion();
        int suplrelease();
        string type();

        ~TapInfo();

};

#endif
