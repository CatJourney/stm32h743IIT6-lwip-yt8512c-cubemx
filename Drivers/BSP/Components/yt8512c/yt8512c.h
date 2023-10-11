/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef YT8512C_H
#define YT8512C_H

#ifdef __cplusplus
 extern "C" {
#endif   
   
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "global.h"

/* PHY芯片寄存器映射表 */ 
#define YT8512C_BCR                            ((uint16_t)0x0000U)
#define YT8512C_BSR                            ((uint16_t)0x0001U)
#define PHY_REGISTER2                           ((uint16_t)0x0002U)
#define PHY_REGISTER3                           ((uint16_t)0x0003U)

/* 操作SCR寄存器的值（一般不需要修改） */
#define YT8512C_BCR_SOFT_RESET                 ((uint16_t)0x8000U)
#define YT8512C_BCR_LOOPBACK                   ((uint16_t)0x4000U)
#define YT8512C_BCR_SPEED_SELECT               ((uint16_t)0x2000U)
#define YT8512C_BCR_AUTONEGO_EN                ((uint16_t)0x1000U)
#define YT8512C_BCR_POWER_DOWN                 ((uint16_t)0x0800U)
#define YT8512C_BCR_ISOLATE                    ((uint16_t)0x0400U)
#define YT8512C_BCR_RESTART_AUTONEGO           ((uint16_t)0x0200U)
#define YT8512C_BCR_DUPLEX_MODE                ((uint16_t)0x0100U) 

/* 操作BSR寄存器的值（一般不需要修改） */   
#define YT8512C_BSR_100BASE_T4                 ((uint16_t)0x8000U)
#define YT8512C_BSR_100BASE_TX_FD              ((uint16_t)0x4000U)
#define YT8512C_BSR_100BASE_TX_HD              ((uint16_t)0x2000U)
#define YT8512C_BSR_10BASE_T_FD                ((uint16_t)0x1000U)
#define YT8512C_BSR_10BASE_T_HD                ((uint16_t)0x0800U)
#define YT8512C_BSR_100BASE_T2_FD              ((uint16_t)0x0400U)
#define YT8512C_BSR_100BASE_T2_HD              ((uint16_t)0x0200U)
#define YT8512C_BSR_EXTENDED_STATUS            ((uint16_t)0x0100U)
#define YT8512C_BSR_AUTONEGO_CPLT              ((uint16_t)0x0020U)
#define YT8512C_BSR_REMOTE_FAULT               ((uint16_t)0x0010U)
#define YT8512C_BSR_AUTONEGO_ABILITY           ((uint16_t)0x0008U)
#define YT8512C_BSR_LINK_STATUS                ((uint16_t)0x0004U)
#define YT8512C_BSR_JABBER_DETECT              ((uint16_t)0x0002U)
#define YT8512C_BSR_EXTENDED_CAP               ((uint16_t)0x0001U)

/* PHY芯片进程状态 */
#define  YT8512C_STATUS_READ_ERROR             ((int32_t)-5)
#define  YT8512C_STATUS_WRITE_ERROR            ((int32_t)-4)
#define  YT8512C_STATUS_ADDRESS_ERROR          ((int32_t)-3)
#define  YT8512C_STATUS_RESET_TIMEOUT          ((int32_t)-2)
#define  YT8512C_STATUS_ERROR                  ((int32_t)-1)
#define  YT8512C_STATUS_OK                     ((int32_t) 0)
#define  YT8512C_STATUS_LINK_DOWN              ((int32_t) 1)
#define  YT8512C_STATUS_100MBITS_FULLDUPLEX    ((int32_t) 2)
#define  YT8512C_STATUS_100MBITS_HALFDUPLEX    ((int32_t) 3)
#define  YT8512C_STATUS_10MBITS_FULLDUPLEX     ((int32_t) 4)
#define  YT8512C_STATUS_10MBITS_HALFDUPLEX     ((int32_t) 5)
#define  YT8512C_STATUS_AUTONEGO_NOTDONE       ((int32_t) 6)

/* PHY地址 ---- 由用户设置 */
#define YT8512C_ADDR                           ((uint16_t)0x0000U)
/* PHY寄存器的数量 */
#define YT8512C_PHY_COUNT                      ((uint16_t)0x001FU)

#define YT8512C_PHYSCSR                        ((uint16_t)0x11)                       /*!< tranceiver status register */
#define YT8512C_SPEED_STATUS                   ((uint16_t)0x4010)                     /*!< configured information of speed: 100Mbit/s */
#define YT8512C_DUPLEX_STATUS                  ((uint16_t)0x2000)                     /*!< configured information of duplex: full-duplex */

/* 定义函数指针 */ 
typedef int32_t  (*yt8512c_init_func)          (void); 
typedef int32_t  (*yt8512c_deinit_func)        (void);
typedef int32_t  (*yt8512c_readreg_func)       (uint32_t, uint32_t, uint32_t *);
typedef int32_t  (*yt8512c_writereg_func)      (uint32_t, uint32_t, uint32_t);
typedef int32_t  (*yt8512c_gettick_func)       (void);

/* PHY共用函数结构体 */ 
typedef struct 
{
    yt8512c_init_func          init;                   /* 指向PHY初始化函数 */ 
    yt8512c_deinit_func        deinit;                 /* 指向PHY反初始化函数 */ 
    yt8512c_writereg_func      writereg;               /* 指向PHY写寄存器函数 */ 
    yt8512c_readreg_func       readreg;                /* 指向PHY读寄存器函数 */ 
    yt8512c_gettick_func       gettick;                /* 指向节拍函数 */ 
} yt8512c_ioc_tx_t;  

/* 注册到组件对象结构体 */
typedef struct 
{
    uint32_t            devaddr;                        /* PHY地址 */
    uint32_t            is_initialized;                 /* 描述该设备是否初始化 */
    yt8512c_ioc_tx_t   io;                             /* 设备调用的函数入口 */
    void                *pdata;                         /* 传入的形参 */
}yt8512c_object_t;


int32_t yt8512c_regster_bus_io(yt8512c_object_t *pobj, yt8512c_ioc_tx_t *ioctx);             /* 将IO函数注册到组件对象 */
int32_t yt8512c_init(yt8512c_object_t *pobj);                                                 /* 初始化YT8512C并配置所需的硬件资源 */
int32_t yt8512c_deinit(yt8512c_object_t *pobj);                                               /* 反初始化YT8512C及其硬件资源 */
int32_t yt8512c_disable_power_down_mode(yt8512c_object_t *pobj);                              /* 关闭YT8512C的下电模式 */
int32_t yt8512c_enable_power_down_mode(yt8512c_object_t *pobj);                               /* 使能YT8512C的下电模式 */
int32_t yt8512c_start_auto_nego(yt8512c_object_t *pobj);                                      /* 启动自动协商过程 */
int32_t yt8512c_get_link_state(yt8512c_object_t *pobj);                                       /* 获取YT8512C设备的链路状态 */
int32_t yt8512c_set_link_state(yt8512c_object_t *pobj, uint32_t linkstate);                   /* 设置YT8512C设备的链路状态 */
int32_t yt8512c_enable_loop_back_mode(yt8512c_object_t *pobj);                                /* 启用环回模式 */
int32_t yt8512c_disable_loop_back_mode(yt8512c_object_t *pobj);                               /* 禁用环回模式 */



#ifdef __cplusplus
}
#endif
#endif /* YT8512C_H */
