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
  char input	= 0;
  time_t timestamp;
  struct tm *ts;

  unsigned char *Zeichencode[127];
  
  // 0 = Kurz ; 1 = Lang
  char A[]     ={0,1};
  char B[]     ={1,0,0,0};
  char C[]     ={1,0,1,0};
  char D[]     ={1,0,0};
  char E[]     ={0};
  char F[]     ={0,0,1,0};
  char G[]     ={1,1,0};
  char H[]     ={0,0,0,0};
  char I[]     ={0,0};
  char J[]     ={0,1,1,1};
  char K[]     ={1,0,1};
  char L[]     ={0,1,0,0};
  char M[]     ={1,1};
  char N[]     ={1,0};
  char O[]     ={1,1,1};
  char P[]     ={0,1,1,0};
  char Q[]     ={1,1,0,1};
  char R[]     ={0,1,0};
  char S[]     ={0,0,0};
  char T[]     ={1};
  char U[]     ={0,0,1};
  char V[]     ={0,0,0,1};
  char W[]     ={0,1,1};
  char X[]     ={1,0,0,1};
  char Y[]     ={1,0,1,1};
  char Z[]     ={1,1,0,0};
  char Zero[]  ={1,1,1,1,1};
  char Eins[]  ={0,1,1,1,1};
  char Zwei[]  ={0,0,1,1,1};
  char Drei[]  ={0,0,0,1,1};
  char Vier[]  ={0,0,0,0,1};
  char Funf[]  ={0,0,0,0,0};
  char Sechs[] ={1,0,0,0,0};
  char Sieben[]={1,1,0,0,0};
  char Acht[]  ={1,1,1,0,0};
  char Neun[]  ={1,1,1,1,0};
  
  // Init Pointer-Array
  for(i=0;i<127;i++)
    Zeichencode[i] = NULL;
    
  Zeichencode[48] = Zero;
  Zeichencode[49] = Eins;
  Zeichencode[50] = Zwei;
  Zeichencode[51] = Drei;
  Zeichencode[52] = Vier;
  Zeichencode[53] = Funf;
  Zeichencode[54] = Sechs;
  Zeichencode[55] = Sieben;
  Zeichencode[56] = Acht;
  Zeichencode[57] = Neun;

  Zeichencode[65]  = A;
  Zeichencode[97]  = A;
  Zeichencode[66]  = B;
  Zeichencode[98]  = B;
  Zeichencode[67]  = C;
  Zeichencode[99]  = C;
  Zeichencode[68]  = D;
  Zeichencode[100] = D;
  Zeichencode[69]  = E;
  Zeichencode[101] = E;
  Zeichencode[70]  = F;
  Zeichencode[102] = F;
  Zeichencode[71]  = G;
  Zeichencode[103] = G;
  Zeichencode[72]  = H;
  Zeichencode[104] = H;
  Zeichencode[73]  = I;
  Zeichencode[105] = I;
  Zeichencode[74]  = J;
  Zeichencode[106] = J;
  Zeichencode[75]  = K;
  Zeichencode[107] = K;
  Zeichencode[76]  = L;
  Zeichencode[108] = L;
  Zeichencode[77]  = M;
  Zeichencode[109] = M;
  Zeichencode[78]  = N;
  Zeichencode[110] = N;
  Zeichencode[79]  = O;
  Zeichencode[111] = O;
  Zeichencode[80]  = P;
  Zeichencode[112] = P;
  Zeichencode[81]  = Q;
  Zeichencode[113] = Q;
  Zeichencode[82]  = R;
  Zeichencode[114] = R;
  Zeichencode[83]  = S;
  Zeichencode[115] = S;
  Zeichencode[84]  = T;
  Zeichencode[116] = T;
  Zeichencode[85]  = U;
  Zeichencode[117] = U;
  Zeichencode[86]  = V;
  Zeichencode[118] = V;
  Zeichencode[87]  = W;
  Zeichencode[119] = W;
  Zeichencode[88]  = X;
  Zeichencode[120] = X;
  Zeichencode[89]  = Y;
  Zeichencode[121] = Y;
  Zeichencode[90]  = Z;
  Zeichencode[122] = Z;
 
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
    input = getch();
    
    aktchar = *Zeichencode[input];
    
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