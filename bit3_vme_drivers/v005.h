// ****************************************************************************
// *     Filename:  v005.h
// *  Description:  
// *   Created by:  Nam Tran (nam at kuno-g.phys.sci.osaka-u.ac.jp)
// *     Time tag:  2013-05-15 14:53:03
// ****************************************************************************
#ifndef V005_WKOBKZNO
#define V005_WKOBKZNO

#include <stdint.h>
#include "mvmestd.h"

#define V005_CH_0 0x0
#define V005_CH_1 0x2
#define V005_CH_2 0x4
#define V005_CH_3 0x6
#define V005_CH_4 0x8
#define V005_CH_5 0xa
#define V005_CH_6 0xc
#define V005_CH_7 0xe

#define V005_DATA_MASK 0x3fff
#define	V005_RESET_OK	 (v005_reset()==0x1)
#define	V005_RESET_FAILED	 (v005_reset()==0x0)

#define V005_DATA_SIZE	MVME_DMODE_D16		//16bit
#define	V005_AM				MVME_AM_A24_ND	//AM: 3D

int v005_clear(MVME_INTERFACE *vme, mvme_addr_t base);
int v005_readLAM(MVME_INTERFACE *vme, mvme_addr_t base, uint16_t ch);
uint32_t v005_adc(MVME_INTERFACE *vme, mvme_addr_t base, uint16_t ch);

#endif /* end of include guard: V005_WKOBKZNO */
