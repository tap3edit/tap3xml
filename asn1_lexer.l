%{
/******************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: asn1_lexer.l,v 1.14 2013/05/12 14:14:03 javier Exp $
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
|* Module: asn1_lexer.l
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

#pragma warning(disable: 4786) /* To clean compile under MSVC */

#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<map>
#include"global.h"
using namespace std;

#include"asn1_parser.hpp"

extern int                      asn1parse();
extern map<string,tagtype_s *>  *spec_tree;

int                             asn1lex(void);
FILE*                           asn1_2nd_in=NULL;


%}

%s APPL
%option never-interactive
%option yylineno
%%

"APPLICATION "      BEGIN APPL;

"SEQUENCE"          return SEQUENCE;

"OF"                return OF;

"OCTET STRING"      {
                        snprintf(asn1lval.str, MAXXMLVALUELEN, "%s", yytext);
                        return WORD;
                    }


"CHOICE"            return CHOICE;

"OPTIONAL"          return OPTIONAL;

"BEGIN"             return BGN;

"END"               return END;

DEFINITIONS         return DEFINITIONS;

IMPLICIT            return IMPLICIT;

TAGS                return TAGS;

IMPORTS             return IMPORTS;

FROM                return FROM;

[a-zA-Z][a-zA-Z0-9-]+   {
                        snprintf(asn1lval.str, MAXXMLVALUELEN, "%s", yytext);
                        return WORD;
                    }

"..."               return ELIPSYS;

    <APPL>[0-9]+    {
                        asn1lval.num = atoi(yytext);
                        return CLASS;
                    }

\{                  return LBRACE;
\}                  return RBRACE;
::=                 return ASSIGN;

\,                  return COMMA;

\;                  return SEMICOLON;

[ \t\[\]\n]+      /* ignore whitespace, tabs and brackets */
[--].*\n            /* ignore comments */
\(.*\)              /* ignore (SIZE(X)) */

remove_unput_warn   unput('n');

%%


int asn1wrap()
{

    if (asn1_2nd_in!=NULL)
    {
        asn1in=asn1_2nd_in;
        asn1_2nd_in=NULL;
        return 0;
    }
    return 1;

}


int run_asn1parser(FILE *_specfile, FILE *_specsuplfile, map<string,tagtype_s *> *_spec_tree)
{

    if (_spec_tree==NULL)
    {
        cerr << "Parameter error\n";
        return 1;
    }

    spec_tree=_spec_tree;

    asn1in=_specfile;
    asn1_2nd_in=_specsuplfile;
    asn1parse();

    return 0;

}

void asn1error(const char *str)
{
    cerr << "Error parsing input file: " << str << " at line " << asn1lineno << endl;
}

