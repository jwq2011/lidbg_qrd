#ifndef _LIGDBG_LOADER__
#define _LIGDBG_LOADER__

extern int load_modules_count;
void  lidbg_insmod(char path[]);

#define  RECOVERY_MODE_DIR "/sbin/recovery"

#define  FLY_MODE_FILE "/flysystem/lib/out/lidbg_loader.ko"
#define LIDBG_MODULE_LOG do{DUMP_BUILD_TIME;load_modules_count++;}while(0)

#endif

