#ifndef L293D_H
#define L293D_H
#include <stdbool.h>
void l293d_init(void);
void l293d_drive(bool reverse);
void l293d_brake(void);
#endif
