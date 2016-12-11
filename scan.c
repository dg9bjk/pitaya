#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <sys/time.h>
//#include <pthread.h>
#include <time.h>
#include <rp.h>

#define MAXRX (1*1024*1024) // Worte (16Bit)

// Globale Variablen
uint16_t RxBuffer[MAXRX];

//------------------------------------------------
void getsignal1(int signum1)
{
  //ADC_BUFFER_SIZE;  // 16 * 1024 = 16384
  int i,j;

  uint32_t wrpointer		= 0; // Schreibzeiger ADC-Buffer
  uint32_t lastwrpt		= 0; // Zwischenspeicher alter Schreibepointer
  uint32_t lastrdpt		= 0; // Zwischenspeicher alter Lesepointer
  uint32_t distance		= 0;
  uint32_t space		= 0;

  uint32_t startpos		= 0;
  uint32_t endpos		= 0;

  // Init the Position of Write-Pointer - Start everything from here
  rp_AcqGetWritePointer(&wrpointer);
  i         = 0;
  j	    = 0;
  lastrdpt  = wrpointer;
  lastwrpt  = wrpointer;

  // Zeigerposition des Schreibzeigers
  rp_AcqGetWritePointer(&wrpointer);	// Actual Postion of Writepointer
    
  if(wrpointer != lastwrpt )	// Write-Pointer moved?
  {
    lastwrpt = wrpointer;	// Store new Position
        
    startpos = lastrdpt;	// Restore last Readpostion
    if(wrpointer > 0)		// If Write-Pointer over the Border
      endpos   = (wrpointer - 1);	// -1 = the last Sample
    else
      endpos   = (ADC_BUFFER_SIZE -1); // borderprotection 

    if(endpos < startpos)	// went over the border ?
      distance = (endpos + ((16*1024)-startpos));
    else
      distance = (endpos - startpos);

    space    = (MAXRX - i);	// Space in RxBuffer
        
    if(space < (distance + 1)) // RxBuffer at the end ?
      endpos = ((startpos + space) % (16*1024)) -1;
        
//printf("Debug1: pter: %u start: %u ende: %u distance:%d \n",wrpointer,startpos,endpos,distance);

    // Read ADC-Buffer
    rp_AcqGetDataPosRaw(RP_CH_1,startpos,endpos,&RxBuffer[i],&space);
//printf("Debug2: count: %i: %u \n",count,i);

    lastrdpt = ((startpos + space) % (16*1024));	// Store last Read Data	
    i = (i + space);					// Store the RxBuffer-position.
  }
}

//------------------------------------------------
int main()
{
  int i,j;
  int pos;
  char display[101] = "";
  
  float Samplefreq		= 0;

  float frequency1		= 1000000.0; // Hz (max. 60MHz)
  float phase1			= 0.0;      // ° (-180.0° ... 0.0° ... 180.0°)
  float amplitude1		= 1.0;	    // Vpp (max. 1.0 Vpp)

  float frequency2		= 1000000.0; // Hz (max. 60MHz)
  float phase2			= 90.0;      // ° (-180.0° ... 0.0° ... 180.0°)
  float amplitude2		= 1.0;	    // Vpp (max. 1.0 Vpp)

  printf("\nScanprogramm \n");
  
  if(rp_Init() != RP_OK)
  {
    fprintf(stderr, "Init-Funktion failed\n\n");
  }

  memset(RxBuffer,0x0,sizeof(RxBuffer));

  // Generator 1
  rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(RP_CH_1,frequency1);
  rp_GenPhase(RP_CH_1,phase1);
  rp_GenAmp(RP_CH_1,amplitude1);
//  rp_GenOutEnable(RP_CH_1);

  // Generator 2
  rp_GenMode(RP_CH_2, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(RP_CH_2, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(RP_CH_2,frequency2);
  rp_GenPhase(RP_CH_2,phase2);
  rp_GenAmp(RP_CH_2,amplitude2);
  rp_GenOutEnable(RP_CH_2);

  // Samplerate
  //rp_AcqSetSamplingRate(RP_SMP_125M);		// :1     = 125,0   MHZ
  rp_AcqSetSamplingRate(RP_SMP_15_625M);	// :8     =  15,625 MHz
  //rp_AcqSetSamplingRate(RP_SMP_1_953M);	// :64    =   1,953 MHz
  //rp_AcqSetSamplingRate(RP_SMP_122_070K);	// :1024  = 122,070 kHz
  //rp_AcqSetSamplingRate(RP_SMP_15_258K);	// :8192  =  15,258 kHz
  //rp_AcqSetSamplingRate(RP_SMP_1_907K);	// :65536 =   1,907 kHz

  rp_AcqGetSamplingRateHz(&Samplefreq);
  printf("Samplefrequenz: %.3f kHz \n",Samplefreq/1000.0);
  
  // Triggersignal für Erfassung (hier: Sofort)
  rp_AcqSetTriggerSrc(RP_TRIG_SRC_DISABLED);

  // Level des Einganges
  rp_AcqSetGain(RP_CH_1,RP_HIGH);
  rp_AcqSetGain(RP_CH_2,RP_HIGH);

  // Start der Erfassung der Messwerte
  rp_AcqSetArmKeep(true);
  rp_AcqStart();

  // Signal auslesen über Timer
  static struct sigaction sa1;
  static struct itimerval timer1;
  
  memset(&sa1,0,sizeof(sa1));
  sigemptyset(&sa1.sa_mask);
  sa1.sa_handler = &getsignal1;
  sigaction(SIGALRM,&sa1,NULL);
  
  timer1.it_value.tv_sec     = 0;
  timer1.it_value.tv_usec    = 60;
  timer1.it_interval.tv_sec  = 0;
  timer1.it_interval.tv_usec = 60;
  
  setitimer(ITIMER_REAL,&timer1,NULL);

//-----------------------
#define SCALA 512
  
/*   for(i=0;i<MAXRX; ++i)
  {
      printf("RxBuffer[%7d] : %5u =",i,RxBuffer[i]);
      memset(display,' ',sizeof(display));
      display[101] = 0x0;
      display[0]   = '|';
      display[100] = '|';
      
      if(RxBuffer[i] < (15*1024))
        pos = (RxBuffer[i] / SCALA) + 49;
      else
        pos = ((RxBuffer[i] - (15*1024))/SCALA )-50;
      
      if(pos > 100)
        pos = 100;
      if(pos < 0)
        pos = 0;

      display[pos] = '*';
      printf(" %s\n",display);
      usleep(15000);
  }
*/

  // ADC-Stop
  rp_AcqStop();
  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    
  // Funktionen disablen
  rp_Release();
  return(0);
}