// ****************************************************************************
// *     Filename:  v1290.c
// *  Description:  
// *   Created by:  Nam Tran (nam at kuno-g.phys.sci.osaka-u.ac.jp)
// *     Time tag:  2013-04-07 11:05:36
// ****************************************************************************
#include <stdio.h>
#include <string.h>
#include "bt617.h"
#include "v1290.h"

uint16_t v1290_Read16(MVME_INTERFACE *mvme, uint32_t base, int offset)
{
  int   cmode;
  mvme_get_dmode(mvme, &cmode);
	mvme_set_am(mvme,  V1290_AM);
	mvme_set_dmode(mvme, V1290_REG_DW);
  uint16_t data = mvme_read_value(mvme, base+offset);
	mvme_set_dmode(mvme, cmode);

	return data;
}

uint32_t v1290_Read32(MVME_INTERFACE *mvme, uint32_t base, int offset)
{
  int   cmode;
  mvme_get_dmode(mvme, &cmode);
	mvme_set_am(mvme,  V1290_AM);
	mvme_set_dmode(mvme, V1290_BUFF_DW);
  uint32_t data = mvme_read_value(mvme, base+offset);

	mvme_set_dmode(mvme, cmode);
	return data;
}

void v1290_Write16(MVME_INTERFACE *mvme, uint32_t base, int offset, uint16_t value)
{
  int   cmode;
  mvme_get_dmode(mvme, &cmode);
	mvme_set_am(mvme,  V1290_AM);
	mvme_set_dmode(mvme, V1290_REG_DW);
  mvme_write_value(mvme, base+offset, value);
	mvme_set_dmode(mvme, cmode);
}

/*****************************************************************/
int v1290_EventRead(MVME_INTERFACE *mvme, uint32_t base, 
		uint32_t *pdest, int *nentry)
{
  uint32_t hdata;
	mvme_set_am(mvme,  V1290_AM);
	mvme_set_dmode(mvme, V1290_BUFF_DW);

  *nentry = 0;
  if (v1290_IsDataReady(mvme, base)) {
    do {
      hdata = mvme_read_value(mvme, base);
    } while (!(hdata & 0x40000000));
    pdest[*nentry] = hdata;
    *nentry += 1;

    do {
      pdest[*nentry] = mvme_read_value(mvme, base);
      *nentry += 1;
    } while (!(pdest[*nentry-1] & 0xc0000000));
  }

  return *nentry;
}

int v1290_DataRead(MVME_INTERFACE *mvme, uint32_t base, uint32_t *pdest, int nentry)
{
	int status;
	mvme_set_am(mvme,  V1290_AM);
	mvme_set_dmode(mvme, V1290_BUFF_DW);
	mvme_set_blt(mvme, MVME_BLT_BLT32);
	status = mvme_read(mvme, pdest, base, sizeof(uint32_t) * nentry);
	mvme_set_blt(mvme, MVME_BLT_NONE);
	return nentry;
    /*
      for (i=0 ; i<nentry ; i++) {
      if (!v1190_DataReady(mvme, base))
      break;
      pdest[i] = mvme_read_value(mvme, base);
      }
      mvme_set_dmode(mvme, cmode);
      return i;
    */
}

/*****************************************************************/
int v1290_MicroWrite(MVME_INTERFACE *mvme, uint32_t base, uint16_t data)
{
  int i;

	mvme_set_am(mvme,  V1290_AM);
	mvme_set_dmode(mvme, V1290_REG_DW);
  for (i=0; i<1000; i++)
  {
    uint16_t microHS = mvme_read_value(mvme, base+V1290_MICRO_HND);
    if (microHS & V1290_MICRO_WR_OK) {
      mvme_write_value(mvme, base+V1290_MICRO, data);
      return 1;
    }
    usleep(1);
  }
  printf("v1290_MicroWrite: Micro not ready for writing!\n");
  return -1;
}

int v1290_MicroRead(MVME_INTERFACE *mvme, const uint32_t base)
{
  int i;
  int reg=-1;
	mvme_set_am(mvme,  V1290_AM);
	mvme_set_dmode(mvme, V1290_REG_DW);

  for (i=100; i>0; i--) {
    uint16_t  microHS = mvme_read_value(mvme, base+V1290_MICRO_HND);
    if (microHS & V1290_MICRO_RD_OK) {
      reg = mvme_read_value(mvme, base+V1290_MICRO);
//      printf("i:%d microHS:%d %x\n", i, microHS, reg);
      return (reg);
    }
    usleep(1);
  };
  return -1;
}

uint16_t v1290_ReadMicro(MVME_INTERFACE *mvme, const uint32_t base, 
		uint16_t opcode)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
  value = v1290_MicroWrite(mvme, base, opcode);
  value = v1290_MicroRead(mvme, base);
  mvme_set_dmode(mvme, cmode);
	return (uint16_t) value;
}

void v1290_WriteMicro(MVME_INTERFACE *mvme, uint32_t base, 
		uint16_t opcode, uint16_t data)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
  value = v1290_MicroWrite(mvme, base, opcode);
  value = v1290_MicroWrite(mvme, base, data);
  mvme_set_dmode(mvme, cmode);
}
/*****************************************************************/

void v1290_TriggerMatchingSet(MVME_INTERFACE *mvme, uint32_t base)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
  value = v1290_MicroWrite(mvme, base, V1290_TRG_MATCH_OPCODE);
  mvme_set_dmode(mvme, cmode);
}

void v1290_ContinuosSet(MVME_INTERFACE *mvme, uint32_t base)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
  value = v1290_MicroWrite(mvme, base, V1290_CONT_STORE_OPCODE);
  mvme_set_dmode(mvme, cmode);
}

int v1290_AcqModeRead(MVME_INTERFACE *mvme, uint32_t base)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
  value = v1290_MicroWrite(mvme, base, V1290_READ_ACQ_MOD_OPCODE);
  value = v1290_MicroRead(mvme, base);
  if ((value & 0x1) == 1)
  {
    printf("AcqMode: trigger matching.\n");
  }
  else if ((value & 0x1) == 0)
  {
    printf("AcqMode: continuous.\n");
  }
  mvme_set_dmode(mvme, cmode);
	return (value & 0x1);
}

/*int v1290_TriggerConfRead(MVME_INTERFACE *mvme, uint32_t base, uint16_t *conf)*/
int v1290_TriggerConfRead(MVME_INTERFACE *mvme, uint32_t base)
{
  int   cmode, value;
  uint16_t width;
  int16_t offset;
  uint16_t extra_width;
  uint16_t reject_margin;
  uint16_t subtraction;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
  value = v1290_MicroWrite(mvme, base, V1290_READ_TRG_CONF_OPCODE);

  width = v1290_MicroRead(mvme, base);
  offset = (int16_t)v1290_MicroRead(mvme, base);
  extra_width = v1290_MicroRead(mvme, base);
  reject_margin = v1290_MicroRead(mvme, base);
  subtraction = v1290_MicroRead(mvme, base);

  printf("Trigger settings:\n");
  printf("- match window width: %d ns\n", width*25);

  printf("- match window offset: %d ns\n", offset*25);

  printf("- extra search window width: %d ns\n", extra_width*25);
  printf("- reject margin: %d ns\n", reject_margin*25);
  printf("- trigger time subtraction: %d\n", subtraction>>15);

  mvme_set_dmode(mvme, cmode);
	return 0;
}

void v1290_SetWindowWidth(MVME_INTERFACE *mvme, uint32_t base, uint32_t width)
{
	v1290_WriteMicro(mvme, base, V1290_SET_WIN_WIDTH_OPCODE, (int16_t)width/25);
}

void v1290_SetWindowOffset(MVME_INTERFACE *mvme, uint32_t base, int32_t offset)
{
  if (offset < -2048*25)
    offset = -2048*25;
  if (offset > +40*25)
    offset = 40*25;

	v1290_WriteMicro(mvme, base, V1290_SET_WIN_OFFSET_OPCODE, (int16_t)offset/25);
}

void v1290_SetExtraMargin(MVME_INTERFACE *mvme, uint32_t base, uint32_t margin)
{
	v1290_WriteMicro(mvme, base, V1290_SET_SW_MARGIN_OPCODE, (uint16_t)margin/25);
}

void v1290_SetRejectMargin(MVME_INTERFACE *mvme, uint32_t base, uint32_t margin)
{
	v1290_WriteMicro(mvme, base, V1290_SET_REJ_MARGIN_OPCODE, (uint16_t) margin/25);
}

void v1290_EnableTriggerSubtraction(MVME_INTERFACE *mvme, uint32_t base, bool en)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
	uint32_t opcode;
	if (en)
		opcode = V1290_EN_SUB_TRG_OPCODE;
	else
		opcode = V1290_DIS_SUB_TRG_OPCODE;

  value = v1290_MicroWrite(mvme, base, opcode);
  mvme_set_dmode(mvme, cmode);
}

// TDC edge and resolution
void v1290_SetEdgeDetection(MVME_INTERFACE *mvme, uint32_t base, uint16_t edge)
{
	v1290_WriteMicro(mvme, base, V1290_SET_DETECTION_OPCODE, edge);
}

void v1290_SetEdgeResolution(MVME_INTERFACE *mvme, uint32_t base, uint16_t res)
{
	v1290_WriteMicro(mvme, base, V1290_SET_TR_LEAD_LSB_OPCODE, res);
}

uint16_t v1290_ReadEdgeResolution(MVME_INTERFACE *mvme, uint32_t base)
{
	uint16_t ret = v1290_ReadMicro(mvme, base, V1290_READ_RES_OPCODE);
  switch (ret)
  {
    case 0:
      printf("Resolution: 800 ps\n");
      break;
    case 1:
      printf("Resolution: 200 ps\n");
      break;
    case 2:
      printf("Resolution: 100 ps\n");
      break;
    case 3:
      printf("Resolution: 25 ps\n");
      break;
    default:
      printf("Invalid setting!\n");
      ret = 100;
      break;
  }
  return ret;
}

uint16_t v1290_ReadEdgeDetection(MVME_INTERFACE *mvme, uint32_t base)
{
	uint16_t ret = v1290_ReadMicro(mvme, base, V1290_READ_DETECTION_OPCODE);
  switch (ret)
  {
    case 0:
      printf("Edge detection: pair\n");
      break;
    case 1:
      printf("Edge detection: trailing\n");
      break;
    case 2:
      printf("Edge detection: leading\n");
      break;
    case 3:
      printf("Edge detection: trailing and leading\n");
      break;
    default:
      printf("Invalid setting!\n");
      ret = 100;
      break;
  }
  return ret;
}

void v1290_SetDeadtime(MVME_INTERFACE *mvme, uint32_t base, uint16_t deadtime)
{
	v1290_WriteMicro(mvme, base, V1290_SET_DEAD_TIME_OPCODE, deadtime);
}

uint16_t v1290_ReadDeadtime(MVME_INTERFACE *mvme, uint32_t base)
{
	uint16_t ret = v1290_ReadMicro(mvme, base, V1290_READ_DEAD_TIME_OPCODE);
  switch (ret)
  {
    case 0:
      printf("Channel deadtime: 5 ns\n");
      break;
    case 1:
      printf("Channel deadtime: 10 ns\n");
      break;
    case 2:
      printf("Channel deadtime: 30 ns\n");
      break;
    case 3:
      printf("Channel deadtime: 100 ns\n");
      break;
    default:
      printf("Invalid setting!\n");
      ret = 100;
      break;
  }

  return ret;
}

//TDC read out
void v1290_EnableHeader(MVME_INTERFACE *mvme, uint32_t base, bool en)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
	uint32_t opcode;
	if (en)
		opcode = V1290_EN_HEAD_TRAILER_OPCODE;
	else
		opcode = V1290_DIS_HEAD_TRAILER_OPCODE;

  value = v1290_MicroWrite(mvme, base, opcode);
  mvme_set_dmode(mvme, cmode);
}

bool v1290_HeaderIsEnabled(MVME_INTERFACE *mvme, uint32_t base)
{
	return (v1290_ReadMicro(mvme, base, V1290_READ_HEAD_TRAILER_OPCODE) & 0x1);
}

void v1290_SetMaxHits(MVME_INTERFACE *mvme, uint32_t base, uint16_t max)
{
	v1290_WriteMicro(mvme, base, V1290_SET_EVENT_SIZE_OPCODE,max);
}

uint16_t v1290_ReadMaxHits(MVME_INTERFACE *mvme, uint32_t base)
{
	return (v1290_ReadMicro(mvme, base, V1290_READ_EVENT_SIZE_OPCODE) &0xf);
}

void v1290_EnableTDCErrorMark(MVME_INTERFACE *mvme, uint32_t base, bool en)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
	uint32_t opcode;
	if (en)
		opcode = V1290_EN_ERROR_MARK_OPCODE;
	else
		opcode = V1290_DIS_ERROR_MARK_OPCODE;

  value = v1290_MicroWrite(mvme, base, opcode);
  mvme_set_dmode(mvme, cmode);
}

void v1290_EnableBypassTDC(MVME_INTERFACE *mvme, uint32_t base, bool en)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
	uint32_t opcode;
	if (en)
		opcode = V1290_EN_ERROR_BYPASS_OPCODE;
	else
		opcode = V1290_DIS_ERROR_BYPASS_OPCODE;

  value = v1290_MicroWrite(mvme, base, opcode);
  mvme_set_dmode(mvme, cmode);
}

void v1290_SetFIFOSize(MVME_INTERFACE *mvme, uint32_t base, uint16_t size)
{
	v1290_WriteMicro(mvme, base, V1290_SET_FIFO_SIZE_OPCODE,size);
}

uint16_t v1290_ReadFIFOSize(MVME_INTERFACE *mvme, uint32_t base)
{
	return (v1290_ReadMicro(mvme, base, V1290_READ_FIFO_SIZE_OPCODE) &0x7);
}
// Channel Enable
void v1290_EnableChannel(MVME_INTERFACE *mvme, uint32_t base,uint16_t chn,bool en)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
	uint32_t opcode;
	if (en)
		opcode = V1290_EN_CHANNEL_OPCODE;
	else
		opcode = V1290_DIS_CHANNEL_OPCODE;

	en += chn;
  value = v1290_MicroWrite(mvme, base, opcode);
  mvme_set_dmode(mvme, cmode);
}

void v1290_EnableAllChannels(MVME_INTERFACE *mvme, uint32_t base)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
  value = v1290_MicroWrite(mvme, base, V1290_EN_ALL_CH_OPCODE);
  mvme_set_dmode(mvme, cmode);
}

void v1290_DisableAllChannels(MVME_INTERFACE *mvme, uint32_t base)
{
  int   cmode, value;
  mvme_get_dmode(mvme, &cmode);
  mvme_set_dmode(mvme, MVME_DMODE_D16);
  value = v1290_MicroWrite(mvme, base, V1290_DIS_ALL_CH_OPCODE);
	printf("value: %d\n", value);
  mvme_set_dmode(mvme, cmode);
}

void v1290_WriteEnablePattern(MVME_INTERFACE *mvme,uint32_t base,uint16_t pattern)
{
	v1290_WriteMicro(mvme, base, V1290_WRITE_EN_PATTERN_OPCODE, pattern);
}

uint16_t v1290_ReadEnablePattern(MVME_INTERFACE *mvme,uint32_t base)
{
	return (v1290_ReadMicro(mvme,base,V1290_READ_EN_PATTERN_OPCODE));
}

// Regiters
void v1290_ReadControlRegister(MVME_INTERFACE *mvme, uint32_t base)
{
	uint16_t reg = v1290_Read16(mvme, base, V1290_CONTROL);
	printf("Control register: 0x%X\n", V1290_CONTROL);
	printf(" - Bus error enable: %d\n", reg & 0x1);
	printf(" - Empty event enable: %d\n", (reg & (0x1<<3))>>3);
	printf(" - Compensation enable: %d\n", (reg & (0x1<<5))>>5);
	printf(" - Event FIFO enable: %d\n", (reg & (0x1<<8))>>8);
	/*showbits(reg);*/
}

void v1290_EnableBusError(MVME_INTERFACE *mvme, uint32_t base, bool en)
{
	uint16_t reg = v1290_Read16(mvme, base, V1290_CONTROL);
	if (en)
		v1290_Write16(mvme, base, V1290_CONTROL, reg + 0x1);
	else
		v1290_Write16(mvme, base, V1290_CONTROL, reg & 0xfffe);
}

void v1290_EnableEmptyEvent(MVME_INTERFACE *mvme, uint32_t base, bool en)
{
	uint16_t reg = v1290_Read16(mvme, base, V1290_CONTROL);
	if (en)
		v1290_Write16(mvme, base, V1290_CONTROL, reg + (0x1<<3));
	else
		v1290_Write16(mvme, base, V1290_CONTROL, reg & (0xfff7));
}

void v1290_EnableEventFIFO(MVME_INTERFACE *mvme, uint32_t base, bool en)
{
	uint16_t reg = v1290_Read16(mvme, base, V1290_CONTROL);
	if (en)
		v1290_Write16(mvme, base, V1290_CONTROL, reg + 0x100);
	else
		v1290_Write16(mvme, base, V1290_CONTROL, reg & 0xfeff);
}

void v1290_ReadStatusRegister(MVME_INTERFACE *mvme, uint32_t base)
{
	uint16_t reg = v1290_Read16(mvme, base, V1290_STATUS);
	printf("Status register: 0x%X\n", V1290_STATUS);
	printf(" - Data ready: %d\n", reg & 0x1);
	printf(" - Almost full: %d\n", (reg & (0x1<<1))>>1);
	printf(" - Full: %d\n", (reg & (0x1<<2))>>2);
	printf(" - Trigger match: %d\n", (reg & (0x1<<3))>>3);
	printf(" - Header enable: %d\n", (reg & (0x1<<4))>>4);
	printf(" - Term on: %d\n", (reg & (0x1<<5))>>5);
	printf(" - Error TDC0: %d\n", (reg & (0x1<<6))>>6);
	printf(" - Error TDC1: %d\n", (reg & (0x1<<7))>>7);
	printf(" - Error TDC2: %d\n", (reg & (0x1<<8))>>8);
	printf(" - Error TDC3: %d\n", (reg & (0x1<<9))>>9);
	printf(" - Bus error: %d\n", (reg & (0x1<<10))>>10);
	printf(" - Purged: %d\n", (reg & (0x1<<11))>>11);
	printf(" - Res 0: %d\n", (reg & (0x1<<12))>>12);
	printf(" - Res 1: %d\n", (reg & (0x1<<13))>>13);
	printf(" - Pair: %d\n", (reg & (0x1<<14))>>14);
	printf(" - Trigger lost: %d\n", (reg & (0x1<<15))>>15);
}

bool v1290_IsDataReady(MVME_INTERFACE *mvme, uint32_t base)
{
	uint16_t reg = v1290_Read16(mvme, base, V1290_STATUS);
	return ((reg & (0x1)));
}

bool v1290_IsAlmostFull(MVME_INTERFACE *mvme, uint32_t base)
{
	uint16_t reg = v1290_Read16(mvme, base, V1290_STATUS);
	return (reg & (0x1<<1))>>1;
}

bool v1290_IsFull(MVME_INTERFACE *mvme, uint32_t base)
{
	uint16_t reg = v1290_Read16(mvme, base, V1290_STATUS);
	return (reg & (0x1<<2))>>2;
}

void v1290_ModuleReset(MVME_INTERFACE *mvme, uint32_t base)
{
	v1290_Write16(mvme,base, V1290_MOD_RESET,0x1);
}

void v1290_SoftClear(MVME_INTERFACE *mvme, uint32_t base)
{
	v1290_Write16(mvme,base , V1290_SW_CLEAR,0x1);
}

void v1290_EventReset(MVME_INTERFACE *mvme, uint32_t base)
{
	v1290_Write16(mvme,base, V1290_SW_EVENT_RESET,0x1);
}

void v1290_SoftTrigger(MVME_INTERFACE *mvme, uint32_t base)
{
	v1290_Write16(mvme, base, V1290_SW_TRIGGER, 0x1);
}

uint32_t v1290_ReadEventCounter(MVME_INTERFACE *mvme, uint32_t base)
{
	return v1290_Read32(mvme, base, V1290_EVENT_COUNTER);
}

uint16_t v1290_ReadEventStored(MVME_INTERFACE *mvme, uint32_t base)
{
	return v1290_Read16(mvme, base, V1290_EVENT_STORED);
}

void v1290_SetAlmostFullLevel(MVME_INTERFACE *mvme, uint32_t base, 
		uint16_t level)
{
	v1290_Write16(mvme, base, V1290_ALMOST_FULL_LVL, level);
}

uint16_t v1290_ReadAlmostFullLevel(MVME_INTERFACE *mvme, 
		uint32_t base, uint16_t level)
{
	return v1290_Read16(mvme, base, V1290_ALMOST_FULL_LVL);
}

void v1290_FWRev(MVME_INTERFACE *mvme, uint32_t base)
{
	unsigned short int reg = v1290_Read16(mvme,base, V1290_FW_REV);
	showbits(reg);
}

void v1290_ReadEventFIFO(MVME_INTERFACE *mvme, uint32_t base, uint16_t *dest)
{
	uint32_t reg = v1290_Read32(mvme, base, V1290_EVENT_FIFO);
	showbits(reg);
	dest[0] = reg&0xffff0000;
	dest[1] = reg&0xffff;
}

uint16_t v1290_ReadEventFIFOStatus(MVME_INTERFACE *mvme, uint32_t base)
{
	return (v1290_Read16(mvme, base, V1290_EVENT_FIFO_STATUS)&0x3);
}

void v1290_Decode(uint32_t data)
{
  uint32_t error_flag;
  switch((data&0xf8000000)>>27) {
    case 0x0:
      printf("Data:\t\t");
      //showbits(data);
      printf("channel: %d\t",(data&V1290_CH_MASK)>>21);
      printf("value(ns): %.4f\t",(float)(data&V1290_DATA_MASK)/40);
      printf("trailing/leading(1/0):%d",(data&(1<<26))>>26);
      putchar('\n');
      break;
    case 0x8:
      printf("Global header\t");
      //showbits(data);
      printf("Event count:%d\t",(data&V1290_GLB_HDR_EVENT_COUNT_MSK)>>5);
      printf("Geo addr:%d",data&V1290_GLB_HDR_GEO_MSK);
      putchar('\n');
      break;
    case 0x1:
      printf("TDC header\t");
      //showbits(data);
      printf("TDC: %d\t",(data&V1290_TDC_HDR_TDC_MSK)>>24);
      printf("EventID: %d\t",(data&V1290_TDC_HDR_EVENT_ID_MSK)>>12);
      printf("BunchID: %d\t",(data&V1290_TDC_HDR_BUNCH_ID_MSK));
      putchar('\n');
      break;
    case 0x3:
      printf("TDC trailer\t");
      //showbits(data);
      printf("TDC: %d\t",(data&V1290_TDC_TRL_TDC_MSK)>>24);
      printf("EventID: %d\t",(data&V1290_TDC_TRL_EVENT_ID_MSK)>>12);
      printf("Word count: %d\t",(data&V1290_TDC_TRL_WCOUNT_MSK));
      putchar('\n');
      break;
    case 0x11:
      printf("Extented trigger time tag: %x\t",data&V1290_GLB_TRG_TIME_TAG_MSK);
      //showbits(data);
      putchar('\n');
      break;
    case 0x10:
      printf("Global Trailer\t\t");
      //showbits(data);
      printf("Error:%d\t",(data&V1290_GLB_TRL_STATUS_MSK)>>24);
      printf("Word count:%d\t",(data&V1290_GLB_TRL_WCOUNT_MSK)>>5);
      printf("Geo addr:%d",data&V1290_GLB_TRL_GEO_MSK);
      putchar('\n');
      break;
    case 0x4:
      printf("TDC error\t");
      printf("TDC: %d\t",(data&V1290_TDC_HDR_TDC_MSK)>>24);
      error_flag = data&V1290_TDC_ERR_ERR_FLAGS_MSK;
      showbits(error_flag);
      //showbits(data);
      putchar('\n');
      break;
    default:
      //printf("invalid data type : %d \n", (data&0xf8000000)>>27);
      /*return(-1);*/
      break;
  }
  /*return((data&0xf8000000)>>27);*/
}

/*#define TEST_V1290*/
#ifdef TEST_V1290

#define V1290N_BASE 0x00200000
#define RPV130_BASE 0x8000
#include "rpv130.h"
void v1290_SetupTrigger(MVME_INTERFACE *myvme);
void v1290_SetupTDC(MVME_INTERFACE *myvme);
void v1290_SetupReadout(MVME_INTERFACE *myvme);
int main()
{
	printf("Test program for V1290\n");
	MVME_INTERFACE *myvme;
	mvme_open(&myvme, 0);

	/*v1290_SoftReset(myvme,V1290N_BASE);*/
  /*v1290_ModuleReset(myvme, V1290N_BASE);*/
  /*sleep(1);*/
	/*v1290_SetupTrigger(myvme);*/
	/*v1290_SetupTDC(myvme);*/
	/*v1290_SetupReadout(myvme);*/
	/*v1290_EnableBusError(myvme, V1290N_BASE, true);*/
	/*v1290_EnableEventFIFO(myvme, V1290N_BASE, true);*/
	/*v1290_EnableEmptyEvent(myvme, V1290N_BASE, true);*/
	/*v1290_ReadControlRegister(myvme, V1290N_BASE);*/

  v1290_SetWindowWidth(myvme, V1290N_BASE, 40000);
  sleep(1);
  v1290_SetWindowOffset(myvme, V1290N_BASE, -10000);
  sleep(1);
  v1290_SetExtraMargin(myvme, V1290N_BASE, 100);
  sleep(1);
  v1290_SetRejectMargin(myvme, V1290N_BASE, 100);
  sleep(1);
	v1290_TriggerConfRead(myvme, V1290N_BASE);

#if 0
	printf("v1290DataReady: %d\n",v1290_IsDataReady(myvme, V1290N_BASE));
	uint16_t fifo[2];
	v1290_ReadEventFIFO(myvme, V1290N_BASE, fifo);
	printf("event cnt: %d, word cnt: %d\n", fifo[0],fifo[1]);
	usleep(1);
	/*v1290_Setup(myvme, V1290N_BASE, 2);*/
	// trigger
	printf("injecting trigger and signals: ");
	rpv130_Pulse(myvme,RPV130_BASE,8); // input 11
	rpv130_Pulse(myvme,RPV130_BASE,2); // input 15
	rpv130_Pulse(myvme,RPV130_BASE,3); // input 6
	rpv130_Pulse(myvme,RPV130_BASE,4); // to discr -> ch0, chn 2, chn 8
	rpv130_Pulse(myvme,RPV130_BASE,7); // trigger
	printf("v1290DataReady: %d\n",v1290_IsDataReady(myvme, V1290N_BASE));

	printf("FIFO stat %d\n", v1290_ReadEventFIFOStatus(myvme, V1290N_BASE));
	v1290_ReadEventFIFO(myvme, V1290N_BASE, fifo);
	printf("event cnt: %d, word cnt: %d\n", fifo[0],fifo[1]);
	printf("event stored: %d\n", v1290_ReadEventStored(myvme, V1290N_BASE));
	sleep(1);

	// readout
	unsigned int buff[200];
	int i;
	for (i=0; i<200; i++)
		buff[i] = 0;

	/*int nentry;*/
	/*v1290_EventRead(myvme,V1290N_BASE,buff,&nentry);*/
	/*for (i=0; i<20; i++)*/
		/*buff[i] = v1290_Read32(myvme,V1290N_BASE,V1290_OUT_BUFFER);*/
	v1290_DataRead(myvme, V1290N_BASE, buff, fifo[1]+1);

	for (i = 0; i < 10; i++) {
		if(V1290_IS_TDC_MEASURE(buff[i]))
		{
			printf("buff[%02d], ch %02d:",i,V1290_GET_TDC_MSR_CHANNEL(buff[i]));
			printf(", value: %d, ", V1290_GET_TDC_MSR_MEASURE(buff[i]));
			showbits(buff[i]);
		}
	}
	printf("v1290DataReady: %d\n",v1290_IsDataReady(myvme, V1290N_BASE));
#endif

	// Finish
	mvme_close(myvme);
	return 0;
}

void v1290_SetupTrigger(MVME_INTERFACE *myvme)
{
	printf("Trigger matching set ...\n");
	v1290_TriggerMatchingSet(myvme, V1290N_BASE);
	sleep(1);
	v1290_SetWindowWidth(myvme, V1290N_BASE, 40000);
	sleep(1);
	v1290_SetWindowOffset(myvme, V1290N_BASE, -10000);
	sleep(1);
	/*v1290_SetExtraMargin(myvme, V1290N_BASE, 0x9);*/
	/*sleep(1);*/
	/*v1290_SetRejectMargin(myvme, V1290N_BASE, 0x8);*/
	/*sleep(1);*/
	v1290_EnableTriggerSubtraction(myvme, V1290N_BASE, true);
	sleep(1);
	v1290_AcqModeRead(myvme, V1290N_BASE);
	sleep(1);
}
void v1290_SetupTDC(MVME_INTERFACE *myvme)
{
	printf("Setting TDC ...\n");
	v1290_SetEdgeDetection(myvme, V1290N_BASE, 0x1);
	sleep(1);
	v1290_SetEdgeResolution(myvme, V1290N_BASE, 0x1);
	sleep(1);
	printf("Edge: %d\n",v1290_ReadEdgeDetection(myvme, V1290N_BASE) );
	sleep(1);
	printf("Res: %d\n",v1290_ReadEdgeResolution(myvme, V1290N_BASE) );
	sleep(2);
	v1290_SetDeadtime(myvme, V1290N_BASE, 0x2);
	sleep(1);
	printf("Deadtime: %d\n",v1290_ReadDeadtime(myvme, V1290N_BASE) );
	sleep(1);
}
void v1290_SetupReadout(MVME_INTERFACE *myvme)
{
	v1290_EnableHeader(myvme, V1290N_BASE, false);
	sleep(1);
	printf("header enable %d\n", v1290_HeaderIsEnabled(myvme, V1290N_BASE));
	sleep(1);
	printf("max hits %d\n", v1290_ReadMaxHits(myvme, V1290N_BASE));
	sleep(1);
	v1290_SetFIFOSize(myvme, V1290N_BASE, 0x6);
	sleep(1);
	printf("fifo size %d\n", v1290_ReadFIFOSize(myvme, V1290N_BASE));
	sleep(1);
	v1290_DisableAllChannels(myvme, V1290N_BASE);
	v1290_WriteEnablePattern(myvme, V1290N_BASE, 0x5);
	v1290_EnableChannel(myvme, V1290N_BASE, 0x0, false);
	sleep(1);
	printf("enable pattern %x\n", v1290_ReadEnablePattern(myvme, V1290N_BASE));
	v1290_EnableChannel(myvme, V1290N_BASE, 0, true);
}
#endif

