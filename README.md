# Phase 4 – FreeRTOS-MPU Based Task Isolation
[MEMORY PROTECTION UNIT - Week3,4 (1).pdf](https://github.com/user-attachments/files/21466157/MEMORY.PROTECTION.UNIT.-.Week3.4.1.pdf)


The MPU SUPPORT offered by FreeRTOS was different compared to the baremetal MPU integration. 
Here the individual tasks could be assigned Privileged and Unprivileged status. 
A total of 8 MPU regions can be configured out of which 4 are configured by the Kernel itself which allowed us to allocate 3 regions for each task. 
We again allocated the region containing the global variable as a privileged read write region for the mpuread() task - privileged task, a privileged read only region for the controlunit() task - privileged task and a read only region for the loggertask() - unprivileged task


In this phase, we integrated the MPU-enabled FreeRTOS kernel to assign privileged and unprivileged modes to individual tasks. Using xTaskCreateRestricted(), each task was configured with custom memory access:

mpuread() → Privileged, with read-write access to shared memory

controlunit() → Privileged, read-write access

loggertask() → Privileged, read-only access

Attempts to violate access rules (e.g., unprivileged write) successfully triggered the MemManage_Handler(), demonstrating robust task-level memory protection.
