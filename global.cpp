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
|* Module: global.cpp
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
*******************************************************************************/


/* 1. Includes */

#include "global.h"


/* 2. Global Variables */

enc_dec_funcs edf[]=
{
    { &bcd2ascii     , &ascii2bcd },
    { &bcd2dec       ,   &dec2bcd },
    { &bcd2hexa      ,  &hexa2bcd },
    { &bcd2hexa_no_f ,  &hexa2bcd }
};


/* Note: because windows does not support map initialization */
/*       the initialization was done into main()             */

map<string, int> simpletypes_map;
map<int, enc_dec_funcs> simpletypes;


/* 3. Class methods */

void treenode::tree_dump()
{
    _tree_dump(this, 0);
}

void treenode::_tree_dump(treenode *tree, int depth)
{
    vector<treenode *>::iterator    vii;
    (void)depth;

    if (tree!=NULL)
    {
        cout << "tagname:   " << tree->tagname << endl;
        cout << "cl:        " << tree->cl << endl;
        if (tree->value!=NULL)
        {
            cout << "value:     " <<  tree->value << endl;
        }
        cout << endl;

        if (tree->children!=NULL)
        {
            for (vii=tree->children->begin();vii!=tree->children->end();vii++)
            {
                _tree_dump(*vii, 0);
            }

        }

    }

}



/******************************************************************************
|* 
|* Function: err_trace
|* 
|* Description: 
|* 
|*     print error messages
|* 
|* Return:
|* 
|*     void
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050908    JG    Initial version
|* 
|* 
******************************************************************************/

void err_trace(const string &file, const int &line)
{

    /* 1. Print line and file where error ocurr */

    cerr << "File: " << file << " Line: " << line << endl ;


    /* 2. Termination */

    exit(1);

}


/******************************************************************************
|* 
|* Function: bcd2ascii
|* 
|* Description: 
|* 
|*     converts a bcd chain into a ascii string
|* 
|* Return:
|* 
|*     the converted string
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050928    JG    Initial version
|* 
|* 
******************************************************************************/

string bcd2ascii(const string &str)
{
    return str;
}


/******************************************************************************
|* 
|* Function: bcd2int
|* 
|* Description: 
|* 
|*     converts a bcd chain into an integer
|* 
|* Return:
|* 
|*     the converted integer as a string
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050928    JG    Initial version
|* 
|* 
******************************************************************************/

string bcd2int(const string &str)
{
    ostringstream value_str;

    int value=0;
    string::const_iterator cii;

    for (cii=str.begin();cii!=str.end();cii++)
    {
        value+=(int)(uchar)*cii;
    }

    value_str << value;

    return value_str.str();

}


/******************************************************************************
|* 
|* Function: bcd2hexa_no_f
|* 
|* Description: 
|* 
|*     converts a bcd chain to a hexa string and remove the filler
|* 
|* Return:
|* 
|*     the converted string
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20110324    JG    Initial version
|* 
******************************************************************************/

string bcd2hexa_no_f(const string &str)
{

    ostringstream value;


    string::const_iterator cii;
    for (cii=str.begin();cii!=str.end();cii++)
    {
        if (cii+1 == str.end() && (int)(uchar)(*cii & 0x0F) == 15) /* 15 = ascii of 'f' */
        {
            value << hex << setw(1) << (int)(uchar)((*cii >> 4) & 0x0F); /* Remove the last byte */
        } 
        else
        {
            value << hex << setw(2) << setfill('0') << (int)(uchar)*cii;
        }
    }


    return value.str();

}


/******************************************************************************
|* 
|* Function: bcd2hexa
|* 
|* Description: 
|* 
|*     converts a bin chain to a hexa string
|* 
|* Return:
|* 
|*     the converted string
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20110324    JG    Initial version
|* 
|* 
******************************************************************************/

string bcd2hexa(const string &str)
{

    ostringstream value;


    string::const_iterator cii;
    for (cii=str.begin();cii!=str.end();cii++)
    {
        value << hex << setw(2) << setfill('0') << (int)(uchar)*cii;
    }


    return value.str();

}

/******************************************************************************
|* 
|* Function: bcd2dec
|* 
|* Description: 
|* 
|*     converts a bcd chain to a decimal value
|* 
|* Return:
|* 
|*     the converted string
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20051007    JG    Initial version
|* 
|* 
******************************************************************************/
string bcd2dec(const string &str)
{
    ostringstream value_str;
    unsigned long long value=0;
    /* int sign; */

    string::const_iterator cii;

    /* sign=str.at(0)>>7 ? -1 : 1; */

    for (cii=str.begin();cii!=str.end();cii++)
    {
        value<<=8;
        value+=(int)(uchar)*cii;
        
    }

    /* We'll skip there's no sign on TAP/NRT
    if (sign == -1)
        value-=(int)(0x01<<(8*str.size()));

    */

    value_str << value;

    return value_str.str();
}


/******************************************************************************
|* 
|* Function: ascii2bcd
|* 
|* Description: 
|* 
|*     converts a ascii string to a bcd chain
|* 
|* Return:
|* 
|*     the converted string
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050928    JG    Initial version
|* 
|* 
******************************************************************************/

string ascii2bcd(const string &str)
{
    return str;
}


/******************************************************************************
|* 
|* Function: int2bcd
|* 
|* Description: 
|* 
|*     converts an integer to a bcd chain
|* 
|* Return:
|* 
|*     the converted string
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050928    JG    Initial version
|* 
|* 
******************************************************************************/

string int2bcd(const string &str)
{

    ostringstream bcd;
    int intstr;


    intstr=atoi(str.c_str());


    while (intstr>0)
    {
        intstr>255 
            ? bcd << (uchar)255
            : bcd << (uchar)(intstr&0xFF);
        intstr-=255;
    }

    return bcd.str();
}


/******************************************************************************
|* 
|* Function: hexa2bcd
|* 
|* Description: 
|* 
|*     converts a hexa string to a bcd chain
|*
|* Return:
|* 
|*     the converted string
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050928    JG    Initial version
|* 
|* 
******************************************************************************/

string hexa2bcd(const string &str)
{
    ostringstream bcd;
    int i;

    for (i=0; i<(int)str.size();i+=2)
        bcd << (uchar)strtol((str.substr(i,2).size()==1 ? str.substr(i,2)+"F" : str.substr(i,2)).c_str(),NULL, 16);

    return bcd.str();

}


/******************************************************************************
|* 
|* Function: dec2bcd
|* 
|* Description: 
|* 
|*     converts a decimal value into a bcd chain
|* 
|* Return:
|* 
|*     the converted string
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20051007    JG    Initial version
|* 
|* 
******************************************************************************/
string dec2bcd(const string &str)
{
    ostringstream bcd, rbcd;
    unsigned long long intstr, msb;
    string bcd_str;


    /* Note: For the moment we will not support numbers bigger than 2**31 */

    intstr=atoll(str.c_str());

    do
    {

        bcd << (uchar)(intstr&0xFF);
        msb=intstr&0x80;
        intstr>>=8;
        
    } while (intstr>0);

    if (msb && ( atoll(str.c_str()) > 0))
        bcd << (uchar)0x00;

    if (!msb && ( atoll(str.c_str()) < 0))
        bcd << (uchar)0xFF;

    bcd_str=bcd.str();
    
    string::reverse_iterator rii=bcd_str.rbegin();
    while (rii!=bcd_str.rend())
    {
        rbcd << *rii++;
    }

    return rbcd.str();
    
}


/******************************************************************************
|* 
|* Function: apply_xml_rules
|* 
|* Description: 
|* 
|*     apply to certain string the W3C XML rules.
|*
|* Return:
|* 
|*     the converted string
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20051005 JG    Initial version
|* 20060710 JG  Add ";" to the metacharacters like "&amp"
|* 
|* 
******************************************************************************/
string apply_xml_rules(string str)
{
    int pos, i;


    static string repl_items[][2]={
         { "\r\n",              "\n" }
        ,{ "<",                 "%%$tap3edit$%%lt;" }
        ,{ ">",                 "%%$tap3edit$%%gt;" }
        ,{ "'",                 "%%$tap3edit$%%apos;" }
        ,{ "\"",                "%%$tap3edit$%%quot;" }
        ,{ "&",                 "%%$tap3edit$%%amp;" }
        ,{ "%%$tap3edit$%%",    "&" }
    };

    for (i=0;i<7;i++)
    {
        while ((pos=str.find(repl_items[i][0])) != (int)string::npos)
        {
            str.replace(str.find(repl_items[i][0]),repl_items[i][0].size(),repl_items[i][1]);
        }
    }

    return str;

}



/******************************************************************************
|* 
|* Function: remove_xml_rules
|* 
|* Description: 
|* 
|*     remove from certain string the W3C XML rules.
|*
|* Return:
|* 
|*     the converted string
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20051005    JG    Initial version
|* 20060710 JG  Add ";" to the metacharacters like "&amp"
|* 
|* 
******************************************************************************/
string remove_xml_rules(string str)
{
    int pos, i;


    string repl_items[][2]={
         { "&lt;",      "<" }
        ,{ "&gt;",      ">" }
        ,{ "&amp;",     "&" }
        ,{ "&apos;",    "'" }
        ,{ "&quot;",    "\"" }
    };

    for (i=0;i<5;i++)
    {
        while ((pos=str.find(repl_items[i][0])) != (int)string::npos)
        {
            str.replace(str.find(repl_items[i][0]),repl_items[i][0].size(),repl_items[i][1]);
        }
    }

    return str;

}



/******************************************************************************
|* 
|* Function: free_treenode
|* 
|* Description: 
|* 
|*     free's the memory allocated when creating the tree of treenode's
|*
|* Return:
|* 
|*     N/A
|* 
|* Author:
|* 
|*     Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20120415    JG    Initial version
|* 
|* 
******************************************************************************/
void free_treenode(treenode *tree)
{
    vector<treenode *>::iterator    vii;

    if (tree!=NULL)
    {
        if (tree->value!=NULL)
        {
            delete tree->value;
        }

        if (tree->children!=NULL)
        {
            for (vii=tree->children->begin();vii!=tree->children->end();vii++)
            {
                free_treenode(*vii);
            }

            delete tree->children;
        }

        delete tree;
    }
}
