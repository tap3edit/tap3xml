
tap3xml & nrt2mxl: both directions XML<->TAP3 and XML<->NRT Converters.


INSTALLATION

Compile and create a version of the package for your platform. Place the
generated binaries in any directory and set the PATH to point to it.

The compilation has two steps:

    make parsers
    make 

Normally the package doesn't need the flex libraries, but in case of a problem
when compiling you can try enabling this feauture in following way: edit the
Makefile and uncomment the line for the variable LEX_FLAGS. This will allow
the compilation with the flag -lfl. The installation of the flex libraries 
is required. 

TAP3, NRTRDE, RAP SPECIFICATIONS

For the conversion between files, tap3xml and nrt2xml need the ASN.1 TAP3 and 
NRTRDE specs from the GSM association for the version of the file to convert. 
All known versions including for RAP files are delivered within this package 
(please read the license for those files).

These specifications should be available for tap3xml and nrt2xml in following 
way:

    a. Set the environment variable TAP3XML_SPEC with the path where the 
       specification are located. e.g:

        unix:       TAP3XML_SPEC=/tap3xml/Specs/
        windows:    set TAP3XML_SPEC="C:\tap3xml\Specs"


    b. tap3xml will search for the specifications also in following directories:

        unix:
            ./Spec
            $HOME/Spec
            Directory where the binary is located
            $PATH

        windows:
            .\Spec
            $HOMEPATH\Spec
            Directory where the binary is located
            %PATH%


The specifiactions filesnames are defined as following:

    XXXYYZZ.asn

    XXX: type of file (TAP or RAP)
    YY:  version
    ZZ:  release

e.g. TAP0311.asn for the spec of TAP files version 3 and release 11.


USAGE

tap3xml -x|-a inputfile [outputfile]

    inputfile:      the file to be converted.
    outputfile (optional):  name of the converted file.
                If the outputfile is not specified, the converted file will 
                stored in the current directory with extension .xml or .asn 
                according to the format.
    -x:         converts the inputfile to XML. (inputfile should be a Tap3 file)
    -a:         converts the inputfile to ASN. (inputfile should be a XML file 
                with Tap3 structure)

BUGS

Please report bugs under https://github.com/tap3edit/tap3xml
I have tested tap3xml on many Tap and Rap files but unfortunately I did not 
have the change to test the conversion XML->TAP3 with other parties XML files.


IDEAS

Please let me know about any comment/ideas about this tool or others on
https://github.com/tap3edit


LICENSE

Copyright (C) 2005-2018  Javier Gutierrez. All rights reserved.
email address <https://github.com/tap3edit/tap3xml>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

This program contains TAP and RAP ASN.1 Specification. The
ownership of the TAP/RAP ASN.1 Specifications belong to
the GSM MoU Association (http://www.gsm.org) and should be
used under following conditions:

Copyright (c) 2000 GSM MoU Association. Restricted − Con­
fidential Information.  Access to and distribution of this
document is restricted to the persons listed under the
heading Security Classification Category*. This document
is confidential to the Association and is subject to copy­
right protection.  This document is to be used only for
the purposes for which it has been supplied and informa­
tion contained in it must not be disclosed or in any other
way made available, in whole or in part, to persons other
than those listed under Security Classification Category*
without the prior written approval of the Association. The
GSM MoU Association (âAssociationâ) makes no representa­
tion, warranty or undertaking (express or implied) with
respect to and does not accept any responsibility for, and
hereby disclaims liability for the accuracy or complete­
ness or timeliness of the information contained in this
document. The information contained in this document may
be subject to change without prior notice.


