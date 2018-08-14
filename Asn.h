/****************************************************************************
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
|* Module: Asn.h
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

#ifndef __Asn_h__
#define __Asn_h__

#define _CRT_SECURE_NO_WARNINGS

/* 1. Includes */

#pragma warning(disable: 4786) /* To clean compile under MSVC */

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>

#include <sstream>
#include <iomanip>

#include <stdio.h>

#include "global.h"
#include "Buffin.h"
#include "AsnItem.h"
#include "TreeParser.h"


/* 2. Defining namespace */

using namespace std;


/* 3. Typedefs and structures */


/* 4. Prototypes */

class Asn
{
    private:

        Buffin*     _bfin;              /* Where the buffer is being read from */
        Buffout*    _bfout;             /* Where the buffer is being written to */
        AsnItem*    _asnitem;           /* To decode/encode Tag and Size */
        treenode*   _tree;              /* Tree to store the whole ASN structure */

        FILE*       _ifile;             /* If _bfin is not used we can use _ifile */
        FILE*       _ofile;             /* If _bfout is not used we can use _ofile */

        FILE*       _specfile;          /* Specification file to give to the parser */
        FILE*       _specsuplfile;      /* Specification of the supplementary file to give to the parser */
                                        /* Used for RAP files when needed also the TAP specification */
        string      _spec_rootnode;     /* First node of the Tree */

        map<string,tagtype_s *>     _spec_tree;

        long        _decode         (tagtype_s *tagtype, treenode *tree, long size, bool is_indef);
        void        _compile_spec   (map<string,tagtype_s *> *tagtype_pool);
        void        _decode_spec    ();

        long        _compile_tree   (tagtype_s *tagtype, treenode *tree);

        void        _free_spec_tree (map<string,tagtype_s *> &spec_tree);

        void        _dump_spec_tree (map<string,tagtype_s *> &spec_tree);

        void        _dump_asn_tree (treenode *tree, int depth);





    public:

        Asn();

        void decode();
        void encode();
        void ofile(const string &filename);
        void ifile(const string &filename);
        void buffin(Buffin *bfin);
        void buffout(Buffout *bfout);
        void tree(treenode *tree);
        void specfile(const string &filename);
        void specsuplfile(const string &filename);
        void specroot(const string spec_rootnode);

        ~Asn();
};

#endif
