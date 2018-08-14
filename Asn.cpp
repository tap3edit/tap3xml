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
|* Module: Asn.cpp
|*
|* Description:
|*
|* Author: Javier Gutierrez (JG)
|*
|* Modifications:
|*
|* When         Who     Pos.    What
|* 20050909     JG              Initial version
|* 20060909     JG      _001_   Bug when collecting indefinite lengths
|*
*******************************************************************************/

/* 1. Includes */

#include "Asn.h"
#include <stdio.h>


/* 2. Global Variables */


/****************************************************************************
|* 
|* Method: Asn Constructor
|* 
|* Description: 
|* 
|*     Constructor
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
Asn::Asn() :
    _bfin(NULL),
    _bfout(NULL),
    _asnitem(NULL),
    _tree(NULL),
    _ifile(NULL),
    _ofile(NULL),
    _specfile(NULL),
    _specsuplfile(NULL)
{
    _asnitem=new AsnItem();
}

/****************************************************************************
|* 
|* Method: buffin
|* 
|* Description: 
|* 
|*     Sets and initializes the input buffer
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::buffin(Buffin *bfin)
{
    _bfin=bfin;
    _asnitem->buffin(_bfin);
}

/****************************************************************************
|* 
|* Method: buffout
|* 
|* Description: 
|* 
|*     Initializes the output buffer
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::buffout(Buffout *bfout)
{
    _bfout=bfout;
    _asnitem->buffout(_bfout);
}

/****************************************************************************
|* 
|* Method: _decode_spec
|* 
|* Description: 
|* 
|*     Decodes the ASN.1 Specifications and stores them into _spec_tree
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Private
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::_decode_spec()
{

    /* 1. Parse Specification */

    if (_specfile==NULL)
    {
        cerr << "Error. Specification File must be specified with method \"specfile\"";
        err_trace(__FILE__, __LINE__);
    }

    extern  int run_asn1parser(FILE *_specfile, FILE *_specsuplfile, map<string,tagtype_s *> *spec_tree);

    if (run_asn1parser(_specfile, _specsuplfile, &_spec_tree))
    {
        cerr << "Error decoding ASN input file\n";
        err_trace(__FILE__, __LINE__);
    }


    /* 2. Compile */

    _compile_spec(&_spec_tree);

}

/****************************************************************************
|* 
|* Method: decode
|* 
|* Description: 
|* 
|*     Prepares for decoding input file. Invokes _decode()
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::decode()
{

    /* 1. Decode Specification */

    _decode_spec();


    /* 2. Prepare Tree to decode */

    if (!_tree)
    {
        cerr << "treenode structure was not set with tree() method";
        err_trace(__FILE__, __LINE__);
    }

    if (!_spec_rootnode.size())
    {
        cerr << "Error. The root node should be specified with the method \"specroot\"";
        err_trace(__FILE__, __LINE__);
    }

    _tree->pc=1;
    _tree->children=NULL;
    _tree->tagname=_spec_rootnode;
    _tree->indef = false;
    _tree->size_d = _bfin->get_end();


    /* 3. Read initial tag if needed */

    _bfin->set_pos(0);

    if ( _spec_tree[_spec_rootnode]->tagid )
    {
        /* The rootnode could have its own tag but not necessarily (like in case of CHOICE)  */
        /* In case it has, we read it here, because in _decode() we assume that the rootnode */
        /* is already decoded. E.g. "DataInterChange" has no own tag, but "Nrtrde" has.      */

        /* 3.1 Eat up the first tag */

        treenode *child=new treenode();

        _asnitem->tree(child);
        _asnitem->decode();

        if (_spec_tree[_spec_rootnode]->tagid == child->tag_d)
        {

            /* 3.1.1. Tag matches */

            _tree->tag_d = child->tag_d;
            _tree->size_d = child->size_d;
            _tree->indef = child->indef;
        }
        else
        {
            /* 3.1.2. Tag does not match */

            delete child; 
            child = NULL;

            cerr << "Error: File does not match the specifications";
            cerr << " tag: " << child->tag_d << " pos: " << _bfin->get_pos() << endl;
            err_trace(__FILE__, __LINE__);
        }

        delete child; 
        child = NULL;

    }

    /* 4. Decode file */

    _decode(_spec_tree[_spec_rootnode], _tree, _tree->size_d, _tree->indef);

}

/****************************************************************************
|* 
|* Method: _decode
|* 
|* Description: 
|* 
|*     Decodes the input file validating its structure with the ASN.1 Specs
|* 
|* Return:
|*      long
|* 
|* Type:
|*      Private
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 20160307     JG              Removed indefinite length list
|* 
****************************************************************************/
long Asn::_decode(
    tagtype_s*  tagtype,    /* ASN.1 Specifications expected TAG */
    treenode*   tree,       /* Current part of the tree */
    long        size,       /* Size within decode */
    bool        is_indef    /* Flag indicating if encoding is indefinite (FALSE/TRUE) */
)
{

    vector<tagelem_s *>*            tagtype_ch;
    vector<tagelem_s *>::iterator   vii;
    long                            dec_size=0, tot_size=0;
    extern map<string, int>         simpletypes_map;
    extern map<int, enc_dec_funcs>      simpletypes;
    bool                            child_used=false;


    if (!size && !is_indef)
    {
        return 0;
    }


    /* 1. CHOICE */

    if (tagtype->action == choice)
    {

        if ( tagtype->children == NULL )
        {
            cerr << "Error: Found CHOICE without children for: " << tagtype->type_name;
            err_trace(__FILE__, __LINE__);
        }


        tagtype_ch=tagtype->children;


        /* Check if we already have a Tag */

        if (tree->tag_d && tree->tagname=="")
        {

            /* We are still analysing previous tag */

            {

                /* Find Tag name from specification */

                for(vii=tagtype_ch->begin();vii!=tagtype_ch->end();vii++)
                {

                    /* Find out if tag matches specification */

                    if ( (*vii)->elem_type_p == NULL)
                    {
                        cerr << "Error. Specification incomplete. Type not found for \"" << (*vii)->elem_name << "\"";
                        err_trace(__FILE__, __LINE__);
                    }

                    if ((*vii)->elem_type_p->tagid == tree->tag_d)
                    {

                        /* Tag matches */

                        tree->tagname=(*vii)->elem_name;

                        if ( (dec_size=_decode((*vii)->elem_type_p, tree, tree->size_d, (bool)tree->indef ) ) < 0)
                        {
                            cerr << "Error decoding file.  pos: " << _bfin->get_pos() << endl;
                            cerr << "tag: " << tree->tag_d << " dec_size: " << dec_size << " size_d: " << tree->size_d;
                            err_trace(__FILE__, __LINE__);
                        }

                        size-=dec_size;
                        tot_size+=dec_size;

                        break; /* In choice once found no need to loop further */
                    }
                    else
                    {
                        /* Tag does not match. Could be CHOICE */

                        if ( (*vii)->elem_type_p->action==choice )
                        {

                            dec_size=_decode((*vii)->elem_type_p, tree, tree->size_d, (bool)tree->indef);
                            if ( dec_size < 0 )
                            {
                                cerr << "Error decoding file.  pos: " << _bfin->get_pos() << endl;
                                err_trace(__FILE__, __LINE__);
                            }


                            size-=dec_size;
                            tot_size+=dec_size;

                        }
                    }
                }
            }
        }
        else
        {

            /* We got a fresh CHOICE  */

            {

                /* Read Tag and Size from file */

                child_used=false;
                treenode *child=new treenode();

                _asnitem->tree(child);
                _asnitem->decode();
                child->children=NULL;

                size-=child->tag_l;
                size-=child->size_l;
                tot_size+=child->tag_l;
                tot_size+=child->size_l;


                /* Did we find 2 null bytes? */

                if ( !child->size_d && !child->tag_d && is_indef)
                {
                    /* End of indefinite length */
                    if (!child_used && child)
                    {
                        delete child; 
                        child = NULL;
                    }
                    goto choice_exit_while;
                }


                /* Scan CHOICE children to find Tag name from specification */

                for(vii=tagtype_ch->begin();vii!=tagtype_ch->end();vii++)
                {

                    /* Find out if tag matches specification */

                    if ( (*vii)->elem_type_p == NULL)
                    {
                        cerr << "Error. Specification incomplete. Type not found for \"" << (*vii)->elem_name << "\"";
                        err_trace(__FILE__, __LINE__);
                    }

                    if ((*vii)->elem_type_p->tagid == child->tag_d)
                    {

                        /* Tag matches */


                        if (tree->children==NULL)
                            tree->children=new vector<treenode *>();

                        child->tagname=(*vii)->elem_name;
                        tree->children->push_back(child);
                        child_used=true;

                        if ( (dec_size=_decode((*vii)->elem_type_p, child, child->size_d, (bool)child->indef) ) < 0)
                        {
                            if (!child_used && child)
                            {
                                delete child; 
                                child = NULL;
                            }
                            cerr << "Error decoding file.  pos: " << _bfin->get_pos() << endl;
                            err_trace(__FILE__, __LINE__);
                        }

                        size-=dec_size;
                        tot_size+=dec_size;

                        break; /* In choice once found no need to loop further */
                    }
                    else
                    {
                        /* Tag does not match. Could be CHOICE again */

                        if ( (*vii)->elem_type_p->action==choice )
                        {

                            dec_size=_decode((*vii)->elem_type_p, child, child->size_d, (bool)child->indef);
                            if ( dec_size < 0 )
                            {
                                if (!child_used && child)
                                {
                                    delete child; 
                                    child = NULL;
                                }
                                cerr << "Error decoding file.  pos: " << _bfin->get_pos() << endl;
                                err_trace(__FILE__, __LINE__);
                            }

                            if (dec_size)
                            {
                                if (tree->children==NULL)
                                    tree->children=new vector<treenode *>();

                                tree->children->push_back(child);
                                child_used=true;
                            }

                            size-=dec_size;
                            tot_size+=dec_size;

                        }
                    }
                }

                if (!child_used && child)
                {
                    delete child; 
                    child = NULL;
                }

                /* If indef we should have two null bytes. */
                if (is_indef)
                {
                    child_used=false;
                    treenode *child=new treenode();

                    _asnitem->tree(child);
                    _asnitem->decode();
                    child->children=NULL;

                    size-=child->tag_l;
                    size-=child->size_l;
                    tot_size+=child->tag_l;
                    tot_size+=child->size_l;

                    if ( ! (!child->size_d && !child->tag_d ) )
                    {

                        delete child; 
                        child = NULL;

                        cerr << "Error: Corresponding null bytes not found after indefinite block";
                        cerr << " tag: " << child->tag_d << " pos: " << _bfin->get_pos() << endl;
                        err_trace(__FILE__, __LINE__);
                    }

                    delete child; 
                    child = NULL;

                }

choice_exit_while:;
            }
        }
    }


    /* 2. SEQUENCE */
    
    if (tagtype->action == sequence)
    {

        if ( tagtype->children == NULL )
        {
            cerr << "Error: Found SEQUENCE without children for: " << tagtype->type_name;
            err_trace(__FILE__, __LINE__);
        }

        tagtype_ch=tagtype->children;
        

        /* Scan SEQUENCE children to find Tag name from specification */

        child_used=false;
        treenode *child=new treenode();
        _asnitem->tree(child);
        _asnitem->decode();
        child->children=NULL;

        size-=child->tag_l;
        size-=child->size_l;
        tot_size+=child->tag_l;
        tot_size+=child->size_l;

        /* Did we find 2 null bytes? */

        if ( !child->size_d && !child->tag_d && is_indef)
        {
            /* End of indefinite length */
            if (!child_used && child)
            {
                delete child; 
                child = NULL;
            }
            goto sequence_exit_while;
        }

        for(vii=tagtype_ch->begin();vii!=tagtype_ch->end() && (size||!child->size_d);vii++) /* When no size we could go beyond file end */
        {

            /* Read Tag and Size from file */

            if (child->tagname.size())
            {
                if (!child_used && child)
                {
                    delete child; 
                    child = NULL;
                }

                child_used=false;
                treenode *childtmp=new treenode();

                child=childtmp;

                _asnitem->tree(child);
                _asnitem->decode();
                child->children=NULL;

                size-=child->tag_l;
                size-=child->size_l;
                tot_size+=child->tag_l;
                tot_size+=child->size_l;


                /* Did we find 2 null bytes? */

                if ( !child->size_d && !child->tag_d && is_indef)
                {
                    /* End of indefinite length */
                    if (!child_used && child)
                    {
                        delete child; 
                        child = NULL;
                    }
                    goto sequence_exit_while;
                }

            }

            /* Find out if tag matches specification */

            if ( (*vii)->elem_type_p == NULL)
            {
                cerr << "Error. Specification incomplete. Type not found for \"" << (*vii)->elem_name << "\"";
                err_trace(__FILE__, __LINE__);
            }

            if ((*vii)->elem_type_p->tagid == child->tag_d)
            {

                /* Tag matches */


                if (tree->children==NULL)
                    tree->children=new vector<treenode *>();

                child->tagname=(*vii)->elem_name;
                tree->children->push_back(child);
                child_used=true;

                if ( (dec_size=_decode((*vii)->elem_type_p, child, child->size_d, (bool)child->indef) ) < 0)
                {
                    if (!child_used && child)
                    {
                        delete child; 
                        child = NULL;
                    }
                    cerr << "Error decoding file.  pos: " << _bfin->get_pos() << endl;
                    cerr << "tag: " << child->tag_d << " dec_size: " << dec_size << " size_d: " << child->size_d;
                    err_trace(__FILE__, __LINE__);
                }

                size-=dec_size;
                tot_size+=dec_size;


            }
            else
            {
                /* Tag does not match. Could be whether CHOICE or OPTIONAL */

                if ( (*vii)->elem_type_p->action==choice )
                {
                    /* Child could be CHOICE */

                    dec_size=_decode((*vii)->elem_type_p, child, child->size_d, (bool)child->indef);
                    if ( dec_size < 0 )
                    {
                        if (!child_used && child)
                        {
                            delete child; 
                            child = NULL;
                        }
                        cerr << "Error decoding file.  pos: " << _bfin->get_pos() << endl;
                        err_trace(__FILE__, __LINE__);
                    }


                    if (!(*vii)->optional && !dec_size)
                    {
                        if (!child_used && child)
                        {
                            delete child; 
                            child = NULL;
                        }
                        cerr << "Error: Mandatory item missing:  " << (*vii)->elem_name;
                        cerr << " pos: " << _bfin->get_pos() << endl;
                        err_trace(__FILE__, __LINE__);
                    }

                    if (dec_size)
                    {
                        if (tree->children==NULL)
                            tree->children=new vector<treenode *>();

                        tree->children->push_back(child);
                        child_used=true;
                    }

                    size-=dec_size;
                    tot_size+=dec_size;

                }
                else
                {
                    /* Then child should be OPTIONAL */

                    if (!(*vii)->optional)
                    {
                        if (!child_used && child)
                        {
                            delete child; 
                            child = NULL;
                        }
                        cerr << "Error: Mandatory item missing:  " << (*vii)->elem_name;
                        cerr << " tag: " << child->tag_d << " pos: " << _bfin->get_pos() << endl;
                        err_trace(__FILE__, __LINE__);

                    }
                }
            }
        }

        if (size < 0 && !is_indef) /* In case we ate a tag too much */
        {
            _bfin->set_pos(_bfin->get_pos()+size);
            tot_size+=size;
        }                       

        if (!child_used && child)
        {
            delete child; 
            child = NULL;
        }

        /* If indef we should have two null bytes. */
        if (is_indef)
        {
            child_used=false;
            treenode *child=new treenode();

            _asnitem->tree(child);
            _asnitem->decode();
            child->children=NULL;

            size-=child->tag_l;
            size-=child->size_l;
            tot_size+=child->tag_l;
            tot_size+=child->size_l;

            if ( ! (!child->size_d && !child->tag_d ) )
            {

                delete child; 
                child = NULL;

                cerr << "Error: Corresponding null bytes not found after indefinite block";
                cerr << " tag: " << child->tag_d << " pos: " << _bfin->get_pos() << endl;
                err_trace(__FILE__, __LINE__);
            }

            delete child; 
            child = NULL;

        }

sequence_exit_while:;
    }


    /* 3. SEQUENCE OF */

    if (tagtype->action == sequenceof)
    {

        if ( tagtype->children == NULL )
        {
            cerr << "Error: Found SEQUENCE without children for: " << tagtype->type_name;
            err_trace(__FILE__, __LINE__);
        }

        tagtype_ch=tagtype->children;


        /* Proceed with all the size of our parent */

        while (size > 0 || is_indef)
        {
            /* Read Tag and Size from file */

            child_used=false;
            treenode *child=new treenode();
            _asnitem->tree(child);
            _asnitem->decode();
            child->children=NULL;

            size-=child->tag_l;
            size-=child->size_l;
            tot_size+=child->tag_l;
            tot_size+=child->size_l;


            /* Did we find 2 null bytes? */

            if ( !child->size_d && !child->tag_d && is_indef)
            {
                /* End of indefinite length */
                if (!child_used && child)
                {
                    delete child; 
                    child = NULL;
                }
                goto sequence_of_exit_while;
            }


            /* Scan SEQUENCE OF children to find Tag name from specification */

            for(vii=tagtype_ch->begin();vii!=tagtype_ch->end();vii++)
            {
                if ( (*vii)->elem_type_p == NULL)
                {
                    if (!child_used && child)
                    {
                        delete child; 
                        child = NULL;
                    }
                    cerr << "Error. Specification incomplete. Type not found for \"" << (*vii)->elem_name << "\"";
                    err_trace(__FILE__, __LINE__);
                }

                if ((*vii)->elem_type_p->tagid == child->tag_d)
                {

                    /* Tag matches */

                    if (tree->children==NULL)
                        tree->children=new vector<treenode *>();

                    child->tagname=(*vii)->elem_name;
                    tree->children->push_back(child);
                    child_used=true;

                    if ( (dec_size=_decode((*vii)->elem_type_p, child, child->size_d, (bool)child->indef) ) < 0)
                    {
                        if (!child_used && child)
                        {
                            delete child; 
                            child = NULL;
                        }
                        cerr << "Error decoding file.  pos: " << _bfin->get_pos() << endl;
                        cerr << "tag: " << child->tag_d << " dec_size: " << dec_size << " size_d: " << child->size_d << endl;
                        err_trace(__FILE__, __LINE__);
                    }

                    size-=dec_size;
                    tot_size+=dec_size;

                }
                else
                {
                    /* Tag does not match. Could be whether CHOICE or OPTIONAL */

                    if ( (*vii)->elem_type_p->action==choice )
                    {
                        /* Child could be CHOICE */

                        dec_size=_decode((*vii)->elem_type_p, child, child->size_d, (bool)child->indef);
                        if ( dec_size < 0 )
                        {
                            if (!child_used && child)
                            {
                                delete child; 
                                child = NULL;
                            }
                            cerr << "Error decoding file.  pos: " << _bfin->get_pos() << endl;
                            err_trace(__FILE__, __LINE__);
                        }

                        if (!(*vii)->optional && !dec_size)
                        {
                            if (!child_used && child)
                            {
                                delete child; 
                                child = NULL;
                            }
                            cerr << "Error: Mandatory item missing:  " << (*vii)->elem_name;
                            cerr << " pos: " << _bfin->get_pos() << endl;
                            err_trace(__FILE__, __LINE__);
                        }

                        if (dec_size)
                        {
                            if (tree->children==NULL)
                                tree->children=new vector<treenode *>();

                            tree->children->push_back(child);
                            child_used=true;
                        }

                        size-=dec_size;
                        tot_size+=dec_size;

                    }
                    else
                    {
                        /* Then child might be OPTIONAL */

                        if (!(*vii)->optional)
                        {
                            if (!child_used && child)
                            {
                                delete child; 
                                child = NULL;
                            }
                            cerr << "Error: Mandatory item missing:  " << (*vii)->elem_name;
                            cerr << " tag: " << child->tag_d << " pos: " << _bfin->get_pos() << endl;
                            err_trace(__FILE__, __LINE__);

                        }
                    }
                }
            }

            if (!child_used && child)
            {
                delete child; 
                child = NULL;
            }

        }

        if (size < 0 && !is_indef) /* In case we ate a tag too much */
        {
            _bfin->set_pos(_bfin->get_pos()+size);
            tot_size+=size;
        }                       

        /* If indef we should have two null bytes. */
        if (is_indef)
        {
            child_used=false;
            treenode *child=new treenode();

            _asnitem->tree(child);
            _asnitem->decode();
            child->children=NULL;

            size-=child->tag_l;
            size-=child->size_l;
            tot_size+=child->tag_l;
            tot_size+=child->size_l;

            if ( ! (!child->size_d && !child->tag_d ) )
            {

                delete child; 
                child = NULL;

                cerr << "Error: Corresponding null bytes not found after indefinite block";
                cerr << " tag: " << child->tag_d << " pos: " << _bfin->get_pos() << endl;
                err_trace(__FILE__, __LINE__);

            }

            delete child; 
            child = NULL;

        }

sequence_of_exit_while:;
    }


    /* 3. SIMPLE TYPE */

    if (tagtype->action == simpletype)
    {
        
        if (tree->pc)
        {
            cerr << "Error: Primitive Tag does not match specification." << endl;
            cerr << "Tag: " << tree->tag_d << " Spec: " << (*vii)->elem_name << endl;
            err_trace(__FILE__, __LINE__);
        }

        _bfin->read(tree->size_d);
        tree->value=new string();
        tree->type=simpletypes_map[tagtype->type_type];
        *tree->value=(*simpletypes[tree->type].decode)(_bfin->get_buf());

        size-=tree->size_d;
        tot_size+=tree->size_d;

    }


    /* 4. End */

    return tot_size;

}


/****************************************************************************
|* 
|* Method: _compile_tree
|* 
|* Description: 
|* 
|*     Matches each element of the decoded tree (_tree) with the 
|*     corresponding one in the ASN.1 specifications and complete the 
|*     necessary information needed for the encoding.
|* 
|* Return:
|*      long
|* 
|* Type:
|*      Private
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
long Asn::_compile_tree(tagtype_s *tagtype, treenode *tree)
{

    vector<tagelem_s *>*            tagtype_ch;
    vector<treenode *>::iterator    vii;
    vector<tagelem_s *>::iterator   vjj;
    long                            tot_size=0;
    extern map<string, int>         simpletypes_map;
    extern map<int, enc_dec_funcs>  simpletypes;
    AsnItem*                        asnitem;
    bool                            found=false;
    tagtype_s*                      tagtype_pa;
    int                             encoded_size;

    asnitem=new AsnItem();


    /* 1. CHOICE */

    if (tagtype->action==choice)
    {
        if (tree->tag_d != -2)
        {
            if (tree->children!=NULL)
            {

                /* 1.1. We got a fresh CHOICE */

                for (vii=tree->children->begin();vii!=tree->children->end();vii++)
                {
                    tagtype_ch=tagtype->children;

                    if (tagtype_ch!=NULL)
                    {
                        found=false;
                        for (vjj=tagtype_ch->begin();vjj!=tagtype_ch->end();vjj++)
                        {

                            if ( (*vii)->tagname == (*vjj)->elem_name )
                            {
                                (*vii)->tag_d=(*vjj)->elem_type_p->tagid;
                                (*vii)->pc=1;
                                tot_size+=_compile_tree((*vjj)->elem_type_p,(*vii));
                                found=true;
                                break;
                            }
                            else
                            {
                                if ( (*vjj)->elem_type_p->action == choice && !(*vjj)->elem_type_p->tagid)
                                {
                                    /* Could be CHOICE */

                                    (*vii)->tag_d=-2;
                                    tot_size+=_compile_tree((*vjj)->elem_type_p,(*vii));

                                    if ((*vii)->tag_d != -2)
                                        found=true;
                                    else
                                        (*vii)->tag_d=0;
                                }
                            }
                        }
                        if (!found)
                        {
                            cerr << "Error. " << (*vii)->tagname << " not found in specification";
                            err_trace(__FILE__, __LINE__);
                        }
                    }
                    else
                    {
                        cerr << "Error. " << tagtype->type_name << "  without children in tree specification";
                        err_trace(__FILE__, __LINE__);
                    }
                }
            }
            else 
            {
                cerr << "Error. \"" << tree->tagname << "\" (CHOICE) has no children";
                err_trace(__FILE__, __LINE__);
            }
        }
        else
        {
            /* 1.2. We are still analysing previous CHOICE */

            tagtype_ch=tagtype->children;

            if (tagtype_ch!=NULL)
            {
                found=false;
                for (vjj=tagtype_ch->begin();vjj!=tagtype_ch->end();vjj++)
                {

                    if ( tree->tagname == (*vjj)->elem_name )
                    {
                        tree->tag_d=(*vjj)->elem_type_p->tagid;
                        tree->pc=1;
                        tot_size+=_compile_tree((*vjj)->elem_type_p,tree);
                        found=true;
                        break;
                    }
                    else
                    {
                        /* Could be CHOICE */

                        if ( (*vjj)->elem_type_p->action == choice && !(*vjj)->elem_type_p->tagid)
                        {
                            tree->tag_d=-2;
                            tot_size+=_compile_tree((*vjj)->elem_type_p,tree);

                            if (tree->tag_d != -2)
                                found=true;
                        }
                    }
                }

            }
            else
            {
                cerr << "Error. " << tagtype->type_name << "  without children in tree specification";
                err_trace(__FILE__, __LINE__);
            }

            delete asnitem; 
            asnitem = NULL;
            return tot_size;
        }
    }


    /* SEQUENCE */

    if (tagtype->action==sequence)
    {

        tagtype_ch=tagtype->children;
        if (tagtype_ch==NULL)
        {
            cerr << "Error. \"" << tagtype->type_name << "\"  without children in tree specification";
            err_trace(__FILE__, __LINE__);
        }

        if (tree->children!=NULL)
        {
            vjj=tagtype_ch->begin();

            for (vii=tree->children->begin();vii!=tree->children->end();vii++)
            {
                found=false;
                while (vjj!=tagtype_ch->end() && !found)
                {

                    if ( (*vii)->tagname == (*vjj)->elem_name )
                    {
                        (*vii)->tag_d=(*vjj)->elem_type_p->tagid;
                        (*vii)->pc=1;
                        tot_size+=_compile_tree((*vjj)->elem_type_p,(*vii));
                        found=true;
                    }
                    else
                    {
                        if ( (*vjj)->elem_type_p == NULL )
                        {
                            cerr << "Error. Specification incomplete. Type not found for \"" << (*vjj)->elem_name << "\"";
                            err_trace(__FILE__, __LINE__);
                        }

                        if ( (*vjj)->elem_type_p->action == choice && !(*vjj)->elem_type_p->tagid)
                        {
                            (*vii)->tag_d=-2;
                            tot_size+=_compile_tree((*vjj)->elem_type_p,(*vii));

                            if ( (*vii)->tag_d!=-2 )
                                found=true;
                            else
                                (*vii)->tag_d=0;
                                
                        } 

                        if ( !found && !(*vjj)->optional )
                        {
                            cerr << "Error. Expected in tree: " << (*vjj)->elem_name << " found: " << (*vii)->tagname;
                            err_trace(__FILE__, __LINE__);
                        }
                    }
                    vjj++;
                }
                if (!found)
                {
                    cerr << "Error. " << (*vii)->tagname << " not found in specification";
                    err_trace(__FILE__, __LINE__);
                }
            }
            for (;vjj!=tagtype_ch->end();vjj++)
            {
                if ( (*vjj)->optional == false )
                {
                    cerr << "Error. Expected in tree: " << (*vjj)->elem_name << " found: " << (*vii)->tagname;
                    err_trace(__FILE__, __LINE__);
                }
            }
        }
        else
        {
            for (vjj=tagtype_ch->begin();vjj!=tagtype_ch->end();vjj++)
            {
                if ((*vjj)->elem_type_p==NULL)
                {
                    cerr << "Error. Type for \"" << (*vjj)->elem_name << "\" not found in the specification";
                    err_trace(__FILE__, __LINE__);
                }

                if (!(*vjj)->optional)
                {
                    cerr << "Error. Expected \"" << (*vjj)->elem_name << "\" under \"" << tree->tagname << "\"";
                    err_trace(__FILE__, __LINE__);
                }
            }


            /* Tag is composed but empty */

            asnitem->tree(tree);

            tree->pc=1;
            tree->size_d=0;

            encoded_size=asnitem->encode(0).size();
            delete asnitem; 
            asnitem = NULL;
            return encoded_size;
        }
    }


    /* SEQUENCE OF */

    if (tagtype->action==sequenceof)
    {
        if (tree->children!=NULL)
        {
            for (vii=tree->children->begin();vii!=tree->children->end();vii++)
            {
                tagtype_ch=tagtype->children;

                if (tagtype_ch==NULL || tagtype_ch->size()>1)
                {
                    cerr << "Error. " << tagtype->type_name << " (SEQUENCE OF) without or with many children in specification";
                    err_trace(__FILE__, __LINE__);
                }

                tagtype_pa=(*tagtype->children)[0]->elem_type_p;

                if (tagtype_pa->action==choice && !tagtype_pa->tagid)
                    tagtype_ch=tagtype_pa->children;

                found=false;
                if (tagtype_ch!=NULL)
                {
                    for (vjj=tagtype_ch->begin();vjj!=tagtype_ch->end();vjj++)
                    {

                        if ( (*vii)->tagname == (*vjj)->elem_name )
                        {
                            (*vii)->tag_d=(*vjj)->elem_type_p->tagid;
                            (*vii)->pc=1;
                            tot_size+=_compile_tree((*vjj)->elem_type_p,(*vii));
                            found=true;
                            break;
                        }
                    }
                }
                else
                {
                    if (tagtype_pa->action==simpletype)
                    {
                        if ( (*vii)->tagname == (*tagtype->children)[0]->elem_name )
                        {
                            (*vii)->tag_d=tagtype_pa->tagid;
                            (*vii)->pc=0;
                            tot_size+=_compile_tree(tagtype_pa,(*vii));
                            found=true;
                            break;
                        }
                    }
                    else
                    {
                        cerr << "Error. " << tagtype_pa->type_name << "  without children in tree specification";
                        err_trace(__FILE__, __LINE__);
                    }
                }
                if (!found)
                {
                    cerr     << "Error. " << tree->tagname <<"("<< tree->tag_d<<")->" 
                        << (*vii)->tagname << " not found in specification under " 
                        << tagtype->type_name << "(" << tagtype->tagid << ")";
                    if (tagtype_pa->action==choice && !tagtype_pa->tagid)
                        cerr     << "->" << tagtype_pa->type_name << "("<<tagtype_pa->tagid<<")";
                    err_trace(__FILE__, __LINE__);
                }
            }
        }
        else
        {
            /* Tag is composed but empty */

            asnitem->tree(tree);

            tree->pc=1;
            tree->size_d=0;

            encoded_size=asnitem->encode(0).size();
            delete asnitem; 
            asnitem = NULL;
            return encoded_size;
        }
    }


    /* SIMPLE TYPE */

    if (tagtype->action==simpletype)
    {
        if (tree->children==NULL)
        {
            asnitem->tree(tree);

            tree->pc=0;
            tree->type=simpletypes_map[tagtype->type_type];
            tree->size_d=(*simpletypes[tree->type].encode)(*tree->value).size();

            encoded_size=asnitem->encode(0).size();
            delete asnitem; 
            asnitem = NULL;
            return encoded_size;
        }
        else
        {
            cerr << "Error. \"" << tree->tagname << "\" expected to be a simple type but got children";
            err_trace(__FILE__, __LINE__);
        }
    }


    tree->size_d=tot_size;

    asnitem->tree(tree);
    encoded_size=asnitem->encode(0).size();
    delete asnitem; 
    asnitem = NULL;
    return tree->size_d+encoded_size;
}


/****************************************************************************
|* 
|* Method: encode
|* 
|* Description: 
|* 
|*     Encodes a decoded tree (_tree) into an ASN.1 formated output file.
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::encode()
{

    /* 1. Decode Specification */

    _decode_spec();


    /* 2. Match our Tree with the specification */

    if (!_spec_rootnode.size())
    {
        cerr << "Error. The root node should be specified with the method \"specroot\"";
        err_trace(__FILE__, __LINE__);
    }

    _compile_tree(_spec_tree[_spec_rootnode],_tree);

    if (!_tree->tag_d)
    {
        _tree->tag_d=_spec_tree[_spec_rootnode]->tagid;
    }

    /* 3. Encode */

    TreeParser *tparser=new TreeParser;
    tparser->handler(_asnitem);
    tparser->buffout(_bfout);
    tparser->tree(_tree);
    tparser->process();
    delete tparser; 
    tparser = NULL;

    _bfout->flush();

}

/****************************************************************************
|* 
|* Method: specfile
|* 
|* Description: 
|* 
|*     Opens the ASN.1 specification file 
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::specfile(const string &filename)
{

    if (!(_specfile=fopen(filename.c_str(), "rb")))
    {
        cerr << "Error trying to open or not found specification file: " << filename;
        if (getenv(TAP3XML_SPEC_VAR) == NULL)
            cerr << "\nYou can set the variable " << TAP3XML_SPEC_VAR << " to point to the directory of the specifications";
        err_trace(__FILE__, __LINE__);
    }

}

/****************************************************************************
|* 
|* Method: specsuplfile
|* 
|* Description: 
|* 
|*     Opens the ASN.1 supplementary specification file (TAP part of the RAP)
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::specsuplfile(const string &filename)
{

    if (!(_specsuplfile=fopen(filename.c_str(), "rb")))
    {
        cerr << "Error trying to open or not found specification file: " << filename;
        err_trace(__FILE__, __LINE__);
    }

}

/****************************************************************************
|* 
|* Method: specroot
|* 
|* Description: 
|* 
|*     Sets the parent node of the ASN.1 structure.
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::specroot(const string spec_rootnode)
{
    _spec_rootnode=spec_rootnode;
}

/****************************************************************************
|* 
|* Method: _compile_spec
|* 
|* Description: 
|* 
|*     After the ASN.1 Spcs are decoded, they are arranged by this function
|*     linking each element with its corresponding type.
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Private
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::_compile_spec(map<string,tagtype_s *> *spec_tree)
{

    vector<tagelem_s *>::iterator       vii;
    map<string,tagtype_s *>::iterator   mii;
    vector<tagelem_s *>*                children;
    map<string,int>::iterator           mjj;
    extern map<string, int>             simpletypes_map;




    /* 1. Compile. Build Tree */

    for(mii=spec_tree->begin();mii!=spec_tree->end();mii++)
    {

        if (mii->second->children==NULL )
        {
            if (mii->second->type_type.size())
            {
                if ( spec_tree->count(mii->second->type_type) )
                {
                    (*spec_tree)[mii->second->type_type]->checked=true;
                }
            }
        } 
        else 
        {

            children=mii->second->children;

            for(vii=children->begin();vii!=children->end();vii++)
            {
                if ( spec_tree->count((*vii)->elem_type) )
                {
                    (*vii)->elem_type_p=(*spec_tree)[(*vii)->elem_type];
                    (*spec_tree)[(*vii)->elem_type]->checked=true;
                }
            }
        }
    }


    /* 2. Arrange Simple types */

    /* Note: some simple types are in fact SEQUENCE or CHOICE    */
    /*       so we need to replace this simple types with their  */
    /*       SEQUENCE or CHOICE values.                          */

    for(mii=spec_tree->begin();mii!=spec_tree->end();mii++)
    {

        while ( mii->second->action==simpletype && 
            spec_tree->count(mii->second->type_type) && 
            !simpletypes_map.count(mii->second->type_type) 
        )
        {
            mii->second->children=(*spec_tree)[mii->second->type_type]->children;
            mii->second->action=(*spec_tree)[mii->second->type_type]->action;
            mii->second->type_type=(*spec_tree)[mii->second->type_type]->type_type;
            mii->second->modified=true;
        }
    }
}


/****************************************************************************
|* 
|* Method: ofile
|* 
|* Description: 
|* 
|*     Sets the name of the output file
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::ofile(const string &filename)
{
    _bfout=new Buffout(filename);
}

/****************************************************************************
|* 
|* Method: ifile
|* 
|* Description: 
|* 
|*     Sets the name of the input file
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::ifile(const string &filename)
{
    (void)filename;
}

/****************************************************************************
|* 
|* Method: tree
|* 
|* Description: 
|* 
|*     Sets the tree container where the decoded structure is contained.
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::tree(treenode *tree)
{
    _tree=tree;
    _asnitem->tree(_tree);

}

/****************************************************************************
|* 
|* Method: _dump_spec_tree
|* 
|* Description: 
|* 
|*     Dumps on screen the content of the specifications tree.
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Private
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::_dump_spec_tree(map<string,tagtype_s *> &spec_tree)
{
    map<string,tagtype_s *>::iterator   mii;
    vector<tagelem_s *>::iterator       vii;
    vector<tagelem_s *>*                children;


    for(mii=spec_tree.begin();mii!=spec_tree.end();mii++)
    {
        if (mii->second!=NULL)
        {
            cout << "type_name:     " << mii->second->type_name << endl;
            cout << "type_type:     " << mii->second->type_name << endl;
            cout << "tagid:         " << mii->second->tagid << endl;
            cout << "action:        " << mii->second->action << endl;
            cout << "modified:      " << mii->second->modified << endl;
            cout << "checked:       " << mii->second->checked << endl;
            if (mii->second->children!=NULL)
            {
                children=mii->second->children;

                for(vii=children->begin();vii!=children->end();vii++)
                {
                    if (*vii!=NULL)
                    {
                        cout << "children:elem_name:    " << (*vii)->elem_name << "\n";
                        cout << "children:elem_type:    " << (*vii)->elem_type << "\n";
                        cout << "children:option:       " << (*vii)->optional << "\n";
                    }
                }
            }
            cout << endl;
        }
    }
}


/****************************************************************************
|* 
|* Method: _free_spec_tree
|* 
|* Description: 
|* 
|*     Free meomory allocated by tree
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Private
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
void Asn::_free_spec_tree(map<string,tagtype_s *> &spec_tree)
{
    map<string,tagtype_s *>::iterator   mii;
    vector<tagelem_s *>::iterator       vii;
    vector<tagelem_s *>*                children;


    for(mii=spec_tree.begin();mii!=spec_tree.end();mii++)
    {
        if (mii->second!=NULL)
        {
            if (mii->second->children!=NULL && mii->second->modified==false)
            {
                children=mii->second->children;

                for(vii=children->begin();vii!=children->end();vii++)
                {
                    if (*vii!=NULL)
                    {
                        delete *vii;
                        *vii=NULL;
                    }
                }

                delete mii->second->children;
                mii->second->children=NULL;
            }

            delete mii->second;
            mii->second=NULL;
        }
    }
}


/****************************************************************************
|* 
|* Method: _dump_asn_tree
|* 
|* Description: 
|* 
|*     Dumps on screen the content of the ASN.1 tree.
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Private
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20160303     JG              Initial version
|* 
****************************************************************************/
void Asn::_dump_asn_tree(
        treenode *tree,
        int depth
        )
{

    vector<treenode *>::iterator    vii;

    cout << string(depth*4, ' ') << tree->tagname << " (" << tree->tag_d << ")";
    if (tree->value)
        cout << " \"" << *(tree->value) << "\"";
    cout << "\n";

    if (tree->children!=NULL)
    {

        for (vii=tree->children->begin();vii!=tree->children->end();vii++)
        {
            _dump_asn_tree(*vii, depth +1);
        }
    }
}


/****************************************************************************
|* 
|* Method: Destructor
|* 
|* Description: 
|* 
|*     Destroyes the Asn Object
|* 
|* Return:
|*      N/A
|* 
|* Type:
|*      Public
|* 
|* Author: Javier Gutierrez (JG)
|* 
|* Modifications:
|* 20050909     JG              Initial version
|* 
****************************************************************************/
Asn::~Asn() 
{
    _free_spec_tree(_spec_tree);

    if (_asnitem)
    {
        delete _asnitem; 
        _asnitem = NULL;
    }

    if (_specfile!=NULL)
    {
        fclose(_specfile);
    }

    if (_specsuplfile!=NULL)
    {
        fclose(_specsuplfile);
    }

    if (_bfout!=NULL)
    {
        delete _bfout; 
        _bfout = NULL;
    }
}
