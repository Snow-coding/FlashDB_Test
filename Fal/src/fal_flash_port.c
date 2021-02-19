/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-26     armink       the first version
 */

#include <fal.h>
#include <stdio.h>

#ifndef FAL_USING_NOR_FLASH_DEV_NAME
#define FAL_USING_NOR_FLASH_DEV_NAME             "norflash0"
#endif

static int init(void);
static int read(long offset, uint8_t *buf, size_t size);
static int write(long offset, const uint8_t *buf, size_t size);
static int erase(long offset, size_t size);

#define FLASH_MAX_SIZE 4096

static uint8_t flashBlk[FLASH_MAX_SIZE];

struct fal_flash_dev nor_flash0 =
{
    .name       = FAL_USING_NOR_FLASH_DEV_NAME,
    .addr       = 0,
    .len        = 4096,
    .blk_size   = 1024,
    .ops        = {init, read, write, erase},
    .write_gran = 8
};

static int init(void)
{
    //memset(flashBlk,0xff,nor_flash0.len);

    //printf("flash init %d\r\n",flashBlk[0]);
    flash_cache_file_reload();

    return 0;
}

static int read(long offset, uint8_t *buf, size_t size)
{
    memcpy(buf, flashBlk + offset, size);

     printf("read data 0x%02x 0x%02x: \r\n",offset,size);
    for(uint8_t i=0;i < size;i++)
    {
        printf("%02x ",buf[i]);
    }
    printf("\r\n");

    return size;
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    printf("write data 0x%02x 0x%02x: \r\n",offset,size);
    for(uint8_t i = 0;i < size;i++)
    {
        printf("%02x ",buf[i]);
    }
    printf("\r\n");

    uint8_t *pBuf = flashBlk + offset;
    for(uint8_t i = 0;i < size;i++)
    {
        if(pBuf[i] != 0xff)
        {
            log_e("ERROR:flash not empty\n");
            return;
        }
    }

    memcpy(pBuf,buf,size);

    return size;
}

static int erase(long offset, size_t size)
{
    printf("erase 0x%02x 0x%02x: \r\n",offset,size);

    memset(flashBlk + offset,0xff,size);

    return size;
}

void flash_cache_file_reload(void)
{
    FILE *fp;
    fp = fopen("/mnt/hgfs/VM Share/flash.bin", "rb");
    if (NULL != fp)
    {
        fread(flashBlk,1,FLASH_MAX_SIZE,fp);
        fclose(fp);
    }
    else
    {
        log_e("ERROR:File open fail\n");
        return;
    }
    printf("file reload\r\n");
}

void flash_cache_file_store(void)
{
    FILE *fp;
    fp = fopen("/mnt/hgfs/VM Share/flash.bin", "wb");
    if (NULL != fp)
    {
        fwrite(flashBlk,1,FLASH_MAX_SIZE,fp);
        fclose(fp);
    }
    else
    {
        log_e("ERROR:File open fail\n");
        return;
    }
    printf("file store\r\n");
}