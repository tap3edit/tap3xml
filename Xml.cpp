/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: Xml.cpp,v 1.11 2012/04/15 11:49:46 javier Exp $
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
