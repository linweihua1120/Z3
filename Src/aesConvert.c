
#include "main.h"
#include "app_bootloader.h"
#include "app_usart.h"
#include "app_i2c.h"
#include "app_flash.h"
#include "app_wdg.h"
#include "usb_config.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define AES_BLOCK_SIZE 16
#define PAGE_SIZE 256

//// AES CBC 解密函数
//void aesCBCDecrypt(uint8_t* input, uint8_t* output, uint8_t* key, uint8_t* iv, int length) {
//    AES_KEY decKey;
//    AES_set_decrypt_key(key, 128, &decKey);
//    
//    uint8_t iv_copy[AES_BLOCK_SIZE];
//    memcpy(iv_copy, iv, AES_BLOCK_SIZE);
//
//    AES_cbc_encrypt(input, output, length, &decKey, iv_copy, AES_DECRYPT);
//}
//
//// MD5 计算函数
//void calculateMD5(uint8_t* data, int length, uint8_t* md5Out) {
//    MD5_CTX md5Ctx;
//    MD5_Init(&md5Ctx);
//    MD5_Update(&md5Ctx, data, length);
//    MD5_Final(md5Out, &md5Ctx);
//}

int flashCopy(uint32_t dAddress, uint32_t sAddress, uint32_t size) {
    uint32_t currentSize = 0;
    uint32_t currentDAddress = dAddress;
    uint32_t currentSAddress = sAddress;
    uint16_t pageNum[1];
	//SEGGER_RTT_printf(0,"flashCopy\n");
	
		while (currentSize < size) {
			// 计算当前页号
			pageNum[0] = (currentDAddress - 0x8000000) / PAGE_SIZE; // 根据实际地址范围调整
		   
	//	SEGGER_RTT_printf(0,"currentDAddress = %x\n",currentDAddress);
			// 检查 FLASH_SR 寄存器，确认没有正在进行的 flash 操作
			if (FLASH_SR_BSY == READ_BIT(FLASH->SR, FLASH_SR_BSY)) {
				APP_Bootloader_SendByte(NACK_BYTE);
				SEGGER_RTT_printf(0, "FLASH_SR_BSY == READ_BIT(FLASH->SR, FLASH_SR_BSY) pageNum=%d \r\n", pageNum[0]);
				return 0;
			}
	
			// 向 FLASH_KEYR 寄存器依次写 KEY1 和 KEY2，解除 FLASH_CR 寄存器的保护
			if (FLASH_CR_LOCK == READ_BIT(FLASH->CR, FLASH_CR_LOCK)) {
				WRITE_REG(FLASH->KEYR, FLASH_KEY1);
				WRITE_REG(FLASH->KEYR, FLASH_KEY2);
				APP_SetFlashParameter();
			}
	
			// 擦除当前页
			int res = APP_PageErase(pageNum, 0, 1);
			if (res == 0) {
				SEGGER_RTT_printf(0, ">>APP_PageErase error !!!! pageNum=%d \r\n", pageNum[0]);
				return 0;
			}

			
 	
 			// 计算写入数据的大小
 			uint32_t bytesToWrite = (size - currentSize >= PAGE_SIZE) ? PAGE_SIZE : (size - currentSize);
 			
 	
	
			// 更新当前地址和当前大小
			currentDAddress += PAGE_SIZE;
			currentSAddress += PAGE_SIZE;
			currentSize += bytesToWrite;
		}

		
      currentSize = 0;
     currentDAddress = dAddress;
      currentSAddress = sAddress;
    while (currentSize < size) {
        // 计算当前页号
        pageNum[0] = (currentDAddress - 0x8000000) / PAGE_SIZE; // 根据实际地址范围调整
       

        // 计算写入数据的大小
        uint32_t bytesToWrite = (size - currentSize >= PAGE_SIZE) ? PAGE_SIZE : (size - currentSize);
		
		//检查 FLASH_SR 寄存器 BSY 位，确认没有正在进行的 flash 操作
		if (FLASH_SR_BSY == READ_BIT(FLASH->SR, FLASH_SR_BSY))
		{
		  APP_Bootloader_SendByte(NACK_BYTE);
            SEGGER_RTT_printf(0, "\r\n write error res = %d\r\n", 9);
		  return 0;
		}
		//向 FLASH_KEYR 寄存器依次写 KEY1 和 KEY2，解除 FLASH_CR 寄存器的保护
		if (FLASH_CR_LOCK == READ_BIT(FLASH->CR, FLASH_CR_LOCK))
		{
		  WRITE_REG(FLASH->KEYR, FLASH_KEY1);
		  WRITE_REG(FLASH->KEYR, FLASH_KEY2);
		  APP_SetFlashParameter();
		}

        // 写入数据
       int res = APP_WriteFlash(currentDAddress, (uint8_t*)currentSAddress, bytesToWrite);
        if (res == 0) {
            SEGGER_RTT_printf(0, ">>APP_WriteFlash error currentAddress = %x\r\n", currentDAddress);
            return 0;
        }
//		print_int_array(0, "d",(uint8_t*)currentDAddress,8);
        // 更新当前地址和当前大小
        currentDAddress += PAGE_SIZE;
        currentSAddress += PAGE_SIZE;
        currentSize += bytesToWrite;
    }


//	SEGGER_RTT_printf(0,"flashCopy  end \n");

    return 1; // 成功完成复制
}

int aesConvert(uint32_t startAddress, uint8_t* key, uint8_t* iv, int pageSize, int totalSize, uint8_t* md5Out) {
    // 临时缓冲区存储解密数据
    uint8_t decryptedPage[PAGE_SIZE];
    
    uint32_t  currentAddress = startAddress;
    uint8_t iv_copy[AES_BLOCK_SIZE];
    uint16_t pageNum[1];
    pageNum[1] = 128;
		
    for (int offset = 0; offset < totalSize; offset += pageSize) {
//		  SEGGER_RTT_printf(0, "\r\noffset = %d\r\n"  ,offset);
        // 将 IV 复制一份，以避免在解密过程中被修改
        memcpy(iv_copy, iv, AES_BLOCK_SIZE);
        
		pageNum[0]= (currentAddress - 0x8000000 )/PAGE_SIZE ;
        // AES 解密操作，解密 256 字节数据
         AESDecode( (uint8_t*)currentAddress,key,decryptedPage,PAGE_SIZE );
		  //检查 FLASH_SR 寄存器 BSY 位，确认没有正在进行的 flash 操作
		 if (FLASH_SR_BSY == READ_BIT(FLASH->SR, FLASH_SR_BSY))
		 {
		   APP_Bootloader_SendByte(NACK_BYTE);
		   return 0;
		 }
		 //向 FLASH_KEYR 寄存器依次写 KEY1 和 KEY2，解除 FLASH_CR 寄存器的保护
		 if (FLASH_CR_LOCK == READ_BIT(FLASH->CR, FLASH_CR_LOCK))
		 {
		   WRITE_REG(FLASH->KEYR, FLASH_KEY1);
		   WRITE_REG(FLASH->KEYR, FLASH_KEY2);
		   APP_SetFlashParameter();
		 }
		//擦除页
     	int res= APP_PageErase((uint16_t *)pageNum, 0, 1);
 		if(res==0)
       	{
 			SEGGER_RTT_printf(0, ">>APP_PageErase error !!!! pageNum=%d \r\n" ,pageNum[0] );
			return 0;
       	}
			 //检查 FLASH_SR 寄存器 BSY 位，确认没有正在进行的 flash 操作
	    if (FLASH_SR_BSY == READ_BIT(FLASH->SR, FLASH_SR_BSY))
	    {
	      APP_Bootloader_SendByte(NACK_BYTE);
		  SEGGER_RTT_printf(0, "\r\n write error  res = %d\r\n"  ,res);
	      return 0;
	    }
	    //向 FLASH_KEYR 寄存器依次写 KEY1 和 KEY2，解除 FLASH_CR 寄存器的保护
	    if (FLASH_CR_LOCK == READ_BIT(FLASH->CR, FLASH_CR_LOCK))
	    {
	      WRITE_REG(FLASH->KEYR, FLASH_KEY1);
	      WRITE_REG(FLASH->KEYR, FLASH_KEY2);
	      APP_SetFlashParameter();
	    }

		//写入数据
	   res = APP_WriteFlash((uint32_t  )currentAddress, decryptedPage, PAGE_SIZE-1);
// 			SEGGER_RTT_printf(0, ">>APP_Write   currentAddress = %x\r\n" ,currentAddress );
// 	   SEGGER_RTT_printf(0, "\r\n write  res = %x\r\n"  ,res);
       if(res==0 )
       	{
 			SEGGER_RTT_printf(0, ">>APP_WriteFlash error  currentAddress = %x\r\n" ,currentAddress );
			return 0;
       	}
        
//		print_int_array( 0 , "newsFlash" , (uint8_t*)currentAddress, 8);
// 		SEGGER_RTT_printf(0, ">>aes  current = %x\r\n" ,currentAddress );
        // 更新当前地址
        currentAddress += pageSize;
//		break;
    }
    
    // 计算解密后的总数据的 MD5 值
	md5_sign(startAddress,totalSize,md5Out);
	print_int_array(0, "mdtout", md5Out, 16);
	return 1;
}
typedef struct {
    uint8_t md5_1[16];
    uint8_t md5_2[16];
    uint8_t md5_3[16];
    uint8_t cmd5_1[16];
    uint8_t cmd5_2[20];
    uint8_t cmd5_3[16];
    uint8_t cmd5_4[16];
    uint32_t start_address1;
    uint32_t start_address2;
    uint32_t length;
} ParsedData;

// 打印解析后的数据（调试用）
void printParsedData(ParsedData *parsed) {
    SEGGER_RTT_printf(0,"MD5_1: ");
    for (int i = 0; i < 16; i++) {
        SEGGER_RTT_printf(0,"%02x", parsed->md5_1[i]);
    }
    SEGGER_RTT_printf(0,"\n");

    SEGGER_RTT_printf(0,"MD5_2: ");
    for (int i = 0; i < 16; i++) {
       SEGGER_RTT_printf(0,"%02x", parsed->md5_2[i]);
    }
    SEGGER_RTT_printf(0,"\n");

    SEGGER_RTT_printf(0,"MD5_3: ");
    for (int i = 0; i < 16; i++) {
       SEGGER_RTT_printf(0,"%02x", parsed->md5_3[i]);
    }
    SEGGER_RTT_printf(0,"\n");

    SEGGER_RTT_printf(0,"Start Address 1: 0x%08x\n", parsed->start_address1);
    SEGGER_RTT_printf(0,"Start Address 2: 0x%08x\n", parsed->start_address2);
    SEGGER_RTT_printf(0,"Length: %u\n", parsed->length);
}

// 解析函数
void parseData(uint8_t *input, ParsedData *parsed) {
    // 解析第一个 MD5 (0-15 字节)
    memcpy(parsed->md5_1, input, 16);
    
    // 解析第二个 MD5 (16-31 字节)
    memcpy(parsed->md5_2, input + 16, 16);
    
    // 解析第三个 MD5 (32-47 字节)
    memcpy(parsed->md5_3, input + 32, 16);
    
    // 解析起始地址 1 (48-51 字节)
    parsed->start_address1 = ((uint32_t)input[51]) | 
                             ((uint32_t)input[50] << 8) |
                             ((uint32_t)input[49] << 16) |
                             ((uint32_t)input[48] << 24);

    // 解析起始地址 2 (52-55 字节)
    parsed->start_address2 = ((uint32_t)input[55]) | 
                             ((uint32_t)input[54] << 8) |
                             ((uint32_t)input[53] << 16) |
                             ((uint32_t)input[52] << 24);
    
    // 解析长度 (56-59 字节)
    parsed->length = ((uint32_t)input[59]) | 
                     ((uint32_t)input[58] << 8) |
                     ((uint32_t)input[57] << 16) |
                     ((uint32_t)input[56] << 24);
}

void verification(){
	uint8_t cmd[60];
	uint8_t aeskey[16];
    APP_Bootloader_SendByte(ACK_BYTE);
	APP_Bootloader_ReadData(cmd,60);
	 ParsedData parsed;
    parseData(cmd, &parsed);
    printParsedData(&parsed);
	md5_sign((uint8_t* )(parsed.start_address1 ),parsed.length,parsed.cmd5_1);
	print_int_array(0 ,"md5 :",  parsed.cmd5_1, 16);
	if(memcmp(parsed.cmd5_1,parsed.md5_2,sizeof(parsed.cmd5_1)))
		{
    		SEGGER_RTT_printf(0,"md51  diffrent\n");
			return;
		}
//	char* key =  "MYSROBOT11235813";
	
	md5_sign((uint8_t* )(OPTR_BASE ),256,aeskey);
	aesConvert( parsed.start_address1, aeskey,  aeskey, 0x100, parsed.length,parsed.cmd5_2);

	if(memcmp(parsed.cmd5_2,parsed.md5_1,sizeof(parsed.cmd5_1)))
		{
    		SEGGER_RTT_printf(0,"md52  diffrent\n");
			return;
		}

	parsed.cmd5_2[16] = 0;
	parsed.cmd5_2[17] = 0;

	parsed.cmd5_2[18] = 0x11;
	parsed.cmd5_2[19] = 0x20;
	print_int_array(0 ,"md52 :",  parsed.cmd5_2, 20);
	md5_sign((uint8_t* )(parsed.cmd5_2 ),20,parsed.cmd5_3);
	if(memcmp(parsed.cmd5_3,parsed.md5_3,16))
		{
			print_int_array(0 ,"md53 :",  parsed.md5_3, 16);
			print_int_array(0 ,"md53 :",  parsed.cmd5_3, 16);
    		SEGGER_RTT_printf(0,"md53  1111 diffrent\n");
			return;
		}
	print_int_array(0 ,"md53 :",  parsed.cmd5_3, 16);
	
//	SEGGER_RTT_printf(0,"flashCopy\n");
//
//	flashCopy( parsed.start_address2, parsed.start_address1, parsed.length);
//    SEGGER_RTT_printf(0,"flashCopy end \n");
//	md5_sign((uint8_t* )(parsed.start_address2 ),parsed.length,parsed.cmd5_4);
//	
//    SEGGER_RTT_printf(0,"md5_sign end \n");
//	print_int_array(0 ,"md54 :",  parsed.cmd5_4, 16);
//
//	int diff = memcmp((uint8_t* )(parsed.start_address2 ),(uint8_t* )(parsed.start_address2 ),parsed.length);
//    SEGGER_RTT_printf(0,"diff =%d \n",diff);
//	print_int_array(0 ,"md54 :", (uint8_t* )(parsed.start_address1 ), 16);
//	print_int_array(0 ,"md54 :", (uint8_t* )(parsed.start_address2 ), 16);
//	if(memcmp(parsed.cmd5_4,parsed.md5_1,sizeof(parsed.cmd5_1)))
//		{
//			print_int_array(0 ,"md5_1 :",  parsed.md5_1, 16);
//			print_int_array(0 ,"cmd5_4 :",  parsed.cmd5_4, 16);
//    		SEGGER_RTT_printf(0,"md54 444 diffrent\n");
//			return;
//		}
//	uint8_t buff[16];
//	uint8_t* p = (uint8_t*) 0x8008000;
//	md5_sign(p,6400,buff);
//	print_int_array(0 ,"md5 :",  buff, 16);
//	print_int_array(0, "cmd:", cmd, 56);
//
//	uint8_t iv[16]={0};
//	uint8_t md5[16]={0};
//	char* key =  "MYSROBOT11235813";
//	aesConvert( 0x8008000, key,  iv, 256, 6400,md5);
    APP_Bootloader_SendByte(ACK_BYTE);

}

