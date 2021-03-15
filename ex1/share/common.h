#ifndef	COMMON_H
#define	COMMON_H

#define SERVER_ADDRESS_STR "127.0.0.1"
#define MAX_BUFFER_SIZE 100
typedef enum { FAIL = 1 } return_values;



//debug 
#define DEBUG(MSG) printf("function:[%s line:%d] %s\n", __func__, __LINE__,MSG)


#endif