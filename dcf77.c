#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <rp.h>

// Globale Variablen
float frequency1		= 3575000.0; // Hz (max. 60MHz)
float amplitude1		= 1.0;	    // Vpp (max. 1.0 Vpp)


//-----------------------------------------------
void sigcode(int Bitinfo)
{
      if(Bitinfo)
      {	// long Pause
        rp_GenAmp(RP_CH_1,amplitude1 * 0.2);
        usleep(200000);
        rp_GenAmp(RP_CH_1,amplitude1);
        usleep(800000);
      }
      else
      {
        rp_GenAmp(RP_CH_1,amplitude1 * 0.2);
        usleep(100000);
        rp_GenAmp(RP_CH_1,amplitude1);
        usleep(900000);
      }
}

//-----------------------------------------------
int main()
{
  int i,n;
  int Bitinfo   = 0; 
  int SwitchSec = 0;
  time_t timestamp;
  struct tm *ts;
  
  printf("\nDCF77-Tester \n");
  printf("Attention: Sending on 77,5kHz ! Only for Testing the Clock!\n");
  
  if(rp_Init() != RP_OK)
  {
    fprintf(stderr, "Init-Funktion failed\n\n");
  }

//Generator CH 1
  float phase1			= 0.0;      // ° (-180.0° ... 0.0° ... 180.0°)
 
  rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(RP_CH_1,frequency1);
  rp_GenPhase(RP_CH_1,phase1);
  rp_GenAmp(RP_CH_1,amplitude1);

  printf("Warte auf Minutenwechsel\n");  
  do{
    timestamp = time(NULL);
    sleep(1);
  }while((timestamp % 60) != 58);

  rp_GenOutEnable(RP_CH_1);
  printf("Startup: 59 [Sync]\n");
  sleep(1);
  
  n= 0;
  while(n < 10)
  {
    timestamp = time(NULL);
    ts=localtime(&timestamp);
    printf("Übertrage Zeit für: %02d.%02d.%04d - %02d:%02d:%02d \n",ts->tm_mday,ts->tm_mon+1,ts->tm_year+1900,ts->tm_hour,ts->tm_min+1,ts->tm_sec);
    for (i=0;i<59;i++)
    {
      printf("Step: %d = %d\n",i,Bitinfo);
      sigcode(Bitinfo);
    }
    if(SwitchSec) 		// if we habe 61 Seconds (Switchseconds)
      sigcode(Bitinfo);
    printf("LastStep: %d [Sync]\n",i);
    usleep(1000000);
    n++;
  }

  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    
  // Funktionen disablen
  rp_Release();
  return(0);
}