/******************************************************************************
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
|* Module: Buffout.cpp
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
*******************************************************************************/

/* 1. Includes */

#include "Buffout.h"


/* 2. Global Variables */


Buffout::Buffout() :
    _curbuf(NULL)
{
    _strm_out = new ostream(cout.rdbuf());
}


Buffout::Buffout(const string filename) :
    _curbuf(NULL)
{

    /* 1. Open file */

    _file_out.open(filename.c_str(), ios::binary | ios::out);

    if (!_file_out.good())
    {
        cerr << "Unable to open file: " << filename ;
        err_trace(__FILE__, __LINE__);
    }

    _strm_out = new ostream(_file_out.rdbuf());
}

void Buffout::write(const string &str)
{
    /* 1. Write to file */

    _strm_out->write(str.c_str(), str.size());

}

void Buffout::flush()
{
    _strm_out->flush();
}


Buffout::~Buffout() 
{
    _file_out.close();
    if (_strm_out != NULL)
        delete _strm_out;

    if (_curbuf != NULL)
        delete [] _curbuf;
}



