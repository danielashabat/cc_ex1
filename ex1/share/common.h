#ifndef	COMMON_H
#define	COMMON_H


#define PACKET_SIZE 1500
#define MAX_BUFFER_SIZE PACKET_SIZE+1
#define SERVER_PORT 8888
#define CHANNEL_PORT 7777
#define SERVER_ADDRESS_STR "127.0.0.1"
#define CHANNEL_ADDRESS_STR "127.0.0.1"
#define FEEDBACK_MSG_LEN 100
#define FILE_LEN 100//verify it

typedef enum { FAIL = 1 } return_values;
typedef enum { EXIT , SEND, RECIEVE,DECODE, ADD_NOISE,FEEDBACK} states;




//debug 
#define DEBUG(MSG) printf("function:[%s line:%d] %s\n", __func__, __LINE__,MSG)


#endif