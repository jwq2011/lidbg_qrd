#include "../soc.h"
#include "fly_target.h"

extern void dsi83_init();

void flyaudio_hw_init(void)
{
    dprintf(INFO, "Flyaudio hardware init. \n");

#ifdef BOOTLOADER_MSM8909
	dprintf(INFO, "LK wakeup LPC, io(%d)\n", g_bootloader_hw.lk_wakeup_lpc_io);
	gpio_set_direction(g_bootloader_hw.lk_wakeup_lpc_io, GPIO_OUTPUT);
	gpio_set_val(g_bootloader_hw.lk_wakeup_lpc_io, 0);
	mdelay(10);
	gpio_set_val(g_bootloader_hw.lk_wakeup_lpc_io, 1);
	mdelay(10);
	gpio_set_val(g_bootloader_hw.lk_wakeup_lpc_io, 0);
	mdelay(10);
	gpio_set_val(g_bootloader_hw.lk_wakeup_lpc_io, 1);
	mdelay(10);
	gpio_set_val(g_bootloader_hw.lk_wakeup_lpc_io, 0);
	mdelay(500);
#endif

    dsi83_init();
}
