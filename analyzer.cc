#include <stdio.h>
#include <string.h>
#include <time.h>

#include "midas.h"

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
typedef struct { INT dummy[8];} DUMMY_PARAM;
DUMMY_PARAM phadc_ana_param;
char *phadc_ana_param_str[] = {" ", NULL};

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


ANA_MODULE *trigger_module[] = {NULL};

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


INT phadc_ana_init(void)
{

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

   return SUCCESS;
}
