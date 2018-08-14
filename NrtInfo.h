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
