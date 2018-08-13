/******************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: Buffout.cpp,v 1.7 2012/04/15 11:49:46 javier Exp $
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



