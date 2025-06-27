
#include "FreeRTOS.h"
#include "task.h"
#include "task1.h"

float yaw_dps[ SHARED_MEMORY_SIZE ] __attribute__( ( aligned( SHARED_MEMORY_SIZE ) ) );


static void mpuinittask( void * pvParameters );

static void mpuinittask( void * pvParameters )
{
	/* Unused parameters. */
	( void ) pvParameters;

	for( ; ; )
	{
		/* This task has RW access to ucSharedMemory and therefore can write to
		 * it. */
		mpuint(NULL);

		/* Wait for a second. */
		vTaskDelete(NULL);
	}
}

static void mpureadtask( void * pvParameters );

static void mpureadtask( void * pvParameters )
{
	/* Unused parameters. */
	( void ) pvParameters;

	for( ; ; )
	{
		/* This task has RW access to ucSharedMemory and therefore can write to
		 * it. */
		mpuread(NULL);

		/* Wait for a second. */
		vTaskDelay( pdMS_TO_TICKS( 1000 ) );
	}
}

static void mpucontroltask( void * pvParameters );

static void mpucontroltask( void * pvParameters )
{
	/* Unused parameters. */
	( void ) pvParameters;

	for( ; ; )
	{
		/* This task has RW access to ucSharedMemory and therefore can write to
		 * it. */
		controlunit();

		/* Wait for a second. */
		vTaskDelay( pdMS_TO_TICKS( 100 ) );
	}
}

static void loggertask( void * pvParameters );
static void loggertask( void * pvParameters )
{
	/* Unused parameters. */
	( void ) pvParameters;

	for( ; ; )
	{
		/* This task has RW access to ucSharedMemory and therefore can write to
		 * it. */
		logger();

		/* Wait for a second. */
		vTaskDelay( pdMS_TO_TICKS( 100 ) );
	}
}
/*-----------------------------------------------------------*/

void vStartMPUDemo( void )
{
/**
 * Since stack of a task is protected using MPU, it must satisfy MPU
 * requirements as mentioned at the top of this file.
 *
 */
	static StackType_t mpuinitTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( configMINIMAL_STACK_SIZE * sizeof( StackType_t ) ) ) );

	TaskParameters_t mpuinitTaskParameters =
	{
		.pvTaskCode		= mpuinittask,
		.pcName			= "RW",
		.usStackDepth	= configMINIMAL_STACK_SIZE,
		.pvParameters	= NULL,
		.uxPriority		=(tskIDLE_PRIORITY|portPRIVILEGE_BIT),
		.puxStackBuffer	= mpuinitTaskStack,
		.xRegions		=	{
								{ 0,	0,	0},
								{ 0,				0,					0														},
								{ 0,				0,					0														},
							}
	};
static StackType_t mpureadTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( configMINIMAL_STACK_SIZE * sizeof( StackType_t ) ) ) );

TaskParameters_t mpureadTaskParameters =
{
	.pvTaskCode		= mpureadtask,
	.pcName			= "RW",
	.usStackDepth	= configMINIMAL_STACK_SIZE,
	.pvParameters	= NULL,
	.uxPriority		= (tskIDLE_PRIORITY|portPRIVILEGE_BIT),
	.puxStackBuffer	= mpureadTaskStack,
	.xRegions		=	{
							{ yaw_dps,	SHARED_MEMORY_SIZE,	portMPU_REGION_READ_WRITE| portMPU_REGION_EXECUTE_NEVER},
							{ 0,				0,					0														},
							{ 0,				0,					0														},
						}
};

static StackType_t mpucontrolTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( configMINIMAL_STACK_SIZE * sizeof( StackType_t ) ) ) );

TaskParameters_t mpucontrolTaskParameters =
{
	.pvTaskCode		= mpucontroltask,
	.pcName			= "RW",
	.usStackDepth	= configMINIMAL_STACK_SIZE,
	.pvParameters	= NULL,
	.uxPriority		= (tskIDLE_PRIORITY|portPRIVILEGE_BIT),
	.puxStackBuffer	= mpucontrolTaskStack,
	.xRegions		=	{
							{ yaw_dps,	SHARED_MEMORY_SIZE,	portMPU_REGION_READ_WRITE| portMPU_REGION_EXECUTE_NEVER},
							{ 0,				0,					0														},
							{ 0,				0,					0														},
						}
};
static StackType_t loggerTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( configMINIMAL_STACK_SIZE * sizeof( StackType_t ) ) ) );

TaskParameters_t loggerTaskParameters =
{
	.pvTaskCode		= loggertask,
	.pcName			= "RO",
	.usStackDepth	= configMINIMAL_STACK_SIZE,
	.pvParameters	= NULL,
	.uxPriority		= tskIDLE_PRIORITY,
	.puxStackBuffer	= loggerTaskStack,
	.xRegions		=	{
							{ yaw_dps,	SHARED_MEMORY_SIZE,	portMPU_REGION_READ_ONLY| portMPU_REGION_EXECUTE_NEVER},
							{ 0,				0,					0														},
							{ 0,				0,					0														},
						}
};


	/* Create an unprivileged task with RW access to ucSharedMemory. */
    xTaskCreateRestricted( &( mpuinitTaskParameters ), NULL );
	xTaskCreateRestricted( &( mpureadTaskParameters ), NULL );
	xTaskCreateRestricted( &( mpucontrolTaskParameters ), NULL );
	xTaskCreateRestricted( &( loggerTaskParameters ), NULL );
}
