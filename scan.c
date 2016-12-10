#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <rp.h>

#define MAXRX (1*1024*1024) // Worte (16Bit)


int main()
{
  int i,j;
  int pos;
  char display[101] = "";
  
  printf("\nScanprogramm \n");
  
  if(rp_Init() != RP_OK)
  {
    fprintf(stderr, "Init-Funktion failed\n\n");
  }

//-----------------------------------------------
//Generator CH 1
  float frequency1		= 1000000.0; // Hz (max. 60MHz)
  float phase1			= 0.0;      // ° (-180.0° ... 0.0° ... 180.0°)
  float amplitude1		= 1.0;	    // Vpp (max. 1.0 Vpp)
 
  rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(RP_CH_1,frequency1);
  rp_GenPhase(RP_CH_1,phase1);
  rp_GenAmp(RP_CH_1,amplitude1);
//  rp_GenOutEnable(RP_CH_1);

//Generator CH 2
  float frequency2		= 1000000.0; // Hz (max. 60MHz)
  float phase2			= 90.0;      // ° (-180.0° ... 0.0° ... 180.0°)
  float amplitude2		= 1.0;	    // Vpp (max. 1.0 Vpp)
  
  rp_GenMode(RP_CH_2, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(RP_CH_2, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(RP_CH_2,frequency2);
  rp_GenPhase(RP_CH_2,phase2);
  rp_GenAmp(RP_CH_2,amplitude2);
  rp_GenOutEnable(RP_CH_2);

//-----------------------------------------------
// Scanner  
  float Samplefreq		= 0;

  uint32_t array_size		= ADC_BUFFER_SIZE;  // 16 * 1024 = 16384

  uint32_t wrpointer		= 0; // Schreibzeiger ADC-Buffer
  uint32_t lastwrpt		= 0; // Zwischenspeicher alter Schreibepointer
  uint32_t lastrdpt		= 0; // Zwischenspeicher alter Lesepointer
  uint32_t distance		= 0;
  uint32_t space		= 0;

  uint32_t startpos		= 0;
  uint32_t endpos		= 0;
  uint32_t count		= 0;
  

  uint16_t RxBuffer[MAXRX];
  
  memset(RxBuffer,0x0,sizeof(RxBuffer));
  
  // Samplerate
//  rp_AcqSetSamplingRate(RP_SMP_125M);		// :1     = 125,0   MHZ
  rp_AcqSetSamplingRate(RP_SMP_15_625M);	// :8     =  15,625 MHz
//  rp_AcqSetSamplingRate(RP_SMP_1_953M);		// :64    =   1,953 MHz
//  rp_AcqSetSamplingRate(RP_SMP_122_070K);	// :1024  = 122,070 kHz
//  rp_AcqSetSamplingRate(RP_SMP_15_258K);	// :8192  =  15,258 kHz
//  rp_AcqSetSamplingRate(RP_SMP_1_907K);		// :65536 =   1,907 kHz
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
  
  // Init the Position of Write-Pointer - Start everything from here
  rp_AcqGetWritePointer(&wrpointer);
  i         = 0;
  j	    = 0;
  lastrdpt  = wrpointer;
  lastwrpt  = wrpointer;
  do
  {  
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

      if(distance > 65000)	// Overflow detection
        printf("Überlauf: %u \n",distance);

      space    = (MAXRX - i);	// Space in RxBuffer
      count    = space ;
        
      if(space < (distance + 1)) // RxBuffer at the end ?
        endpos = ((startpos + space) % (16*1024)) -1;
        
//printf("Debug1: pter: %u start: %u ende: %u distance:%d \n",wrpointer,startpos,endpos,distance);

      // Read ADC-Buffer
      rp_AcqGetDataPosRaw(RP_CH_1,startpos,endpos,&RxBuffer[i],&count);
//printf("Debug2: count: %i: %u \n",count,i);

      lastrdpt = ((startpos + count) % (16*1024));	// Store last Read Data	
      i = (i + count);					// Store the RxBuffer-position.
    }

  }while(i < MAXRX);  // Stop at full RxBuffer

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