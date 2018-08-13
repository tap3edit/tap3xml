/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: Buffin.h,v 1.8 2011/04/01 16:03:10 javier Exp $
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
|* Module: Buffin.h
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

#ifndef __Buffin_h__
#define __Buffin_h__

#pragma warning(disable: 4786) /* To clean compile under MSVC */


/* 1. Includes */

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>

#include <sstream>
#include <iomanip>

#include "global.h"


/* 2. Defining namespace */

using namespace std;


/* 3. Typedefs and structures */


/* 4. Prototypes */

class Buffin
{
    private:
        uchar*      _buffin;        /* Content of the file */
        uchar*      _curbuf;        /* Last readed */
        long        _curbuf_len;    /* Length of last readed */
        ifstream    _file_in;       /* Input File Object */
        long        _pos;           /* Current possition into _buffin */
        long        _end;           /* End of _buffin */
        int         _mode;          /* 0=File is read completly at once, */
                                    /* 1=File is being read on request byte by byte */

    public:
        Buffin();
        Buffin(const string filename);
        Buffin(const string filename, int mode);

        long    get_pos     ();
        void    set_pos     (const long &pos);
        long    get_end     ();
        void    read        (const long len);
        string  get_buf     ();
        long    get_buflen  ();
        uchar*  all_ori_buf (); 
        void    new_file    (const char *filename, int mode);

        ~Buffin();
};

#endif
