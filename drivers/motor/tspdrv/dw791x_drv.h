/*
 * dw791x.h  --  Vibrator driver for dw7912/dw7914
 *
 * Copyright (C) 2017 Dongwoon Anatech Co. Ltd. All Rights Reserved.
 *
 * license : GPL/BSD Dual 
 *
 */
#include <linux/i2c.h>


#define BST_OPT
#define TRIG_OPT
#define INT_EVENT_OPT


int dw791x_a2v_seq_write(struct i2c_client *client, u8* data, u32 size);
static void dw791x_vd_clamp_set(struct i2c_client *client, u32 vol);
static void dw791x_bst_option(struct i2c_client *client, u32 lv, u32 limit, u32 ofs);
static int dw791x_device_init(struct i2c_client *client);
static int dw791x_byte_write(struct i2c_client *client, u8 addr, u8 data);
static int dw791x_word_write(struct i2c_client *client, u8 addr, u32 data);
int dw791x_byte_read(struct i2c_client *client, u8 addr);
int dw791x_play_mode_sel(struct i2c_client *client, int play_mode, u32 set1, u32 set2, u32 set3);
static int dw791x_seq_write(struct i2c_client *client, u32 addr, u32 ram_addr, u32 ram_bit, u8* data, u32 size);
static void dw791x_fifo_size_set(struct i2c_client *client, u32 size);
static int dw7914_checksum(struct i2c_client *client, u32 type, u32 page);
static void dw7914_trigctrl_set(struct i2c_client *client, u8 type, u8 bitset);
static void dw7914_autovib_set(struct i2c_client *client);
static void dw7914_mode_set(struct i2c_client *client, u8 type, u8 bitset);
static int transfer_atuovib_wave(struct i2c_client *client, u8* wave);
static int request_transfer_mem_wave(struct i2c_client *client, u32 page, u32 point, u8* fw_data, u32 size);
static int request_transfer_rtp_wave(struct i2c_client *client, u8* wave, u32 repeat);


#ifdef INT_EVENT_OPT
static int irq_rtp_exit(int index);	
#endif

static struct kobject *android_vibrator_kobj;
//static struct dw791x_priv *dw791x = NULL;

struct autovib_reg {
	u8 track_ctrl0;
	u8 track_ctrl1;
	u8 track_ctrl2;
	u8 track0_wave;
	u8 track1_wave;
	u8 brake0_twave;
	u8 brake1_twave;
	u8 brake_at_ctrl;
};

/*----------- Common Register ------------*/
#define DW7912 			0x32
#define DW7914			0x40
#define CHIP_ID			0x59
#define DW791x_PID		0x00
#define DW791x_STATUS 	0x01

#define SRAM_BASE		0x4000
#define BIN_BASE		0x3000
#define RAM_ADDR0		0
#define RAM_ADDR16		16
#define I2C_TRANS_RTP 	1024
//#define I2C_TRANS_RTP 	512
#define I2C_TRANS_MEM	1024
#define FIFO_2KB		2
#define FIFO_4KB		4
#define FIFO_8KB		8




/*----------- Common Bit Register ---------*/
#define BITSET			1
#define BITCLR			0
#define RTP				0
#define MEM				1
#define PWM_24K			0
#define PWM_48K			1
#define PWM_96K			2
#define PWM_12K			3
#define BST_ADAPT 		8
#define BST_LUMP 		4
#define BST_BYPASS		2
#define DRV_STOP		0
#define DRV_PLAY		1


/*----------- DW7912 Register ------------*/
#define DW7912_INTEN		0x02
#define DW7912_MODE			0x03
#define DW7912_PWM			0x04
#define DW7912_BST_OUTPUT	0x05
#define DW7912_BST_MODE		0x06
#define DW7912_VD_CLAMP		0x08
#define DW7912_PLAYBACK		0x09
#define DW7912_RTP_INPUT	0x0A
#define DW7912_MEM_GAIN		0x0B
#define DW7912_WAVQ1		0x0C
#define DW7912_WAVQ2		0x0D
#define DW7912_WAVQ3		0x0E
#define DW7912_WAVQ4		0x0F
#define DW7912_WAVQ5		0x10
#define DW7912_WAVQ6		0x11
#define DW7912_WAVQ7		0x12
#define DW7912_WAVQ8		0x13
#define DW7912_MEM_LOOP		0x18
#define DW7912_TRIG_RWAV	0x19
#define DW7912_TRIG_FWAV	0x1A
#define DW7912_RAM_ADDR		0x1B
#define DW7912_FIFO_ADDRH	0x1E
#define DW7912_FIFO_ADDRL	0x1F
#define DW7912_BST_OPTION	0x23
#define DW7912_SW_RESET		0x2F



/*----------- DW7914 Register ------------*/
#define DW7914_STATUS0		0x01
#define DW7914_INTEN		0x02
#define DW7914_TRIG_CTRL	0x03
#define DW7914_PWM			0x04
#define DW7914_BST_OUTPUT	0x05
#define DW7914_BST_OPTION	0x06
#define DW7914_BST_MODE		0x08
#define DW7914_VMH          0x09
#define DW7914_VD_CLAMP		0x0A
#define DW7914_MODE			0x0B
#define DW7914_PLAYBACK		0x0C
#define DW7914_RTP_INPUT	0x0D
#define DW7914_MEM_GAIN		0x0E
#define DW7914_WAVQ1		0x0F
#define DW7914_WAVQ2		0x10
#define DW7914_WAVQ3		0x11
#define DW7914_WAVQ4		0x12
#define DW7914_WAVQ5		0x13
#define DW7914_WAVQ6		0x14
#define DW7914_WAVQ7		0x15
#define DW7914_WAVQ8		0x16
#define DW7914_WAVE_SEQ_LOOP0		0x17
#define DW7914_WAVE_SEQ_LOOP1		0x18
#define DW7914_WAVE_SEQ_LOOP2		0x19
#define DW7914_WAVE_SEQ_LOOP3		0x1A
#define DW7914_MEM_LOOP		0x1B
#define DW7914_TRIG2_R_WAVE	0x26
#define DW7914_TRIG2_F_WAVE	0x29
#define DW7914_TRACK_CTRL0	0x34
#define DW7914_TRACK_CTRL1 	0x35
#define DW7914_TRACK_CTRL2 	0x36
#define DW7914_TRACK0_WAVE 	0x37
#define DW7914_TRACK1_WAVE 	0x38
#define DW7914_BRAKE0_TWAVE	0x39
#define DW7914_BRAKE1_TWAVE	0x3A
#define DW7914_BRAKE_ATWAVE	0x3B
#define DW7914_ZXD_CTRL1	0x3C
#define DW7914_ZXD_CTRL2	0x3D
#define DW7914_LRA_F0_CAL	0x3E
#define DW7914_LRA_F0_INH	0x3F
#define DW7914_LRA_F0_INL	0x40
#define DW7914_TRIG_DET_EN	0x45
#define DW7914_FIFO_LEVEL_SETH	0x4B
#define DW7914_FIFO_LEVEL_SETL	0x4C
#define DW7914_FIFO_LEVEL	0x4D
#define DW7914_RAM_CHKSUM3	0x4F
#define DW7914_RAM_CHKSUM2	0x50
#define DW7914_RAM_CHKSUM1	0x51
#define DW7914_RAM_CHKSUM0	0x52
#define DW7914_STATUS1		0x44
#define DW7914_RAM_ADDR		0x46
#define DW7914_FIFO_ADDRH	0x49
#define DW7914_FIFO_ADDRL	0x4A
#define DW7914_SW_RESET		0x5F



/* DW7914 MODE Register */
#define D14_FIFO_FLUSH		0x20
#define D14_CHKSUM			0x10
#define D14_AUTO_TRACK		0x08
#define D14_AUTO_BRAKE		0x04
#define D14_DIGA_MODE		0x02
#define D14_MEM_MODE		0x01

/* DW7914 TRIG CTRL Register */
#define D14_SOUND_MODE		0x80
#define D14_TRIG2_MASTER	0x40
#define D14_TRIG1_MODE		0x20
#define D14_TRIG1_LEVEL		0x10
#define D14_TRIG_PRIORITY	0x01

/* DW7914 ATUO TRACK_BRAKE Register */
#define D14_TRACK_PATTERN	0x01



/* 
About modeling waves - auto track base m1
- Motor frequency 	: 170Hz
- NULL / GND time 	: 0x3C@0x33(100us)
- ZXD time 			: 0x3D@0x67(450us)
- Cover tolerance range for actuator : 147Hz ~ 193Hz
*/
const unsigned char auto_track_base_m1[117] = {
0x00,	0x01,	// memory address
0x02,	0x7C,	0x00,	0x6E,	0x1E,	// header (1.2V)
0x17,	0x2A,	0x39,	0x46,	0x50,	0x59,	0x60,	0x65,	0x6A,	0x6E,	0x71,	0x73,	0x76,	0x77,	0x79,	0x7A,	// data
0x7B,	0x7C,	0x7C,	0x7D,	0x7D,	0x7D,	0x7E,	0x7E,	0x7E,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	
0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	
0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	
0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	
0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	
0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F,	0x7F
};

//file1
//0x027c=(127*5)+1
const u8 haptic_h1[5] = {
0x02, 0x7C, 0x01, 0x60, 0xE1
};
const u8 haptic_h1_0[5] = {
0x02, 0x7C, 0x01, 0x60, 0x00
};
const u8 haptic_h1_1[5] = {
0x02, 0x7C, 0x01, 0x60, 0x7F
};
const u8 haptic_h1_2[5] = {
0x02, 0x7C, 0x01, 0x60, 0xE1
};
const u8 haptic_h1_3[5] = {
0x02, 0x7C, 0x01, 0x60, 0xFF
};


const u8 haptic_pcm1[352] = {
0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x22, 0x27, 0x2C, 0x31,
0x35, 0x3A, 0x3E, 0x42, 0x47, 0x4B, 0x4F, 0x52, 0x56, 0x5A, 0x5D, 0x61, 0x64, 0x67, 0x6A, 0x6C, 0x6F, 0x71, 0x73, 0x75, 0x77, 0x79, 0x7A, 0x7B, 0x7D, 0x7D, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0x7D, 0x7D, 0x7B, 0x7A, 0x79, 0x77, 0x75, 0x73, 0x71, 0x6F, 0x6C, 0x6A, 0x67, 0x64, 0x61, 0x5D, 0x5A, 0x56, 0x52, 0x4F, 0x4B, 0x47, 0x42, 0x3E, 0x3A, 0x35, 0x31, 0x2C, 0x27, 0x22, 0x1E, 0x19, 0x14, 0x0F, 0x0A, 0x05, 0x00, 0xFA, 0xF5, 0xF0, 0xEB, 0xE6, 0xE1, 0xDD, 0xD8, 0xD3, 0xCE, 0xCA, 0xC5, 0xC1, 0xBD, 0xB8, 0xB4, 0xB0, 0xAD, 0xA9, 0xA5, 0xA2, 0x9E, 0x9B, 0x98, 0x95, 0x93, 0x90, 0x8E, 0x8C, 0x8A, 0x88, 0x86, 0x85, 0x84, 0x82, 0x82, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x82, 0x82, 0x84, 0x85, 0x86, 0x88, 0x8A, 0x8C, 0x8E, 0x90, 0x93, 0x95, 0x98, 0x9B, 0x9E, 0xA2, 0xA5, 0xA9, 0xAD, 0xB0, 0xB4, 0xB8, 0xBD, 0xC1, 0xC5, 0xCA, 0xCE, 0xD3, 0xD8, 0xDD, 0xE1, 0xE6, 0xEB, 0xF0, 0xF5, 0xFA, 0xFF, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x22, 0x27, 0x2C, 0x31, 0x35, 0x3A, 0x3E, 0x42, 0x47, 0x4B, 0x4F, 0x52, 0x56, 0x5A, 0x5D, 0x61, 0x64, 0x67, 0x6A, 0x6C, 0x6F, 0x71, 0x73, 0x75, 0x77, 0x79, 0x7A, 0x7B, 0x7D, 0x7D, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0x7D, 0x7D, 0x7B, 0x7A, 0x79, 0x77, 0x75, 0x73, 0x71, 0x6F, 0x6C, 0x6A, 0x67, 0x64, 0x61, 0x5D, 0x5A, 0x56, 0x52, 0x4F, 0x4B, 0x47, 0x42, 0x3E, 0x3A, 0x35, 0x31, 0x2C, 0x27, 0x22, 0x1E, 0x19, 0x14, 0x0F, 0x0A, 0x05, 0x00, 0xFA, 0xF5, 0xF0, 0xEB, 0xE6, 0xE1, 0xDD, 0xD8, 0xD3, 0xCE, 0xCA, 0xC5, 0xC1, 0xBD, 0xB8, 0xB4, 0xB0, 0xAD, 0xA9, 0xA5, 0xA2, 0x9E, 0x9B, 0x98, 0x95, 0x93, 0x90, 0x8E, 0x8C, 0x8A, 0x88, 0x86, 0x85, 0x84, 0x82, 0x82, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x82, 0x82, 0x84, 0x85, 0x86, 0x88, 0x8A, 0x8C, 0x8E, 0x90, 0x93, 0x95, 0x98, 0x9B, 0x9E, 0xA2, 0xA5, 0xA9, 0xAD, 0xB0, 0xB4, 0xB8, 0xBD, 0xC1, 0xC5, 0xCA, 0xCE, 0xD3, 0xD8, 0xDD, 0xE1, 0xE6, 0xEB, 0xF0, 0xF5, 0xFA, 0xFF, 0x11, 0x21, 0x31, 0x40, 0x4D, 0x5A, 0x65, 0x6E, 0x75, 0x7B, 0x7E, 0x7F, 0x7E, 0x7B, 0x75, 0x6E, 0x65, 0x5A, 0x4D, 0x40, 0x31, 0x21, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};

const u8 haptic_h2[5] = {
0x03, 0xDC, 0x02, 0x23, 0x64 //4V
};
const u8 haptic_h2_0[5] = {
0x03, 0xDC, 0x02, 0x23, 0x00 //0V
};
const u8 haptic_h2_1[5] = {
0x03, 0xDC, 0x02, 0x23, 0x3F //2.5V
};
const u8 haptic_h2_2[5] = {
0x03, 0xDC, 0x02, 0x23, 0x64 //4V
};
const u8 haptic_h2_3[5] = {
0x03, 0xDC, 0x02, 0x23, 0xC8 //7.2V
};

const u8 haptic_pcm2[547] = {
0x04,0x08,0x0C,0x10,0x14,0x18,0x1C,0x20,0x25,0x28,0x2C,0x30,0x34,0x38,0x3C,0x3F,0x43,0x46,0x4A,0x4D,0x50,0x54,0x57,
0x5A,0x5D,0x5F,0x62,0x65,0x67,0x6A,0x6C,0x6E,0x70,0x72,0x74,0x75,0x77,0x78,0x7A,0x7B,0x7C,0x7D,0x7D,0x7E,0x7E,0x7F,
0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7D,0x7D,0x7C,0x7B,0x7A,0x78,0x77,0x75,0x74,0x72,0x70,0x6E,0x6C,0x6A,0x67,0x65,0x62,
0x5F,0x5D,0x5A,0x57,0x54,0x50,0x4D,0x4A,0x46,0x43,0x3F,0x3C,0x38,0x34,0x30,0x2C,0x28,0x25,0x20,0x1C,0x18,0x14,0x10,
0x0C,0x08,0x04,0x00,0xFC,0xF8,0xF4,0xF0,0xEC,0xE8,0xE4,0xE0,0xDB,0xD8,0xD4,0xD0,0xCC,0xC8,0xC4,0xC1,0xBD,0xBA,0xB6,
0xB3,0xB0,0xAC,0xA9,0xA6,0xA3,0xA1,0x9E,0x9B,0x99,0x96,0x94,0x92,0x90,0x8E,0x8C,0x8B,0x89,0x88,0x86,0x85,0x84,0x83,
0x83,0x82,0x82,0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x83,0x83,0x84,0x85,0x86,0x88,0x89,0x8B,0x8C,0x8E,0x90,0x92,0x94,
0x96,0x99,0x9B,0x9E,0xA1,0xA3,0xA6,0xA9,0xAC,0xB0,0xB3,0xB6,0xBA,0xBD,0xC1,0xC4,0xC8,0xCC,0xD0,0xD4,0xD8,0xDB,0xE0,
0xE4,0xE8,0xEC,0xF0,0xF4,0xF8,0xFC,0x00,0x04,0x08,0x0C,0x10,0x14,0x18,0x1C,0x20,0x25,0x28,0x2C,0x30,0x34,0x38,0x3C,
0x3F,0x43,0x46,0x4A,0x4D,0x50,0x54,0x57,0x5A,0x5D,0x5F,0x62,0x65,0x67,0x6A,0x6C,0x6E,0x70,0x72,0x74,0x75,0x77,0x78,
0x7A,0x7B,0x7C,0x7D,0x7D,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7D,0x7D,0x7C,0x7B,0x7A,0x78,0x77,0x75,0x74,
0x72,0x70,0x6E,0x6C,0x6A,0x67,0x65,0x62,0x5F,0x5D,0x5A,0x57,0x54,0x50,0x4D,0x4A,0x46,0x43,0x3F,0x3C,0x38,0x34,0x30,
0x2C,0x28,0x25,0x20,0x1C,0x18,0x14,0x10,0x0C,0x08,0x04,0x00,0xFC,0xF8,0xF4,0xF0,0xEC,0xE8,0xE4,0xE0,0xDB,0xD8,0xD4,
0xD0,0xCC,0xC8,0xC4,0xC1,0xBD,0xBA,0xB6,0xB3,0xB0,0xAC,0xA9,0xA6,0xA3,0xA1,0x9E,0x9B,0x99,0x96,0x94,0x92,0x90,0x8E,
0x8C,0x8B,0x89,0x88,0x86,0x85,0x84,0x83,0x83,0x82,0x82,0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x83,0x83,0x84,0x85,0x86,
0x88,0x89,0x8B,0x8C,0x8E,0x90,0x92,0x94,0x96,0x99,0x9B,0x9E,0xA1,0xA3,0xA6,0xA9,0xAC,0xB0,0xB3,0xB6,0xBA,0xBD,0xC1,
0xC4,0xC8,0xCC,0xD0,0xD4,0xD8,0xDB,0xE0,0xE4,0xE8,0xEC,0xF0,0xF4,0xF8,0xFC,0x00,0x06,0x0D,0x13,0x1A,0x20,0x27,0x2D,
0x33,0x39,0x3F,0x45,0x4A,0x50,0x55,0x5A,0x5E,0x63,0x67,0x6A,0x6E,0x71,0x74,0x77,0x79,0x7B,0x7C,0x7D,0x7E,0x7F,0x7F,
0x7F,0x7E,0x7D,0x7C,0x7B,0x79,0x77,0x74,0x71,0x6E,0x6A,0x67,0x63,0x5E,0x5A,0x55,0x50,0x4A,0x45,0x3F,0x39,0x33,0x2D,
0x27,0x20,0x1A,0x13,0x0D,0x06,0x00,0x06,0x0B,0x11,0x17,0x1D,0x23,0x28,0x2E,0x33,0x38,0x3D,0x42,0x46,0x4A,0x4E,0x52,
0x55,0x58,0x5B,0x5E,0x60,0x62,0x63,0x64,0x65,0x65,0x65,0x65,0x65,0x64,0x62,0x61,0x5E,0x5C,0x59,0x56,0x53,0x50,0x4C,
0x48,0x43,0x3F,0x3A,0x35,0x30,0x2A,0x25,0x1F,0x19,0x13,0x0D,0x08,0x02,0xF2,0xE3,0xD4,0xC7,0xBA,0xAE,0xA3,0x99,0x91,
0x8B,0x86,0x83,0x81,0x81,0x83,0x87,0x8D,0x94,0x9C,0xA6,0xB2,0xBE,0xCB,0xD9,0xE8,0xF6,0xF3,0xE6,0xD9,0xCD,0xC2,0xB8,
0xB0,0xA8,0xA2,0x9E,0x9B,0x9A,0x9B,0x9E,0xA2,0xA8,0xB0,0xB8,0xC2,0xCE,0xD9,0xE6,0xF3,0x00
};

const u8 haptic_h3[5] = {
0x05, 0xFF, 0x03, 0x7A, 0x64 //4V
};
const u8 haptic_h3_0[5] = {
0x05, 0xFF, 0x03, 0x7A, 0x00 //0V
};
const u8 haptic_h3_1[5] = {
0x05, 0xFF, 0x03, 0x7A, 0x3F //2.5V
};
const u8 haptic_h3_2[5] = {
0x05, 0xFF, 0x03, 0x7A, 0x64 //4V
};
const u8 haptic_h3_3[5] = {
0x05, 0xFF, 0x03, 0x7A, 0x7D //5V
};

const u8 haptic_pcm3[890] = {
0x04,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0C,0x0E,0x10,0x12,0x15,0x17,0x1B,0x1F,0x23,0x28,0x2E,0x34,0x3C,0x43,0x4B,
0x51,0x57,0x5C,0x60,0x64,0x68,0x6A,0x6D,0x6F,0x71,0x73,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7B,0x7C,0x7C,0x7D,0x7D,
0x7D,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7E,0x7E,
0x7E,0x7D,0x7D,0x7D,0x7C,0x7C,0x7C,0x7B,0x7A,0x7A,0x79,0x78,0x77,0x76,0x74,0x73,0x71,0x6F,0x6C,0x6A,0x67,0x63,0x5F,
0x5B,0x55,0x50,0x49,0x41,0x38,0x2E,0x23,0x16,0x07,0xF8,0xE9,0xDC,0xD1,0xC7,0xBE,0xB6,0xAF,0xAA,0xA4,0xA0,0x9C,0x98,
0x95,0x93,0x90,0x8E,0x8C,0x8B,0x89,0x88,0x87,0x86,0x85,0x85,0x84,0x83,0x83,0x83,0x82,0x82,0x82,0x81,0x81,0x81,0x81,
0x81,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x82,0x83,0x83,0x83,0x84,0x85,0x85,0x86,0x87,
0x88,0x89,0x8B,0x8C,0x8E,0x90,0x93,0x95,0x98,0x9C,0xA0,0xA4,0xAA,0xAF,0xB6,0xBE,0xC7,0xD1,0xDC,0xE9,0xF8,0x07,0x16,
0x23,0x2E,0x38,0x41,0x49,0x50,0x55,0x5B,0x5F,0x63,0x67,0x6A,0x6C,0x6F,0x71,0x73,0x74,0x76,0x77,0x78,0x79,0x7A,0x7A,
0x7B,0x7C,0x7C,0x7C,0x7D,0x7D,0x7D,0x7E,0x7E,0x7E,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7E,0x7E,0x7E,
0x7D,0x7D,0x7D,0x7C,0x7C,0x7C,0x7B,0x7A,0x7A,0x79,0x78,0x77,0x76,0x74,0x73,0x71,0x6F,0x6C,0x6A,0x67,0x63,0x5F,0x5B,
0x55,0x50,0x49,0x41,0x38,0x2E,0x23,0x16,0x07,0xF8,0xE9,0xDC,0xD1,0xC7,0xBE,0xB6,0xAF,0xAA,0xA4,0xA0,0x9C,0x98,0x95,
0x93,0x90,0x8E,0x8C,0x8B,0x89,0x88,0x87,0x86,0x85,0x85,0x84,0x83,0x83,0x83,0x82,0x82,0x82,0x81,0x81,0x81,0x81,0x81,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x82,0x83,0x83,0x83,0x84,0x85,0x85,0x86,0x87,0x88,
0x89,0x8B,0x8C,0x8E,0x90,0x93,0x95,0x98,0x9C,0xA0,0xA4,0xA9,0xAF,0xB6,0xBD,0xC6,0xD0,0xDB,0xE8,0xF7,0x07,0x15,0x22,
0x2D,0x37,0x40,0x47,0x4E,0x53,0x58,0x5D,0x60,0x63,0x66,0x68,0x6A,0x6B,0x6C,0x6D,0x6D,0x6D,0x6D,0x6C,0x6B,0x6A,0x68,
0x66,0x63,0x60,0x5D,0x58,0x53,0x4E,0x47,0x40,0x37,0x2D,0x22,0x15,0x07,0xF7,0xE8,0xDB,0xD0,0xC6,0xBE,0xB6,0xAF,0xA9,
0xA4,0xA0,0x9C,0x98,0x95,0x93,0x91,0x8F,0x8D,0x8B,0x8A,0x89,0x88,0x87,0x86,0x86,0x85,0x85,0x85,0x85,0x85,0x85,0x85,
0x85,0x85,0x85,0x86,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8D,0x8F,0x91,0x93,0x96,0x99,0x9C,0xA0,0xA5,0xAA,0xB0,0xB6,0xBE,
0xC7,0xD1,0xDC,0xE9,0xF8,0x07,0x16,0x23,0x2F,0x38,0x41,0x49,0x50,0x55,0x5B,0x5F,0x63,0x67,0x6A,0x6C,0x6F,0x71,0x73,
0x74,0x76,0x77,0x78,0x79,0x7A,0x7A,0x7B,0x7C,0x7C,0x7C,0x7D,0x7D,0x7D,0x7E,0x7E,0x7E,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,
0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,
0x7E,0x7E,0x7E,0x7D,0x7D,0x7D,0x7C,0x7C,0x7C,0x7B,0x7A,0x7A,0x79,0x78,0x77,0x76,0x74,0x73,0x71,0x6F,0x6C,0x6A,0x67,
0x63,0x5F,0x5B,0x55,0x50,0x49,0x41,0x38,0x2E,0x23,0x16,0x07,0xF8,0xE9,0xDC,0xD1,0xC7,0xBE,0xB6,0xAF,0xAA,0xA4,0xA0,
0x9C,0x98,0x95,0x93,0x90,0x8E,0x8C,0x8B,0x89,0x88,0x87,0x86,0x85,0x85,0x84,0x83,0x83,0x83,0x82,0x82,0x82,0x81,0x81,
0x81,0x81,0x81,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x82,0x83,0x83,0x83,0x84,0x85,0x85,0x86,0x87,0x88,0x89,0x8B,
0x8C,0x8E,0x90,0x93,0x95,0x98,0x9C,0xA0,0xA4,0xAA,0xAF,0xB6,0xBE,0xC7,0xD1,0xDC,0xE9,0xF8,0x07,0x16,0x23,0x2E,0x38,
0x41,0x49,0x50,0x55,0x5B,0x5F,0x63,0x67,0x6A,0x6C,0x6F,0x71,0x73,0x74,0x76,0x77,0x78,0x79,0x7A,0x7A,0x7B,0x7C,0x7C,
0x7C,0x7D,0x7D,0x7D,0x7E,0x7E,0x7E,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7E,0x7E,0x7E,0x7D,0x7D,0x7D,0x7C,0x7C,0x7C,0x7B,
0x7A,0x7A,0x79,0x78,0x77,0x76,0x74,0x73,0x71,0x6F,0x6C,0x6A,0x67,0x63,0x5F,0x5B,0x56,0x50,0x49,0x41,0x39,0x2F,0x23,
0x16,0x08,0xF8,0xE9,0xDC,0xD1,0xC7,0xBF,0xB7,0xB0,0xAB,0xA6,0xA1,0x9E,0x9A,0x98,0x95,0x93,0x92,0x90,0x8F,0x8E,0x8E,
0x8E,0x8E,0x8E,0x8E,0x8F,0x90,0x91,0x93,0x95,0x97,0x9A,0x9D,0xA0,0xA5,0xA9,0xAF,0xB5,0xBD,0xC4,0xCC,0xD2,0xD8,0xDD,
0xE1,0xE5,0xE9,0xEC,0xEE,0xF1,0xF3,0xF5,0xF6,0xF8,0xF9,0xFA,0xFC,0xFD,0xFE,0xFF
};

//trigger3, gamma
const u8 haptic_h4[5] = {
0x09, 0x79, 0x02, 0x23, 0x64 //4V
};
const u8 haptic_pcm4[547] = {
0x04,0x08,0x0C,0x10,0x14,0x18,0x1C,0x20,0x25,0x28,0x2C,0x30,0x34,0x38,0x3C,0x3F,0x43,0x46,0x4A,0x4D,0x50,0x54,0x57,
0x5A,0x5D,0x5F,0x62,0x65,0x67,0x6A,0x6C,0x6E,0x70,0x72,0x74,0x75,0x77,0x78,0x7A,0x7B,0x7C,0x7D,0x7D,0x7E,0x7E,0x7F,
0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7D,0x7D,0x7C,0x7B,0x7A,0x78,0x77,0x75,0x74,0x72,0x70,0x6E,0x6C,0x6A,0x67,0x65,0x62,
0x5F,0x5D,0x5A,0x57,0x54,0x50,0x4D,0x4A,0x46,0x43,0x3F,0x3C,0x38,0x34,0x30,0x2C,0x28,0x25,0x20,0x1C,0x18,0x14,0x10,
0x0C,0x08,0x04,0x00,0xFC,0xF8,0xF4,0xF0,0xEC,0xE8,0xE4,0xE0,0xDB,0xD8,0xD4,0xD0,0xCC,0xC8,0xC4,0xC1,0xBD,0xBA,0xB6,
0xB3,0xB0,0xAC,0xA9,0xA6,0xA3,0xA1,0x9E,0x9B,0x99,0x96,0x94,0x92,0x90,0x8E,0x8C,0x8B,0x89,0x88,0x86,0x85,0x84,0x83,
0x83,0x82,0x82,0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x83,0x83,0x84,0x85,0x86,0x88,0x89,0x8B,0x8C,0x8E,0x90,0x92,0x94,
0x96,0x99,0x9B,0x9E,0xA1,0xA3,0xA6,0xA9,0xAC,0xB0,0xB3,0xB6,0xBA,0xBD,0xC1,0xC4,0xC8,0xCC,0xD0,0xD4,0xD8,0xDB,0xE0,
0xE4,0xE8,0xEC,0xF0,0xF4,0xF8,0xFC,0x00,0x04,0x08,0x0C,0x10,0x14,0x18,0x1C,0x20,0x25,0x28,0x2C,0x30,0x34,0x38,0x3C,
0x3F,0x43,0x46,0x4A,0x4D,0x50,0x54,0x57,0x5A,0x5D,0x5F,0x62,0x65,0x67,0x6A,0x6C,0x6E,0x70,0x72,0x74,0x75,0x77,0x78,
0x7A,0x7B,0x7C,0x7D,0x7D,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7D,0x7D,0x7C,0x7B,0x7A,0x78,0x77,0x75,0x74,
0x72,0x70,0x6E,0x6C,0x6A,0x67,0x65,0x62,0x5F,0x5D,0x5A,0x57,0x54,0x50,0x4D,0x4A,0x46,0x43,0x3F,0x3C,0x38,0x34,0x30,
0x2C,0x28,0x25,0x20,0x1C,0x18,0x14,0x10,0x0C,0x08,0x04,0x00,0xFC,0xF8,0xF4,0xF0,0xEC,0xE8,0xE4,0xE0,0xDB,0xD8,0xD4,
0xD0,0xCC,0xC8,0xC4,0xC1,0xBD,0xBA,0xB6,0xB3,0xB0,0xAC,0xA9,0xA6,0xA3,0xA1,0x9E,0x9B,0x99,0x96,0x94,0x92,0x90,0x8E,
0x8C,0x8B,0x89,0x88,0x86,0x85,0x84,0x83,0x83,0x82,0x82,0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x83,0x83,0x84,0x85,0x86,
0x88,0x89,0x8B,0x8C,0x8E,0x90,0x92,0x94,0x96,0x99,0x9B,0x9E,0xA1,0xA3,0xA6,0xA9,0xAC,0xB0,0xB3,0xB6,0xBA,0xBD,0xC1,
0xC4,0xC8,0xCC,0xD0,0xD4,0xD8,0xDB,0xE0,0xE4,0xE8,0xEC,0xF0,0xF4,0xF8,0xFC,0x00,0x06,0x0D,0x13,0x1A,0x20,0x27,0x2D,
0x33,0x39,0x3F,0x45,0x4A,0x50,0x55,0x5A,0x5E,0x63,0x67,0x6A,0x6E,0x71,0x74,0x77,0x79,0x7B,0x7C,0x7D,0x7E,0x7F,0x7F,
0x7F,0x7E,0x7D,0x7C,0x7B,0x79,0x77,0x74,0x71,0x6E,0x6A,0x67,0x63,0x5E,0x5A,0x55,0x50,0x4A,0x45,0x3F,0x39,0x33,0x2D,
0x27,0x20,0x1A,0x13,0x0D,0x06,0x00,0x06,0x0B,0x11,0x17,0x1D,0x23,0x28,0x2E,0x33,0x38,0x3D,0x42,0x46,0x4A,0x4E,0x52,
0x55,0x58,0x5B,0x5E,0x60,0x62,0x63,0x64,0x65,0x65,0x65,0x65,0x65,0x64,0x62,0x61,0x5E,0x5C,0x59,0x56,0x53,0x50,0x4C,
0x48,0x43,0x3F,0x3A,0x35,0x30,0x2A,0x25,0x1F,0x19,0x13,0x0D,0x08,0x02,0xF2,0xE3,0xD4,0xC7,0xBA,0xAE,0xA3,0x99,0x91,
0x8B,0x86,0x83,0x81,0x81,0x83,0x87,0x8D,0x94,0x9C,0xA6,0xB2,0xBE,0xCB,0xD9,0xE8,0xF6,0xF3,0xE6,0xD9,0xCD,0xC2,0xB8,
0xB0,0xA8,0xA2,0x9E,0x9B,0x9A,0x9B,0x9E,0xA2,0xA8,0xB0,0xB8,0xC2,0xCE,0xD9,0xE6,0xF3,0x00
};

//tigger3, 0832
const u8 haptic_h5[5] = {
0x0B, 0x9C, 0x03, 0x7A, 0x64 //4V
};
const u8 haptic_pcm5[890] = {
0x04,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0C,0x0E,0x10,0x12,0x15,0x17,0x1B,0x1F,0x23,0x28,0x2E,0x34,0x3C,0x43,0x4B,
0x51,0x57,0x5C,0x60,0x64,0x68,0x6A,0x6D,0x6F,0x71,0x73,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7B,0x7C,0x7C,0x7D,0x7D,
0x7D,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7E,0x7E,
0x7E,0x7D,0x7D,0x7D,0x7C,0x7C,0x7C,0x7B,0x7A,0x7A,0x79,0x78,0x77,0x76,0x74,0x73,0x71,0x6F,0x6C,0x6A,0x67,0x63,0x5F,
0x5B,0x55,0x50,0x49,0x41,0x38,0x2E,0x23,0x16,0x07,0xF8,0xE9,0xDC,0xD1,0xC7,0xBE,0xB6,0xAF,0xAA,0xA4,0xA0,0x9C,0x98,
0x95,0x93,0x90,0x8E,0x8C,0x8B,0x89,0x88,0x87,0x86,0x85,0x85,0x84,0x83,0x83,0x83,0x82,0x82,0x82,0x81,0x81,0x81,0x81,
0x81,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x82,0x83,0x83,0x83,0x84,0x85,0x85,0x86,0x87,
0x88,0x89,0x8B,0x8C,0x8E,0x90,0x93,0x95,0x98,0x9C,0xA0,0xA4,0xAA,0xAF,0xB6,0xBE,0xC7,0xD1,0xDC,0xE9,0xF8,0x07,0x16,
0x23,0x2E,0x38,0x41,0x49,0x50,0x55,0x5B,0x5F,0x63,0x67,0x6A,0x6C,0x6F,0x71,0x73,0x74,0x76,0x77,0x78,0x79,0x7A,0x7A,
0x7B,0x7C,0x7C,0x7C,0x7D,0x7D,0x7D,0x7E,0x7E,0x7E,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7E,0x7E,0x7E,
0x7D,0x7D,0x7D,0x7C,0x7C,0x7C,0x7B,0x7A,0x7A,0x79,0x78,0x77,0x76,0x74,0x73,0x71,0x6F,0x6C,0x6A,0x67,0x63,0x5F,0x5B,
0x55,0x50,0x49,0x41,0x38,0x2E,0x23,0x16,0x07,0xF8,0xE9,0xDC,0xD1,0xC7,0xBE,0xB6,0xAF,0xAA,0xA4,0xA0,0x9C,0x98,0x95,
0x93,0x90,0x8E,0x8C,0x8B,0x89,0x88,0x87,0x86,0x85,0x85,0x84,0x83,0x83,0x83,0x82,0x82,0x82,0x81,0x81,0x81,0x81,0x81,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x82,0x83,0x83,0x83,0x84,0x85,0x85,0x86,0x87,0x88,
0x89,0x8B,0x8C,0x8E,0x90,0x93,0x95,0x98,0x9C,0xA0,0xA4,0xA9,0xAF,0xB6,0xBD,0xC6,0xD0,0xDB,0xE8,0xF7,0x07,0x15,0x22,
0x2D,0x37,0x40,0x47,0x4E,0x53,0x58,0x5D,0x60,0x63,0x66,0x68,0x6A,0x6B,0x6C,0x6D,0x6D,0x6D,0x6D,0x6C,0x6B,0x6A,0x68,
0x66,0x63,0x60,0x5D,0x58,0x53,0x4E,0x47,0x40,0x37,0x2D,0x22,0x15,0x07,0xF7,0xE8,0xDB,0xD0,0xC6,0xBE,0xB6,0xAF,0xA9,
0xA4,0xA0,0x9C,0x98,0x95,0x93,0x91,0x8F,0x8D,0x8B,0x8A,0x89,0x88,0x87,0x86,0x86,0x85,0x85,0x85,0x85,0x85,0x85,0x85,
0x85,0x85,0x85,0x86,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8D,0x8F,0x91,0x93,0x96,0x99,0x9C,0xA0,0xA5,0xAA,0xB0,0xB6,0xBE,
0xC7,0xD1,0xDC,0xE9,0xF8,0x07,0x16,0x23,0x2F,0x38,0x41,0x49,0x50,0x55,0x5B,0x5F,0x63,0x67,0x6A,0x6C,0x6F,0x71,0x73,
0x74,0x76,0x77,0x78,0x79,0x7A,0x7A,0x7B,0x7C,0x7C,0x7C,0x7D,0x7D,0x7D,0x7E,0x7E,0x7E,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,
0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,
0x7E,0x7E,0x7E,0x7D,0x7D,0x7D,0x7C,0x7C,0x7C,0x7B,0x7A,0x7A,0x79,0x78,0x77,0x76,0x74,0x73,0x71,0x6F,0x6C,0x6A,0x67,
0x63,0x5F,0x5B,0x55,0x50,0x49,0x41,0x38,0x2E,0x23,0x16,0x07,0xF8,0xE9,0xDC,0xD1,0xC7,0xBE,0xB6,0xAF,0xAA,0xA4,0xA0,
0x9C,0x98,0x95,0x93,0x90,0x8E,0x8C,0x8B,0x89,0x88,0x87,0x86,0x85,0x85,0x84,0x83,0x83,0x83,0x82,0x82,0x82,0x81,0x81,
0x81,0x81,0x81,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x82,0x83,0x83,0x83,0x84,0x85,0x85,0x86,0x87,0x88,0x89,0x8B,
0x8C,0x8E,0x90,0x93,0x95,0x98,0x9C,0xA0,0xA4,0xAA,0xAF,0xB6,0xBE,0xC7,0xD1,0xDC,0xE9,0xF8,0x07,0x16,0x23,0x2E,0x38,
0x41,0x49,0x50,0x55,0x5B,0x5F,0x63,0x67,0x6A,0x6C,0x6F,0x71,0x73,0x74,0x76,0x77,0x78,0x79,0x7A,0x7A,0x7B,0x7C,0x7C,
0x7C,0x7D,0x7D,0x7D,0x7E,0x7E,0x7E,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7E,0x7E,0x7E,0x7D,0x7D,0x7D,0x7C,0x7C,0x7C,0x7B,
0x7A,0x7A,0x79,0x78,0x77,0x76,0x74,0x73,0x71,0x6F,0x6C,0x6A,0x67,0x63,0x5F,0x5B,0x56,0x50,0x49,0x41,0x39,0x2F,0x23,
0x16,0x08,0xF8,0xE9,0xDC,0xD1,0xC7,0xBF,0xB7,0xB0,0xAB,0xA6,0xA1,0x9E,0x9A,0x98,0x95,0x93,0x92,0x90,0x8F,0x8E,0x8E,
0x8E,0x8E,0x8E,0x8E,0x8F,0x90,0x91,0x93,0x95,0x97,0x9A,0x9D,0xA0,0xA5,0xA9,0xAF,0xB5,0xBD,0xC4,0xCC,0xD2,0xD8,0xDD,
0xE1,0xE5,0xE9,0xEC,0xEE,0xF1,0xF3,0xF5,0xF6,0xF8,0xF9,0xFA,0xFC,0xFD,0xFE,0xFF
};
