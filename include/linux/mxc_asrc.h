/*
 * Copyright 2008-2013 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @file mxc_asrc.h
 *
 * @brief i.MX Asynchronous Sample Rate Converter
 *
 * @ingroup Audio
 */

#ifndef __MXC_ASRC_H__
#define __MXC_ASRC_H__

#define ASRC_IOC_MAGIC		'C'

#define ASRC_REQ_PAIR		_IOWR(ASRC_IOC_MAGIC, 0, struct asrc_req)
#define ASRC_CONFIG_PAIR	_IOWR(ASRC_IOC_MAGIC, 1, struct asrc_config)
#define ASRC_RELEASE_PAIR	_IOW(ASRC_IOC_MAGIC, 2, enum asrc_pair_index)
#define ASRC_CONVERT		_IOW(ASRC_IOC_MAGIC, 3, struct asrc_convert_buffer)
#define ASRC_START_CONV		_IOW(ASRC_IOC_MAGIC, 4, enum asrc_pair_index)
#define ASRC_STOP_CONV		_IOW(ASRC_IOC_MAGIC, 5, enum asrc_pair_index)
#define ASRC_STATUS		_IOW(ASRC_IOC_MAGIC, 6, struct asrc_status_flags)
#define ASRC_FLUSH		_IOW(ASRC_IOC_MAGIC, 7, enum asrc_pair_index)

enum asrc_pair_index {
	ASRC_UNVALID_PAIR = -1,
	ASRC_PAIR_A = 0,
	ASRC_PAIR_B = 1,
	ASRC_PAIR_C = 2,
};

#define ASRC_PAIR_MAX_NUM	(ASRC_PAIR_C + 1)

enum asrc_inclk {
	INCLK_NONE = 0x03,
	INCLK_ESAI_RX = 0x00,
	INCLK_SSI1_RX = 0x01,
	INCLK_SSI2_RX = 0x02,
	INCLK_SSI3_RX = 0x07,
	INCLK_SPDIF_RX = 0x04,
	INCLK_MLB_CLK = 0x05,
	INCLK_PAD = 0x06,
	INCLK_ESAI_TX = 0x08,
	INCLK_SSI1_TX = 0x09,
	INCLK_SSI2_TX = 0x0a,
	INCLK_SSI3_TX = 0x0b,
	INCLK_SPDIF_TX = 0x0c,
	INCLK_ASRCK1_CLK = 0x0f,
};

enum asrc_outclk {
	OUTCLK_NONE = 0x03,
	OUTCLK_ESAI_TX = 0x00,
	OUTCLK_SSI1_TX = 0x01,
	OUTCLK_SSI2_TX = 0x02,
	OUTCLK_SSI3_TX = 0x07,
	OUTCLK_SPDIF_TX = 0x04,
	OUTCLK_MLB_CLK = 0x05,
	OUTCLK_PAD = 0x06,
	OUTCLK_ESAI_RX = 0x08,
	OUTCLK_SSI1_RX = 0x09,
	OUTCLK_SSI2_RX = 0x0a,
	OUTCLK_SSI3_RX = 0x0b,
	OUTCLK_SPDIF_RX = 0x0c,
	OUTCLK_ASRCK1_CLK = 0x0f,
};

enum asrc_word_width {
	ASRC_WIDTH_24_BIT = 0,
	ASRC_WIDTH_16_BIT = 1,
	ASRC_WIDTH_8_BIT = 2,
};

struct asrc_config {
	enum asrc_pair_index pair;
	unsigned int channel_num;
	unsigned int buffer_num;
	unsigned int dma_buffer_size;
	unsigned int input_sample_rate;
	unsigned int output_sample_rate;
	enum asrc_word_width input_word_width;
	enum asrc_word_width output_word_width;
	enum asrc_inclk inclk;
	enum asrc_outclk outclk;
};

struct asrc_pair {
	unsigned int start_channel;
	unsigned int chn_num;
	unsigned int chn_max;
	unsigned int active;
	unsigned int overload_error;
};

struct asrc_req {
	unsigned int chn_num;
	enum asrc_pair_index index;
};

struct asrc_querybuf {
	unsigned int buffer_index;
	unsigned int input_length;
	unsigned int output_length;
	unsigned long input_offset;
	unsigned long output_offset;
};

struct asrc_convert_buffer {
	void *input_buffer_vaddr;
	void *output_buffer_vaddr;
	unsigned int input_buffer_length;
	unsigned int output_buffer_length;
};

struct asrc_buffer {
	unsigned int index;
	unsigned int length;
	unsigned int output_last_length;
	int buf_valid;
};

struct asrc_status_flags {
	enum asrc_pair_index index;
	unsigned int overload_error;
};

#define ASRC_BUF_NA	-35	/* ASRC DQ's buffer is NOT available */
#define ASRC_BUF_AV	 35	/* ASRC DQ's buffer is available */
enum asrc_error_status {
	ASRC_TASK_Q_OVERLOAD		= 0x01,
	ASRC_OUTPUT_TASK_OVERLOAD	= 0x02,
	ASRC_INPUT_TASK_OVERLOAD	= 0x04,
	ASRC_OUTPUT_BUFFER_OVERFLOW	= 0x08,
	ASRC_INPUT_BUFFER_UNDERRUN	= 0x10,
};

#ifdef __KERNEL__
#include <linux/scatterlist.h>
#include <mach/dma.h>

#define ASRC_DMA_BUFFER_NUM		2
#define ASRC_INPUTFIFO_THRESHOLD	32
#define ASRC_OUTPUTFIFO_THRESHOLD	32
#define ASRC_DMA_BUFFER_SIZE		(1024 * 48 * 4)
#define ASRC_MAX_BUFFER_SIZE		(1024 * 48)
#define ASRC_OUTPUT_LAST_SAMPLE_DEFAULT	8


/* Ideal Ratio mode doesn't care the outclk frequency, so be fixed */
#define ASRC_PRESCALER_IDEAL_RATIO	7
/* SPDIF rxclk pulse rate is 128 * samplerate, so 2 ^ 7 */
#define ASRC_PRESCALER_SPDIF_RX		7
/* SPDIF txclk pulse rate is 64 * samplerate, so 2 ^ 6 */
#define ASRC_PRESCALER_SPDIF_TX		6
/* I2S bclk is 16 * 2 = 32, so 2 ^ 5 */
#define ASRC_PRESCALER_I2S_16BIT	5
/* I2S bclk is 24 * 2 = 48 -> 64, so 2 ^ 6 */
#define ASRC_PRESCALER_I2S_24BIT	6


#define ASRC_REG_ASRCTR			0x00
#define ASRC_REG_ASRIER			0x04
#define ASRC_REG_ASRCNCR		0x0C
#define ASRC_REG_ASRCFG			0x10
#define ASRC_REG_ASRCSR			0x14

#define ASRC_REG_ASRCDR1		0x18
#define ASRC_REG_ASRCDR2		0x1C
#define ASRC_REG_ASRCDR(x)		((x < 2) ? ASRC_REG_ASRCDR1 : ASRC_REG_ASRCDR2)

#define ASRC_REG_ASRSTR			0x20
#define ASRC_REG_ASRRA			0x24
#define ASRC_REG_ASRRB			0x28
#define ASRC_REG_ASRRC			0x2C
#define ASRC_REG_ASRPM1			0x40
#define ASRC_REG_ASRPM2			0x44
#define ASRC_REG_ASRPM3			0x48
#define ASRC_REG_ASRPM4			0x4C
#define ASRC_REG_ASRPM5			0x50
#define ASRC_REG_ASRTFR1		0x54
#define ASRC_REG_ASRCCR			0x5C

#define ASRC_REG_ASRDIA			0x60
#define ASRC_REG_ASRDOA			0x64
#define ASRC_REG_ASRDIB			0x68
#define ASRC_REG_ASRDOB			0x6C
#define ASRC_REG_ASRDIC			0x70
#define ASRC_REG_ASRDOC			0x74
#define ASRC_REG_ASRDI(x)		(ASRC_REG_ASRDIA + (x << 3))
#define ASRC_REG_ASRDO(x)		(ASRC_REG_ASRDOA + (x << 3))

#define ASRC_REG_ASRIDRHA		0x80
#define ASRC_REG_ASRIDRLA		0x84
#define ASRC_REG_ASRIDRHB		0x88
#define ASRC_REG_ASRIDRLB		0x8C
#define ASRC_REG_ASRIDRHC		0x90
#define ASRC_REG_ASRIDRLC		0x94
#define ASRC_REG_ASRIDRH(x)		(ASRC_REG_ASRIDRHA + (x << 3))
#define ASRC_REG_ASRIDRL(x)		(ASRC_REG_ASRIDRLA + (x << 3))

#define ASRC_REG_ASR76K			0x98
#define ASRC_REG_ASR56K			0x9C

#define ASRC_REG_ASRMCRA		0xA0
#define ASRC_REG_ASRFSTA		0xA4
#define ASRC_REG_ASRMCRB		0xA8
#define ASRC_REG_ASRFSTB		0xAC
#define ASRC_REG_ASRMCRC		0xB0
#define ASRC_REG_ASRFSTC		0xB4
#define ASRC_REG_ASRMCR(x)		(ASRC_REG_ASRMCRA + (x << 3))
#define ASRC_REG_ASRFST(x)		(ASRC_REG_ASRFSTA + (x << 3))

#define ASRC_REG_ASRMCR1A		0xC0
#define ASRC_REG_ASRMCR1B		0xC4
#define ASRC_REG_ASRMCR1C		0xC8
#define ASRC_REG_ASRMCR1(x)		(ASRC_REG_ASRMCR1A + (x << 2))


/* REG0 0x00 ASRC_REG_ASRCTR */
#define ASRCTR_ATSx_SHIFT(x)		(20 + x)
#define ASRCTR_ATSx_MASK(x)		(1 << ASRCTR_ATSx_SHIFT(x))
#define ASRCTR_ATS(x)			(1 << ASRCTR_ATSx_SHIFT(x))
#define ASRCTR_USRx_SHIFT(x)		(14 + (x << 1))
#define ASRCTR_USRx_MASK(x)		(1 << ASRCTR_USRx_SHIFT(x))
#define ASRCTR_USR(x)			(1 << ASRCTR_USRx_SHIFT(x))
#define ASRCTR_IDRx_SHIFT(x)		(13 + (x << 1))
#define ASRCTR_IDRx_MASK(x)		(1 << ASRCTR_IDRx_SHIFT(x))
#define ASRCTR_IDR(x)			(1 << ASRCTR_IDRx_SHIFT(x))
#define ASRCTR_SRST_SHIFT		4
#define ASRCTR_SRST_MASK		(1 << ASRCTR_SRST_SHIFT)
#define ASRCTR_SRST			(1 << ASRCTR_SRST_SHIFT)
#define ASRCTR_ASRCEx_SHIFT(x)		(1 + x)
#define ASRCTR_ASRCEx_MASK(x)		(1 << ASRCTR_ASRCEx_SHIFT(x))
#define ASRCTR_ASRCE(x)			(1 << ASRCTR_ASRCEx_SHIFT(x))
#define ASRCTR_ASRCEN_SHIFT		0
#define ASRCTR_ASRCEN_MASK		(1 << ASRCTR_ASRCEN_SHIFT)
#define ASRCTR_ASRCEN			(1 << ASRCTR_ASRCEN_SHIFT)

/* REG1 0x04 ASRC_REG_ASRIER */
#define ASRIER_AFPWE_SHIFT		7
#define ASRIER_AFPWE_MASK		(1 << ASRIER_AFPWE_SHIFT)
#define ASRIER_AFPWE			(1 << ASRIER_AFPWE_SHIFT)
#define ASRIER_AOLIE_SHIFT		6
#define ASRIER_AOLIE_MASK		(1 << ASRIER_AOLIE_SHIFT)
#define ASRIER_AOLIE			(1 << ASRIER_AOLIE_SHIFT)
#define ASRIER_ADOEx_SHIFT(x)		(3 + x)
#define ASRIER_ADOEx_MASK(x)		(1 << ASRIER_ADOEx_SHIFT(x))
#define ASRIER_ADOE(x)			(1 << ASRIER_ADOEx_SHIFT(x))
#define ASRIER_ADIEx_SHIFT(x)		(0 + x)
#define ASRIER_ADIEx_MASK(x)		(1 << ASRIER_ADIEx_SHIFT(x))
#define ASRIER_ADIE(x)			(1 << ASRIER_ADIEx_SHIFT(x))

/* REG2 0x0C ASRC_REG_ASRCNCR */
#define ASRCNCR_ANCA_MASK(b)		((1 << b) - 1)
#define ASRCNCR_ANCA_get(v, b)		(v & ASRCNCR_ANCA_MASK(b))
#define ASRCNCR_ANCB_MASK(b)		(((1 << b) - 1) << b)
#define ASRCNCR_ANCB_get(v, b)		((v & ASRCNCR_ANCB_MASK(b)) >> b)
#define ASRCNCR_ANCC_MASK(b)		(((1 << b) - 1) << (b << 1))
#define ASRCNCR_ANCC_get(v, b)		((v & ASRCNCR_ANCC_MASK(b)) >> (b << 1))

/* REG3 0x10 ASRC_REG_ASRCFG */
#define ASRCFG_INIRQx_SHIFT(x)		(21 + x)
#define ASRCFG_INIRQx_MASK(x)		(1 << ASRCFG_INIRQx_SHIFT(x))
#define ASRCFG_INIRQx			(1 << ASRCFG_INIRQx_SHIFT(x))
#define ASRCFG_NDPRx_SHIFT(x)		(18 + x)
#define ASRCFG_NDPRx_MASK(x)		(1 << ASRCFG_NDPRx_SHIFT(x))
#define ASRCFG_NDPRx			(1 << ASRCFG_NDPRx_SHIFT(x))
#define ASRCFG_POSTMODx_SHIFT(x)	(8 + (x << 2))
#define ASRCFG_POSTMODx_WIDTH		2
#define ASRCFG_POSTMODx_MASK(x)		(((1 << ASRCFG_POSTMODx_WIDTH) - 1) << ASRCFG_POSTMODx_SHIFT(x))
#define ASRCFG_POSTMOD(x, v)		((v) << ASRCFG_POSTMODx_SHIFT(x))
#define ASRCFG_POSTMODx_UP(x)		(0 << ASRCFG_POSTMODx_SHIFT(x))
#define ASRCFG_POSTMODx_DCON(x)		(1 << ASRCFG_POSTMODx_SHIFT(x))
#define ASRCFG_POSTMODx_DOWN(x)		(2 << ASRCFG_POSTMODx_SHIFT(x))
#define ASRCFG_PREMODx_SHIFT(x)		(6 + (x << 2))
#define ASRCFG_PREMODx_WIDTH		2
#define ASRCFG_PREMODx_MASK(x)		(((1 << ASRCFG_PREMODx_WIDTH) - 1) << ASRCFG_PREMODx_SHIFT(x))
#define ASRCFG_PREMOD(x, v)		((v) << ASRCFG_PREMODx_SHIFT(x))
#define ASRCFG_PREMODx_UP(x)		(0 << ASRCFG_PREMODx_SHIFT(x))
#define ASRCFG_PREMODx_DCON(x)		(1 << ASRCFG_PREMODx_SHIFT(x))
#define ASRCFG_PREMODx_DOWN(x)		(2 << ASRCFG_PREMODx_SHIFT(x))
#define ASRCFG_PREMODx_BYPASS(x)	(3 << ASRCFG_PREMODx_SHIFT(x))

/* REG4 0x14 ASRC_REG_ASRCSR */
#define ASRCSR_AxCSx_WIDTH		4
#define ASRCSR_AxCSx_MASK		((1 << ASRCSR_AxCSx_WIDTH) - 1)
#define ASRCSR_AOCSx_SHIFT(x)		(12 + (x << 2))
#define ASRCSR_AOCSx_MASK(x)		(((1 << ASRCSR_AxCSx_WIDTH) - 1) << ASRCSR_AOCSx_SHIFT(x))
#define ASRCSR_AOCS(x, v)		((v) << ASRCSR_AOCSx_SHIFT(x))
#define ASRCSR_AICSx_SHIFT(x)		(x << 2)
#define ASRCSR_AICSx_MASK(x)		(((1 << ASRCSR_AxCSx_WIDTH) - 1) << ASRCSR_AICSx_SHIFT(x))
#define ASRCSR_AICS(x, v)		((v) << ASRCSR_AICSx_SHIFT(x))

/* REG5&6 0x18 & 0x1C ASRC_REG_ASRCDR1 & ASRCDR2 */
#define ASRCDRx_AxCPx_WIDTH		3
#define ASRCDRx_AICPx_SHIFT(x)		(0 + (x % 2) * 6)
#define ASRCDRx_AICPx_MASK(x)		(((1 << ASRCDRx_AxCPx_WIDTH) - 1) << ASRCDRx_AICPx_SHIFT(x))
#define ASRCDRx_AICP(x, v)		((v) << ASRCDRx_AICPx_SHIFT(x))
#define ASRCDRx_AICDx_SHIFT(x)		(3 + (x % 2) * 6)
#define ASRCDRx_AICDx_MASK(x)		(((1 << ASRCDRx_AxCPx_WIDTH) - 1) << ASRCDRx_AICDx_SHIFT(x))
#define ASRCDRx_AICD(x, v)		((v) << ASRCDRx_AICDx_SHIFT(x))
#define ASRCDRx_AOCPx_SHIFT(x)		((x < 2) ? 12 + x * 6 : 6)
#define ASRCDRx_AOCPx_MASK(x)		(((1 << ASRCDRx_AxCPx_WIDTH) - 1) << ASRCDRx_AOCPx_SHIFT(x))
#define ASRCDRx_AOCP(x, v)		((v) << ASRCDRx_AOCPx_SHIFT(x))
#define ASRCDRx_AOCDx_SHIFT(x)		((x < 2) ? 15 + x * 6 : 9)
#define ASRCDRx_AOCDx_MASK(x)		(((1 << ASRCDRx_AxCPx_WIDTH) - 1) << ASRCDRx_AOCDx_SHIFT(x))
#define ASRCDRx_AOCD(x, v)		((v) << ASRCDRx_AOCDx_SHIFT(x))

/* REG7 0x20 ASRC_REG_ASRSTR */
#define ASRSTR_DSLCNT_SHIFT		21
#define ASRSTR_DSLCNT			(1 << ASRSTR_DSLCNT_SHIFT)
#define ASRSTR_ATQOL_SHIFT		20
#define ASRSTR_ATQOL			(1 << ASRSTR_ATQOL_SHIFT)
#define ASRSTR_AOOLx_SHIFT(x)		(17 + x)
#define ASRSTR_AOOL(x)			(1 << ASRSTR_AOOLx_SHIFT(x))
#define ASRSTR_AIOLx_SHIFT(x)		(14 + x)
#define ASRSTR_AIOL(x)			(1 << ASRSTR_AIOLx_SHIFT(x))
#define ASRSTR_AODOx_SHIFT(x)		(11 + x)
#define ASRSTR_AODO(x)			(1 << ASRSTR_AODOx_SHIFT(x))
#define ASRSTR_AIDUx_SHIFT(x)		(8 + x)
#define ASRSTR_AIDU(x)			(1 << ASRSTR_AIDUx_SHIFT(x))
#define ASRSTR_FPWT_SHIFT		7
#define ASRSTR_FPWT			(1 << ASRSTR_FPWT_SHIFT)
#define ASRSTR_AOLE_SHIFT		6
#define ASRSTR_AOLE			(1 << ASRSTR_AOLE_SHIFT)
#define ASRSTR_AODEx_SHIFT(x)		(3 + x)
#define ASRSTR_AODF(x)			(1 << ASRSTR_AODEx_SHIFT(x))
#define ASRSTR_AIDEx_SHIFT(x)		(0 + x)
#define ASRSTR_AIDE(x)			(1 << ASRSTR_AIDEx_SHIFT(x))

/* REG10 0x54 ASRC_REG_ASRTFR1 */
#define ASRTFR1_TF_BASE_WIDTH		7
#define ASRTFR1_TF_BASE_SHIFT		6
#define ASRTFR1_TF_BASE_MASK		(((1 << ASRTFR1_TF_BASE_WIDTH) - 1) << ASRTFR1_TF_BASE_SHIFT)
#define ASRTFR1_TF_BASE(x)		((x) << ASRTFR1_TF_BASE_SHIFT)

/*
 * REG22 0xA0 ASRC_REG_ASRMCRA
 * REG24 0xA8 ASRC_REG_ASRMCRB
 * REG26 0xB0 ASRC_REG_ASRMCRC
 */
#define ASRMCRx_ZEROBUFx_SHIFT		23
#define ASRMCRx_ZEROBUFxCLR_MASK	(1 << ASRMCRx_ZEROBUFx_SHIFT)
#define ASRMCRx_ZEROBUFxCLR		(1 << ASRMCRx_ZEROBUFx_SHIFT)
#define ASRMCRx_EXTTHRSHx_SHIFT		22
#define ASRMCRx_EXTTHRSHx_MASK		(1 << ASRMCRx_EXTTHRSHx_SHIFT)
#define ASRMCRx_EXTTHRSHx		(1 << ASRMCRx_EXTTHRSHx_SHIFT)
#define ASRMCRx_BUFSTALLx_SHIFT		21
#define ASRMCRx_BUFSTALLx_MASK		(1 << ASRMCRx_BUFSTALLx_SHIFT)
#define ASRMCRx_BUFSTALLx		(1 << ASRMCRx_BUFSTALLx_SHIFT)
#define ASRMCRx_BYPASSPOLYx_SHIFT	20
#define ASRMCRx_BYPASSPOLYx_MASK	(1 << ASRMCRx_BYPASSPOLYx_SHIFT)
#define ASRMCRx_BYPASSPOLYx		(1 << ASRMCRx_BYPASSPOLYx_SHIFT)
#define ASRMCRx_OUTFIFO_THRESHOLD_WIDTH	6
#define ASRMCRx_OUTFIFO_THRESHOLD_SHIFT	12
#define ASRMCRx_OUTFIFO_THRESHOLD_MASK	(((1 << ASRMCRx_OUTFIFO_THRESHOLD_WIDTH) - 1) << ASRMCRx_OUTFIFO_THRESHOLD_SHIFT)
#define ASRMCRx_OUTFIFO_THRESHOLD(v)	(((v) << ASRMCRx_OUTFIFO_THRESHOLD_SHIFT) & ASRMCRx_OUTFIFO_THRESHOLD_MASK)
#define ASRMCRx_RSYNIFx_SHIFT		11
#define ASRMCRx_RSYNIFx_MASK		(1 << ASRMCRx_RSYNIFx_SHIFT)
#define ASRMCRx_RSYNIFx			(1 << ASRMCRx_RSYNIFx_SHIFT)
#define ASRMCRx_RSYNOFx_SHIFT		10
#define ASRMCRx_RSYNOFx_MASK		(1 << ASRMCRx_RSYNOFx_SHIFT)
#define ASRMCRx_RSYNOFx			(1 << ASRMCRx_RSYNOFx_SHIFT)
#define ASRMCRx_INFIFO_THRESHOLD_WIDTH	6
#define ASRMCRx_INFIFO_THRESHOLD_SHIFT	0
#define ASRMCRx_INFIFO_THRESHOLD_MASK	(((1 << ASRMCRx_INFIFO_THRESHOLD_WIDTH) - 1) << ASRMCRx_INFIFO_THRESHOLD_SHIFT)
#define ASRMCRx_INFIFO_THRESHOLD(v)	(((v) << ASRMCRx_INFIFO_THRESHOLD_SHIFT) & ASRMCRx_INFIFO_THRESHOLD_MASK)

/*
 * REG23 0xA4 ASRC_REG_ASRFSTA
 * REG25 0xAC ASRC_REG_ASRFSTB
 * REG27 0xB4 ASRC_REG_ASRFSTC
 */
#define ASRFSTx_OAFx_SHIFT		23
#define ASRFSTx_OAFx_MASK		(1 << ASRFSTx_OAFx_SHIFT)
#define ASRFSTx_OAFx			(1 << ASRFSTx_OAFx_SHIFT)
#define ASRFSTx_OUTPUT_FIFO_WIDTH	7
#define ASRFSTx_OUTPUT_FIFO_SHIFT	12
#define ASRFSTx_OUTPUT_FIFO_MASK	(((1 << ASRFSTx_OUTPUT_FIFO_WIDTH) - 1) << ASRFSTx_OUTPUT_FIFO_SHIFT)
#define ASRFSTx_IAEx_SHIFT		11
#define ASRFSTx_IAEx_MASK		(1 << ASRFSTx_OAFx_SHIFT)
#define ASRFSTx_IAEx			(1 << ASRFSTx_OAFx_SHIFT)
#define ASRFSTx_INPUT_FIFO_WIDTH	7
#define ASRFSTx_INPUT_FIFO_SHIFT	0
#define ASRFSTx_INPUT_FIFO_MASK		((1 << ASRFSTx_INPUT_FIFO_WIDTH) - 1)

/* REG28 0xC0 & 0xC4 & 0xC8 ASRC_REG_ASRMCR1x */
#define ASRMCR1x_IWD_WIDTH		3
#define ASRMCR1x_IWD_SHIFT		9
#define ASRMCR1x_IWD_MASK		(((1 << ASRMCR1x_IWD_WIDTH) - 1) << ASRMCR1x_IWD_SHIFT)
#define ASRMCR1x_IWD(v)			((v) << ASRMCR1x_IWD_SHIFT)
#define ASRMCR1x_IMSB_SHIFT		8
#define ASRMCR1x_IMSB_MASK		(1 << ASRMCR1x_IMSB_SHIFT)
#define ASRMCR1x_IMSB_MSB		(1 << ASRMCR1x_IMSB_SHIFT)
#define ASRMCR1x_IMSB_LSB		(0 << ASRMCR1x_IMSB_SHIFT)
#define ASRMCR1x_OMSB_SHIFT		2
#define ASRMCR1x_OMSB_MASK		(1 << ASRMCR1x_OMSB_SHIFT)
#define ASRMCR1x_OMSB_MSB		(1 << ASRMCR1x_OMSB_SHIFT)
#define ASRMCR1x_OMSB_LSB		(0 << ASRMCR1x_OMSB_SHIFT)
#define ASRMCR1x_OSGN_SHIFT		1
#define ASRMCR1x_OSGN_MASK		(1 << ASRMCR1x_OSGN_SHIFT)
#define ASRMCR1x_OSGN			(1 << ASRMCR1x_OSGN_SHIFT)
#define ASRMCR1x_OW16_SHIFT		0
#define ASRMCR1x_OW16_MASK		(1 << ASRMCR1x_OW16_SHIFT)
#define ASRMCR1x_OW16(v)		((v) << ASRMCR1x_OW16_SHIFT)


struct dma_block {
	unsigned int index;
	unsigned int length;
	void *dma_vaddr;
	dma_addr_t dma_paddr;
	struct list_head queue;
};

struct asrc_p2p_params {
	u32 p2p_rate;				/* ASRC output rate for p2p */
	enum asrc_word_width p2p_width;		/* ASRC output wordwidth for p2p */
};

struct asrc_pair_params {
	enum asrc_pair_index index;
	wait_queue_head_t input_wait_queue;
	wait_queue_head_t output_wait_queue;
	unsigned int input_counter;
	unsigned int output_counter;
	struct dma_chan *input_dma_channel;
	struct dma_chan *output_dma_channel;
	unsigned int input_buffer_size;
	unsigned int output_buffer_size;
	unsigned int buffer_num;
	unsigned int pair_hold;
	unsigned int asrc_active;
	unsigned int channel_nums;
	struct dma_block input_dma_total;
	struct dma_block input_dma[ASRC_DMA_BUFFER_NUM];
	struct dma_block output_dma_total;
	struct dma_block output_dma[ASRC_DMA_BUFFER_NUM];
	struct dma_block output_last_period;
	struct dma_async_tx_descriptor *desc_in;
	struct dma_async_tx_descriptor *desc_out;
	struct work_struct task_output_work;
	unsigned int input_sg_nodes;
	unsigned int output_sg_nodes;
	struct scatterlist input_sg[4], output_sg[4];
	enum asrc_word_width input_word_width;
	enum asrc_word_width output_word_width;
	u32 input_sample_rate;
	u32 output_sample_rate;
	u32 input_wm;
	u32 output_wm;
	struct completion output_completion;
	unsigned int last_period_sample;
	struct imx_dma_data input_dma_data;
	struct imx_dma_data output_dma_data;
};

struct asrc_data {
	struct asrc_pair asrc_pair[3];
	struct proc_dir_entry *proc_asrc;
	unsigned long vaddr;
	unsigned long paddr;
	int dmarx[3];
	int dmatx[3];
	struct class *asrc_class;
	int asrc_major;
	struct clk *asrc_clk;
	unsigned int channel_bits;
	int clk_map_ver;
	int irq;
	struct device *dev;
};

struct asrc_p2p_ops {
	void (*asrc_p2p_start_conv)(enum asrc_pair_index);
	void (*asrc_p2p_stop_conv)(enum asrc_pair_index);
	int (*asrc_p2p_get_dma_request)(enum asrc_pair_index, bool);
	u32 (*asrc_p2p_per_addr)(enum asrc_pair_index, bool);
	int (*asrc_p2p_req_pair)(int, enum asrc_pair_index *index);
	int (*asrc_p2p_config_pair)(struct asrc_config *config);
	void (*asrc_p2p_release_pair)(enum asrc_pair_index);
	void (*asrc_p2p_finish_conv)(enum asrc_pair_index);
};

extern void asrc_p2p_hook(struct asrc_p2p_ops *asrc_p2p_ct);

extern int asrc_req_pair(int chn_num, enum asrc_pair_index *index);
extern void asrc_release_pair(enum asrc_pair_index index);
extern int asrc_config_pair(struct asrc_config *config);
extern void asrc_get_status(struct asrc_status_flags *flags);
extern void asrc_start_conv(enum asrc_pair_index index);
extern void asrc_stop_conv(enum asrc_pair_index index);
extern u32 asrc_get_per_addr(enum asrc_pair_index index, bool i);
extern int asrc_get_dma_request(enum asrc_pair_index index, bool i);
extern void asrc_finish_conv(enum asrc_pair_index index);
extern int asrc_set_watermark(enum asrc_pair_index index,
		u32 in_wm, u32 out_wm);
extern void sdma_set_event_pending(struct dma_chan *chan);

#endif/* __KERNEL__ */
#endif/* __MXC_ASRC_H__ */