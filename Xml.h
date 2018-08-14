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
