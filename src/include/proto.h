#ifndef PROTO_H__
#define PROTO_H__

#include <site_type.h>

//多播组的地址和端口
#define DEFAULT_MGROUP			"224.2.2.2"
#define DEFAULT_RCVPORT			"1989"

//总体的频道号
#define CHNNR					100

//当前节目单的专有频道号
#define LISTCHNID				0

//最小频道号和最大频道号
#define MINCHNID				1	
#define MAXCHNID				(MINCHNID+CHNNR-1)

//频道内容数据包大小的上限
#define MSG_CHANNEL_MAX			(65536-20-8)
#define MAX_DATA				(MSG_CHANNEL_MAX-sizeof(chnid_t))

//节目单数据包大小的上限
#define MSG_LIST_MAX			(65536-20-8)
#define MAX_ENTRY				(MSG_LIST_MAX-sizeof(chnid_t))

struct msg_channel_st
{
	chnid_t chnid;			/*must between [MINCHNID,MAXCHNID]*/
	uint8_t data[1];
}__attribute__((packed));

struct msg_listentry_st
{
	chnid_t chnid;
	uint16_t len;
	uint8_t desc[1];
}__attribute__((packed));

struct msg_list_st
{
	chnid_t chnid;			/*must be LISTCHNID*/
	struct msg_listentry_st entry[1];
}__attribute__((packed));


#endif





