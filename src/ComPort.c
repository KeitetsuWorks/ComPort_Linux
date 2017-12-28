/**
 * @file        ComPort.c
 * @brief       シリアルポート通信ライブラリ
 * @author      Keitetsu
 * @date        2016/04/11
 * @copyright   Copyright (c) 2016 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "ComPort.h"


COMPORT ComPort_openComPort(const char *comName, unsigned int comBaudRate)
{
    COMPORT comPort;

    /* インスタンスの生成 */
    comPort = (COMPORT)malloc(sizeof(COMPORT_T));
    if (comPort == NULL) {
        return NULL;
    }
    bzero(comPort, sizeof(COMPORT_T));

    /* デバイスのオープン */
    comPort->fd = open(comName, O_RDWR);
    if (comPort->fd < 0) {
        perror(comName);
        free(comPort);
        return NULL;
    }

    /* 端末制御設定の初期化 */
    comPort->comBaudRate = comBaudRate;
    comPort->comByteSize = CS8;         /* データビット: 8bit */
    comPort->comParity = 0;             /* パリティビット: None */
    comPort->comStopBits = 0;           /* ストップビット: 1bit */
    comPort->comVMin = 0;               /* 最小受信データバイト数: 0Byte */
    comPort->comVTime = 1;              /* タイムアウト時間: 100ms */
    
    /* 端末制御設定のバックアップ */
    tcgetattr(comPort->fd, &(comPort->comTioBackup));
    
    /* 端末制御設定 */
    ComPort_setTermios(comPort);

    /* 端末入力のフラッシュ */
    tcflush(comPort->fd, TCIFLUSH);

    return comPort;
}


COMPORT ComPort_closeComPort(COMPORT comPort)
{
    /* 端末制御設定の復元 */
    tcsetattr(comPort->fd, TCSANOW, &(comPort->comTioBackup));

    /* デバイスのクローズ */
    close(comPort->fd);

    free(comPort);
    comPort = NULL;
    
    return comPort;
}


int ComPort_readData(COMPORT comPort, void *comRxData, unsigned int comRxBytes)
{
    int retval;
    
    retval = read(comPort->fd, comRxData, (size_t)comRxBytes);
    if (retval != (int)comRxBytes) {
        return FALSE;
    }
    
    return TRUE;
}


int ComPort_writeData(COMPORT comPort, void *comTxData, unsigned int comTxBytes)
{
    int retval;

    retval = write(comPort->fd, comTxData, (size_t)comTxBytes);
    if (retval != (int)comTxBytes) {
        return FALSE;
    }

    return TRUE;
}


void ComPort_setTermios(COMPORT comPort)
{
    struct termios comTio;

    bzero(&comTio, sizeof(struct termios));

    comTio.c_iflag = 0;
    comTio.c_oflag = 0;
    comTio.c_cflag = (
        comPort->comBaudRate |
        comPort->comByteSize |
        comPort->comParity |
        comPort->comStopBits |
        CLOCAL |                /* ローカル接続 */
        CREAD
    );

    comTio.c_cc[VMIN] = comPort->comVMin;
    comTio.c_cc[VTIME] = comPort->comVTime;

    /* 端末制御設定の反映 */
    tcsetattr(comPort->fd, TCSANOW, &comTio);

    return;
}

