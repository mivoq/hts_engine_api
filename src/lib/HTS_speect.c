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
/* DATE    : June 2012                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Helper functions used in Speect.                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef HTS_SPEECT_C
#define HTS_SPEECT_C

#ifdef __cplusplus
#define HTS_SPEECT_C_START extern "C" {
#define HTS_SPEECT_C_END   }
#else
#define HTS_SPEECT_C_START
#define HTS_SPEECT_C_END
#endif                          /* __CPLUSPLUS */

HTS_SPEECT_C_START;

/* hts_engine libraries */
#include "HTS_hidden.h"


/* HTS_Speect_ModelSet_get_nstate: get number of state */
int HTS_Speect_ModelSet_get_nstate(HTS_Engine * engine)
{
	return HTS_ModelSet_get_nstate(&(engine->ms));
}


/* HTS_Speect_SStreamSet_get_duration: get state duration */
int HTS_Speect_SStreamSet_get_duration(HTS_Engine * engine, int state_index)
{
	return HTS_SStreamSet_get_duration(&(engine->sss), state_index);
}


/* HTS_Speect_GStreamSet_get_total_nsample: get total number of sample */
int HTS_Speect_GStreamSet_get_total_nsample(HTS_Engine * engine)
{
	return HTS_GStreamSet_get_total_nsample(&(engine->gss));
}


/* HTS_Speect_GStreamSet_get_speech: get synthesized speech parameter */
short HTS_Speect_GStreamSet_get_speech(HTS_Engine * engine, int sample_index)
{
	return HTS_GStreamSet_get_speech(&(engine->gss), sample_index);
}


HTS_SPEECT_C_END;

#endif                          /* !HTS_SPEECT_C */
