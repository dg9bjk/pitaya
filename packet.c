#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <rp.h>

// Globale Variablen:
float frequency1		= 3575000.0; // Betriebsfrequenz (Mittenfrequenz) in Hz (max. 60MHz)
float amplitude1		= 1.0;	     // Pegel bei Signalausgabe in Vpp (max. 1.0 Vpp)
float shift			= 200.0;     // Frequenzversatz in Hz	
int speed			= 333;	     // Zeichenlänge bei 300 Baud in µs 
//-------------------------------------------------------------------
// Bell 103 Norm 300 8N1
void sigcode(int Bitinfo)
{
  switch(Bitinfo)
  {
    case '0': 
            {	// Space
            rp_GenFreq(RP_CH_1,frequency1 - (shift/2));
            usleep(speed);
            }
            break;

    case '1': 
            { // Mark
            rp_GenFreq(RP_CH_1,frequency1 + (shift/2));
            usleep(speed);
            }
            break;

    case '<': 
            {	// Space
            rp_GenFreq(RP_CH_1,frequency1 - (shift/2));
            usleep(speed);
            }
            break;

    case '>': 
            { // Mark
            rp_GenFreq(RP_CH_1,frequency1 + (shift/2));
            usleep(speed);
            }
            break;
  }
}

//-------------------------------------------------------------------
void decode(char *input,char *bitfolge)
{

  int i;
  int maske;
  
  bitfolge[0] = '>'; //Startbit
  maske = 0x1;
  for(i=1;i<9;i++)
  {
    if(input && maske)
      bitfolge[i]=1;
    else
      bitfolge[i]=0;
      
    maske = maske << 1;
  }
  bitfolge[9]='<';// Stopbit
  bitfolge[10]=0x0;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
int main()
{
  int i,n;
  int Bitinfo		= 0; 
  char input  		= 0;
  char aktchar[11];
  char TextArray[] = "Automatic Generated Red Pitaya Test Bake de DG9BJK";
  time_t timestamp;
  struct tm *ts;

  printf("\n Packet-Bake \n");
  
  if(rp_Init() != RP_OK)
  {
    fprintf(stderr, "Init-Funktion failed\n\n");
  }

//Generator CH 1
  float phase1			= 0.0;      // ° (-180.0° ... 0.0° ... 180.0°)
  rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(RP_CH_1,frequency1 - (shift/2));
  rp_GenPhase(RP_CH_1,phase1);
  rp_GenAmp(RP_CH_1,amplitude1); 

  rp_GenOutEnable(RP_CH_1);

  // Preamble for Sync
  for(i=0;i<10;i++)
  {
    sigcode(1);
    sigcode(0);
  }
  
  timestamp = time(NULL);
  ts=localtime(&timestamp);
  printf("Zeit: %02d.%02d.%04d - %02d:%02d:%02d \n",ts->tm_mday,ts->tm_mon+1,ts->tm_year+1900,ts->tm_hour,ts->tm_min,ts->tm_sec);
  
  for(n=0;(n< strlen(TextArray)) & (TextArray[n] != 0);n++)
  {
    input = TextArray[n];
    decode(&input,aktchar);
    
    if(aktchar != 0)
    {
      for(i=1;(i< strlen(aktchar)) & (aktchar[i] != 0);i++)
      {
        printf("Debug: %c\n",aktchar[i]);
        sigcode(aktchar[i]);
      }
    }      
  }
  
  // Postamble
  for(i=0;i<10;i++)
  {
    sigcode(1);
    sigcode(0);
  }

  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    
  // Funktionen disablen
  rp_Release();
  return(0);
}