/****************************************************************************
|*
|* tap3xml: both directions XML<->TAP3 Converter.
|* 
|* $Id: NrtInfo.cpp,v 1.4 2012/04/15 11:49:46 javier Exp $
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
|* Module: NrtInfo.cpp
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

#include "NrtInfo.h"


/* 2. Global Variables */


NrtInfo::NrtInfo() 
{
    _version=-1;
    _release=-1;
}


void NrtInfo::buffin(Buffin *bfin)
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
        pair<string,string>("61",      "NRT"),    /* Nrt file */
    };

    for(i=0;i<2;i++)
    {
        if (str.substr(0,types[i].first.size())==types[i].first)
        {
            _type=types[i].second;
        }
    }


    /* 2. Find its version */

    pair<string, int *> version[] =
    {
        pair<string,int *>("5f2901",  &_version),
        pair<string,int *>("5f2501",  &_release)
    };

    for(i=0;i<2;i++)
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

    if (_version<0 || _release<0)
        _type="ERR";


    _bfin->set_pos(bfin_pos);

}

void NrtInfo::_findtreeversion(treenode *tree, int depth)
{
    vector<treenode *>::iterator    vii;
    int                 i;

    if (depth>2)
        return;

    pair<string, string> types[] =
    {
        pair<string,string>("Nrtrde",          "NRT")       /* Nrt file */
    };

    pair<string, int *> version[] =
    {
        pair<string,int *>("specificationVersionNumber",    &_version),
        pair<string,int *>("releaseVersionNumber",          &_release)
    };

    for (i=0;i<1;i++)
    {
        if (tree->tagname==types[i].first)
        {
            _type=types[i].second;
        }
    }

    if (tree->children!=NULL)
    {
        for (vii=tree->children->begin();vii!=tree->children->end();vii++)
        {
            for (i=0;i<2;i++)
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
    

void NrtInfo::tree(treenode *tree)
{
    _version=_release=-1;
    _type="ERR";
    _findtreeversion(tree, 0);

}

int NrtInfo::version()
{
    return _version;
}

int NrtInfo::release()
{
    return _release;
}

string NrtInfo::type()
{
    return _type;
}

NrtInfo::~NrtInfo() {}

