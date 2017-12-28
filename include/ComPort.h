/**
 * @file        ComPort.h
 * @brief       シリアルポート通信ライブラリ
 * @author      Keitetsu
 * @date        2016/04/11
 * @copyright   Copyright (c) 2016 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#pragma once    /* __COMPORT_H__ */

#include <termios.h>


/**
 * @name    TRUE/FALSE
 */
/*! @{ */
#ifndef TRUE
#define TRUE        1               /*!< TRUE */
#endif

#ifndef FALSE
#define FALSE       0               /*!< FALSE */
#endif
/*! @} */


/**
 * @struct  ComPort_st
 * @brief   シリアルポート情報構造体
 *
 * @typedef COMPORT_T
 * @brief   シリアルポート情報構造体
 *
 * @typedef COMPORT
 * @brief   シリアルポート情報構造体のポインタ
 */
typedef struct ComPort_st {
    int fd;                         /*!< ファイルディスクリプタ */
    struct termios comTioBackup;    /*!< 端末制御設定のバックアップ */
    tcflag_t comBaudRate;           /*!< ボーレート */
    tcflag_t comByteSize;           /*!< データビット */
    tcflag_t comParity;             /*!< パリティビット */
    tcflag_t comStopBits;           /*!< ストップビット */
    unsigned int comVMin;           /*!< 最小受信データバイト数 */
    unsigned int comVTime;          /*!< タイムアウト時間 */
} COMPORT_T, *COMPORT;


/**
 * @brief   シリアルポートを開く
 * @param[in]       comName         シリアルポートのデバイスファイル名
 * @param[in]       comBaudRate     ボーレート
 * retval           NULL            異常終了
 * retval           Others          シリアルポート情報構造体のポインタ
 */
COMPORT ComPort_openComPort(const char *comName, unsigned int comBaudRate);


/**
 * @brief   シリアルポートを閉じる
 * @param[in]       comPort         シリアルポート情報構造体のポインタ
 * retval           NULL            正常終了
 * retval           Others          異常終了
 */
COMPORT ComPort_closeComPort(COMPORT comPort);


/**
 * @brief   シリアルポートから受信データを読込む
 * @param[in]       comPort         シリアルポート情報構造体のポインタ
 * @param[out]      comRxData       受信データバッファのポインタ
 * @param[in]       comRxBytes      受信データのバイト数
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
int ComPort_readData(COMPORT comPort, void *comRxData, unsigned int comRxBytes);


/**
 * @brief   シリアルポートに送信データを書込む
 * @param[in]       comPort         シリアルポート情報構造体のポインタ
 * @param[in]       comTxData       送信データバッファのポインタ
 * @param[in]       comTxBytes      送信データのバイト数
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
int ComPort_writeData(COMPORT comPort, void *comTxData, unsigned int comTxBytes);


/**
 * @brief   シリアルポートの制御設定を行う
 * @param[in]       comPort         シリアルポート情報構造体のポインタ
 */
void ComPort_setTermios(COMPORT comPort);

