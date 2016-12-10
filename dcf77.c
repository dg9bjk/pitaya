#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <rp.h>


int main()
{
  int i,j;
  int pos;
  char display[101] = "";
  
  printf("\nDCF77-Tester \n");
  printf("Attention: Sendung on 77,5kHz ! Only for Testing the Clock!\n");
  
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
  rp_GenAmp(RP_CH_1,amplitude1);
  rp_GenOutEnable(RP_CH_1);
  
  sleep(5);
  
  while(1)
  {
    for (i=0;i<59;i++)
    {
    printf("Step: %d\n",i);
    rp_GenAmp(RP_CH_1,0.2);
    usleep(200000);
    rp_GenAmp(RP_CH_1,1.0);
    usleep(800000);
    }
    printf("LastStep: %d\n",i);
    usleep(1000000);
  }

  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    
  // Funktionen disablen
  rp_Release();
  return(0);
}