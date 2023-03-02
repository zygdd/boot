/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-06     RT-Thread    first version
 */

#include <rtthread.h>
#include <fal.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

int main(void)
{
    fal_init();
    int count = 1;
    LOG_D("Basic firmware added successfully");

    while (count++)
    {
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}


void factory_commander()
{
    rt_uint8_t buf[1024];
    int i;
    for(i=0;i<sizeof(buf);i++)
    {
        buf[i]=1;
    }

    const struct fal_partition * part = NULL;//收纳分区名


    if(fal_partition_find("factory_cmd") != NULL)
    {
     part = fal_partition_find("factory_cmd");
    }

    fal_partition_erase(part, 10, sizeof(buf));
    //清除相关flash内存中的数据，为重新写入做准备，又因为对函数不熟悉，所以选择擦除的大一点

    fal_partition_write(part, 10, buf, sizeof(buf));
    //改变相关flash内存中的命令标志位，表示需要回复出厂设置

    /* wait some time for terminal response finish */
            rt_thread_delay(rt_tick_from_millisecond(200));

            /* Reset the device, Start new firmware */
            extern void rt_hw_cpu_reset(void);
            rt_hw_cpu_reset();

            /* wait some time for terminal response finish */
            rt_thread_delay(rt_tick_from_millisecond(200));
    //重启CPU
}
MSH_CMD_EXPORT(factory_commander, OTA_factory start);


