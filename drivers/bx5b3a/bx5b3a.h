#ifndef __BX5B3A_H__
#define __BX5B3A_H__

#define 	BX5B3A_I2C_ADDR	       (0x64)



typedef unsigned short u16;
typedef unsigned int u32;
typedef u16 UINT16;
typedef u32 UINT32;



struct QL_VX_INIT_INFO
{
    UINT16 address;
    UINT32 data;
};

//Porting: put your init seq. here.
static struct QL_VX_INIT_INFO ql_initialization_setting[] =
{
    {0x700 , 0x4C900040 },
    {0x704 , 0x1F0343 },
    {0x70C , 0x00004604 },
    {0x710 , 0x54D0007 },
    {0x714 , 0x0 },
    {0x718 , 0x00000102 },
    {0x71C , 0x154000B },
    {0x720 , 0x400 },
    {0x154 , 0x00000000 },
    {0x154 , 0x80000000 },
    {0x700 , 0x4C900840 },
    {0x70C , 0x4626 },
    {0x718 , 0x00000002 },
    {0x154 , 0x00000000 },
    {0x154 , 0x80000000 },
    {0x120 , 0x5 },
    {0x124 , 0x512C400 },
    {0x128 , 0x103C0F },
    {0x12C , 0x93 },
    {0x130 , 0x3C70 },
    {0x134 , 0x5 },
    {0x138 , 0x1F8000 },
    {0x13C , 0x8000000 },
    {0x140 , 0x10008 },
    {0x20C , 0x24 },
    {0x21C , 0x0 },
    {0x224 , 0x0 },
    {0x228 , 0x50001 },
    {0x22C , 0xFF03 },
    {0x230 , 0x1 },
    {0x234 , 0xCA033E10 },
    {0x238 , 0x00000060 },
    {0x23C , 0x82E86030 },
    {0x244 , 0x001E0285 },
    {0x258 , 0x50014 },
    {0x158 , 0x0 },
    {0x158 , 0x1 },
    {0x37C , 0x00001063 },
    {0x380 , 0x82A86030 },
    {0x384 , 0x2861408B },
    {0x388 , 0x00130285 },
    {0x38C , 0x10630009 },
    {0x394 , 0x400B82A8 },
    {0x608 , 0x50F },
    {0x154 , 0x00000000 },
    {0x154 , 0x80000000 },

    //  {0x300,	0x1       },
};


#if 0
//Porting: put your standby seq. here.
static struct QL_VX_INIT_INFO ql_standby_setting[] =
{

};
#endif

#if 0
static struct QL_VX_INIT_INFO ql_lp_setting[] =
{
    {0x11c,  0x985},
};

static struct QL_VX_INIT_INFO ql_hs_setting[] =
{
    {0x11c,  0x984},
};

//Porting: put your DPI turn on seq. here. It's used after the panel on command....
static struct QL_VX_INIT_INFO ql_dpi_on_setting[] =
{

    {0x710 , 0x4D0097},
    {0x154  , 0x00000000},
    {0x154 , 0x80000000},
    {0x300 , 0x0},
    {0x300 , 0x1},
};
#endif

#endif
