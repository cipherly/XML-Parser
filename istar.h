#define ISTAR_ITEM_NUMBER        	2000

#define ISTAR_CATEGORY_LEN_MAX     20
#define ISTAR_TITLE_LEN_MAX     	128
#define ISTAR_THUMB_URL_MAX    	128
#define ISTAR_VIDEO_URL_MAX    	256 //128
#define ISTAR_NUM_TOTAL_MAX     4
#define ISTAR_TYPE_LEN_MAX      20

typedef enum
{
	EN_TYPE_IPTV,
	EN_TYPE_MOVIE,
	EN_TYPE_SERIES,
	EN_TYPE_SHARE,
	EN_TYPE_NUM
}EN_ISTAR_SERVER_TYPE;

struct ISTAR_IPTV_XML_ITEM 
{
	char category[ISTAR_CATEGORY_LEN_MAX];
    char title[ISTAR_TITLE_LEN_MAX];
    char thumbnail[ISTAR_THUMB_URL_MAX];
	char videourl[ISTAR_VIDEO_URL_MAX];
};

struct ISTAR_MOVIE_XML_ITEM 
{
	char category[ISTAR_CATEGORY_LEN_MAX];
    char title[ISTAR_TITLE_LEN_MAX];
    char thumbnail[ISTAR_THUMB_URL_MAX];
	char videourl[ISTAR_VIDEO_URL_MAX];
	char type[ISTAR_TYPE_LEN_MAX];
};

struct ISTAR_SERIES_XML_ITEM 
{
	char category[ISTAR_CATEGORY_LEN_MAX];
	char title[ISTAR_TITLE_LEN_MAX];
	char thumbnail[ISTAR_THUMB_URL_MAX];
	char videourl[ISTAR_VIDEO_URL_MAX];
	char total[ISTAR_NUM_TOTAL_MAX];
	char type[ISTAR_TYPE_LEN_MAX];
};

struct ISTAR_IPTV_INFO 
{
    int num_entries;
    struct ISTAR_IPTV_XML_ITEM list[ISTAR_ITEM_NUMBER];
};

//Movie&Series
struct ISTAR_SERIES_INFO 
{
    int num_entries;
    struct ISTAR_SERIES_XML_ITEM list[ISTAR_ITEM_NUMBER];
};
