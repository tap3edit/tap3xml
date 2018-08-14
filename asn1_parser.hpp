/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_ASN1_ASN1_PARSER_HPP_INCLUDED
# define YY_ASN1_ASN1_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int asn1debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    WORD = 258,
    CLASS = 259,
    ELIPSYS = 260,
    LBRACE = 261,
    RBRACE = 262,
    ASSIGN = 263,
    COMMA = 264,
    SEQUENCE = 265,
    OF = 266,
    OPTIONAL = 267,
    CHOICE = 268,
    BGN = 269,
    END = 270,
    DEFINITIONS = 271,
    IMPLICIT = 272,
    TAGS = 273,
    IMPORTS = 274,
    FROM = 275,
    SEMICOLON = 276
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 64 "asn1_parser.ypp" /* yacc.c:1909  */

    char                    str[MAXXMLVALUELEN];
    int                     num;
    tagtype_s*              tagtype;
    tagelem_s*              tagelem;
    vector<tagelem_s *>*    vect;

#line 84 "asn1_parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE asn1lval;

int asn1parse (void);

#endif /* !YY_ASN1_ASN1_PARSER_HPP_INCLUDED  */
