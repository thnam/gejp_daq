#include <stdio.h>
#include <string.h>
#include <time.h>

#include "midas.h"

#include "TH1I.h"

char *analyzer_name = "Analyzer";

/* analyzer_loop is called with this interval in ms (0 to disable)  */
INT analyzer_loop_period = 0;

/* default ODB size */
INT odb_size = DEFAULT_ODB_SIZE;

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


ANA_MODULE *trigger_module[] = {&phadc_ana_module, NULL};

BANK_LIST ana_trigger_bank_list[] = {
  {"PHA", TID_INT, 1, NULL},
  {""},
};

ANALYZE_REQUEST analyze_request[] = {
  {"VME Bit3 Trigger",                  /* equipment name */
    {1,                         /* event ID */
      TRIGGER_ALL,               /* trigger mask */
      GET_NONBLOCKING,           /* get events without blocking producer */
      "SYSTEM",                  /* event buffer */
      TRUE,                      /* enabled */
      "", "",}
    ,
      NULL,                       /* analyzer routine */
      trigger_module,             /* module list */
      ana_trigger_bank_list,      /* bank list */
      1000,                       /* RWNT buffer size */
      TRUE,                       /* Use tests for this event */
  }
  ,
    {""}
  ,
};

/*-- Analyzer Init -------------------------------------------------*/

INT analyzer_init()
{
  return SUCCESS;
}

/*-- Analyzer Exit -------------------------------------------------*/

INT analyzer_exit()
{
  return CM_SUCCESS;
}

/*-- Begin of Run --------------------------------------------------*/

INT ana_begin_of_run(INT run_number, char *error)
{
  return CM_SUCCESS;
}

/*-- End of Run ----------------------------------------------------*/

INT ana_end_of_run(INT run_number, char *error)
{

  return CM_SUCCESS;
}

/*-- Pause Run -----------------------------------------------------*/

INT ana_pause_run(INT run_number, char *error)
{
  return CM_SUCCESS;
}

/*-- Resume Run ----------------------------------------------------*/

INT ana_resume_run(INT run_number, char *error)
{
  return CM_SUCCESS;
}

/*-- Analyzer Loop -------------------------------------------------*/

INT analyzer_loop()
{
  return CM_SUCCESS;
}

/*------------------------------------------------------------------*/

static TH1I *hPHADC;
#include "TObjArray.h"
#include "TFolder.h"
extern TObjArray* gHistoFolderStack;
extern TFolder* gManaHistosFolder;

INT phadc_ana_init(void)
{

  hPHADC = new TH1I("PHADC", "PHADC", 16384, 0, 16383);
  if (!gHistoFolderStack->Last())
    gManaHistosFolder->Add(hPHADC);
  else
    ((TFolder *) gHistoFolderStack->Last())->Add(hPHADC);
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
