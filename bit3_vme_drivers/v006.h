// ****************************************************************************
// *     Filename:  v006.h
// *  Description:  
// *   Created by:  Nam Tran (nam at kuno-g.phys.sci.osaka-u.ac.jp)
// *     Time tag:  2013-05-15 14:46:03
// ****************************************************************************
#ifndef V006_693FE8CW
#define V006_693FE8CW
#include <stdint.h>
#include "mvmestd.h"

#define V006_CH_0 0x0
#define V006_CH_1 0x2
#define V006_CH_2 0x4
#define V006_CH_3 0x6
#define V006_CH_4 0x8
#define V006_CH_5 0xa
#define V006_CH_6 0xc
#define V006_CH_7 0xe

#define V006_DATA_MASK 0x3fff
#define	V006_RESET_OK	 (v006_reset()==0x1)
#define	V006_RESET_FAILED	 (v006_reset()==0x0)

#define V006_DATA_SIZE	MVME_DMODE_D16		//16bit
#define	V006_AM				MVME_AM_A24_ND	//AM: 3D

int v006_clear(MVME_INTERFACE *vme, mvme_addr_t base);
int v006_readLAM(MVME_INTERFACE *vme, mvme_addr_t base, uint16_t ch);
uint32_t v006_adc(MVME_INTERFACE *vme, mvme_addr_t base, uint16_t ch);
void read_V006(char *pevent);

#endif /* end of include guard: V006_693FE8CW */
