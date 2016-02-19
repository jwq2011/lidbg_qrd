
#include "lidbg.h"
extern char **lmk_white_list;

char *lmk_protect_list[] =
{
	"d.process.acore",
	"ndroid.launcher",
	"d.process.media",
	"roid.flyaudioui",
	"flyaudioservice",
	".flybootservice",
	"dio.flyaudioram",
	"mediatorservice",
	"droid.launcher3",
	".flyaudio.media",
	"ndroid.systemui",
	"lyaudio.Weather",
	"oadcastreceiver",
	"c2739.mainframe",
	"alcomm.fastboot",
	"udio.navigation",
	"dio.osd.service",
	"droid.deskclock",
	"ys.DeviceHealth",
	"io.proxyservice",
	"mobile.rateflow",
	"goodocom.gocsdk",
	"tonavi.amapauto",
	"com.sygic.aura",
	"mediaserver",
	"system_server",
	"system",
	"logcat",
	NULL,
};

static int lowmemorykillprotectlist_init(void)
{
    DUMP_BUILD_TIME;
    lmk_white_list = lmk_protect_list;
    return 0;
}


module_init(lowmemorykillprotectlist_init);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("flyaudio");
