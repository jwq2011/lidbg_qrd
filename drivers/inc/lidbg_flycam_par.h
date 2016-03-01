#ifndef __LIDBG_FLYCAM_PAR_
#define __LIDBG_FLYCAM_PAR_

#define REARVIEW_ID	0
#define DVR_ID				1

typedef enum {
  NR_BITRATE,
  NR_RESOLUTION,
  NR_PATH,
  NR_TIME,
  NR_FILENUM,
  NR_TOTALSIZE,
  NR_START_REC,
  NR_STOP_REC,
  NR_SET_PAR,
}cam_ctrl_t;

typedef enum {
  NR_STATUS,
}status_ctrl_t;

typedef enum {
  NR_VERSION,
}fw_ctrl_t;

#define FLYCAM_FRONT_REC_IOC_MAGIC  'F'
#define FLYCAM_FRONT_ONLINE_IOC_MAGIC  'f'
#define FLYCAM_BACK_REC_IOC_MAGIC  'B'
#define FLYCAM_BACK_ONLINE_IOC_MAGIC  'b'
#define FLYCAM_STATUS_IOC_MAGIC  's'
#define FLYCAM_FW_IOC_MAGIC  'w'

typedef enum {
  RET_SUCCESS,
  RET_NOTVALID,
  RET_NOTSONIX,
  RET_FAIL,
  RET_IGNORE,
  RET_REPEATREQ,
}cam_ioctl_ret_t;

typedef enum {
  RET_DEFALUT,
  RET_START,
  RET_STOP,
  RET_EXCEED_UPPER_LIMIT,
  RET_DISCONNECT,
  RET_INSUFFICIENT_SPACE_CIRC,
  RET_INSUFFICIENT_SPACE_STOP,
  RET_INIT_INSUFFICIENT_SPACE_STOP,
  RET_SONIX,
  RET_NOT_SONIX,
}cam_read_ret_t;

//for hub
#define	FRONT_NODE		"1-1.2"	
#define	BACK_NODE		"1-1.3"

#endif
