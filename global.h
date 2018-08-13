/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: global.h,v 1.17 2013/05/12 14:15:44 javier Exp $
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
|* Module: global.h
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

#ifndef __global_h__
#define __global_h__


/* 1. Includes and defines */

#pragma warning(disable: 4786) /* To clean compile under MSVC */

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>

#define TAP3XML_SPEC_VAR "TAP3XML_SPEC"


/* 2. Defines and namespace */

using namespace std;

#define MAXTAGINFO 560
#define MAXTAGTYPE 6
#define MAXXMLVALUELEN 512

#if defined(_MSC_VER)
#   define atoll _atoi64
#   define snprintf _snprintf_s
#endif


/* 3. Typedefs and structures */



enum    action_e {choice, sequence, simpletype, sequenceof};

struct  tagelem_s;

struct  tagtype_s
{
    string                  type_name;
    string                  type_type;
    int                     tagid;
    enum action_e           action;
    bool                    checked;
    bool                    modified;
    vector<tagelem_s *>*    children;

    tagtype_s() :
        type_name(""),
        type_type(""),
        tagid(-2),
        action(choice),
        checked(false),
        modified(false),
        children(NULL)
    {};

};

struct    tagelem_s
{
    string              elem_name;
    string              elem_type;
    bool                optional;
    struct tagtype_s*   elem_type_p;

    tagelem_s() :
        elem_name(""),
        elem_type(""),
        optional(false),
        elem_type_p(NULL)
    {};
};


struct enc_dec_funcs
{
    string  (*decode)(const string &str);
    string  (*encode)(const string &str);

};



typedef unsigned char uchar;


struct treenode
{
    private:
        void _tree_dump(treenode *tree, int depth);

    public: 

        string                  tagname;
        short unsigned int      cl: 2;      /* Class */
        short unsigned int      pc: 1;      /* Primitive/Constructed */
        short unsigned int      type: 5;    /* If primitive type, the type in simpletypes_map.*/
        short unsigned int      indef: 1;   /* If indefinite length or not, 1: Yes, 0: No */
        /* We support int just 5 bits= 0-31 */
        int                     tag_d;      /* Tag: decimal format */
        uchar                   tag_x[4];   /* Tag: bcd format */
        int                     tag_l;      /* Tag: length in bytes */
        long                    size_d;     /* Size: decimal format */
        uchar                   size_x[5];  /* Size: bcd format */
        int                     size_l;     /* Size: length in bytes */
        string*                 value;      /* Value of the asn item */
        vector<treenode *>*     children;

        treenode() :
            tagname(""),
            cl(0),
            pc(0),
            type(0),
            tag_d(0),
            tag_l(0),
            size_d(0),
            size_l(0),
            value(NULL),
            children(NULL)
    {};

        void tree_dump();
};



/* 4. Prototypes */

void    err_trace           (const string &file, const int &line);
void    tagname_init        (void);
string  bcd2ascii           (const string &str);
string  bcd2int             (const string &str);
string  bcd2hexa            (const string &str);
string  bcd2hexa_no_f       (const string &str);
string  bcd2dec             (const string &str);
string  ascii2bcd           (const string &str);
string  int2bcd             (const string &str);
string  hexa2bcd            (const string &str);
string  dec2bcd             (const string &str);
string  apply_xml_rules     (const string &str);
string  remove_xml_rules    (const string &str);
void    free_treenode       (treenode *tree);

#endif
