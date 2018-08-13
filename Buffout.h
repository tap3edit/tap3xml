/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: Buffout.h,v 1.6 2007/06/28 04:14:14 javier Exp $
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
