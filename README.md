#Phase 4 – FreeRTOS-MPU Based Task Isolation

In this phase, we integrated the FreeRTOS kernel with Memory Protection Unit (MPU) support to enable task-level isolation using privileged and unprivileged execution modes. Unlike the bare-metal MPU implementation, FreeRTOS allows individual tasks to be assigned specific privilege levels and access rights through xTaskCreateRestricted().

The MPU supports a total of 8 regions, 4 of which are reserved by the FreeRTOS kernel. This left us with 3 configurable regions per task. We used this capability to define controlled access to a shared global variable as follows:

mpuread() – Privileged task with read-write access

controlunit() – Privileged task with read-only access

loggertask() – Unprivileged task with read-only access

This setup effectively enforced memory access boundaries. Any unauthorized access attempts—such as an unprivileged task trying to write to protected memory—correctly triggered the MemManage_Handler(), validating the MPU’s task isolation mechanism under FreeRTOS.
