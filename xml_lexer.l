%{
/******************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|*
|* $Id: xml_lexer.l,v 1.13 2013/05/12 14:14:03 javier Exp $
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
|* Module: xml_lexer.l
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
|*
******************************************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include "global.h"
using namespace std;

#include "xml_parser.hpp"

extern treenode *rootnode;
extern int xmlparse();


%}

%x COMMENT
%x CDATA

%option noyywrap
%option never-interactive
%option yylineno

%%

\<\?.+\?\>              return PROLOG;

"<!--"                  BEGIN(COMMENT);     /* Comments will be ignore */
<COMMENT>.|\r|\n        ;
<COMMENT>"-->"          BEGIN(INITIAL);

"<![CDATA["             BEGIN(CDATA);       /* We will ignore for the moment CDATA. I don't think we'll need it */
<CDATA>.|\r|\n          ;
<CDATA>"]]>"            BEGIN(INITIAL);

\>[^A-Z|^a-z|^0-9]*\<   unput ('<');return GRT;
\<                      return LSS;
\>                      return GRT;
\<\/                    return LSS_SLSH;
[^<>]+                  {
                            snprintf(xmllval.str, MAXXMLVALUELEN, "%s", yytext);
                            return WORD;
                        }

%%


int run_xmlparser(FILE *ifile, treenode *tree)
{

    if (tree==NULL)
    {
        cerr << "Parameter error\n";
        return 1;
    }

    /* 1. Set the input stream */
    //xml_scan_buffer((char *)base, size);
    xmlin=ifile;


    /* 2. Parse xml structure */

    xmlparse();

    if (rootnode!=NULL)
    {
        *tree = *rootnode;
        delete rootnode;
    }
    else
    {
        return 1;
    }

    return 0;
}

void xmlerror(const char *str)
{
    cerr << "Error parsing input file: " << str << " at line " << xmllineno << endl;
}
