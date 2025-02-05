#ifndef __AES_H__
#define __AES_H__
 
#include "main.h"
//#include "comtypes.h"
#include <stdlib.h>
 
 
uint8_t* Robot_AES_Code(int is_encrypt,uint8_t*databuf,uint16_t databuflen,uint16_t*outputlen);
 
#endif