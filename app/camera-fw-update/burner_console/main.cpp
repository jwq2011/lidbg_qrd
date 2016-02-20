#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..//common//usb.h"
#include "..//common//my_type.h"
#include "..//common//debug.h"
#include "..//common//CamEnum.h"
#include "..//BurnMgr//BurnMgr.h"
#include "../../inc/lidbg_servicer.h"

#define	STR_INTRO		"Program : SONiX F/W Update Tool on Linux"
#define	STR_VERSION		"Version : v1.0.5.8" // yiling 2015/08/21 modify
#define	STR_DATE		"Date    : 2015/08/21" // yiling 2015/08/21 modify

#define	FILE_INI		"/storage/udisk/update.src"
#define CNTNUM			10

static const int id_num = 8;	// shawn 2009/11/03 for 232 and 275
ID_TABLE_S	id_table[id_num] = 
{
	{0x0c45, 0x62c0},
	{0x0c45, 0x62d0},
	{0x0c45, 0x62e0},
	{0x0c45, 0x62f0},
	{0x0c45, 0x6300},
	{0x0c45, 0x6310},
	{0x0c45, 0x6320},	// shawn 2009/11/03 add 232
	{0x0c45, 0x6350}	// shawn 2009/11/03 add 275
};

int nFileNum = 0;	// shawn 2010/11/12 add for fixing "No such device" issue
unsigned char camCnt = 0;//current update Camera num
char tmpCmd[100] = {0};


void Print_CamArray(struct usb_device* CamArray[MAX_CAM_NUM], int &nCamNum);
bool Burn_To_Flash(CBurnMgr	&burn_mgr);
bool Burn_To_File(CBurnMgr	&burn_mgr, char szTarget[256]);

void Print_CamArray(struct usb_device* CamArray[MAX_CAM_NUM], int &nCamNum)
{
	struct usb_device *pDev;
	struct usb_device_descriptor *des;
	usb_dev_handle *udev;
	int		i;
	int		vid, pid;
	char	szMfg[64];
	
	LIDBG_PRINT("# of webcam found = %d\n", nCamNum);
	for (i=0; i<nCamNum; i++)
	{
		pDev = CamArray[i];
		udev = usb_open(pDev);
		if (!udev)
			continue;

		des = &(pDev->descriptor);
		vid = des->idVendor;
		pid = des->idProduct;
		usb_get_string_simple(udev, des->iManufacturer, szMfg, 64*sizeof(char));
		LIDBG_PRINT("%3d: vid = 0x%.4x, pid = 0x%.4x, Manufacturer = %s\n", 
				i, vid, pid, szMfg);

		usb_close(udev);
	}
}

bool Burn_To_File(CBurnMgr	&burn_mgr, char szTarget[256])
{
	burn_mgr.Set_Target_File(true);
	burn_mgr.Set_Save_FileName(szTarget);
	burn_mgr.Set_Save_All(true);
	burn_mgr.Set_Save_ROM(true);
	burn_mgr.Set_Save_Des(true);
	burn_mgr.Set_Save_Param(true);
	burn_mgr.Set_Save_ISP(true);
	 
	LIDBG_PRINT("Target : Burn to FILE - %s\n\n", szTarget);
	
	LIDBG_PRINT("Start the burning process ...\n");	

	LIDBG_PRINT("Step : Init -----------------> ");
	if (!burn_mgr.Load_Source_Data())
	{
		DBG_Print("Burn_To_File : Load_Source_Data() - Fail!\n");
		LIDBG_PRINT("Fail!\n");
		return false;
	}
	LIDBG_PRINT("Pass!\n");
	
	LIDBG_PRINT("Step : Save Des -------------> ");
	if (!burn_mgr.Burn_Save_Des())
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}
	LIDBG_PRINT("Pass!\n");
	
	LIDBG_PRINT("step : Save Param -----------> ");
	if (!burn_mgr.Burn_Save_Param())
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}
	LIDBG_PRINT("Pass!\n");
	
	LIDBG_PRINT("Step : Save ISP -------------> ");
	if (!burn_mgr.Burn_Save_ISP())
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}
	LIDBG_PRINT("Pass!\n");
	
	LIDBG_PRINT("Step : Save All -------------> ");
	if (!burn_mgr.Burn_Save_All())
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}
	LIDBG_PRINT("Pass!\n");
	LIDBG_PRINT("Burn to File : Success!\n");
	return true;
	
fail_quit:
	return false;
}

bool Burn_To_Flash(CBurnMgr	&burn_mgr)
{
	char szFlashCodeVer[25] = {0};	// carol 2013/12/16 add
	char szFlashVendorVer[13] = {0};
	
	// shawn 2010/11/12 for fixing "No such device" issue +++++
#if defined _REINIT_	
	CCamEnum	cam_enum;
	struct usb_device* CamArray[MAX_CAM_NUM];
	int 		nCamNum;
#endif
	// shawn 2010/11/12 for fixing "No such device" issue -----
	
	burn_mgr.Set_Target_Flash(true);
	LIDBG_PRINT("Target : Burn to FLASH\n");

	// carol 2013/12/16 add +++++
	if(!burn_mgr.Get_CodeVersion(szFlashCodeVer))
	{
		DBG_Print("Get FW version Fail!\n");
		LIDBG_PRINT("Get FW version Fail!\n");
	}
	szFlashCodeVer[24] = '\0';
	LIDBG_PRINT("Current FW version: %s\n", szFlashCodeVer);
	// carol 2013/12/16 add -----
    
    // wayne 2014/06/13 add +++++
	if(!burn_mgr.Get_VendorVersion(szFlashVendorVer))
	{
		DBG_Print("Get Vendor version Fail!\n");
		LIDBG_PRINT("Get Vendor version Fail!\n");
	}
	szFlashVendorVer[13] = '\0';
	LIDBG_PRINT("Current Vendor version: %s\n", szFlashVendorVer);
    // wayne 2014/06/13 add +++++


	LIDBG_PRINT("Start the burning process ...\n");
	// ----------------- Step 0 : INIT -----------------
	LIDBG_PRINT("Step 0 : INIT ---------------> ");
	// shawn 2009/08/14 fix WP issue +++++
	if (!burn_mgr.Load_Source_Data())
	{
		DBG_Print("Burn_To_Flash : Load_Source_Data() - Fail!\n");
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}
	if (!burn_mgr.Burn_Init())
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}
	// shawn 2009/08/14 fix WP issue -----
	LIDBG_PRINT("Pass!\n");
	
	// ----------------- Step 1 : ERASE -----------------	
	LIDBG_PRINT("Step 1 : ERASE --------------> ");
	if (!burn_mgr.Burn_Erase())
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}
	LIDBG_PRINT("Pass!\n");

	// shawn 2010/11/12 for fixing "No such device" issue +++++
#if defined _REINIT_
	sleep(3);//sleep(1);
	burn_mgr.Cam_DeSelect();
	cam_enum.Set_IDCheckTable(id_table, id_num);
	if (!cam_enum.Enum_Cam(CamArray, nCamNum))
	{
		LIDBG_PRINT("enumerate webcam error!\n");
		goto fail_quit;
	}
	if (nCamNum == 0)
	{
		LIDBG_PRINT("NO webcam is found!\n");
		goto fail_quit;
	}
	
	if (!burn_mgr.Cam_Select(CamArray[0], nFileNum))
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}
	
	if (!burn_mgr.Set_Source_File_From_INI((char *)FILE_INI))
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}

	if (!burn_mgr.Load_Source_Data())
	{
		DBG_Print("Burn_To_Flash : Load_Source_Data() - Fail!\n");
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}

	if (!burn_mgr.Burn_Init())
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}
#endif
	// shawn 2010/11/12 for fixing "No such device" issue -----

	// ----------------- Step 2 : CHECK -----------------
	LIDBG_PRINT("Step 2 : CHECK --------------> ");
	if (!burn_mgr.Burn_Check())
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}
	LIDBG_PRINT("Pass!\n");
	// ----------------- Step 3 : PROGRAM -----------------	
	LIDBG_PRINT("Step 3 : PROGRAM ------------> ");
	if (!burn_mgr.Burn_Program())
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}
	LIDBG_PRINT("Pass!\n");
	// ----------------- Step 4 : VERIFY -----------------	
	LIDBG_PRINT("Step 4 : VERIFY -------------> ");
	if (!burn_mgr.Burn_Verify())
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}	
	LIDBG_PRINT("Pass!\n");

	// carol 2013/12/16 add +++++
	if(!burn_mgr.Get_CodeVersion(szFlashCodeVer))
	{
		DBG_Print("Get FW version Fail!\n");
		LIDBG_PRINT("Get FW version Fail!\n");
	}
	szFlashCodeVer[24] = '\0';
	// carol 2013/12/16 add -----
    // wayne 2014/06/13 add +++++
	if(!burn_mgr.Get_VendorVersion(szFlashVendorVer))
	{
		DBG_Print("Get Vendor version Fail!\n");
		LIDBG_PRINT("Get Vendor version Fail!\n");
	}
	szFlashVendorVer[13] = '\0';
	LIDBG_PRINT("Current Vendor version: %s\n", szFlashVendorVer);
    // wayne 2014/06/13 add +++++

	// ----------------- Step 5 : END PROCESS -----------------
	LIDBG_PRINT("Step 5 : END PROCESS --------> ");
	if (!burn_mgr.Burn_EndProc(true))
	{
		LIDBG_PRINT("Fail!\n");
		goto fail_quit;
	}	
	LIDBG_PRINT("Pass!\n");
	LIDBG_PRINT("Burn to Flash : Success!\n");

	LIDBG_PRINT("New FW version: %s\n", szFlashCodeVer);
	LIDBG_PRINT("New Vendor version: %s\n", szFlashVendorVer);
	sprintf(tmpCmd, "echo flyaudio:touch /dev/log/FWUpdateSuccess%d.txt > /dev/lidbg_misc0",camCnt + 1);
	system(tmpCmd);
	return true;

fail_quit:	
	LIDBG_PRINT("END PROCESS ...");
	burn_mgr.Burn_EndProc(false);
	return false;
}
int main(int argc, char *argv[])
{
	CCamEnum	cam_enum;
	struct usb_device* CamArray[MAX_CAM_NUM];
	int 		nCamNum;
	CBurnMgr	burn_mgr;
	int 		circnt = CNTNUM;

	if (argc > 1)
	{
		if ( strcmp(argv[1], "-1") == 0)
			nFileNum = 1;
		else if ( strcmp(argv[1], "-2") == 0) // carol 2013/08/29 add
			nFileNum = 2;
		else if ( strcmp(argv[1], "-3") == 0)
			nFileNum = 3;
		else if ( strcmp(argv[1], "-h") == 0)
		{
			//LIDBG_PRINT("./fw_update [-1] [-2] [-3] [-h]\n\n"); // carol 2013/10/30 temp mark for not show -3
			LIDBG_PRINT("./fw_update [-1] [-2] [-h]\n\n"); // carol 2013/10/30 add
			LIDBG_PRINT("\n-1     Burn 64k single file.\n");
			LIDBG_PRINT("\n-2     Burn 128k single file.\n"); // carol 2013/08/29 add
			//LIDBG_PRINT("\n-3     Burn three files (ROM, Parameter and Sensor Table).\n"); // carol 2013/10/30 temp mark for never try
			LIDBG_PRINT("\n-h     Help.\n");
			goto exit;
		}
		else
			;
	}
	
	if (nFileNum == 0)
	{
		LIDBG_PRINT("Input argument is wrong!\n");
		goto exit;
	}
	
	DEBUG_INIT();
	system("rm -f /dev/log/FWUpdate*.txt");
	
	LIDBG_PRINT("\n%s\n%s\n%s\n\n", STR_INTRO, STR_VERSION, STR_DATE);
	LIDBG_PRINT("Prepare : enumerate webcam ...\n");
	//cam_enum.Set_IDCheckTable(id_table, id_num); // carol 2013/08/29 mark
	if (!cam_enum.Enum_Cam(CamArray, nCamNum))
	{
		LIDBG_PRINT("enumerate webcam error!\n");
		goto exit;
	}
	if (nCamNum == 0)
	{
		LIDBG_PRINT("NO webcam is found!\n");
		goto exit;
	}
	Print_CamArray(CamArray, nCamNum);
	LIDBG_PRINT("\n");

	LIDBG_PRINT("Prepare : select webcam #0 ... \n");
	
	/*burn FW to all Camera flash*/
	for(camCnt = 0;camCnt < nCamNum;camCnt++)
	{
		LIDBG_PRINT("=========start [%d] Camera update===========\n",camCnt);
		if (!burn_mgr.Cam_Select(CamArray[camCnt], nFileNum))
		{
			LIDBG_PRINT("Cam_Select Fail!\n");
			goto exit;
		}
		LIDBG_PRINT("Cam_Select OK!\n");

		LIDBG_PRINT("Prepare : read .ini file - %s ... ", FILE_INI);
		if (!burn_mgr.Set_Source_File_From_INI((char *)FILE_INI))
		{
			LIDBG_PRINT("Set_Source_File_From_INI Fail!\n");
			goto exit;
		}
		LIDBG_PRINT("Set_Source_File_From_INI OK!\n");
		LIDBG_PRINT("\n");
		
		if (nFileNum == 1)
			LIDBG_PRINT("Source : 64K from FILE                   - %s\n", burn_mgr.Get_RomFile());
		else if(nFileNum == 2) // carol 2013/08/29 add
			LIDBG_PRINT("Source : 128K from FILE                   - %s\n", burn_mgr.Get_RomFile());
		else
		{
			LIDBG_PRINT("Source : ROM from FILE                   - %s\n", burn_mgr.Get_RomFile());
			LIDBG_PRINT("Source : Parameter from FILE             - %s\n", burn_mgr.Get_ParamFile());
			LIDBG_PRINT("Source : Init Sensor Parameter from FILE - %s\n", burn_mgr.Get_ISPFile());
		}

		//Burn_To_File(burn_mgr, (char *)"target");
		
		LIDBG_PRINT("FW Burn to Flash : begin!\n");
		while((circnt--) && (!Burn_To_Flash(burn_mgr)))
		{
			LIDBG_PRINT("FW Burn to Flash fail! circnt = %d\n", CNTNUM-circnt);
			sprintf(tmpCmd, "echo flyaudio:touch /dev/log/FWUpdateFail%d.txt > /dev/lidbg_misc0",camCnt + 1);
			system(tmpCmd);
		}


		burn_mgr.Cam_DeSelect();

		LIDBG_PRINT("FW update Exit program!\n");
		LIDBG_PRINT("\nPlease restart the computer to make the new FW become effective !\n");
	}
	sprintf(tmpCmd, "echo flyaudio:touch /dev/log/FWUpdateDone.txt > /dev/lidbg_misc0");
	system(tmpCmd);
	return 0;
	
exit:
	LIDBG_PRINT("Exit program!\n");
	sprintf(tmpCmd, "echo flyaudio:touch /dev/log/FWUpdateFailAll.txt > /dev/lidbg_misc0");
	system(tmpCmd);
	sprintf(tmpCmd, "echo flyaudio:touch /dev/log/FWUpdateDone.txt > /dev/lidbg_misc0");
	system(tmpCmd);
	return 0;
}

