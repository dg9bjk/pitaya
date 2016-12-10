#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <rp.h>

void sigcode(int Bitinfo)
{
      if(Bitinfo)
      {	// long 
        rp_GenAmp(RP_CH_1,1.0);
        usleep(200000);
        rp_GenAmp(RP_CH_1,0.0);
        usleep(100000);
      }
      else
      { // short
        rp_GenAmp(RP_CH_1,1.0);
        usleep(100000);
        rp_GenAmp(RP_CH_1,0.0);
        usleep(100000);
      }
}

int main()
{
  int i,n;
  int Bitinfo   = 0; 
  int SwitchSec = 0;
  time_t timestamp;
  struct tm *ts;
  
  printf("\n CW-Geber \n");
  
  if(rp_Init() != RP_OK)
  {
    fprintf(stderr, "Init-Funktion failed\n\n");
  }

//-----------------------------------------------
//Generator CH 1
  float frequency1		= 3575000.0; // Hz (max. 60MHz)
  float phase1			= 0.0;      // ° (-180.0° ... 0.0° ... 180.0°)
  float amplitude1		= 1.0;	    // Vpp (max. 1.0 Vpp)
 
  rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(RP_CH_1,frequency1);
  rp_GenPhase(RP_CH_1,phase1);
  rp_GenAmp(RP_CH_1,0.0); // No Signal

  rp_GenOutEnable(RP_CH_1);

  printf("Startup: 59 [Sync]\n");
  sleep(1);
  
  n= 0;
  while(1)
  {
    timestamp = time(NULL);
    ts=localtime(&timestamp);
    printf("Zeit: %02d.%02d.%04d - %02d:%02d:%02d \n",ts->tm_mday,ts->tm_mon+1,ts->tm_year+1900,ts->tm_hour,ts->tm_min,ts->tm_sec);

    for(i=0;i<20;i++)
      sigcode(1);
    n++;
  }

  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    
  // Funktionen disablen
  rp_Release();
  return(0);
}