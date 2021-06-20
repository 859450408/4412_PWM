#ifndef FS4412_BEE_H
#define FS4412_BEE_H

#define BEE_MAGIC 'R'
/*
 * need arg = 1/2 
 */

#define BEE_ON	_IOW(BEE_MAGIC, 0, int)
#define BEE_OFF	_IOW(BEE_MAGIC, 1, int)

#endif
