/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2002, 2003 Intel Corporation.  All rights reserved.
 * 
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel
 * Corporation or its suppliers or licensors.  Title to the
 * Material remains with Intel Corporation or its suppliers and
 * licensors.  The Material contains trade secrets and proprietary
 * and confidential information of Intel or its suppliers and
 * licensors. The Material is protected by worldwide copyright and
 * trade secret laws and treaty provisions.  No part of the Material
 * may be used, copied, reproduced, modified, published, uploaded,
 * posted, transmitted, distributed, or disclosed in any way without
 * Intel's prior express written permission.
 
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you
 * by disclosure or delivery of the Materials, either expressly, by
 * implication, inducement, estoppel or otherwise. Any license
 * under such intellectual property rights must be express and
 * approved by Intel in writing.
 * 
 * $Workfile: ILibParsers.h
 * $Revision: #1.0.1678.19507
 * $Author:   Intel Corporation, Intel Device Builder
 * $Date:     Friday, August 06, 2004
 *
 *
 *
 */

struct parser_result_field
{
    char* data;
    int   datalength;
    struct parser_result_field *NextResult;
};

struct parser_result
{
    struct parser_result_field *FirstResult;
    struct parser_result_field *LastResult;
    int NumResults;
};

struct ILibXMLNode 
{
    char* Name;
    int NameLength;
    
    int StartTag;
    int EmptyTag;
    
    void *Reserved;
    struct ILibXMLNode *Next;
    struct ILibXMLNode *Parent;
};

struct ILibXMLAttribute
{
    char* Name;
    int NameLength;
    
    struct ILibXMLNode *Parent;

    char* Value;
    int ValueLength;
    struct ILibXMLAttribute *Next;
};

struct ILibXMLNode *ILibParseXML(char *buffer, int offset, int length);
struct ILibXMLAttribute *ILibGetXMLAttributes(struct ILibXMLNode *node);
void ILibDestructXMLNodeList(struct ILibXMLNode *node);
void ILibDestructXMLAttributeList(struct ILibXMLAttribute *attribute);

struct parser_result* ILibParseString( char* buffer, 
                                       int   offset, 
                                       int   length, 
                                       char* Delimiter, 
                                       int   DelimiterLength);

struct parser_result* ILibParseStringAdv( char* buffer, 
                                          int   offset, 
                                          int   length, 
                                          char* Delimiter, 
                                          int   DelimiterLength);

void ILibDestructParserResults(struct parser_result *result);

