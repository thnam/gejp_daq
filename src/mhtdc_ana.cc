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

static TH1F *hTdiff[2];
static TH1F *hTdc[3];

INT mhtdc_ana_init(void)
{

  hTdiff[0] = h1_book<TH1F>("tdiff1", "Time diff chn1 - chn0", 4096, -10E3, 10E3);
  hTdiff[1] = h1_book<TH1F>("tdiff2", "Time diff chn2 - chn0", 4096, -10E3, 10E3);
  hTdc[0] = h1_book<TH1F>("tdc0", "TDC0", 4096, -10E3, 20E3);
  hTdc[1] = h1_book<TH1F>("tdc1", "TDC1", 4096, -10E3, 20E3);
  hTdc[2] = h1_book<TH1F>("tdc2", "TDC2", 4096, -10E3, 20E3);
  hTdiff[0]->SetXTitle("Time [ns]");
  hTdiff[1]->SetXTitle("Time [ns]");
  hTdc[0]->SetXTitle("Time [ns]");
  hTdc[1]->SetXTitle("Time [ns]");
  hTdc[2]->SetXTitle("Time [ns]");
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
    //printf("bank not found\n");
    return 1;
  }

  int t[3];
  int fake_value = -200E3;
  t[0] = t[1] = t[2] = fake_value;
  for (int ib = 0; ib < nbyte; ++ib)
  {
    //v1290_Decode((uint32_t)pdata[ib]);
		//if(V1290_IS_TDC_MEASURE(pdata[ib]) && V1290_GET_TDC_MSR_CHANNEL(pdata[ib]) == 1)
		//{
      //printf("event %d: ", pheader->serial_number);
      //printf("ch %02d:",V1290_GET_TDC_MSR_CHANNEL(pdata[ib]));
      //printf(", value: %d, ", V1290_GET_TDC_MSR_MEASURE(pdata[ib]));
      //printf("\n");
    //}

    if(V1290_IS_TDC_MEASURE(pdata[ib]))
    {
      int chn = V1290_GET_TDC_MSR_CHANNEL(pdata[ib]);
      t[chn] = V1290_GET_TDC_MSR_MEASURE(pdata[ib])*25/1000;
    }
  }
  if (t[0] > fake_value)
  {
    hTdc[0]->Fill(t[0]);
    if (t[1] > fake_value)
    {
      hTdc[1]->Fill(t[1]);
      hTdiff[0]->Fill(t[1] - t[0]);
    }
    if (t[2] > fake_value)
    {
      hTdc[2]->Fill(t[2]);
      hTdiff[1]->Fill(t[2] - t[0]);
    }
  }

  return SUCCESS;
}
