#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <rp.h>

// Globale Variablen
float frequency1		= 3575000.0; // Hz (max. 60MHz)
float amplitude1		= 1.0;	    // Vpp (max. 1.0 Vpp)


//-----------------------------------------------
int parity(int value)
{
  int count = 0;
  int maske = 1;
  int i;

  for(i=0;i<8;i++)
  {
    if(value && maske)
      count++;
    maske = maske << 1;
  }  
    
  if(count % 2)
    return(1);
  else
    return(0);
}

//-----------------------------------------------
void sigcode(int Bitinfo)
{

      if(Bitinfo)
      {	// long Pause
        printf(" 1\n");
        rp_GenAmp(RP_CH_1,amplitude1 * 0.2);
        usleep(200000);
        rp_GenAmp(RP_CH_1,amplitude1);
        usleep(800000);
      }
      else
      {
        printf(" 0\n");
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
  int Sommerzeit= 0;
  int parity1	= 0;
  int parity2	= 0;
  int parity3	= 0;
  int parity4	= 0;
  int paritysum	= 0;

  
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
    printf("Aktuelle Zeit     :%02d, %02d.%02d.%04d - %02d:%02d:%02d,%1d \n",ts->tm_wday,ts->tm_mday,ts->tm_mon+1,ts->tm_year+1900,ts->tm_hour,ts->tm_min,ts->tm_sec,ts->tm_isdst);
    if(ts->tm_wday == 0) // Sonntag ist = 7 in DL.
      ts->tm_wday = 7;

    if(ts->tm_min < 59)
    {
      ts->tm_min = ts->tm_min +1;
    }
    else
    {
      ts->tm_min = 0;
      ts->tm_hour = ts->tm_hour +1;
    }
    printf("Übertrage Zeit für:%02d, %02d.%02d.%04d - %02d:%02d:%02d,%1d \n",ts->tm_wday,ts->tm_mday,ts->tm_mon+1,ts->tm_year+1900,ts->tm_hour,ts->tm_min,ts->tm_sec,ts->tm_isdst);

    for (i=0;i<59;i++)
    {
      printf("Step: %02d =",i);
      switch(i)
      {
        case 0:	 sigcode(0); // Immer 0
                break;
        case 16: sigcode(Sommerzeit); // MEZ-MESZ Umstellung in dieser Stunde
                break;
        case 17: sigcode(ts->tm_isdst); // MESZ aktiv
                break;
        case 18: sigcode(! ts->tm_isdst); // MEZ aktiv
                break;
        case 19: sigcode(SwitchSec); // Schaltsekunde am Ende der Stunde
                break;
        case 20: sigcode(1); // Beginn Zeitinformation immer 1
                break;
        case 21: sigcode(ts->tm_min & 0x1); // Minute Bit für 1
                break;
        case 22: sigcode(ts->tm_min & 0x2); // Minute Bit für 2
                break;
        case 23: sigcode(ts->tm_min & 0x4); // Minute Bit für 4
                break;
        case 24: sigcode(ts->tm_min & 0x8); // Minute Bit für 8
                break;
        case 25: sigcode(ts->tm_min & 0x10); // Minute Bit für 10
                break;
        case 26: sigcode(ts->tm_min & 0x20); // Minute Bit für 20
                break;
        case 27: sigcode(ts->tm_min & 0x40); // Minute Bit für 40
                break;
        case 28: sigcode(parity(ts->tm_min)); // Parität Minute
                break;
        case 29: sigcode(ts->tm_hour & 0x1); // Stunde Bit für 1
                break;
        case 30: sigcode(ts->tm_hour & 0x2); // Stunde Bit für 2
                break;
        case 31: sigcode(ts->tm_hour & 0x4); // Stunde Bit für 4
                break;
        case 32: sigcode(ts->tm_hour & 0x8); // Stunde Bit für 8
                break;
        case 33: sigcode(ts->tm_hour & 0x10); // Stunde Bit für 10
                break;
        case 34: sigcode(ts->tm_hour & 0x20); // Stunde Bit für 20
                break;
        case 35: sigcode(parity(ts->tm_hour)); // Parität Stunde
                break;
        case 36: sigcode(ts->tm_mday & 0x1); // Kalendertag Bit für 1
                break;
        case 37: sigcode(ts->tm_mday & 0x2); // Kalendertag Bit für 2
                break;
        case 38: sigcode(ts->tm_mday & 0x4); // Kalendertag Bit für 4
                break;
        case 39: sigcode(ts->tm_mday & 0x8); // Kalendertag Bit für 8
                break;
        case 40: sigcode(ts->tm_mday & 0x10); // Kalendertag Bit für 10
                break;
        case 41: sigcode(ts->tm_mday & 0x20); // Kalendertag Bit für 20
                break;
        case 42: sigcode(ts->tm_wday & 0x1); // Wochentag Bit für 1
                break;
        case 43: sigcode(ts->tm_wday & 0x2); // Wochentag Bit für 2
                break;
        case 44: sigcode(ts->tm_wday & 0x4); // Wochentag Bit für 4
                break;
        case 45: sigcode(ts->tm_mon & 0x1); // Monat Bit für 1
                break;
        case 46: sigcode(ts->tm_mon & 0x2); // Monat Bit für 2
                break;
        case 47: sigcode(ts->tm_mon & 0x4); // Monat Bit für 4
                break;
        case 48: sigcode(ts->tm_mon & 0x8); // Monat Bit für 8
                break;
        case 49: sigcode(ts->tm_mon & 0x10); // Monat Bit für 10
                break;
        case 50: sigcode(ts->tm_year+1900 & 0x1); // Jahr Bit für 1
                break;
        case 51: sigcode(ts->tm_year+1900 & 0x2); // Jahr Bit für 2
                break;
        case 52: sigcode(ts->tm_year+1900 & 0x4); // Jahr Bit für 4
                break;
        case 53: sigcode(ts->tm_year+1900 & 0x8); // Jahr Bit für 8
                break;
        case 54: sigcode(ts->tm_year+1900 & 0x10); // Jahr Bit für 10
                break;
        case 55: sigcode(ts->tm_year+1900 & 0x20); // Jahr Bit für 20
                break;
        case 56: sigcode(ts->tm_year+1900 & 0x40); // Jahr Bit für 40
                break;
        case 57: sigcode(ts->tm_year+1900 & 0x80); // Jahr Bit für 80
                break;
        case 58: parity1 = parity(ts->tm_mday);
                 parity2 = parity(ts->tm_wday);
                 parity3 = parity(ts->tm_mon);
                 parity4 = parity(ts->tm_year+1900);
                 paritysum = (parity1 + parity2 + parity3 + parity4) % 2 ;
                 sigcode(paritysum); // Parität Datum
                break;

        default: sigcode(0);
                break;
      }
    }
    if(SwitchSec & 0) 		// if we habe 61 Seconds (Switchseconds)
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