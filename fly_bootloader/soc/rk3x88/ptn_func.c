#include "../soc.h"

unsigned page_mask = 0;

int ptn_read(char *ptn_name,unsigned  long len,unsigned char *buf)
{
    fbt_partition_t* ptn;

	ptn = fastboot_find_ptn(ptn_name);
	printf("Read partition %s, ptn = %d \n", ptn_name, ptn);

	if(ptn){
		if(CopyFlash2Memory(buf, ptn, DIV_ROUND_UP(len, RK_BLK_SIZE))<0)
		{
			printf("read %s filad\n", ptn_name);
			return -1;
		}
	}else{
		printf("Find %s ptn filad\n", ptn_name);
		return -1;
	}

	return  0;
}

