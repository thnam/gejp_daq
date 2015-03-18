#include <stdio.h>
#include <string.h>
#include <time.h>

#include "midas.h"

char *analyzer_name = "Analyzer";

/* analyzer_loop is called with this interval in ms (0 to disable)  */
INT analyzer_loop_period = 0;

/* default ODB size */
INT odb_size = DEFAULT_ODB_SIZE;

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
   FILE *f;
   time_t now;
   char str[256];
   int size;
   double n;
   HNDLE hDB;
   BOOL flag;


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
