#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#include "midas.h"

#define FE_NAME "Test DAQ"
#define EQ_NAME "VME Bit3 Trigger"
#define EQ_EVID 1

#include "bt617_vmedrv.h"
#include "bt617.h"
#include "rpv130.h"
#include "v006.h"

#define RPV130_BASE 0x8000
#define V006_BASE 0xEFFF00

#define DEBUG
#undef DEBUG

/* make frontend functions callable from the C framework */
#ifdef __cplusplus
extern "C" {
#endif

  /*-- Globals -------------------------------------------------------*/
  /* The frontend name (client name) as seen by other MIDAS clients   */
  char *frontend_name = (char*)FE_NAME;
  /* The frontend file name, don't change it */
  char *frontend_file_name = (char*) __FILE__;
  /* frontend_loop is called periodically if this variable is TRUE    */
  BOOL frontend_call_loop = TRUE;
  /* a frontend status page is displayed with this frequency in ms */
  //INT display_period = 3000;
  INT display_period = 1000;
  /* maximum event size produced by this frontend */
  INT max_event_size      = 20000;
  INT max_event_size_frag = 0;
  /* buffer size to hold events */
  INT event_buffer_size = 10*max_event_size;

  /*-- Function declarations -----------------------------------------*/
  INT frontend_init();
  INT frontend_exit();
  INT begin_of_run(INT run_number, char *error);
  INT end_of_run(INT run_number, char *error);
  INT pause_run(INT run_number, char *error);
  INT resume_run(INT run_number, char *error);
  INT frontend_loop();

  HNDLE hDB;
  int LAM;

  int read_trigger_event(char *pevent, int off);

  /*-- Equipment list ------------------------------------------------*/
  EQUIPMENT equipment[] = {
    { EQ_NAME,         /* equipment name */
      {
        EQ_EVID, (1<<EQ_EVID), /* event ID, trigger mask */
        "SYSTEM",              /* event buffer */
        EQ_POLLED,          /* equipment type */
        0,                    /* event source */
        "MIDAS",              /* format */
        TRUE,                 /* enabled */
        RO_RUNNING|RO_ODB, // when to read
        //RO_RUNNING|RO_STOPPED|RO_PAUSED|RO_ODB, [> When to read <]
        10,                 /* poll every so milliseconds */
        0,                    /* stop run after this event limit */
        0,                    /* number of sub events */
        0,                    // log history
        "", "", ""
      },
      read_trigger_event, /* readout routine */
    },
    { "" }
  };

#ifdef __cplusplus
}
#endif

/********************************************************************\
  Callback routines for system transitions

  These routines are called whenever a system transition like start/
  stop of a run occurs. The routines are called on the following
occations:

frontend_init:  When the frontend program is started. This routine
should initialize the hardware.
frontend_exit:  When the frontend program is shut down. Can be used
to releas any locked resources like memory, commu-
nications ports etc.

begin_of_run:   When a new run is started. Clear scalers, open
rungates, etc.
end_of_run:     Called on a request to stop a run. Can send 
end-of-run event and close run gates.
pause_run:      When a run is paused. Should disable trigger events.
resume_run:     When a run is resumed. Should enable trigger events.
\********************************************************************/

int evt_cnt = 0;

MVME_INTERFACE *myvme;

/*-- Frontend Init -------------------------------------------------*/
INT frontend_init()
{
  // Init VME interface
	mvme_open(&myvme, 0);

  // Clear modules
  v006_clear(myvme, V006_BASE);



  return SUCCESS;
}

/*-- Frontend Exit -------------------------------------------------*/
INT frontend_exit()
{
  mvme_close(myvme);
  return SUCCESS;
}

/*-- Begin of Run --------------------------------------------------*/
INT begin_of_run(INT run_number, char *error)
{
#ifdef DEBUG
  cm_msg(MINFO, frontend_name, "Begin run %d\n", run_number);
#endif
  evt_cnt = 0;
  // Clear busy status
  rpv130_ClearBusy1(myvme, RPV130_BASE);
  rpv130_ClearBusy3(myvme, RPV130_BASE);
  rpv130_Pulse(myvme, RPV130_BASE, 1);
  rpv130_Pulse(myvme, RPV130_BASE, 2);

  return SUCCESS;
}

/*-- End of Run ----------------------------------------------------*/
INT end_of_run(INT run_number, char *error)
{
  rpv130_Pulse(myvme, RPV130_BASE, 1);
#ifdef DEBUG
  cm_msg(MINFO, frontend_name, "End run %d!\n", run_number);
  cm_msg(MINFO, frontend_name, "read %d triggered events", evt_cnt);
#endif
  return SUCCESS;
}

/*-- Pause Run -----------------------------------------------------*/
INT pause_run(INT run_number, char *error)
{
  return SUCCESS;
}

/*-- Resume Run ----------------------------------------------------*/
INT resume_run(INT run_number, char *error)
{
  return SUCCESS;
}

/*-- Frontend Loop -------------------------------------------------*/
INT frontend_loop()
{
  /* if frontend_call_loop is true, this routine gets called when
     the frontend is idle or once between every event */
  //printf("frontend_loop!\n");
  ss_sleep(10);
  return SUCCESS;
}

/*------------------------------------------------------------------*/
/********************************************************************\
  Readout routines for different events
  \********************************************************************/
extern "C" INT poll_event(INT source, INT count, BOOL test)
/* Polling routine for events. Returns TRUE if event
   is available. If test equals TRUE, don't return. The test
   flag is used to time the polling */
{
  if (test) {
    ss_sleep (count);
  }
  else
    for (int i = 0; i < count; i++) 
    {
      if (rpv130_IsBusy3(myvme, RPV130_BASE))
      {
#ifdef DEBUG
        cm_msg(MINFO, frontend_name, "Triggered %d\n", evt_cnt);
#endif
        rpv130_ClearBusy3(myvme, RPV130_BASE);
        return 1;
      }
      else
        ss_sleep(1);
    }
  return (0);
}

/*-- Interrupt configuration ---------------------------------------*/
extern "C" INT interrupt_configure(INT cmd, INT source, PTYPE adr)
{
  switch(cmd)
  {
    case CMD_INTERRUPT_ENABLE:
      break;
    case CMD_INTERRUPT_DISABLE:
      break;
    case CMD_INTERRUPT_ATTACH:
      break;
    case CMD_INTERRUPT_DETACH:
      break;
  }
  return SUCCESS;
}

/*-- Event readout -------------------------------------------------*/
double get_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + 0.000001*tv.tv_usec;
}

int read_trigger_event(char *pevent, int off)
{
  bk_init32(pevent);
  evt_cnt++;

  int* pdatad;
  bk_create(pevent, "PHA", TID_INT, &pdatad);
  int lam_v006 = v006_readLAM(myvme, V006_BASE, 0);
  if (lam_v006)
  {
    pdatad[0] = (int) v006_adc(myvme, V006_BASE, 0);
#ifdef DEBUG
    cm_msg(MINFO, frontend_name, "adc:  %d ", pdatad[0]);
#endif
  }
  else
  {
#ifdef DEBUG
    cm_msg(MINFO, frontend_name, "No data ..");
#endif
    ss_sleep(1);
  }
  bk_close(pevent, pdatad + 1);

  // reset pulse to V006
  rpv130_Pulse(myvme, RPV130_BASE, 2);
  // reset flip/flop for a new event
  rpv130_Pulse(myvme, RPV130_BASE, 1);

  return bk_size(pevent);
}
// end file
