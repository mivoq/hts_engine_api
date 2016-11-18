/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                                                                                  */
/* Permission is hereby granted, free of charge, to any person obtaining a copy     */
/* of this software and associated documentation files (the "Software"), to deal    */
/* in the Software without restriction, including without limitation the rights     */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        */
/* copies of the Software, and to permit persons to whom the Software is            */
/* furnished to do so, subject to the following conditions:                         */
/* The above copyright notice and this permission notice shall be included in       */
/* all copies or substantial portions of the Software.                              */
/*                                                                                  */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        */
/* THE SOFTWARE.                                                                    */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : 14 May 2012                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Added mixed excitation, see code at end of file.                                 */
/* function HTS_GStreamSet_create_me, used by HTS_Engine_create_gstream_me in       */
/* HTS_engine.c                                                                     */
/*                                                                                  */
/************************************************************************************/


/* ----------------------------------------------------------------- */
/*           The HMM-Based Speech Synthesis Engine "hts_engine API"  */
/*           developed by HTS Working Group                          */
/*           http://hts-engine.sourceforge.net/                      */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2001-2011  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/*                2001-2008  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the HTS working group nor the names of its  */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#ifndef HTS_GSTREAM_C
#define HTS_GSTREAM_C

#ifdef __cplusplus
#define HTS_GSTREAM_C_START extern "C" {
#define HTS_GSTREAM_C_END   }
#else
#define HTS_GSTREAM_C_START
#define HTS_GSTREAM_C_END
#endif                          /* __CPLUSPLUS */

HTS_GSTREAM_C_START;

/* hts_engine libraries */
#include "HTS_hidden.h"

/* HTS_GStreamSet_initialize: initialize generated parameter stream set */
void HTS_GStreamSet_initialize(HTS_GStreamSet * gss)
{
	gss->nstream = 0;
	gss->total_frame = 0;
	gss->total_nsample = 0;
	gss->gstream = NULL;
	gss->gspeech = NULL;
}

/* HTS_GStreamSet_create: generate speech */
/* (stream[0] == spectrum && stream[1] == lf0) */
void HTS_GStreamSet_create(HTS_GStreamSet * gss, HTS_PStreamSet * pss,
                           int stage, HTS_Boolean use_log_gain,
                           int sampling_rate, int fperiod, double alpha,
                           double beta,
                           HTS_Boolean * stop, double volume, HTS_Audio * audio)
{
	int i, j, k;
	int msd_frame;
	HTS_Vocoder v;
	int nlpf = 0;
	double *lpf = NULL;

	/* check */
	if (gss->gstream || gss->gspeech)
		HTS_error(1,
				  "HTS_GStreamSet_create: HTS_GStreamSet is not initialized.\n");

	/* initialize */
	gss->nstream = HTS_PStreamSet_get_nstream(pss);
	gss->total_frame = HTS_PStreamSet_get_total_frame(pss);
	gss->total_nsample = fperiod * gss->total_frame;
	gss->gstream = (HTS_GStream *) HTS_calloc(gss->nstream, sizeof(HTS_GStream));
	for (i = 0; i < gss->nstream; i++) {
		gss->gstream[i].static_length = HTS_PStreamSet_get_static_length(pss, i);
		gss->gstream[i].par =
			(double **) HTS_calloc(gss->total_frame, sizeof(double *));
		for (j = 0; j < gss->total_frame; j++)
			gss->gstream[i].par[j] =
				(double *) HTS_calloc(gss->gstream[i].static_length,
									  sizeof(double));
	}
	gss->gspeech = (short *) HTS_calloc(gss->total_nsample, sizeof(short));

	/* copy generated parameter */
	for (i = 0; i < gss->nstream; i++) {
		if (HTS_PStreamSet_is_msd(pss, i)) {      /* for MSD */
			for (j = 0, msd_frame = 0; j < gss->total_frame; j++)
				if (HTS_PStreamSet_get_msd_flag(pss, i, j)) {
					for (k = 0; k < gss->gstream[i].static_length; k++)
						gss->gstream[i].par[j][k] =
							HTS_PStreamSet_get_parameter(pss, i, msd_frame, k);
					msd_frame++;
				} else
					for (k = 0; k < gss->gstream[i].static_length; k++)
						gss->gstream[i].par[j][k] = LZERO;
		} else {                  /* for non MSD */
			for (j = 0; j < gss->total_frame; j++)
				for (k = 0; k < gss->gstream[i].static_length; k++)
					gss->gstream[i].par[j][k] =
						HTS_PStreamSet_get_parameter(pss, i, j, k);
		}
	}

	/* check */
	if (gss->nstream != 2 && gss->nstream != 3)
		HTS_error(1,
				  "HTS_GStreamSet_create: The number of streams should be 2 or 3.\n");
	if (HTS_PStreamSet_get_static_length(pss, 1) != 1)
		HTS_error(1,
				  "HTS_GStreamSet_create: The size of lf0 static vector should be 1.\n");
	if (gss->nstream >= 3 && gss->gstream[2].static_length % 2 == 0)
		HTS_error(1,
				  "HTS_GStreamSet_create: The number of low-pass filter coefficient should be odd numbers.");

	/* synthesize speech waveform */
	HTS_Vocoder_initialize(&v, gss->gstream[0].static_length - 1, stage,
						   use_log_gain, sampling_rate, fperiod);
	if (gss->nstream >= 3)
		nlpf = (gss->gstream[2].static_length - 1) / 2;
	for (i = 0; i < gss->total_frame && (*stop) == FALSE; i++) {
		if (gss->nstream >= 3)
			lpf = &gss->gstream[2].par[i][0];
		HTS_Vocoder_synthesize(&v, gss->gstream[0].static_length - 1,
							   gss->gstream[1].par[i][0],
							   &gss->gstream[0].par[i][0], nlpf, lpf, alpha, beta,
							   volume, &gss->gspeech[i * fperiod], audio);
	}
	HTS_Vocoder_clear(&v);
	if (audio)
		HTS_Audio_flush(audio);
}

/* HTS_GStreamSet_get_total_nsample: get total number of sample */
int HTS_GStreamSet_get_total_nsample(HTS_GStreamSet * gss)
{
	return gss->total_nsample;
}

/* HTS_GStreamSet_get_total_frame: get total number of frame */
int HTS_GStreamSet_get_total_frame(HTS_GStreamSet * gss)
{
	return gss->total_frame;
}

/* HTS_GStreamSet_get_static_length: get static features length */
int HTS_GStreamSet_get_static_length(HTS_GStreamSet * gss, int stream_index)
{
	return gss->gstream[stream_index].static_length;
}

/* HTS_GStreamSet_get_speech: get synthesized speech parameter */
short HTS_GStreamSet_get_speech(HTS_GStreamSet * gss, int sample_index)
{
	return gss->gspeech[sample_index];
}

/* HTS_GStreamSet_get_parameter: get generated parameter */
double HTS_GStreamSet_get_parameter(HTS_GStreamSet * gss, int stream_index,
                                    int frame_index, int vector_index)
{
	return gss->gstream[stream_index].par[frame_index][vector_index];
}

/* HTS_GStreamSet_clear: free generated parameter stream set */
void HTS_GStreamSet_clear(HTS_GStreamSet * gss)
{
	int i, j;

	if (gss->gstream) {
		for (i = 0; i < gss->nstream; i++) {
			for (j = 0; j < gss->total_frame; j++)
				HTS_free(gss->gstream[i].par[j]);
			HTS_free(gss->gstream[i].par);
		}
		HTS_free(gss->gstream);
	}
	if (gss->gspeech)
		HTS_free(gss->gspeech);
	HTS_GStreamSet_initialize(gss);
}


/************************************************************************************/
/*                                                                                  */
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : 14 May 2012                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Mixed excitation                                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 * From SPTK 3.4.1 (New and Simplified BSD license)
 * Standard Form Digital Filter
 * latice digital filter
 */
static double dfs_me(double x, double *a, int m, double *b, int n, double *buf, int *bufp)
{
	double y = 0.0;
	int i, p;
	int max;

	n++;
	m++;

	(m < n) ? (max = n) : (max = m);

	x = x * a[0];
	for (i = 1; i < m; i++) {
		if ((p = *bufp + i) >= max)
			p -= max;
		x -= buf[p] * a[i];
	}
	buf[*bufp] = x;
	for (i = 0; i < n; i++) {
		if ((p = *bufp + i) >= max)
			p -= max;
		y += buf[p] * b[i];
	}

	if (--*bufp < 0)
		*bufp += max;

	return (y);
}


/* Default Values */
#define PD_FILTER_BUFF_SIZE 520 /* for max 64 kHz */


/* HTS_GStreamSet_create_me: generate speech (mixed excitation)
 * stream[0] == spectrum
 * stream[1] == lf0
 * stream[2] == band strengths
 */
void HTS_GStreamSet_create_me(HTS_GStreamSet * gss, HTS_PStreamSet * pss,
							  int stage, HTS_Boolean use_log_gain,
							  int sampling_rate, int fperiod, double alpha,
							  double beta,
							  HTS_Boolean * stop, double volume, HTS_Audio * audio,
							  int me_num_filters, int me_filter_order, double **me_filter,
							  double *xp_sig, double *xn_sig,
							  double *hp, double *hn,
							  double *pd_filter, int pd_filter_order)
{
	int i, j, k;
	int msd_frame;
	HTS_Vocoder v;
	HTS_Vocoder_ME v_me;
	int nlpf = 0;
	double *lpf = NULL;


	/* check */
	if (gss->gstream || gss->gspeech)
		HTS_error(1,
				  "HTS_GStreamSet_create_me: HTS_GStreamSet is not initialized.\n");

	/* ABY: hts engine structure in mixed excitation structure */
	v_me.v = &v;

	/* initialize */
	gss->nstream = HTS_PStreamSet_get_nstream(pss);
	gss->total_frame = HTS_PStreamSet_get_total_frame(pss);
	gss->total_nsample = fperiod * gss->total_frame;
	gss->gstream = (HTS_GStream *) HTS_calloc(gss->nstream, sizeof(HTS_GStream));
	for (i = 0; i < gss->nstream; i++)
	{
		gss->gstream[i].static_length = HTS_PStreamSet_get_static_length(pss, i);
		gss->gstream[i].par =
			(double **) HTS_calloc(gss->total_frame, sizeof(double *));
		for (j = 0; j < gss->total_frame; j++)
			gss->gstream[i].par[j] =
				(double *) HTS_calloc(gss->gstream[i].static_length,
									  sizeof(double));
	}

	gss->gspeech = (short *) HTS_calloc(gss->total_nsample, sizeof(short));

	/* copy generated parameter */
	for (i = 0; i < gss->nstream; i++)
	{
		if (HTS_PStreamSet_is_msd(pss, i))  /* for MSD */
		{
			for (j = 0, msd_frame = 0; j < gss->total_frame; j++)
				if (HTS_PStreamSet_get_msd_flag(pss, i, j))
				{
					for (k = 0; k < gss->gstream[i].static_length; k++)
						gss->gstream[i].par[j][k] =
							HTS_PStreamSet_get_parameter(pss, i, msd_frame, k);
					msd_frame++;
				}
				else
					for (k = 0; k < gss->gstream[i].static_length; k++)
						gss->gstream[i].par[j][k] = LZERO;
		}
		else /* for non MSD */
		{
			for (j = 0; j < gss->total_frame; j++)
				for (k = 0; k < gss->gstream[i].static_length; k++)
					gss->gstream[i].par[j][k] =
						HTS_PStreamSet_get_parameter(pss, i, j, k);
		}
	}

	/* check */
	if (gss->nstream != 2 && gss->nstream != 3)
		HTS_error(1,
				  "HTS_GStreamSet_create: The number of streams should be 2 or 3.\n");
	if (HTS_PStreamSet_get_static_length(pss, 1) != 1)
		HTS_error(1,
				  "HTS_GStreamSet_create: The size of lf0 static vector should be 1.\n");

	/* ABY: we don't use low-pass filters, stream 2 is used for
	 * band strengths.
	 */
#if 0
	if (gss->nstream >= 3 && gss->gstream[2].static_length % 2 == 0)
		HTS_error(1,
				  "HTS_GStreamSet_create: The number of low-pass filter coefficient should be odd numbers.");
#endif

	/* synthesize speech waveform */
	HTS_Vocoder_initialize_me(&v_me, gss->gstream[0].static_length - 1, stage,
							  use_log_gain, sampling_rate, fperiod,
							  me_num_filters, me_filter_order, me_filter, xp_sig, xn_sig,
							  hp, hn);
	if (gss->nstream >= 3)
		nlpf = (gss->gstream[2].static_length - 1) / 2;

	/* ABY: synthesis */
	for (i = 0; i < gss->total_frame && (*stop) == FALSE; i++)
	{
		/* ABY: we don't use low-pass filters, this stream is used for
		 * band strengths.
		 */
#if 0
		if (gss->nstream >= 3)
			lpf = &gss->gstream[2].par[i][0];
#endif
		HTS_Vocoder_synthesize_me(&v_me, gss->gstream[0].static_length - 1,
								  gss->gstream[1].par[i][0],  /* log f0 */
								  &gss->gstream[0].par[i][0], /* spectrum */
								  &gss->gstream[2].par[i][0], /* band strengths */
								  alpha, beta, volume, &gss->gspeech[i * fperiod], audio);
	}


	/* pulse dispersion filter */
	if (pd_filter != NULL)
	{
		double a = 1.0;
		double d[PD_FILTER_BUFF_SIZE];
		int bufp = 0;
		int counter;

		for (counter = 0; counter < PD_FILTER_BUFF_SIZE; counter++)
			d[counter] = 0.0;

		pd_filter_order = pd_filter_order - 1; /* 1 added in dfs_me */

		for (i = 0; i < gss->total_nsample; i++)
		{
			/* na = 0 */
			gss->gspeech[i] = (short)dfs_me((double)gss->gspeech[i], &a, 0, pd_filter,
											pd_filter_order, d, &bufp);
		}
	}


	HTS_Vocoder_clear_me(&v_me);
	if (audio)
		HTS_Audio_flush(audio);
}


/************************************************************************************/
/*                                                                                  */
/* Mixed excitation end                                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


HTS_GSTREAM_C_END;

#endif                          /* !HTS_GSTREAM_C */
