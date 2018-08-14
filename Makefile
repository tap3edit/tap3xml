###############################################################################
## 
##  tap3xml: both directions XML<->TAP3 Converter.
##  nrt2xml: both directions XML<->NRT Converter.
##  
##  tap3edit Tools (https://github.com/tap3edit)
##  
##  Copyright (C) 2005-2018  Javier Gutierrez. All rights reserved.
##  email address <https://github.com/tap3edit/tap3xml>
##  
##  Permission to use, copy, modify, and/or distribute this software for any
##  purpose with or without fee is hereby granted, provided that the above
##  copyright notice and this permission notice appear in all copies.
##  
##  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
##  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
##  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
##  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
##  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
##  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
##  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
## 
## 
##  Module: Makefile
## 
##  Description: 
## 
##      - To be used with gnu make
##      - Usage:
## 
##          make all:     will create the binary nrt2xml
## 
##          make clean:   will delete all objects and the binary
## 
##          make parsers: The provided parsers that analyze the ASN.1 definitions
##                        and the XML structure were built with flex (2.5.35) and 
##                        bison (2.4.1). In case they need to be re-generated
##                        this macro can be used.
##                        NOTE: For compiling with Microsoft Visual C++ now I 
##                        include the option --nounistd of flex. Also the option
##                        "%option never-interactive" was added to the lexical
##                        parsers.
## 
##  Return:
## 
##  Author: Javier Gutierrez (JG)
## 
##  Modifications:
## 
##  When         Who     Pos.    What
##  20050909     JG              Initial version
##  20071029     JG              Support for NRT Files
##  20110228     JG              Removed the flex skeleton and "parsers_unix"
##                               for causing many problems. Removed also the
##                               the compiling flag -lfl
## 
###############################################################################

## Definitions ################################################################


CC           = g++
CFLAGS       = -g -Wall -Wno-unknown-pragmas # -pg
LEX          = flex
# LEX_FLAGS  = -lfl                     # With new versions of flex we shouldn't
                                        # need this flag when compiling, but 
                                        # if having problems, we can uncomment
                                        # this line, we need though to install 
                                        # the flex libraries.
YACC         = bison
PKG_VER      = 0.19

SOURCES      = global.cpp
SOURCES     += Buffin.cpp
SOURCES     += Buffout.cpp
SOURCES     += Asn.cpp
SOURCES     += AsnItem.cpp
SOURCES     += TreeItem.cpp
SOURCES     += TreeParser.cpp
SOURCES     += Xml.cpp
SOURCES     += XmlItem.cpp

OBJECTS     += $(SOURCES:.cpp=.o)
HEADERS	 	+= $(SOURCES:.cpp=.h)

SRC_PRS		 = asn1_lexer.cpp
SRC_PRS		+= asn1_parser.cpp
SRC_PRS		+= xml_lexer.cpp
SRC_PRS		+= xml_parser.cpp
OBJ_PRS     += $(SRC_PRS:.cpp=.o)
HDR_PRS      = asn1_parser.hpp
HDR_PRS     += xml_parser.hpp

SRC_TAP     += tap3xml.cpp
SRC_TAP     += TapInfo.cpp
OBJ_TAP     += $(SRC_TAP:.cpp=.o)
HDR_TAP      = TapInfo.h

SRC_NRT     += nrt2xml.cpp
SRC_NRT     += NrtInfo.cpp
OBJ_NRT     += $(SRC_NRT:.cpp=.o)
HDR_NRT      = NrtInfo.h

TAP3XML      = tap3xml
NRT2XML      = nrt2xml

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

## Compilation ################################################################

all: $(TAP3XML) $(NRT2XML)

$(TAP3XML): $(OBJECTS) $(OBJ_PRS) $(OBJ_TAP)
	$(CC) $(OBJ_TAP) $(OBJ_PRS) $(OBJECTS) -o $@ $(LEX_FLAGS) # -pg

$(NRT2XML): $(OBJECTS) $(OBJ_PRS) $(OBJ_NRT)
	$(CC) $(OBJ_NRT) $(OBJ_PRS) $(OBJECTS) -o $@ $(LEX_FLAGS) # -pg

## Dependences ################################################################

asn1_lexer.o:   asn1_lexer.cpp  \
                asn1_parser.hpp \
                asn1_parser.cpp

asn1_parser.o:  asn1_parser.cpp \
                asn1_parser.hpp

xml_lexer.o:    xml_lexer.cpp   \
                xml_parser.hpp  \
                xml_parser.cpp

xml_parser.o:   xml_parser.cpp  \
                xml_parser.hpp

global.o:       global.cpp      \
                global.h

Buffin.o:       Buffin.cpp      \
                Buffin.h        \
                global.cpp      \
                global.h

Buffout.o:      Buffout.cpp     \
                Buffout.h       \
                global.cpp      \
                global.h

Asn.o:          Asn.cpp         \
                Asn.h           \
                AsnItem.h       \
                AsnItem.cpp     \
                Buffin.h        \
                Buffin.cpp      \
                global.cpp      \
                global.h        \
                TreeItem.h      \
                TreeItem.cpp

AsnItem.o:      AsnItem.h       \
                AsnItem.cpp     \
                Buffin.h        \
                Buffin.cpp      \
                Buffout.h       \
                Buffout.cpp     \
                TreeItem.h      \
                TreeItem.cpp    

TreeItem.o:     TreeItem.h      \
                TreeItem.cpp    \
                global.cpp      \
                global.h     

TreeParser.o:   TreeParser.h    \
                TreeParser.cpp  \
                TreeItem.h      \
                TreeItem.cpp    \
                Buffout.h       \
                Buffout.cpp

Xml.o:          Xml.h           \
                Xml.cpp         \
                XmlItem.h       \
                XmlItem.cpp     \
                TreeParser.h    \
                TreeParser.cpp

XmlItem.o:      XmlItem.h       \
                XmlItem.cpp

tap3xml.o:      Makefile        \
                tap3xml.cpp     \
                Asn.cpp         \
                Asn.h           \
                AsnItem.h       \
                AsnItem.cpp     \
                Buffin.h        \
                Buffin.cpp      \
                Xml.cpp         \
                Xml.h           \
                XmlItem.cpp     \
                XmlItem.h       \
                global.cpp      \
                global.h

nrt2xml.o:      Makefile        \
                nrt2xml.cpp     \
                Asn.cpp         \
                Asn.h           \
                AsnItem.h       \
                AsnItem.cpp     \
                Buffin.h        \
                Buffin.cpp      \
                Xml.cpp         \
                Xml.h           \
                XmlItem.cpp     \
                XmlItem.h       \
                global.cpp      \
                global.h

NrtInfo.o:      NrtInfo.cpp     \
                NrtInfo.h       \
                Buffin.cpp      \
                Buffin.h        \
                global.cpp      \
                global.h


## Compile parsers ############################################################

parsers:        asn1_lexer      \
                asn1_parser     \
                xml_lexer       \
                xml_parser

asn1_lexer:     asn1_lexer.l    \
                asn1_parser.ypp
	$(LEX) --nounistd -B -Pasn1 -oasn1_lexer.cpp asn1_lexer.l

asn1_parser:    asn1_parser.ypp
	$(YACC) -p asn1 -d -o asn1_parser.cpp asn1_parser.ypp

xml_lexer:      xml_lexer.l     \
        xml_parser.ypp
	$(LEX) --nounistd -B -Pxml -oxml_lexer.cpp xml_lexer.l

xml_parser:     xml_parser.ypp
	$(YACC) -p xml -d -o xml_parser.cpp xml_parser.ypp

PARSERS = asn1_lexer.l \
		  asn1_parser.ypp \
		  xml_lexer.l \
		  xml_parser.ypp


## Package ####################################################################

PKG_BASE_DIR = tap3xml
PKG_NAME = $(TAP3XML)-$(PKG_VER).zip
PKG_TMP_DIR = tmp

PKG_SRC = $(SOURCES) \
		  $(HEADERS) \
		  $(SRC_TAP) \
		  $(HDR_TAP) \
		  $(SRC_NRT) \
		  $(HDR_NRT) \
		  $(TAP3XML) \
		  $(NRT2XML) \
		  $(PARSERS) \
		  $(SRC_PRS) \
		  $(HDR_PRS) \
		  ChangeLog \
		  README.txt \
		  Makefile \
		  Spec \
		  MSVC \
		  n2x

pkg: make_dir \
	copy_src \
	tar_module \
	rm_dir

make_dir:
	@mkdir -p $(PKG_TMP_DIR)/$(PKG_BASE_DIR)/.

copy_src:
	@cp -rp $(PKG_SRC) $(PKG_TMP_DIR)/$(PKG_BASE_DIR)/.

tar_module:
	cd $(PKG_TMP_DIR); zip -r ../$(PKG_NAME) $(PKG_BASE_DIR)

rm_dir: 
	rm -rf $(PKG_TMP_DIR)



## Other macros ###############################################################

clean:
	$(RM) *.o $(OBJECTS) $(OBJ_PRS) $(OBJ_TAP) $(OBJ_NRT) $(TAP3XML) $(NRT2XML) $(PKG_NAME)

laugh:
	@echo "hahahha"
