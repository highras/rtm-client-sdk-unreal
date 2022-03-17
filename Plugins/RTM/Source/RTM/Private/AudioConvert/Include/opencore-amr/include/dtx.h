/*
 ** Copyright 2003-2010, VisualOn, Inc.
 **
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 **
 **     http://www.apache.org/licenses/LICENSE-2.0
 **
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 */


/*--------------------------------------------------------------------------*
 *                         DTX.H					    *
 *--------------------------------------------------------------------------*
 *       Static memory, constants and frametypes for the DTX 		    *
 *--------------------------------------------------------------------------*/

#ifndef __DTX_H__
#define __DTX_H__
#include "typedef.h"
#include "voMem.h"
#include "oscl_base.h"

#define DTX_MAX_EMPTY_THRESH 50
#define DTX_HIST_SIZE 8
#define DTX_HIST_SIZE_MIN_ONE 7
#define DTX_ELAPSED_FRAMES_THRESH (24 + 7 -1)
#define DTX_HANG_CONST 7                   /* yields eight frames of SP HANGOVER  */
#define INV_MED_THRESH 14564
#define ISF_GAP  128                       /* 50 */
#define ONE_MINUS_ISF_GAP 16384 - ISF_GAP
#define ISF_GAP   128
#define ISF_DITH_GAP   448
#define ISF_FACTOR_LOW 256
#define ISF_FACTOR_STEP 2
#define GAIN_THR 180
#define GAIN_FACTOR 75

#define M            16
//#ifndef __FIX_DEFINE_INT_
//#define __FIX_DEFINE_INT_
/*typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;*/
//#endif

typedef struct
{
    Word16 isf_hist[M * DTX_HIST_SIZE];
    Word16 log_en_hist[DTX_HIST_SIZE];
    Word16 hist_ptr;
    Word16 log_en_index;
    Word16 cng_seed;
    /* DTX handler stuff */
    Word16 dtxHangoverCount;
    Word16 decAnaElapsedCount;
    Word32 D[28];
    Word32 sumD[DTX_HIST_SIZE];
} dtx_encState;

#define SPEECH 0
#define DTX 1
#define DTX_MUTE 2

#define TX_SPEECH 0
#define TX_SID_FIRST 1
#define TX_SID_UPDATE 2
#define TX_NO_DATA 3

#define RX_SPEECH_GOOD 0
#define RX_SPEECH_PROBABLY_DEGRADED 1
#define RX_SPEECH_LOST 2
#define RX_SPEECH_BAD 3
#define RX_SID_FIRST 4
#define RX_SID_UPDATE 5
#define RX_SID_BAD 6
#define RX_NO_DATA 7

/*****************************************************************************
 *
 * DEFINITION OF DATA TYPES
 *****************************************************************************/

Word16 dtx_enc_init(dtx_encState ** st, Word16 isf_init[], VO_MEM_OPERATOR *pMemOP);
Word16 dtx_enc_reset(dtx_encState * st, Word16 isf_init[]);
void dtx_enc_exit(dtx_encState ** st, VO_MEM_OPERATOR *pMemOP);

Word16 dtx_enc(
     dtx_encState * st,                    /* i/o : State struct                                         */
     Word16 isf[M],                        /* o   : CN ISF vector                                        */
     Word16 * exc2,                        /* o   : CN excitation                                        */
     Word16 ** prms
);

Word16 dtx_buffer(
     dtx_encState * st,                    /* i/o : State struct                    */
     Word16 isf_new[],                     /* i   : isf vector                      */
     Word32 enr,                           /* i   : residual energy (in L_FRAME)    */
     Word16 codec_mode
);

void tx_dtx_handler(dtx_encState * st,     /* i/o : State struct           */
     Word16 vad_flag,                      /* i   : vad decision           */
     Word16 * usedMode                     /* i/o : mode changed or not    */
);

void Qisf_ns(
     Word16 * isf1,                        /* input : ISF in the frequency domain (0..0.5) */
     Word16 * isf_q,                       /* output: quantized ISF                        */
     Word16 * indice                       /* output: quantization indices                 */
);


void Disf_ns(
     Word16 * indice,                      /* input:  quantization indices                  */
     Word16 * isf_q                        /* input : ISF in the frequency domain (0..0.5)  */
);

typedef struct
{
	int16_t since_last_sid;
	int16_t true_sid_period_inv;
	int16_t log_en;
	int16_t old_log_en;
	int16_t level;
	int16_t isf[M];
	int16_t isf_old[M];
	int16_t cng_seed;

	int16_t isf_hist[M * DTX_HIST_SIZE];
	int16_t log_en_hist[DTX_HIST_SIZE];
	int16_t hist_ptr;

	int16_t dtxHangoverCount;
	int16_t decAnaElapsedCount;

	int16_t sid_frame;
	int16_t valid_data;
	int16_t dtxHangoverAdded;

	int16_t dtxGlobalState;                 /* contains previous state */
	/* updated in main decoder */

	int16_t data_updated;                   /* marker to know if CNI data is ever renewed */

	int16_t dither_seed;
	int16_t CN_dith;

} dtx_decState;

int16 dtx_dec_amr_wb_reset(dtx_decState * st, const int16 isf_init[]);

int16 rx_amr_wb_dtx_handler(dtx_decState * st, int16 frame_type);

int16 dtx_dec_amr_wb_reset(dtx_decState * st, const int16 isf_init[]);

int16 dtx_dec_amr_wb(
        dtx_decState * st,                    /* i/o : State struct                                          */
        int16 * exc2,                        /* o   : CN excitation                                          */
        int16 new_state,                     /* i   : New DTX state                                          */
        int16 isf[],                         /* o   : CN ISF vector                                          */
        int16 ** prms
    );

void dtx_dec_amr_wb_activity_update(
        dtx_decState * st,
        int16 isf[],
        int16 exc[]);

void CN_dithering(
        int16 isf[M],
        int32 * L_log_en_int,
        int16 * dither_seed
    );

#endif  //__DTX_H__

