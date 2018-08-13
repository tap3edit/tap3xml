/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: TapInfo.cpp,v 1.12 2012/04/15 11:49:46 javier Exp $
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
|* Module: TapInfo.cpp
|*
|* Description: 
|*
|* Author: Javier Gutierrez (JG)
|*
|* Modifications:
|*
|* When         Who     Pos.    What
|* 20051026     JG              Initial version
|* 20060505     JG              When Version of tap file in the rap file
|*                              is not provided, we decode with default
|*                              values (XML)
|* 20060510     JG              Same but for ASN.1 Files
|* 20060510     JG              Solved problem identifying XML 
|*                              Acknowledgment files
|*
****************************************************************************/

/* 1. Includes */

#include "TapInfo.h"


/* 2. Global Variables */


TapInfo::TapInfo() 
{
    _version=-1;
    _release=-1;
    _suplversion=-1;
    _suplrelease=-1;
}


void TapInfo::buffin(Buffin *bfin)
{
    string str;
    int pos, bfin_pos, i;

    _bfin=bfin;
    bfin_pos=_bfin->get_pos();
    _bfin->set_pos(0);
    _bfin->read(_bfin->get_end() > 500 ? 500 : _bfin->get_end());

    str=bcd2hexa(_bfin->get_buf());


    /* 1. Find out which kind of file it is */

    pair<string, string> types[] =
    {
        pair<string,string>("",        "ERR"),    /* Not recognized file */
        pair<string,string>("61",      "TAP"),    /* Tap file */
        pair<string,string>("62",      "TAP"),    /* Notification file */
        pair<string,string>("7f8416",  "RAP"),    /* Rap file */
        pair<string,string>("7f8417",  "ACK")    /* Acknowledgment file */
    };

    for(i=0;i<5;i++)
    {
        if (str.substr(0,types[i].first.size())==types[i].first)
        {
            _type=types[i].second;
        }
    }


    /* 2. Find its version */

    pair<string, int *> version[] =
    {
        pair<string,int *>("5f814901",  &_version),
        pair<string,int *>("5f813d01",  &_release),
        pair<string,int *>("5f842001",  &_suplversion),
        pair<string,int *>("5f841f01",  &_suplrelease)
    };

    if (_type=="ACK" )
    {
        _version=1;
        _release=1;
        _suplversion=3;
        _suplrelease=1;
        _type="RAP";
    }
    else
    {
        for(i=0;i<4;i++)
        {
            if ((pos=str.find(version[i].first)) != (int)string::npos)
            {
                *(version[i].second)=atoi(bcd2dec(hexa2bcd(str.substr(pos+version[i].first.size(),2))).c_str());
            }
            else
            {
                *(version[i].second)=-1;
            }
        }

        if (_type=="RAP")
        {
            i=_version;
            _version=_suplversion;
            _suplversion=i;

            i=_release;
            _release=_suplrelease;
            _suplrelease=i;

            if ( _suplversion < 0 )
                _suplversion=3;

            if ( _suplrelease < 0 )
                _suplrelease=1;

        }

        if (_version<0 || _release<0)
            _type="ERR";

    }

    _bfin->set_pos(bfin_pos);

}

void TapInfo::_findtreeversion(treenode *tree, int depth)
{
    vector<treenode *>::iterator    vii;
    int                 i;

    if (depth>2)
        return;

    pair<string, string> types[] =
    {
        pair<string,string>("transferBatch",   "TAP"),      /* Tap file */
        pair<string,string>("notification",    "TAP"),      /* Notification file */
        pair<string,string>("returnBatch",     "RAP"),      /* Rap file */
        pair<string,string>("acknowledgement", "ACK")       /* Acknowledgment file */
    };

    pair<string, int *> version[] =
    {
        pair<string,int *>("specificationVersionNumber",    &_version),
        pair<string,int *>("releaseVersionNumber",          &_release),
        pair<string,int *>("rapSpecificationVersionNumber", &_suplversion),
        pair<string,int *>("rapReleaseVersionNumber",       &_suplrelease)
    };

    for (i=0;i<4;i++)
        if (tree->tagname==types[i].first)
            _type=types[i].second;

    if (tree->children!=NULL)
    {
        for (vii=tree->children->begin();vii!=tree->children->end();vii++)
        {
            for (i=0;i<4;i++)
            {
                if ((*vii)->tagname==version[i].first)
                    if ( (*vii)->value!=NULL )
                        *(version[i].second)=atoi((*(*vii)->value).c_str());
            }
        }
    }

    if (_version<0)
    {
        if (tree->children!=NULL)
        {
            _findtreeversion((*tree->children)[0],depth+1);
        }
    }


    return;

}
    

void TapInfo::tree(treenode *tree)
{
    int i;

    _version=_release=_suplversion=_suplrelease=-1;
    _type="ERR";
    _findtreeversion(tree, 0);

    if (_type=="RAP")
    {
        i=_version;
        _version=_suplversion;
        _suplversion=i;

        i=_release;
        _release=_suplrelease;
        _suplrelease=i;

        if ( _suplversion < 0 )
            _suplversion=3;

        if ( _suplrelease < 0 )
            _suplrelease=1;

    }

    if (_type=="ACK" )
    {
        _version=1;
        _release=1;
        _suplversion=3;
        _suplrelease=1;
        _type="RAP";
    }
}

int TapInfo::version()
{
    return _version;
}

int TapInfo::release()
{
    return _release;
}

int TapInfo::suplrelease()
{
    return _suplrelease;
}

int TapInfo::suplversion()
{
    return _suplversion;
}

string TapInfo::type()
{
    return _type;
}

TapInfo::~TapInfo() {}

