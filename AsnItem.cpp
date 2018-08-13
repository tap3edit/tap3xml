/******************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: AsnItem.cpp,v 1.10 2012/04/15 11:49:46 javier Exp $
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
|* Module: AsnItem.cpp
|*
|* Description: 
|*
|* Author: Javier Gutierrez (JG)
|*
|* Modifications:
|*
|* When         Who     Pos.    What
|* 20050910     JG              Initial version
|*
*******************************************************************************/

/* 1. Includes */

#include "AsnItem.h"


/* 2. Global Variables */


AsnItem::AsnItem() 
{
    _tree=NULL;
    _bfin=NULL;
    _bfout=NULL;
}

void AsnItem::decode()
{
    int i;
    string buffin;

    if (!_tree)
    {
        cerr << "treenode structure was not set with tree() method";
        err_trace(__FILE__, __LINE__);
    }

    if (!_bfin)
    {
        cerr << "buffin was not set with buffin() method";
        err_trace(__FILE__, __LINE__);
    }

    _tree->tag_d=0;
    _tree->tag_l=0;
    memset(_tree->tag_x,  '\0', 4);

    _tree->size_d=0;
    _tree->size_l=0;
    _tree->indef=0;
    memset(_tree->size_x, '\0', 5);


    /* 1. Decode Tag */

    {

        /* 1.1. Read first byte of tag */

        _bfin->read(1);
        buffin=_bfin->get_buf();


        /* 1.2. Store class, primitive/constructed info and hexa tag */

        _tree->cl=buffin[0]>>6;
        _tree->pc=(buffin[0]>>5)&0x1;
        _tree->tag_x[0]=(uchar)buffin[0];
        _tree->tag_l=1;


        /* 1.3. Work according to number of tag octets */

        if ( ( buffin[0]&0x1F ) == 0x1F )
        {
            /* 1.3.1 Tag has more than one octet */


            for(i=1;i<=4;i++)
            {
                _bfin->read(1);
                buffin=_bfin->get_buf();

                _tree->tag_d<<=7;
                _tree->tag_d+=(int)(((uchar)buffin[0])&0x7F);
                _tree->tag_x[i]=(uchar)buffin[0];
                _tree->tag_l+=1;

                if ( ! (buffin[0]>>7) )
                    break;

            }

            if ( i>3 )
            {
                cerr << "Found tag bigger than 4 bytes at file position: " << _bfin->get_pos();
                err_trace(__FILE__, __LINE__);
            }

        } 
        else
        {
            /* 1.3.2 Tag has just one octet */

            _tree->tag_d=(int)buffin[0]&0x1F;
        }

    }


    /* 2. Decode Size */

    {

        /* 2.1. Read first byte of tag */

        _bfin->read(1);
        buffin=_bfin->get_buf();


        /* 2.2. Storing size_x */

        _tree->size_x[0]=buffin[0];
        _tree->size_l=1;


        /* 2.3. Work according the number of octets */

        if (buffin[0]>>7)
        {
            /* 2.3.1. Size with more than one octet */

            for(i=1;(i<=(int)(_tree->size_x[0]&0x7F)) && (i<=4);i++)
            {
                _bfin->read(1);
                buffin=_bfin->get_buf();

                _tree->size_d<<=8;
                _tree->size_d+=(int)((uchar)buffin[0]);
                _tree->size_x[i]=buffin[0];
                _tree->size_l+=1;
            }

            if ( i>7 )
            {
                cerr << "Found size bigger than 8 bytes at file position: " << _bfin->get_pos();
                err_trace(__FILE__, __LINE__);
            }

            /* 2.3.2. If the size is still 0, then it's a tag with indefinite length */
             
            if (_tree->size_d == 0)
            {
                _tree->indef = 1;
            }

        }
        else
        {
            /* 2.3.3. Size with just one octet */

            _tree->size_d=(int)(buffin[0]);

        }

    }

}

string AsnItem::encode(int depth)
{

    long                            tmp_int;
    int                             i=1, j;
    ostringstream                   enc_str;
    string                          str, enc;
    extern map<int, enc_dec_funcs>  simpletypes;

    (void) depth; // not used

    if (!_tree)
    {
        cerr << "treenode structure was not set with tree() method";
        err_trace(__FILE__, __LINE__);
    }

    if (!_tree->tag_d)
        return "";

    _tree->tag_l=0;
    memset(_tree->tag_x,  '\0', 4);

    _tree->size_l=0;
    memset(_tree->size_x, '\0', 5);


    /* 1. Build Tag since _tree->tag_x is not mandatory */

    _tree->cl=1;
    _tree->tag_x[0]=(_tree->cl<<6)+(_tree->pc<<5)+(_tree->tag_d&0x1F);
    _tree->tag_l=1;

    if ( _tree->tag_d >> 5 || _tree->tag_d == 0x1F)
    {
        /* 1.1 Tag should have more than one octet */

        if ( _tree->tag_d >> 26 )
        {
            /* Note: we need to count just the bits used by the tag  */
            /*       without clase, primitive/contructed and so on.  */
            /*       That is why we take 26 and not 32.              */

            cerr << "Found tag bigger than 4 bytes. Tag:" << _tree->tag_d;
            err_trace(__FILE__, __LINE__);
        }

        _tree->tag_x[0]=(_tree->cl<<6)+(_tree->pc<<5)+(0x1F);

        tmp_int=_tree->tag_d;

        while (tmp_int)
        {
            for(j=i++;j>0;j--)
                _tree->tag_x[j]=_tree->tag_x[j-1];
                
            _tree->tag_x[1]=(uchar)((1<<7)+(tmp_int&0x7F));
            tmp_int>>=7;
        }

        _tree->tag_x[i-1]&=0x7F;
        _tree->tag_l=i;

    }


    /* 2. Build Size */

    if ( _tree->size_d >> 7 )
    {
        /* Then size should have more than one octet */


        tmp_int=_tree->size_d;
        i=1;

        while (tmp_int)
        {
            for(j=i++;j>0;j--)
                _tree->size_x[j]=_tree->size_x[j-1];
                
            _tree->size_x[1]=(uchar)tmp_int&0xFF;
            tmp_int>>=8;
        }

        _tree->size_x[0]=(uchar)((1<<7)+(i-1));
        _tree->size_l=i;
        
    }
    else
    {
        /* Size should have just one octet */

        _tree->size_x[0]=(uchar)_tree->size_d;
        _tree->size_l=1;

    }


    /* 3. Prepare encoded String */

    if (!_tree->pc )
    {
        str.assign((char *)_tree->tag_x,_tree->tag_l);
        enc+=str;
        str.assign((char *)_tree->size_x,_tree->size_l);
        enc+=str;
        enc+=(*simpletypes[_tree->type].encode)(*_tree->value);
    }
    else
    {
        str.assign((char *)_tree->tag_x,_tree->tag_l);
        enc+=str;
        str.assign((char *)_tree->size_x,_tree->size_l);
        enc+=str;
    }

    return enc;

}

string AsnItem::enc_close(int depth)
{

    /* ASN.1 does not use any close statement for tags.    */
    /* You know when a tag finishes, from its size.        */
    /* Anyway, this could be useful to create files with    */
    /* indefinite length. \0\0 Should come here.        */

    (void) depth;
    return string("");
}

void AsnItem::buffin(Buffin *bfin)
{
    _bfin=bfin;
}

void AsnItem::buffout(Buffout *bfout)
{
    _bfout=bfout;
}

void AsnItem::tree(treenode *tree)
{
    _tree=tree;

}


AsnItem::~AsnItem() {}
