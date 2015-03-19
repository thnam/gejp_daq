#include <stdio.h>
#include <string.h>
#include <time.h>

#include "midas.h"

#include "TH1I.h"

INT phadc_ana(EVENT_HEADER *, void *);
INT phadc_ana_init(void);
INT phadc_ana_bor(INT run_number);
INT phadc_ana_eor(INT run_number);

// dummy structures ...
typedef struct { INT dummy[1];} DUMMY_PARAM;
DUMMY_PARAM phadc_ana_param;
char *phadc_ana_param_str[] = {
  "[.]", "Dummy = INT[1] :", "[0] 0", NULL};

ANA_MODULE phadc_ana_module = {
  "PHADC analysis",           /* module name           */
  "NT",               /* author                */
  phadc_ana,                   /* event routine         */
  phadc_ana_bor,               /* BOR routine           */
  phadc_ana_eor,               /* EOR routine           */
  phadc_ana_init,              /* init routine          */
  NULL,                        /* exit routine          */
  &phadc_ana_param,             /* parameter structure   */
  sizeof(phadc_ana_param),      /* structure size        */
  phadc_ana_param_str,   /* initial parameters    */
};

/*------------------------------------------------------------------*/

static TH1I *hPHADC;

INT phadc_ana_init(void)
{

  hPHADC = h1_book<TH1I>("PHADC", "PHADC", 16384, 0, 16383);
  return SUCCESS;
}

/*-- BOR routine ---------------------------------------------------*/

INT phadc_ana_bor(INT run_number)
{
  return SUCCESS;
}

/*-- eor routine ---------------------------------------------------*/

INT phadc_ana_eor(INT run_number)
{
  return SUCCESS;
}

/*-- event routine -------------------------------------------------*/

INT phadc_ana(EVENT_HEADER * pheader, void *pevent)
{
  int *pdata;
  if (!bk_locate(pevent, "PHA", &pdata))
  {
    printf("bank not found\n");
    return 1;
  }

  hPHADC->Fill(pdata[0]);

  return SUCCESS;
}
