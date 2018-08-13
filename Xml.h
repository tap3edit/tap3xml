/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: Xml.h,v 1.8 2011/04/01 16:03:10 javier Exp $
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
|* Module: Xml.h
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

#ifndef __Xml_h__
#define __Xml_h__

#define _CRT_SECURE_NO_WARNINGS

/* 1. Includes */

#include <iostream>
#include <string>
#include "XmlItem.h"
#include "TreeItem.h"
#include "TreeParser.h"
#include "Buffin.h"
#include "Buffout.h"


/* 2. Defining namespace */

using namespace std;


/* 3. Typedefs and structures */


/* 4. Prototypes */

class Xml
{
    private:
        Buffin*     _bfin;      /* Where the buffer is being read from */
        Buffout*    _bfout;     /* Where the buffer is being written to */
        XmlItem*    _xmlitem;   /* To decode/encode */
        treenode*   _tree;      /* Tree to store the whole ASN structure */
        FILE*       _ifile;     /* If _bfin is not used we can use _ifile */
        FILE*       _ofile;     /* If _bfout is not used we can use _ofile */
        string*      _header;    /* It will be printed as the header of the XML file */


    public:

        Xml();

        void decode();
        void encode();
        void ofile(const string &filename);
        void ifile(const string &filename);
        void buffin(Buffin *bfin);
        void buffout(Buffout *bfout);
        void tree(treenode *tree);
        void set_header(string *header);
        string get_header();


        ~Xml();
};

#endif
