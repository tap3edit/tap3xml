/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* tap3edit Tools (https://github.com/tap3edit)
|* 
|* Copyright (C) 2005-2019  Javier Gutierrez. All rights reserved.
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
|* Module: Xml.cpp
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

/* 1. Includes */

#include "Xml.h"


/* 2. Global Variables */


Xml::Xml() 
{
    _xmlitem=new XmlItem;
    _tree=NULL;
    _bfin=NULL;
    _bfout=NULL;
    _ofile=NULL;
    _ifile=NULL;
    _header=NULL;
}

void Xml::decode()
{

    extern int run_xmlparser(FILE *ifile, treenode *tree);


    /* 1. Parse xml structure */

    if (_ifile == NULL )
    {
        cerr << "The XML input file should be specified with the \"ifile\" method";
        err_trace(__FILE__, __LINE__);
    }

    if (run_xmlparser(_ifile, _tree))
    {
        cerr << "Error decoding XML input file\n";
        err_trace(__FILE__, __LINE__);
    }

}

void Xml::encode()
{

    _bfout->write(get_header());

    TreeParser *tparser=new TreeParser;
    tparser->handler(_xmlitem);
    tparser->buffout(_bfout);
    tparser->tree(_tree);
    tparser->process();
    delete tparser;

    _bfout->flush();
}

void Xml::ofile(const string &filename)
{
    _bfout=new Buffout(filename);
}

void Xml::ifile(const string &filename)
{

    if (!(_ifile=fopen(filename.c_str(), "rb")))
    {
        cerr << "Error trying to open file: " << filename;
        err_trace(__FILE__, __LINE__);
    }

}

void Xml::buffin(Buffin *bfin)
{
    _bfin=bfin;
}

void Xml::buffout(Buffout *bfout)
{
    _bfout=bfout;
}

void Xml::tree(treenode *tree)
{
    _tree=tree;
}

void Xml::set_header(string *header)
{
    _header=header;
}

string Xml::get_header()
{
    if (_header != NULL)
    {
        return *_header;
    }
    else
    {
        return "";
    }
}

Xml::~Xml() 
{
    delete _xmlitem;

    if (_ifile!=NULL)
    {
        fclose(_ifile);
    }
}
