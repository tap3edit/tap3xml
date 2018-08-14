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
