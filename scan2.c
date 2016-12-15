#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <rp.h>

#define MAXRX (1*1024*1024) // Worte (16Bit)

// Globale Variablen
static uint16_t RxBuffer[MAXRX];	// Empfangsbuffer
static uint32_t RxBufferPos;		// Position im RXBuffer
static uint32_t lastwrpt;		// Zwischenspeicher alter Schreibepointer
static uint32_t lastrdpt;		// Zwischenspeicher alter Lesepointer

static struct sigaction sa1;		// Interrupt Handler
static struct itimerval timer1;		// Interrupt Steuerung
//------------------------------------------------
void getempty1(int signum1)
{
  ;
}
void getsignal1(int signum1)
{
  //ADC_BUFFER_SIZE;  // 16 * 1024 = 16384
  uint32_t distance	= 0;

  // Zeigerposition des Schreibzeigers
  rp_AcqGetWritePointer(&lastwrpt);
  
  if(lastwrpt < lastrdpt)	// went over the border ?
    distance = (lastwrpt + (ADC_BUFFER_SIZE - lastrdpt));
  else
    distance = (lastwrpt - lastrdpt);
  
  if((RxBufferPos + distance) > MAXRX)
    distance = MAXRX - RxBufferPos;
    
  // Read ADC-Buffer
  rp_AcqGetDataRaw(RP_CH_1, lastrdpt, &distance, &RxBuffer[RxBufferPos]);
  RxBufferPos = RxBufferPos + distance;
  lastrdpt = (lastrdpt + distance) % ADC_BUFFER_SIZE;	// Store last Read Data	
}

//------------------------------------------------
void TestGenerator(float freq)
{

  float frequency1		= 6000000.0; // Hz (max. 60MHz)
  float phase1			= 0.0;      // ° (-180.0° ... 0.0° ... 180.0°)
  float amplitude1		= 1.0;	    // Vpp (max. 1.0 Vpp)

  float frequency2		= 6000000.0; // Hz (max. 60MHz)
  float phase2			= 90.0;      // ° (-180.0° ... 0.0° ... 180.0°)
  float amplitude2		= 1.0;	    // Vpp (max. 1.0 Vpp)

  frequency1 = freq;
  frequency2 = freq;

  // Generator 1
  rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(RP_CH_1,frequency1);
  rp_GenPhase(RP_CH_1,phase1);
  rp_GenAmp(RP_CH_1,amplitude1);
  //rp_GenOutEnable(RP_CH_1);

  // Generator 2
  rp_GenMode(RP_CH_2, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(RP_CH_2, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(RP_CH_2,frequency2);
  rp_GenPhase(RP_CH_2,phase2);
  rp_GenAmp(RP_CH_2,amplitude2);
  rp_GenOutEnable(RP_CH_2);
}
//------------------------------------------------
int Samplespeed(int Samplerate)
{
int timerrate =0;
    
  switch(Samplerate)
  {
      case 1:	rp_AcqSetSamplingRate(RP_SMP_125M);		// :1     = 125,0   MHZ
                timerrate = 44;					// ms
            break;
      case 2:   rp_AcqSetSamplingRate(RP_SMP_15_625M);		// :8     =  15,625 MHz
                timerrate = 350;				// ms
            break;
      case 3:	rp_AcqSetSamplingRate(RP_SMP_1_953M);		// :64    =   1,953 MHz
                timerrate = 2796;				// ms
            break;
      case 4:	rp_AcqSetSamplingRate(RP_SMP_122_070K);		// :1024  = 122,070 kHz
                timerrate = 44734;				// ms
            break;
      case 5:	rp_AcqSetSamplingRate(RP_SMP_15_258K);		// :8192  =  15,258 kHz
                timerrate = 357667;				// ms
            break;
      case 6:	rp_AcqSetSamplingRate(RP_SMP_1_907K);		// :65536 =   1,907 kHz
                timerrate = 2863000;				// ms
            break;
  }
  return(timerrate);
}

//------------------------------------------------
int main()
{
  int i,j;
  int pos;
  int timerrate			= 0 ; // Sampleratenauswahl 1-6
  char display[101]		= ""; // Displayausgabe
  float Samplefreq		= 0 ; // Anzeige der Samplefrequenz

  printf("\nScanprogramm \n");
  
  if(rp_Init() != RP_OK)
  {
    fprintf(stderr, "Init-Funktion failed\n\n");
    return(-1);
  }
  // Init vom SignalSpeicher
  memset(RxBuffer,0x0,sizeof(RxBuffer));
  RxBufferPos = 0;
  
  // Einstellung der Samplefrequenz
  timerrate = Samplespeed(4);

  // Ausgabe der Samplerate  
  rp_AcqGetSamplingRateHz(&Samplefreq);
  printf("Samplefrequenz: %.3f kHz \n",Samplefreq/1000.0);

//  TestGenerator(Samplefreq / 12); // Testsignal ein

  // Triggersignal für Erfassung (hier: Sofort)
  rp_AcqSetTriggerSrc(RP_TRIG_SRC_DISABLED);

  // Level des Einganges
  rp_AcqSetGain(RP_CH_1,RP_HIGH);
  rp_AcqSetGain(RP_CH_2,RP_HIGH);

  // Start der Erfassung der Messwerte
  rp_AcqSetArmKeep(true);
  rp_AcqStart();

  // Signal auslesen über Timer
  memset(&sa1,0,sizeof(sa1));
  sigemptyset(&sa1.sa_mask);
  sa1.sa_handler = &getsignal1;
  sigaction(SIGALRM,&sa1,NULL);
  
  timer1.it_value.tv_sec     = 0;
  timer1.it_value.tv_usec    = timerrate;
  timer1.it_interval.tv_sec  = 0;
  timer1.it_interval.tv_usec = timerrate;
  setitimer(ITIMER_REAL,&timer1,NULL);

  // Init the Position of Read-Pointer - Start everything from here
  rp_AcqGetWritePointer(&lastrdpt);

//-----------------------
// Ausgabe des Puffers
  #define SCALA 256
  while(RxBufferPos<MAXRX);	// Warte bis voll
  
   for(i=0;i<MAXRX; ++i)
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
  }

  // Leerer Interrput -> sonst Sigfault
  memset(&sa1,0,sizeof(sa1));
  sigemptyset(&sa1.sa_mask);
  sa1.sa_handler = &getempty1;
  sigaction(SIGALRM,&sa1,NULL);

  timer1.it_value.tv_sec     = 0;
  timer1.it_value.tv_usec    = 1000000;
  timer1.it_interval.tv_sec  = 0;
  timer1.it_interval.tv_usec = 1000000;
  setitimer(ITIMER_REAL,&timer1,NULL);

  // ADC-Stop
  rp_AcqStop();

  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    

  // Funktionen disablen
  rp_Release();
  return(0);
}