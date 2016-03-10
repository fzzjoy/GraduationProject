/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: USB drive control */
//#include "atadrive.h"	/* Example: ATA drive control */
//#include "sdcard.h"		/* Example: MMC/SDC contorl */
#include "stm32_eval_sdio_sd.h"

/* Definitions of physical drive number for each media */
//#define ATA		0
//#define MMC		1
//#define USB		2
#define SD_CARD 0

extern SD_CardInfo SDCardInfo;
SD_Error status;

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case SD_CARD:
		status = SD_Init();
		if(status != SD_OK)
			return STA_NOINIT;
		else
			return RES_OK;
//	case ATA :
//		result = ATA_disk_initialize();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case MMC :
//		result = MMC_disk_initialize();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case USB :
//		result = USB_disk_initialize();
//
//		// translate the reslut code here
//
//		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case SD_CARD:
		status = SD_GetCardInfo(&SDCardInfo);
		if(status != SD_OK)
			return STA_NOINIT;
		else
			return RES_OK;
//	case ATA :
//		result = ATA_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case MMC :
//		result = MMC_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case USB :
//		result = USB_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {	
	case SD_CARD:
//  	for (secNum = 0; secNum < count && status == SD_OK; secNum++)
//   	{
//   		status = SD_ReadBlock((sector+secNum)*512, (u32*)(buff+512*secNum),512);          
//   	}
		if(count == 1)
			status = SD_ReadBlock((u8*)buff, sector*512, 512);	
		else
			status = SD_ReadMultiBlocks((u8*)buff, sector*512, 512, count);
		if(status != SD_OK)
			return RES_ERROR;
		else
			return RES_OK;
//	case ATA :
//		// translate the arguments here
//
//		result = ATA_disk_read(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case MMC :
//		// translate the arguments here
//
//		result = MMC_disk_read(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case USB :
//		// translate the arguments here
//
//		result = USB_disk_read(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD_CARD:
		if(count == 1)
			status = SD_WriteBlock((u8*)buff, sector*512, 512);
		else
			status = SD_WriteMultiBlocks((u8*)buff, sector*512, 512, count);
		if(status != SD_OK)
			return RES_ERROR;
		else
			return RES_OK;
//	case ATA :
//		// translate the arguments here
//
//		result = ATA_disk_write(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case MMC :
//		// translate the arguments here
//
//		result = MMC_disk_write(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case USB :
//		// translate the arguments here
//
//		result = USB_disk_write(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD_CARD:
		switch (cmd)
		{
			case CTRL_SYNC:
				// no synchronization to do since not buffering in this module
				return RES_OK;
			case GET_SECTOR_SIZE:
				*(WORD*)buff = 512;
				return RES_OK;
			case GET_SECTOR_COUNT:
				*(DWORD*)buff = SDCardInfo.CardCapacity/512;
				return RES_OK;
			case GET_BLOCK_SIZE:
				*(DWORD*)buff = 512;
				return RES_OK;
      }
		return res;
//	case ATA :
//		// pre-process here
//
//		result = ATA_disk_ioctl(cmd, buff);
//
//		// post-process here
//
//		return res;
//
//	case MMC :
//		// pre-process here
//
//		result = MMC_disk_ioctl(cmd, buff);
//
//		// post-process here
//
//		return res;
//
//	case USB :
//		// pre-process here
//
//		result = USB_disk_ioctl(cmd, buff);
//
//		// post-process here
//
//		return res;
	}
	return RES_PARERR;
}
#endif

DWORD get_fattime (void)
{
}
