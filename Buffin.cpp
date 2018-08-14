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
|* Module: Buffin.cpp
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
*******************************************************************************/

/* 1. Includes */

#include "Buffin.h"


/* 2. Global Variables */


Buffin::Buffin() :
    _buffin(NULL),
    _curbuf(NULL)
{}

Buffin::Buffin(
    const string filename    /* Filename to be buffered */
) :
    _buffin(NULL),
    _curbuf(NULL)
{
    Buffin(filename, 0);
}

Buffin::Buffin(
    const string filename,  /* Filename to be buffered */
    int mode=0              /* 0=Read complete file, 1=Read bytes on request from file */
) :
    _buffin(NULL),
    _curbuf(NULL)
{
    /* 1. Declarations and definitions */
    _mode=mode;
    _pos=0;


    /* 2. Open file */

    _file_in.open(filename.c_str(), ios::binary | ios::in);

    if (!_file_in.good())
    {
        cerr << "Unable to open file: " << filename << endl;
        err_trace(__FILE__, __LINE__);
    }


    /* 3. Getting file size */

    _file_in.seekg(0, ios_base::end);
    _end=_file_in.tellg();
    _file_in.seekg(0, ios_base::beg);

    if (!_end)
    {
        cerr << "File: " << filename << " is empty" << endl;
        err_trace( __FILE__, __LINE__) ;
    }


    /* 4. Proceed according to mode */

    if (mode)
    {
        /* 4.1. File read on request */
        ;
    } else {

        /* 4.2. File read complete into _buffin */

        {
            /* 4.2.2. Reading the whole file in buffin */

            try
            {
                _buffin=new uchar [_end+2];
            }
            catch(const bad_alloc &ba)
            {
                cerr << "Out of memory: " << ba.what() << endl ;
                err_trace( __FILE__, __LINE__) ;
            }

            _file_in.read((char *)_buffin, _end);

            /* Note: yy_scan_buffer() from flex needs two */
            /*       null bytes at the end of the string  */

            _buffin[_end+0]='\0';
            _buffin[_end+1]='\0';


            /* 4.2.3. File is not needed anymore */

            // _file_in.close();
        }

    }

}


long Buffin::get_pos()
{
    return _pos;
}

void  Buffin::set_pos(const long &pos)
{
    if (pos > this->get_end())
    {
        cerr << "Trying to read beyond the end of file. Pos: " << pos << " End: " << this->get_end() << endl;
        err_trace(__FILE__, __LINE__);
    }

    _pos=pos;
    if (!_mode)
    {
        _file_in.seekg(pos);
    }
}

long Buffin::get_end()
{
    return _end;
}

void  Buffin::read(const long len)
{

    /* 1. Allocation of memory */

    if (_curbuf != NULL)
        delete [] _curbuf;
    
    _curbuf=new uchar[len];


    /* 2. Reading into current buffer */

    if(_mode)
    {
        ;
    } 
    else 
    {
        if (this->get_pos() + len <= this->get_end())
            memcpy(_curbuf, _buffin+this->get_pos(), len);
        else
        {
            cerr << "Trying to read " << len << " bytes beyond end of file from position "<<this->get_pos();
            cerr << ". File might be corrupted." << endl;
            err_trace( __FILE__, __LINE__) ;
        }

    }

    _curbuf_len=len;


    /* 3. Update position */

    this->set_pos(this->get_pos()+len);
}

uchar  *Buffin::all_ori_buf()
{
    return _buffin;
}

string Buffin::get_buf()
{
    string str;
    str.assign((const char *)_curbuf, this->get_buflen());
    return str;
}

long Buffin::get_buflen()
{
    return _curbuf_len;
}

void Buffin::new_file(
    const char *filename,   /* Filename to be buffered */
    int mode                /* 0=Read complete file, 1=Read bytes on request from file */
)
{
    Buffin(filename, mode);
}

Buffin::~Buffin()
{
    if (_mode)
        _file_in.close();
    if (_buffin != NULL)
        delete [] _buffin ;
    if (_curbuf != NULL)
        delete [] _curbuf;
}

