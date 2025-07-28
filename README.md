## Phase 4 – FreeRTOS-MPU Based Task Isolation

In this phase, we integrated the FreeRTOS kernel with Memory Protection Unit (MPU) support to enable task-level isolation using **privileged** and **unprivileged** execution modes. Unlike bare-metal MPU integration, FreeRTOS allows individual tasks to be configured with specific access rights using `xTaskCreateRestricted()`.

The ARM Cortex-M MPU supports **8 regions**, out of which **4 are reserved by the FreeRTOS kernel**. This leaves **3 configurable regions per task**, which we used to control access to a shared global variable:

- `mpuread()` → **Privileged** task with **read-write** access  
- `controlunit()` → **Privileged** task with **read-only** access  
- `loggertask()` → **Unprivileged** task with **read-only** access

This configuration effectively enforced memory protection boundaries. Attempts to perform unauthorized memory operations (e.g., an unprivileged task trying to write to a restricted region) correctly triggered the `MemManage_Handler()`, validating robust task-level isolation through the MPU-enabled FreeRTOS kernel.
****
