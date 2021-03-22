#ifndef	COMMON_H
#define	COMMON_H

#define MAX_BUFFER_SIZE 50
#define SERVER_PORT 8888
#define CHANNEL_PORT 7777
#define SERVER_ADDRESS_STR "127.0.0.1"
#define CHANNEL_ADDRESS_STR "127.0.0.1"
typedef enum { FAIL = 1 } return_values;



//debug 
#define DEBUG(MSG) printf("function:[%s line:%d] %s\n", __func__, __LINE__,MSG)


#endif