#include <stdio.h>
#include <string.h>
#include <time.h>

#include "midas.h"

INT mhtdc_ana(EVENT_HEADER *, void *);
INT mhtdc_ana_init(void);
INT mhtdc_ana_bor(INT run_number);
INT mhtdc_ana_eor(INT run_number);

// dummy structures ...
typedef struct { INT dummy[1];} DUMMY_PARAM;
DUMMY_PARAM mhtdc_ana_param;
char *mhtdc_ana_param_str[] = {
  "[.]", "Dummy = INT[1] :", "[0] 0", NULL};

ANA_MODULE mhtdc_ana_module = {
  "MHTDC analysis",           /* module name           */
  "NT",               /* author                */
  mhtdc_ana,                   /* event routine         */
  mhtdc_ana_bor,               /* BOR routine           */
  mhtdc_ana_eor,               /* EOR routine           */
  mhtdc_ana_init,              /* init routine          */
  NULL,                        /* exit routine          */
  &mhtdc_ana_param,             /* parameter structure   */
  sizeof(mhtdc_ana_param),      /* structure size        */
  mhtdc_ana_param_str,   /* initial parameters    */
};

/*------------------------------------------------------------------*/

#include "v1290.h"
#include "TH1F.h"

static TH1F *hMHTDC;

INT mhtdc_ana_init(void)
{

  hMHTDC = h1_book<TH1F>("MHTDC", "MHTDC", 16384, 0, 16383);
  return SUCCESS;
}

/*-- BOR routine ---------------------------------------------------*/

INT mhtdc_ana_bor(INT run_number)
{
  return SUCCESS;
}

/*-- eor routine ---------------------------------------------------*/

INT mhtdc_ana_eor(INT run_number)
{
  return SUCCESS;
}

/*-- event routine -------------------------------------------------*/

INT mhtdc_ana(EVENT_HEADER * pheader, void *pevent)
{
  int *pdata;
  int nbyte = bk_locate(pevent, "TDC", &pdata);

  if (!nbyte)
  {
    printf("bank not found\n");
    return 1;
  }

  for (int ib = 0; ib < nbyte; ++ib)
  {
    //v1290_Decode((uint32_t)pdata[ib]);
		if(V1290_IS_TDC_MEASURE(pdata[ib]) && V1290_GET_TDC_MSR_CHANNEL(pdata[ib]) == 1)
		{
      printf("event %d: ", pheader->serial_number);
      printf("ch %02d:",V1290_GET_TDC_MSR_CHANNEL(pdata[ib]));
      printf(", value: %d, ", V1290_GET_TDC_MSR_MEASURE(pdata[ib]));
      printf("\n");
    }
  }

  return SUCCESS;
}
