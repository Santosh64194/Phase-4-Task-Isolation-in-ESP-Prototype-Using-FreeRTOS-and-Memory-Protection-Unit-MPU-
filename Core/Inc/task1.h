/*
 * task1.h
 *
 *  Created on: Jun 17, 2025
 *      Author: uma_r
 */

#ifndef INC_TASK1_H_
#define INC_TASK1_H_

#define DEVADDRESS 0x68
#define GYRO_CONFIG 27
#define PWR_MANG1 107

#define GYROZ 71
#define SHARED_MEMORY_SIZE 32


void mpuread(void *params);

void mpuint(void *params);

void controlunit(void );

void logger(void);

#endif /* INC_TASK1_H_ */
