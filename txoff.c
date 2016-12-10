#include <stdio.h>
#include <stdlib.h>
#include <rp.h>


int main()
{
  printf("\nAusschalten alle Funktionen \n");
  
  if(rp_Init() != RP_OK)
  {
    fprintf(stderr, "Init-Funktion failed\n\n");
  }

  // ADC-Stop
  rp_AcqStop();
  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    
  // Funktionen disablen
  rp_Release();
  return(0);
}