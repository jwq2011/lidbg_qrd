
#include "lidbg.h"

LIDBG_DEFINE;

typedef enum
{
	DEV_ACCEL,
	DEV_GPS,
	DEV_DISPLAY,
	DEV_LED,
	DEV_RADIO,
	DEV_CARPLAY,
} i2cdev_type;

static int scanflag;

struct probe_device
{
	i2cdev_type type;
	int (*i2c_bus)(void);
    char chip_addr;
    unsigned int sub_addr;
    char *name;
    void (*reset)(void);
    void (*find_cb)(void);
    bool origin_system_probe_only;

};

void mc3x_find_cb(void)
{
	msleep(10);
	lidbg_shell_cmd("chmod 777 /sys/class/sensors/mc3xxx-accel/*");
	MSM_ACCEL_POWER_OFF;
	return;
}

int accel_i2c_bus(void)
{
	return ACCEL_I2C_BUS;
}

int saf7741_i2c_bus(void)
{
	return SAF7741_I2C_BUS;
}

int tef6638_i2c_bus(void)
{
	return TEF6638_I2C_BUS;
}

int gps_i2c_bus(void)
{
	return GPS_I2C_BUS;
}

int display_i2c_bus(void)
{
	return DSI83_I2C_BUS;
}

int pca9634_i2c_bus(void)
{
	return PCA9634_I2C_BUS;
}

int fm1388_i2c_bus(void)
{
	return FM1388_I2C_BUS;
}
void radio_reset_lpc(void)
{
	LPC_CMD_RADIORST_L;
	msleep(1000);
	LPC_CMD_RADIORST_H;
	msleep(1000);
}

void accel_power_enable(void)
{
	MSM_ACCEL_POWER_ON;
	msleep(10);
}

void gps_power_enable(void)
{
	GPS_POWER_ON;
	msleep(100);
}

struct probe_device i2c_probe_dev[] =
{
  	{DEV_ACCEL, accel_i2c_bus, 0x18, 0x00, "bma2x2.ko", NULL, NULL},
  	{DEV_ACCEL, accel_i2c_bus, 0x4c, 0x00, "mc3xxx.ko", accel_power_enable, mc3x_find_cb},

	{DEV_GPS, gps_i2c_bus, 0x42, 0x00, "lidbg_gps.ko", gps_power_enable, NULL ,0},
	{DEV_DISPLAY, display_i2c_bus, 0x2d, 0x00, "dsi83.ko", NULL, NULL ,0},
	{DEV_LED, pca9634_i2c_bus, 0x70, 0x00, "lidbg_rgb_led.ko", NULL, NULL ,0},
#ifndef MUC_CONTROL_DSP
	{DEV_RADIO, saf7741_i2c_bus, 0x1c, 0x00, "saf7741.ko", radio_reset_lpc, NULL ,1},
	{DEV_RADIO, tef6638_i2c_bus, 0x63, 0x00, "tef6638.ko", radio_reset_lpc, NULL ,1},
#endif
	{DEV_CARPLAY, fm1388_i2c_bus, 0x2c, 0x00, "lidbg_spi_fm1388.ko", NULL, NULL ,1},
};

void parse_ts_info(struct probe_device *i2cdev_info)
{
    char path[100];

    if(gboot_mode == MD_FLYSYSTEM)
    {
        sprintf(path, "/flysystem/lib/out/%s", i2cdev_info->name);
        lidbg_insmod( path );

    }
    else if(gboot_mode == MD_DEBUG)
    {
        sprintf(path, "/data/out/%s", i2cdev_info->name);
        lidbg_insmod( path );
    }
    else
    {
        sprintf(path, "/system/lib/modules/out/%s", i2cdev_info->name);
        lidbg_insmod( path );
    }
}

void i2c_devices_scan(struct probe_device *i2cdev, int size)
{
    static unsigned int loop = 0;
    int32_t rc1, rc2;
    int i;
    u8 tmp;

    for(i = 0; i < size; i++)
    {
    		if(i2cdev->origin_system_probe_only )
				if(g_var.is_fly)
					continue;
				
		if(	(i2cdev->i2c_bus() != LPC_I2_ID) && (i2cdev->i2c_bus() >= 0) && (((1<<(int)i2cdev->type) & scanflag) == 0))
		{
			if(i2cdev->reset != NULL)
				i2cdev->reset();
			rc1 = SOC_I2C_Rec_Simple(i2cdev->i2c_bus(), i2cdev->chip_addr, &tmp, 1 );
			rc2 = SOC_I2C_Rec(i2cdev->i2c_bus(), i2cdev->chip_addr, i2cdev->sub_addr, &tmp, 1 );
			//SOC_I2C_Rec(i2cdev->i2c_bus, 0x12, 0x00, &tmp, 1 ); //let i2c bus release		
			if ((rc1 < 0) && (rc2 < 0))
			{
				loop++;
				lidbg("fail.%d:[0x%x] rc1=%x,rc2=%x,i2c_bus=%d\n", loop, i2cdev->chip_addr, rc1, rc2, i2cdev->i2c_bus());
			}
			else
			{
				lidbg("found:[0x%x,%s],i2c_bus=%d\n", i2cdev->chip_addr, i2cdev->name, i2cdev->i2c_bus());	
				parse_ts_info(i2cdev);
				if(i2cdev->find_cb != NULL)
					i2cdev->find_cb();
				
				scanflag |= (1 << (int)i2cdev->type);
				if((i+1) < size)
				{
					while((i2cdev+1)->type == i2cdev->type)
					{
						i++;
						i2cdev++;
					}
				}
			}
		}
		i2cdev++;	
    }
    return ;
}

int i2c_devices_probe_thread(void *data)
{
    int cnt = 3;
#if 0//def SUSPEND_ONLINE
    g_var.acc_flag = SOC_IO_Input(MCU_ACC_STATE_IO, MCU_ACC_STATE_IO, GPIO_CFG_PULL_UP);
    LIDBG_WARN("<g_var.acc_flag.1:%d>\n",g_var.acc_flag);
    while(g_var.recovery_mode == 0&&g_var.acc_flag==FLY_ACC_OFF)
    {
        msleep(200);
    }
    LIDBG_WARN("<g_var.acc_flag.2:%d>\n",g_var.acc_flag);
#endif
    while(cnt--)
    {
		i2c_devices_scan(i2c_probe_dev, SIZE_OF_ARRAY(i2c_probe_dev));
        	msleep(500);
    }
    return 0;
}

static int i2c_devices_probe_init(void)
{
    DUMP_BUILD_TIME;
    LIDBG_GET;
    CREATE_KTHREAD(i2c_devices_probe_thread, NULL);
    return 0;
}

static void i2c_devices_probe_exit(void)
{
}

module_init(i2c_devices_probe_init);
module_exit(i2c_devices_probe_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lwy.");

