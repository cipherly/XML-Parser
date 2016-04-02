#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ILibParsers.h"
#include "istar.h"

#define ERR(s)  {fprintf(stderr, s);}

#define	RSP_OK					0
#define	RSP_FAIL				1
static char xml_type[ISTAR_CATEGORY_LEN_MAX];

//Get the xml element's type name
static int istar_sub_type_parser(struct ILibXMLNode **inxml)
{
    int nRet = RSP_FAIL;
    struct ILibXMLNode *xml = *inxml;
    struct ILibXMLAttribute *attribs = NULL, *attr = NULL;

    memset(xml_type, '\0', sizeof(xml_type));
	
    // Get the attributes 
    attribs = attr = ILibGetXMLAttributes(xml);

    while (attr != NULL)
    {
        if (attr->NameLength == 4 && memcmp(attr->Name, "type", 4) == 0
		|| attr->NameLength == 4 && memcmp(attr->Name, "lang", 4) == 0)
        {
        	if(attr->ValueLength > 0)
        	{
        		memcpy(xml_type, attr->Value, attr->ValueLength);
				nRet = RSP_OK;
        	}
        }
        attr = attr->Next;
    }

    // Destruct XML attribute list
    ILibDestructXMLAttributeList(attribs);

    //*inxml = xml;

    return nRet;
}

/* Get the IPTV xml element's attributes */
static int istar_iptv_attr_parser(struct ILibXMLNode **inxml, struct ISTAR_IPTV_XML_ITEM *lpEntry)
{
    int nRet = RSP_FAIL;
    struct ILibXMLNode *xml = *inxml;
    struct ILibXMLAttribute *attribs = NULL, *attr = NULL;
    
    // Get elements' attribute
    attribs = attr = ILibGetXMLAttributes(xml);

    while (attr != NULL)
    {
        nRet = RSP_FAIL;
        if (attr->ValueLength > 0 && attr->NameLength == 7 && memcmp(attr->Name, "picture", 7) == 0)
        {
			if(attr->ValueLength > ISTAR_THUMB_URL_MAX){
				ERR("The thumbnail value is too big!");}
			else
				memcpy(lpEntry->thumbnail, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }
		else if (attr->ValueLength > 0 && attr->NameLength == 4 && memcmp(attr->Name, "link", 4) == 0)
        {
            if(attr->ValueLength > ISTAR_VIDEO_URL_MAX){
				ERR("The videourl value is too big!");}
			else
				memcpy(lpEntry->videourl, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }
		else if (attr->ValueLength > 0 && attr->NameLength == 4 && memcmp(attr->Name, "name", 4) == 0)
        {
            if(attr->ValueLength > ISTAR_TITLE_LEN_MAX){
				ERR("The title value is too big!");}
			else
				memcpy(lpEntry->title, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }
		attr = attr->Next; 
		
    }
	
    // Destruct XML attribute list
    ILibDestructXMLAttributeList(attribs);
    
    return nRet;
}

/* Get the movie's xml element's attributes */
static int istar_movie_attr_parser(struct ILibXMLNode **inxml, struct ISTAR_MOVIE_XML_ITEM *lpEntry)
{
    int nRet = RSP_FAIL;
    struct ILibXMLNode *xml = *inxml;
    struct ILibXMLAttribute *attribs = NULL, *attr = NULL;
    
    // Get elements' attribute
    attribs = attr = ILibGetXMLAttributes(xml);

    while (attr != NULL)
    {
        nRet = RSP_FAIL;
        if (attr->ValueLength > 0 && attr->NameLength == 7 && memcmp(attr->Name, "picture", 7) == 0)
        {
			if(attr->ValueLength > ISTAR_THUMB_URL_MAX){
				ERR("The thumbnail value is too big!");}
			else
				memcpy(lpEntry->thumbnail, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }
		else if (attr->ValueLength > 0 && attr->NameLength == 4 && memcmp(attr->Name, "link", 4) == 0)
        {
            if(attr->ValueLength > ISTAR_VIDEO_URL_MAX){
				ERR("The videourl value is too big!");}
			else
				memcpy(lpEntry->videourl, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }
		else if (attr->ValueLength > 0 && attr->NameLength == 4 && memcmp(attr->Name, "name", 4) == 0)
        {
            if(attr->ValueLength > ISTAR_TITLE_LEN_MAX){
				ERR("The title value is too big!");}
			else
				memcpy(lpEntry->title, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }
		else if (attr->ValueLength > 0 && attr->NameLength == 4 && memcmp(attr->Name, "type", 4) == 0)
        {
            if(attr->ValueLength > ISTAR_TITLE_LEN_MAX){
				ERR("The type value is too big!");}
			else
				memcpy(lpEntry->type, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }	
		attr = attr->Next; 
		
    }
	
    // Destruct XML attribute list
    ILibDestructXMLAttributeList(attribs);
    
    return nRet;
}

/* Get the series's xml element's attributes */
static int istar_series_attr_parser(struct ILibXMLNode **inxml, struct ISTAR_SERIES_XML_ITEM *lpEntry)
{
    int nRet = RSP_FAIL;
    struct ILibXMLNode *xml = *inxml;
    struct ILibXMLAttribute *attribs = NULL, *attr = NULL;
    
    // Get elements' attribute
    attribs = attr = ILibGetXMLAttributes(xml);

    while (attr != NULL)
    {
        nRet = RSP_FAIL;
        if (attr->ValueLength > 0 && attr->NameLength == 7 && memcmp(attr->Name, "picture", 7) == 0)
        {
			if(attr->ValueLength > ISTAR_THUMB_URL_MAX){
				ERR("The thumbnail value is too big!");}
			else
				memcpy(lpEntry->thumbnail, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }
		else if (attr->ValueLength > 0 && attr->NameLength == 4 && memcmp(attr->Name, "link", 4) == 0)
        {
            if(attr->ValueLength > ISTAR_VIDEO_URL_MAX){
				ERR("The videourl value is too big!");}
			else
				memcpy(lpEntry->videourl, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }
		else if (attr->ValueLength > 0 && attr->NameLength == 4 && memcmp(attr->Name, "name", 4) == 0)
        {
            if(attr->ValueLength > ISTAR_TITLE_LEN_MAX){
				ERR("The title value is too big!");}
			else
				memcpy(lpEntry->title, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }
		else if (attr->ValueLength > 0 && attr->NameLength == 5 && memcmp(attr->Name, "total", 5) == 0)
        {
            if(attr->ValueLength > ISTAR_TITLE_LEN_MAX){
				ERR("The total value is too big!");}
			else
				memcpy(lpEntry->total, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }
		else if (attr->ValueLength > 0 && attr->NameLength == 4 && memcmp(attr->Name, "type", 4) == 0)
        {
            if(attr->ValueLength > ISTAR_TITLE_LEN_MAX){
				ERR("The type value is too big!");}
			else
				memcpy(lpEntry->type, attr->Value, attr->ValueLength);
			nRet = RSP_OK;
        }	
		attr = attr->Next; 
		
    }
	
    // Destruct XML attribute list
    ILibDestructXMLAttributeList(attribs);
    
    return nRet;
}

int main(void)
{
	struct ISTAR_SERIES_INFO series_info;
    //struct ISTAR_IPTV_INFO iptv_info;
	char lpBuffer[1000000];//= NULL;	
	int i;
	int nRet = RSP_FAIL;
	int buf_size = 0;
	struct ILibXMLNode *rootXML = NULL, *xml = NULL;
	struct ISTAR_SERIES_XML_ITEM *lpEntry = NULL;
	
	FILE *fp_r, *fp_w;
	unsigned long file_length;
	
	lpEntry = series_info.list;
	//lpEntry = iptv_info.list;	
	
	fp_r = fopen("Seriese_content.xml", "r");
	//fp_r = fopen("movie_content.xml", "r");
	//fp_r = fopen("Online-Tv .xml", "r");

	// The test result will be saved here
	fp_w = fopen("output.txt", "a+");
	fseek(fp_r, 0, SEEK_END);
	file_length = ftell(fp_r);
	fseek(fp_r, 0, SEEK_SET);
	
	if((buf_size = fread(lpBuffer, 1, file_length, fp_r)) <= 0){
		ERR("No data!!!");
		return -1;
	}
	
	if(lpBuffer && buf_size > 0 && lpEntry)
	{
		//Get the xml_name list
		rootXML = xml = ILibParseXML(lpBuffer, 0, buf_size);

		nRet = RSP_FAIL;
		while(xml != NULL)
		{
			if(xml->NameLength == 8 && memcmp(xml->Name, "catagory", 8) == 0
			|| xml->NameLength == 8 && memcmp(xml->Name, "language", 8) == 0)
			{
				nRet = istar_sub_type_parser(&xml);
				if(nRet)
					continue;
				xml = xml->Next;
				
				while(xml!=NULL && *(xml->Name-1)!='/' && i<ISTAR_ITEM_NUMBER)
				{
					nRet = RSP_FAIL;
					if(xml->EmptyTag){
						if(xml->Next == NULL)
							break;
						else xml = xml->Next;
					}	
					if(xml->NameLength==7 && memcmp(xml->Name, "channel", 7)==0
					|| xml->NameLength==6 && memcmp(xml->Name, "series", 6)==0
					|| xml->NameLength==5 && memcmp(xml->Name, "movie", 5)==0)
					{
						memcpy(lpEntry[i].category, xml_type, ISTAR_CATEGORY_LEN_MAX);
						nRet = istar_series_attr_parser(&xml, &lpEntry[i]);
					}
					if(nRet == RSP_OK)
						i++;
					xml = xml->Next;
				}
				if(xml == NULL || i >= ISTAR_ITEM_NUMBER)
					break;
			}
			xml = xml->Next;
		}
		
		// Destruct XML node list
        ILibDestructXMLNodeList(rootXML);
	}
	for(i=0; i<ISTAR_ITEM_NUMBER; i++){
		fwrite(lpEntry[i].category, 1, ISTAR_CATEGORY_LEN_MAX, fp_w);
		fwrite("\r\n", 1, 2, fp_w);
		fwrite(lpEntry[i].thumbnail, 1, ISTAR_THUMB_URL_MAX, fp_w);
		fwrite("\r\n", 1, 2, fp_w);
		fwrite(lpEntry[i].videourl, 1, ISTAR_VIDEO_URL_MAX, fp_w);
		fwrite("\r\n", 1, 2, fp_w);
		fwrite(lpEntry[i].title, 1, ISTAR_TITLE_LEN_MAX, fp_w);
		fwrite("\r\n", 1, 2, fp_w);

		fwrite(lpEntry[i].type, 1, ISTAR_TYPE_LEN_MAX, fp_w);
		fwrite("\r\n\r\n", 1, 4, fp_w);
	}		
	fclose(fp_r);
	fclose(fp_w);
	
	return nRet;
}