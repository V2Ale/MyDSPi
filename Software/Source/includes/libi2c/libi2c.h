#ifndef __LIBI2C_H__
#define __LIBI2C_H__

extern int i2c_fd ;				/* Device node exemple : /dev/i2c-1 */
extern unsigned long i2c_funcs;	/* Support flags */

f_return i2c_init(const char *node);
void i2c_close(void);
int scan_bus();

#endif
