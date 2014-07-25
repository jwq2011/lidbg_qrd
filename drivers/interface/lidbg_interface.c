
#include "lidbg.h"
#include "lidbg_target.c"

#define HAL_SO "/flysystem/lib/out/lidbg_loader.ko"
LIDBG_DEFINE;

void interface_func_tbl_default(void)
{
    lidbgerr("interface_func_tbl_default:this func not ready!\n");
    //print who call this
    dump_stack();
}

bool iSOC_IO_ISR_Add(u32 irq, u32  interrupt_type, pinterrupt_isr func, void *dev)
{
    bool ret = 0;
    struct io_int_config io_int_config1;

    io_int_config1.ext_int_num = GPIO_TO_INT(irq);
    io_int_config1.irqflags = interrupt_type;
    io_int_config1.pisr = func;
    io_int_config1.dev = dev;

    lidbg("ext_int_num:%d \n", irq);

    ret =  soc_io_irq(&io_int_config1);
    return ret;
}

bool iSOC_IO_ISR_Enable(u32 irq)
{
    soc_irq_enable(GPIO_TO_INT(irq));
    return 1;
}

bool iSOC_IO_ISR_Disable(u32 irq)
{
    soc_irq_disable(GPIO_TO_INT(irq));
    return 1;
}
bool iSOC_IO_ISR_Del (u32 irq)
{
    free_irq(GPIO_TO_INT(irq), NULL);
    return 1;
}

bool iSOC_IO_Config(u32 index, bool direction, u32 pull, u32 drive_strength)
{
    return soc_io_config( index,  direction, pull, drive_strength, 1);
}

void iSOC_IO_Output_Ext(u32 group, u32 index, bool status, u32 pull, u32 drive_strength)
{
    soc_io_config( index,  GPIO_CFG_OUTPUT, pull, drive_strength, 1);
    soc_io_output(group, index,  status);
}

void iSOC_IO_Output(u32 group, u32 index, bool status)
{
    soc_io_config( index,  GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA, 0);
    soc_io_output(group, index,  status);
}

bool iSOC_IO_Input(u32 group, u32 index, u32 pull)
{
    soc_io_config( index,  GPIO_CFG_INPUT, pull/*GPIO_CFG_NO_PULL*/, GPIO_CFG_16MA, 0);
    return soc_io_input(index);
}

//return mv
bool iSOC_ADC_Get (u32 channel , u32 *value)
{
    *value = 0xffffffff;
    *value = soc_ad_read(channel)/1000;
    if(*value == 0xffffffff)
        return 0;
    return 1;
}

void iSOC_Key_Report(u32 key_value, u32 type)
{
    lidbg_key_report(key_value, type);
}

// 7bit i2c sub_addr
int iSOC_I2C_Send(int bus_id, char chip_addr, char *buf, unsigned int size)
{
    return  i2c_api_do_send( bus_id,  chip_addr,  0,  buf,  size);
}
int iSOC_I2C_Rec(int bus_id, char chip_addr, unsigned int sub_addr, char *buf, unsigned int size)
{
    return  i2c_api_do_recv( bus_id,  chip_addr,  sub_addr, buf,  size);
}

int iSOC_I2C_Rec_2B_SubAddr(int bus_id, char chip_addr, unsigned int sub_addr, char *buf, unsigned int size)
{
    return  i2c_api_do_recv_sub_addr_2bytes( bus_id,  chip_addr,  sub_addr, buf,  size);
}

int iSOC_I2C_Rec_3B_SubAddr(int bus_id, char chip_addr, unsigned int sub_addr, char *buf, unsigned int size)
{
    return  i2c_api_do_recv_sub_addr_3bytes( bus_id,  chip_addr,  sub_addr, buf,  size);
}

int iSOC_I2C_Rec_Simple(int bus_id, char chip_addr, char *buf, unsigned int size)
{
    return  i2c_api_do_recv_no_sub_addr( bus_id,  chip_addr,  0, buf,  size);
}

int i2c_api_do_recv_SAF7741(int bus_id, char chip_addr, unsigned int sub_addr, char *buf, unsigned int size);
int i2c_api_do_send_TEF7000(int bus_id, char chip_addr, unsigned int sub_addr, char *buf, unsigned int size);
int i2c_api_do_recv_TEF7000(int bus_id, char chip_addr, unsigned int sub_addr, char *buf, unsigned int size);

int iSOC_I2C_Rec_SAF7741(int bus_id, char chip_addr, unsigned int sub_addr, char *buf, unsigned int size)
{
    return  i2c_api_do_recv_SAF7741( bus_id,  chip_addr, sub_addr, buf,  size);
}

int iSOC_I2C_Send_TEF7000(int bus_id, char chip_addr, unsigned int sub_addr, char *buf, unsigned int size)
{
    return  i2c_api_do_send_TEF7000( bus_id,  chip_addr, sub_addr, buf,  size);
}

int iSOC_I2C_Rec_TEF7000(int bus_id, char chip_addr, unsigned int sub_addr, char *buf, unsigned int size)
{
    return  i2c_api_do_recv_TEF7000( bus_id,  chip_addr, sub_addr, buf,  size);
}
/////////spi
/*
int iSOC_SPI_Mode_Set(int bus_id, u8 mode,u8 bits_per_word,u32 max_speed_hz)
{
   return spi_api_do_set( bus_id,  mode,bits_per_word, max_speed_hz);
}
int iSOC_SPI_Rec(int bus_id,char *buf, unsigned int size)
{
    return   spi_api_do_read( bus_id, buf,  size);
}

int iSOC_SPI_Send(int bus_id,char *buf, unsigned int size)
{
    return   spi_api_do_write( bus_id, buf,  size);
}

int iSOC_SPI_Do_Write_Then_Read(int bus_id, const u8 *txbuf, unsigned n_tx, u8 *rxbuf, unsigned n_rx)
{
   return spi_api_do_write_then_read(bus_id, txbuf,  n_tx, rxbuf, n_rx);
}
*/
void iSOC_PWM_Set(int pwm_id, int duty_ns, int period_ns)
{
    soc_pwm_set(pwm_id, duty_ns, period_ns);
}

//0~255
int iSOC_BL_Set( u32 bl_level)
{
    soc_bl_set(bl_level);
    return 1;

}

int iSOC_Display_Get_Res(u32 *screen_x, u32 *screen_y)
{
    return soc_get_screen_res(screen_x, screen_y);
}

struct fly_smem *iSOC_Get_Share_Mem(void)
{
    return p_fly_smem;
}

void iSOC_System_Status(FLY_SYSTEM_STATUS status)
{
    lidbg("SOC_System_Status=%d\n", status);
    g_var.system_status = status;
    //lidbg_notifier_call_chain(NOTIFIER_VALUE(NOTIFIER_MAJOR_SYSTEM_STATUS_CHANGE, g_var.system_status));
}

int iSOC_Get_CpuFreq(void)
{
    char buf[16];
    int cpu_freq;
    lidbg_readwrite_file("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq", buf, NULL, 16);
    cpu_freq = simple_strtoul(buf, 0, 0);
    g_var.cpu_freq = cpu_freq;

    return cpu_freq;
}


static void set_func_tbl(void)
{
    //io
    plidbg_dev->soc_func_tbl.pfnSOC_IO_Output = iSOC_IO_Output;
    plidbg_dev->soc_func_tbl.pfnSOC_IO_Input = iSOC_IO_Input;
    plidbg_dev->soc_func_tbl.pfnSOC_IO_Output_Ext = iSOC_IO_Output_Ext;
    plidbg_dev->soc_func_tbl.pfnSOC_IO_Config = iSOC_IO_Config;

    //i2c
    plidbg_dev->soc_func_tbl.pfnSOC_I2C_Send = iSOC_I2C_Send;
    plidbg_dev->soc_func_tbl.pfnSOC_I2C_Rec = iSOC_I2C_Rec;
    plidbg_dev->soc_func_tbl.pfnSOC_I2C_Rec_Simple = iSOC_I2C_Rec_Simple;
    plidbg_dev->soc_func_tbl.pfnSOC_I2C_Rec_2B_SubAddr = iSOC_I2C_Rec_2B_SubAddr;

    plidbg_dev->soc_func_tbl.pfnSOC_I2C_Rec_SAF7741 = iSOC_I2C_Rec_SAF7741;
    plidbg_dev->soc_func_tbl.pfnSOC_I2C_Send_TEF7000 = iSOC_I2C_Send_TEF7000;
    plidbg_dev->soc_func_tbl.pfnSOC_I2C_Rec_TEF7000 = iSOC_I2C_Rec_TEF7000;

    //spi
/*
    plidbg_dev->soc_func_tbl.pfnSOC_SPI_Mode_Set=iSOC_SPI_Mode_Set;
    plidbg_dev->soc_func_tbl.pfnSOC_SPI_Send =iSOC_SPI_Send;
    plidbg_dev->soc_func_tbl.pfnSOC_SPI_Rec =iSOC_SPI_Rec;
    plidbg_dev->soc_func_tbl.pfnSOC_SPI_Send_Rec = iSOC_SPI_Do_Write_Then_Read;
*/
    //io-irq
    plidbg_dev->soc_func_tbl.pfnSOC_IO_ISR_Add = iSOC_IO_ISR_Add;
    plidbg_dev->soc_func_tbl.pfnSOC_IO_ISR_Enable = iSOC_IO_ISR_Enable;
    plidbg_dev->soc_func_tbl.pfnSOC_IO_ISR_Disable = iSOC_IO_ISR_Disable;
    plidbg_dev->soc_func_tbl.pfnSOC_IO_ISR_Del = iSOC_IO_ISR_Del;

    //ad
    plidbg_dev->soc_func_tbl.pfnSOC_ADC_Get = iSOC_ADC_Get;

    //key
    plidbg_dev->soc_func_tbl.pfnSOC_Key_Report = iSOC_Key_Report;

    //bl
    plidbg_dev->soc_func_tbl.pfnSOC_BL_Set = iSOC_BL_Set;

    //display/touch
    plidbg_dev->soc_func_tbl.pfnSOC_Display_Get_Res = iSOC_Display_Get_Res;

 //   plidbg_dev->soc_func_tbl.pfnSOC_Get_Share_Mem = iSOC_Get_Share_Mem;
    plidbg_dev->soc_func_tbl.pfnSOC_System_Status = iSOC_System_Status;
    plidbg_dev->soc_func_tbl.pfnSOC_WakeLock_Stat  = lidbg_wakelock_register;
    plidbg_dev->soc_func_tbl.pfnSOC_Get_CpuFreq  = iSOC_Get_CpuFreq;

	

}

int interface_open(struct inode *inode, struct file *filp)
{
    return 0;
}
int interface_release(struct inode *inode, struct file *filp)
{
    return 0;
}
ssize_t interface_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
    unsigned int count = 4;
    int ret = 0;
    u32 read_value = 0;
    read_value = (u32)plidbg_dev;

    lidbg("interface_read:read_value=%x,read_count=%d\n", (u32)read_value, count);
    if (copy_to_user(buf, &read_value, count))
    {
        ret =  - EFAULT;
    }
    else
    {
        ret = count;
    }

    return count;
}
static ssize_t interface_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
    return size;
}

#define DEVICE_NAME "lidbg_interface"
static struct file_operations dev_fops =
{
    .owner	=	THIS_MODULE,
    .open   =   interface_open,
    .read   =   interface_read,
    .write   =   interface_write,
    .release =  interface_release,
};


static struct miscdevice misc =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &dev_fops,

};

static int thread_dump_trace_msg(void *data)
{
	msleep(1000*200);
	kmsg_fifo_save();
	return 0;
}


int fly_interface_init(void)
{
    int ret;
	int p ; 
    DUMP_BUILD_TIME;
    ret = misc_register(&misc);

    plidbg_dev = kmalloc(sizeof(struct lidbg_interface), GFP_KERNEL);
		
    if (plidbg_dev == NULL)
    {

        LIDBG_ERR("kmalloc.\n");
        return 0;
    }

	p = (int) & (plidbg_dev->soc_func_tbl);
    {
        int i;
        for(i = 0; i < sizeof(plidbg_dev->soc_func_tbl) / 4; i++)
        {
           // (((int *) & (plidbg_dev->soc_func_tbl))[i]) = interface_func_tbl_default;
           *((int*)(p+i*4)) = (int)interface_func_tbl_default;
        }
    }
    memset(&(plidbg_dev->soc_pvar_tbl), (int)NULL, sizeof(struct lidbg_pvar_t));

    set_func_tbl();

    g_var.temp = 0;
    g_var.system_status = FLY_ACC_ON;
    g_var.machine_id = get_machine_id();


    g_var.is_fly = 0;
    g_var.fake_suspend = 0;
    g_var.acc_flag = 1;
    g_var.ws_lh = NULL;
	g_var.fb_on = true;

    if( fs_is_file_exist(RECOVERY_MODE_DIR))
    {
	    g_var.recovery_mode = 1;
		lidbg("system mode is recovery_mode\n");
	}
    else
	{
	    g_var.recovery_mode = 0;
		lidbg("system mode is normal_mode\n");
	}

    if(fs_is_file_exist(HAL_SO))
    {
        lidbg("is product\n");
        g_var.is_fly = true;
    }
    else
	{
	    g_var.is_fly = false;
		lidbg("is origin\n");
	}

    memset(&g_var.hw_info, 0, sizeof(struct hw_info));
	
    FS_REGISTER_INT(g_var.hw_info.hw_version, "hw_version", 0, NULL);
    FS_REGISTER_INT(g_var.hw_info.ts_type, "ts_type", 0, NULL);
    FS_REGISTER_INT(g_var.hw_info.ts_config, "ts_config", 0, NULL);
    FS_REGISTER_INT(g_var.hw_info.lcd_type, "lcd_type", 0, NULL);
	
	if(g_var.hw_info.hw_version == 0)
	{
#ifdef BOARD_V1
	g_var.hw_info.hw_version = 1;
#endif
#ifdef BOARD_V2
	g_var.hw_info.hw_version = 2;
#endif
#ifdef BOARD_V3
	g_var.hw_info.hw_version = 3;
#endif
#ifdef BOARD_V4
	g_var.hw_info.hw_version = 4;
#endif
#ifdef BOARD_V5
	g_var.hw_info.hw_version = 5;
#endif

	}
	lidbg("hw_version=%d\n",g_var.hw_info.hw_version);


    //if((g_var.recovery_mode == 0) && ( fs_is_file_exist(LIDBG_PATH)))
    {
		g_var.is_first_update = is_out_updated;
		lidbg("g_var.is_first_update=%d\n",g_var.is_first_update);
    }

	
#if 1 //for test
	if(g_var.is_first_update == 1)
	{
		CREATE_KTHREAD(thread_dump_trace_msg, NULL);
	}
#endif
    return 0;
}

void fly_interface_deinit(void){}

module_init(fly_interface_init);
module_exit(fly_interface_deinit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Flyaudad Inc.");


