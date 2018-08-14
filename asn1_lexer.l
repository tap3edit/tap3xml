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

