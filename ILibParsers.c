#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ILibParsers.h"

//Frees resources from an XMLNodeList structure that was returned from ILibParseXML
void ILibDestructXMLNodeList(struct ILibXMLNode *node)
{
    struct ILibXMLNode *temp;
    while(node!=NULL)
    {
        temp = node->Next;
        free(node);
        node = temp;
    }
}

//Frees resources from an AttributeList that was returned from ILibGetXMLAttributes
void ILibDestructXMLAttributeList(struct ILibXMLAttribute *attribute)
{
    struct ILibXMLAttribute *temp;
    while(attribute!=NULL)
    {
        temp = attribute->Next;
        free(attribute);
        attribute = temp;
    }
}


// Determines if a buffer offset is a delimiter
int ILibIsDelimiter( char* buffer,         
                     int   offset, 
                     int   buffersize, 
                     char* Delimiter, 
                     int   DelimiterLength )
{
    int i=0;
    int RetVal = 1;
    if(DelimiterLength>buffersize)
    {
        return(0);
    }
    
    for(i=0;i<DelimiterLength;++i)
    {
        if(buffer[offset+i]!=Delimiter[i])
        {
            RetVal = 0;
            break;
        }
    }
    return(RetVal);
}

// Parse the attributes from an XML node
struct ILibXMLAttribute *ILibGetXMLAttributes(struct ILibXMLNode *node)
{
    struct ILibXMLAttribute *RetVal = NULL;
    struct ILibXMLAttribute *current = NULL;
    char *c;
    int EndReserved = (node->EmptyTag==0)?1:2;
    int i;
    
    struct parser_result *xml;
    struct parser_result_field *field;
    struct parser_result *temp3;
    
    c = (char*)node->Reserved - 1;
	//Goto the begining of the node
    while(*c!='<')
    {
        c = c -1; 
    }
    c = c +1;
    
    //Replace linefeeds and carriage-return-linefeeds to spaces
    for(i=0;i<(int)((char*)node->Reserved - c - EndReserved);++i)
    {
        if(c[i]==10 || c[i]==13)
        {
            c[i]=' ';
        }
    }

	//Parse the string between the '<' and '>'
    xml = ILibParseStringAdv(c,0,(int)((char*)node->Reserved - c -EndReserved)," ",1);
    field = xml->FirstResult;

    // We skip the first token, because the first token, is the Element name
    if(field!=NULL) 
		field = field->NextResult;

    while(field!=NULL)
    {
        if(field->datalength>0 && field->data[0]!=10 && field->data[0]!=13)
        {
            if(RetVal==NULL)
            {
                // If we haven't already created an Attribute node, create it now
                RetVal = (struct ILibXMLAttribute*)malloc(sizeof(struct ILibXMLAttribute));
                RetVal->Next = NULL;
            }
            else
            {
                // Create a attached one
                current = (struct ILibXMLAttribute*)malloc(sizeof(struct ILibXMLAttribute));
                current->Next = RetVal;
                RetVal = current;
            }
			temp3 = ILibParseStringAdv(field->data,
										0,
										field->datalength,
										"=",
										1);
           
            RetVal->Parent = node;
            RetVal->Name = temp3->FirstResult->data;
            RetVal->NameLength = temp3->FirstResult->datalength;
            RetVal->Value = temp3->LastResult->data;
            RetVal->ValueLength = temp3->LastResult->datalength;
			
			//Remove the empty element tag
			if(RetVal->Value[RetVal->ValueLength-1]=='/')
				RetVal->ValueLength--;
			
            // Remove the Quote or Apostraphe if it exists
            if(RetVal->ValueLength>=2 && (RetVal->Value[0]=='"' || RetVal->Value[0]=='\''))
            {
                RetVal->Value += 1;
                RetVal->ValueLength -= 2;
            }
            ILibDestructParserResults(temp3);
        }
        field = field->NextResult;
    }
    
    ILibDestructParserResults(xml);
    return(RetVal);
}

// Parse an XML file
struct ILibXMLNode *ILibParseXML(char *buffer, int offset, int length) /* {{{1 */
{
    struct parser_result *xml;
    struct parser_result_field *field;
    struct parser_result *temp = NULL;
    struct parser_result *temp2 = NULL;
    struct parser_result *temp3 = NULL;
    char* TagName;
    int TagNameLength;
    int StartTag;
    int EmptyTag;
    int i;
    
    struct ILibXMLNode *RetVal = NULL;
    struct ILibXMLNode *current = NULL;
    struct ILibXMLNode *x = NULL;
    
    // All XML Elements should start with a '<' character.
    xml = ILibParseString( buffer,
                           offset,
                           length,
                           "<",
                           1 );
						   
    field = xml->FirstResult;

    while(field!=NULL)
	{
       // The XML declarator.
       if(memcmp(field->data,"?",1) == 0)
	   {
	       temp2 = ILibParseString(field->data,
		                           0,
		                           field->datalength,
		                           "?>",
		                           2);
 
			// If not found the end flag, parse stop.
	 		if(temp2->NumResults <=1)
	 		{
	 			field = field->NextResult;
	 			ILibDestructParserResults(temp2);
	 			break;
	 		}	

	 		StartTag = -1;
	 		EmptyTag = -1;
	 				
	 		temp = ILibParseString( temp2->FirstResult->data,
	 		                        0,
	 		                        temp2->FirstResult->datalength,
	 		                        " ",
	 		                        1 );
 
	 		TagName       = temp->FirstResult->data;
	 		TagNameLength = temp->FirstResult->datalength;
 
 			if(TagNameLength)
	 		{
	 			x = (struct ILibXMLNode*)malloc(sizeof(struct ILibXMLNode));
	 			memset(x,0,sizeof(struct ILibXMLNode));
	 			x->Name         = TagName;
	 			x->NameLength   = TagNameLength;
	 			x->StartTag     = StartTag;
	 			x->Reserved = temp2->LastResult->data;
	 
	 			if(RetVal==NULL)
	 			{
	 				RetVal = x;
	 			}
	 			else
	 			{
	 				current->Next = x;
	 			}
	 			current = x;
	 			
	 			if(EmptyTag!=0)
	 			{
	 				x = (struct ILibXMLNode*)malloc(sizeof(struct ILibXMLNode));
	 				memset(x,0,sizeof(struct ILibXMLNode));
	 				x->Name         = TagName;
	 				x->NameLength   = TagNameLength;
	 				        
	 				x->Reserved     = current->Reserved;
	 				current->EmptyTag = -1;
	 				current->Next   = x;
	 				current         = x;
	 			}
	 		}
 		
	 		ILibDestructParserResults(temp2);
	 		ILibDestructParserResults(temp);
 		
    	}
	    // The XML comment
		else if(memcmp(field->data,"!",1) == 0)
		{
			int txt_type;
	        StartTag = -1;
	        EmptyTag = -1;
		    TagName     = field->data;
			if(memcmp(field->data,"![CDATA[", 8) == 0)
			{
		              TagNameLength = 8;

				txt_type = 0;
			}
			else if(memcmp(field->data,"!--", 3) == 0)
			{
		              TagNameLength = 3;

				txt_type = 1;
			}
			else
			{
	        	field = field->NextResult;
				continue;
			}
			while(field != NULL)
			{
	            // If we look for the '>' we can find the end of this element.
	            temp2 = ILibParseString( field->data,
	                                     0,
	                                     field->datalength,
	                                     txt_type?"-->":"]]>",
	                                     3 );
				if(1 < temp2->NumResults)
				{
					break;
				}
				else
				{
		        	field = field->NextResult;
					ILibDestructParserResults(temp2);
					continue;
				}
			}

			// If not found end flag, parse stop
			if(1 >= temp2->NumResults)
			{
				field = field->NextResult;
				ILibDestructParserResults(temp2);
				break;
			}
		
            x = (struct ILibXMLNode*)malloc(sizeof(struct ILibXMLNode));
            memset(x,0,sizeof(struct ILibXMLNode));
            x->Name         = TagName;
            x->NameLength   = TagNameLength;
            x->StartTag     = StartTag;
			
            if(StartTag==0)
            {
                // The Reserved field of StartElements point to te first character before
                // the next '<'.
                x->Reserved = field->data;
                do
                {
                    x->Reserved -= 1;
                }while(*((char*)x->Reserved)=='<');
            }
            else
            {
                // The Reserved field of EndElements point to the end of the element
                x->Reserved = temp2->LastResult->data;
            }
            
            if(RetVal==NULL)
            {
                RetVal = x;
            }
            else
            {
                current->Next = x;
            }
            current = x;
            if(EmptyTag!=0)
            {
                x = (struct ILibXMLNode*)malloc(sizeof(struct ILibXMLNode));
                memset(x,0,sizeof(struct ILibXMLNode));
                x->Name         = TagName;
                x->NameLength   = TagNameLength;
                                    
                x->Reserved     = current->Reserved;
                current->EmptyTag = -1;
                current->Next   = x;
                current         = x;
            }
			
            ILibDestructParserResults(temp2);
    	}
		else
    	{
            EmptyTag = 0;
            if(memcmp(field->data,"/",1)==0)
            {
                // The first character after the '<' was a '/', so we know this is the
                // EndElement
                StartTag = 0;
                field->data = field->data+1;
                field->datalength -= 1;
  
                // If we look for the '>' we can find the end of this element
                temp2 = ILibParseString( field->data,
                                         0,
                                         field->datalength,
                                         ">",
                                         1 );
            }
            else
            {
                // The first character after the '<' was not a '/' so we know this is a 
                // StartElement
                StartTag = -1;

                // If we look for the '>' we can find the end of this element
                temp2 = ILibParseString( field->data,
                                         0,
                                         field->datalength,
                                         ">",
                                         1 );

                if(temp2->FirstResult->data[temp2->FirstResult->datalength-1]=='/')
                {
                    // If this element ended with a '/' this is an EmptyElement
                    EmptyTag = -1;
                }
            }
            // Parsing on the ' ', we can isolate the Element name from the attributes. 
            // The first token, being the element name
            temp = ILibParseString( temp2->FirstResult->data,
                                    0,
                                    temp2->FirstResult->datalength,
                                    " ",
                                    1 );
			TagName = temp->FirstResult->data;
			TagNameLength = temp->FirstResult->datalength;

            // Get the exact length , as well as check to see if it's an empty element
            for(i=0;i<TagNameLength;++i)
            {
                if( (TagName[i]==' ') ||
                    (TagName[i]=='/') ||
                    (TagName[i]=='>') ||
                    (TagName[i]=='\r')||
                    (TagName[i]=='\n')  )
                {
                    if(i!=0)
                    {
                        if(TagName[i]=='/')
                        {
                            EmptyTag = -1;
                        }
                        TagNameLength = i;
                        break;
                    }
                }
            }
            
            if(TagNameLength > 0)
            {
                x = (struct ILibXMLNode*)malloc(sizeof(struct ILibXMLNode));
                memset(x,0,sizeof(struct ILibXMLNode));
                x->Name         = TagName;
                x->NameLength   = TagNameLength;
                x->StartTag     = StartTag;
                
                if(StartTag==0)
                {
                    // The Reserved field of StartElements point to te first character before
                    // the '<'
                    x->Reserved = field->data;
                    do
                    {
                       x->Reserved -= 1;
                    }while(*((char*)x->Reserved)=='<');
                }
                else
                {
                    // The Reserved field of EndElements point to the end of the element
                    x->Reserved = temp2->LastResult->data;
                }
                
                if(RetVal==NULL)
                {
                    RetVal = x;
                }
                else
                {
                    current->Next = x;
                }
                current = x;
                if(EmptyTag!=0)
                {
                    // If this was an empty element, we need to create a bogus EndElement 
                    //to maintain the structure consistently.
                    x = (struct ILibXMLNode*)malloc(sizeof(struct ILibXMLNode));
                    memset(x,0,sizeof(struct ILibXMLNode));
                    x->Name         = TagName;
                    x->NameLength   = TagNameLength;
                                        
                    x->Reserved     = current->Reserved;
                    current->EmptyTag = -1;
                    current->Next   = x;
                    current         = x;
                }
            }
            
            ILibDestructParserResults(temp2);
            ILibDestructParserResults(temp);
        }
        field = field->NextResult;
    }

    ILibDestructParserResults(xml);
    return(RetVal);
}

//Parses a string into a linked list of tokens.
//Differs from ILibParseString, this method ingore the string between quotes.
struct parser_result* ILibParseStringAdv( char* buffer,
                                          int   offset, 
                                          int   length,
                                          char* Delimiter, 
                                          int   DelimiterLength )
{
    struct parser_result* RetVal = (struct parser_result*)malloc(sizeof(struct parser_result));
    int i=0;    
    char* Token = NULL;
    int TokenLength = 0;
    struct parser_result_field *p_resultfield;
    int Ignore = 0;
    char StringDelimiter=0;
    
    RetVal->FirstResult = NULL;
    RetVal->NumResults = 0;
    
    Token = buffer + offset;
    for(i=offset;i<(length+offset);++i)
    {
        if(StringDelimiter==0)
        {
            if(buffer[i]=='"') 
            {
                // Ignore everything inside double quotes
                StringDelimiter='"';
                Ignore=1;
            }
            else
            {
                if(buffer[i]=='\'')
                {
                    // Ignore everything inside single quotes
                    StringDelimiter='\'';
                    Ignore=1;
                }
            }
        }
        else
        {
            // Once we isolated everything inside double or single quotes, we can get
            // on with the real parsing
            if(buffer[i]==StringDelimiter)
            {
                Ignore=((Ignore==0)?1:0);
            }
        }
        if(Ignore==0 && ILibIsDelimiter(buffer,i,length,Delimiter,DelimiterLength))
        {
            p_resultfield = (struct parser_result_field*)malloc(sizeof(struct parser_result_field));
            p_resultfield->data = Token;
            p_resultfield->datalength = TokenLength;
            p_resultfield->NextResult = NULL;
            if(RetVal->FirstResult != NULL)
            {
                RetVal->LastResult->NextResult = p_resultfield;
                RetVal->LastResult = p_resultfield;
            }
            else
            {
                RetVal->FirstResult = p_resultfield;
                RetVal->LastResult = p_resultfield;
            }

            ++RetVal->NumResults;
            i = i + DelimiterLength -1;
            Token = Token + TokenLength + DelimiterLength;
            TokenLength = 0;    
        }
        else
        	++TokenLength;
    }

    p_resultfield = (struct parser_result_field*)malloc(sizeof(struct parser_result_field));
    p_resultfield->data = Token;
    p_resultfield->datalength = TokenLength;
    p_resultfield->NextResult = NULL;
    if(RetVal->FirstResult != NULL)
    {
        RetVal->LastResult->NextResult = p_resultfield;
        RetVal->LastResult = p_resultfield;
    }
    else
    {
        RetVal->FirstResult = p_resultfield;
        RetVal->LastResult = p_resultfield;
    }   
    ++RetVal->NumResults;
    
    return RetVal;
}

// Parses a string into a linked list of tokens.
struct parser_result* ILibParseString( char* buffer,
                                       int   offset, 
                                       int   length, 
                                       char* Delimiter, 
                                       int   DelimiterLength )
{
    struct parser_result* RetVal = (struct parser_result*)malloc(sizeof(struct parser_result));
    int i=0;    
    char* Token = NULL;
    int TokenLength = 0;
    struct parser_result_field *p_resultfield;
    
    RetVal->FirstResult = NULL;
    RetVal->NumResults = 0;
    
    Token = buffer + offset;
    for(i=offset;i<length;++i)
    {
        if(ILibIsDelimiter(buffer,i,length,Delimiter,DelimiterLength))
        {
            // We found a delimiter in the string
            p_resultfield = (struct parser_result_field*)malloc(sizeof(struct parser_result_field));
            p_resultfield->data = Token;
            p_resultfield->datalength = TokenLength;
            p_resultfield->NextResult = NULL;
            if(RetVal->FirstResult != NULL)
            {
                RetVal->LastResult->NextResult = p_resultfield;
                RetVal->LastResult = p_resultfield;
            }
            else
            {
                RetVal->FirstResult = p_resultfield;
                RetVal->LastResult = p_resultfield;
            }

            ++RetVal->NumResults;
            i = i + DelimiterLength -1;
            Token = Token + TokenLength + DelimiterLength;
            TokenLength = 0;    
        }
        else
        {
            ++TokenLength;
        }
    }

    // If there are no more delimiters, create a result for the rest of data.
    p_resultfield = (struct parser_result_field*)malloc(sizeof(struct parser_result_field));
    p_resultfield->data = Token;
    p_resultfield->datalength = TokenLength;
    p_resultfield->NextResult = NULL;
    if(RetVal->FirstResult != NULL)
    {
        RetVal->LastResult->NextResult = p_resultfield;
        RetVal->LastResult = p_resultfield;
    }
    else
    {
        RetVal->FirstResult = p_resultfield;
        RetVal->LastResult = p_resultfield;
    }   
    ++RetVal->NumResults;
    
    return(RetVal);
}

// Frees resources associated with the list of tokens returned from ILibParseString
// and ILibParseStringAdv.
void ILibDestructParserResults(struct parser_result *result)
{
    struct parser_result_field *node = result->FirstResult;
    struct parser_result_field *temp;
    
    while(node!=NULL)
    {
        temp = node->NextResult;
        free(node);
        node = temp;
    }
    free(result);
}
