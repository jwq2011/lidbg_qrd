
#include "lidbg.h"

LIDBG_DEFINE;
int find_ad_key(struct ad_key_remap *p)
{
    int val = 0;
    int i;

    if(SOC_ADC_Get(p->ch, &val)==0)
		return 0xffffffff;
    if(val > p->max)
        return 0xffffffff;

    for(i = 0; i < SIZE_OF_ARRAY(p->key_item); i++)
    {
            if((val > p->key_item[i].ad_value - p->offset) && (val < p->key_item[i].ad_value + p->offset))
            {
                lidbg("find_ad_key:ch%d=%d,key_id=%d,sendkey=%d\n", p->ch, val, i, p->key_item[i].send_key);
                return p->key_item[i].send_key;
            }
    }

    return 0xffffffff;

}

void key_scan(void)
{
    static int old_key = 0xffffffff;
    int key = 0xffffffff;
	int i;

	for(i=0;i<SIZE_OF_ARRAY(g_hw.ad_key);i++)
	{
	    key = find_ad_key(&(g_hw.ad_key[i]));
		
	    if(key != 0xffffffff) goto find_key;
	}

find_key:

    if((old_key == 0xffffffff) && (key != 0xffffffff))
    {
	    if(g_var.recovery_mode == 1)
			 SOC_Key_Report(KEY_POWER, KEY_PRESSED_RELEASED);
		 else
	        SOC_Key_Report(key, KEY_PRESSED_RELEASED);
    }
    old_key = key;
}
int thread_key(void *data)
{  
	int ad_en;
	DUMP_FUN;
    FS_REGISTER_INT(ad_en, "ad_en", 1, NULL);
    if(ad_en)
    {
	    while(1)
	    {
	        if(g_var.fb_on)
	        {
	            key_scan();
	            msleep(100);
	        }
	        else
	            msleep(1000);
	    }
    }
    return 0;
}



static int  ad_init(void)
{
	lidbg("ad_driver");
        LIDBG_GET;
     	if((g_var.is_fly == 0) || (g_var.recovery_mode == 1))
	{
        CREATE_KTHREAD(thread_key, NULL);
	}

	
	
	return 0;


}

static void  ad_exit(void)
{
	printk("chdrv_exit\n");


	

}

module_init(ad_init);
module_exit(ad_exit);



MODULE_AUTHOR("fly, <fly@gmail.com>");
MODULE_DESCRIPTION("Devices Driver");
MODULE_LICENSE("GPL");




