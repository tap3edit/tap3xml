%{
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
