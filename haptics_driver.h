#ifndef _HAPTICS_DRIVER_H_
#define _HAPTICS_DRIVER_H_
void haptics_init(void);
unsigned char haptics_test_cal_diags(void);
unsigned char haptics_test_run1(void);
unsigned char haptics_test_run2(void);
unsigned char haptics_test_run3(void);
unsigned char haptics_test_run4(void);
void haptics_test_halt(void);
#endif
