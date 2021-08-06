/* Copyright Franco Bucafusco
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Implementation of pvPortMalloc() and vPortFree() that relies on TLSF
 * memory management algorithm .
 */

#include <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
all the API functions to use the MPU wrappers.  That should only be done when
task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "task.h"
#include "tlsf.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#if( configSUPPORT_DYNAMIC_ALLOCATION == 0 )
	#error This file must not be used if configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

/* Allocate the memory for the heap. */
static uint8_t ucHeap[ configTOTAL_HEAP_SIZE + sizeof(control_t) ];
static tlsf_t  tlsf_hnd;

void prvHeapInit();

/*-----------------------------------------------------------*/

void *pvPortMalloc( size_t xWantedSize )
{
	void *pvReturn;

	vTaskSuspendAll();
	{
		/* If this is the first call to malloc  */

        control_t* control = (control_t*) ucHeap;

		if( control->block_null.next_free == NULL )
		{
			prvHeapInit();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}


		pvReturn = tlsf_malloc( tlsf_hnd , xWantedSize );
		traceMALLOC( pvReturn, xWantedSize );
	}

	( void ) xTaskResumeAll();

	#if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if( pvReturn == NULL )
		{
			extern void vApplicationMallocFailedHook( void );
			vApplicationMallocFailedHook();
		}
	}
	#endif

	return pvReturn;
}

/*-----------------------------------------------------------*/
void vPortFree( void *pv )
{
	if( pv )
	{
		vTaskSuspendAll();
		{
			tlsf_free( tlsf_hnd, pv );
			traceFREE( pv, 0 );
		}

		( void ) xTaskResumeAll();
	}
}

/*-----------------------------------------------------------*/
void prvHeapInit()
{
  tlsf_hnd = tlsf_create_with_pool( ucHeap , configTOTAL_HEAP_SIZE + sizeof(control_t) );
}

