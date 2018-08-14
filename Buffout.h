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
|* Module: Buffout.h
|*
|* Description: 
|*
|* Author: Javier Gutierrez (JG)
|*
|* Modifications:
|*
|* When         Who     Pos.    What
|* 20050909     JG              Initial version
|* 20070622     JG              Adaption to write on standard output
|*
****************************************************************************/

#ifndef __Buffout_h__
#define __Buffout_h__


/* 1. Includes */

#include <iostream>
#include <ostream>
#include <string>
#include <fstream>

#include <sstream>
#include <iomanip>

#include "global.h"


/* 2. Defining namespace */

using namespace std;


/* 3. Typedefs and structures */


/* 4. Prototypes */

class Buffout
{
    private:
        uchar*      _buffout;       /* Buffer to be written to */
        uchar*      _curbuf;        /* Last written */
        long        _curbuf_len;    /* Length of last written */
        ofstream    _file_out;      /* Output file object */
        ostream*    _strm_out;      /* Stream to write: file, stdout, etc */

    public:
        Buffout();
        Buffout(const string filename);

        void write(const string &str);
        void flush();

        ~Buffout();
};


#endif
