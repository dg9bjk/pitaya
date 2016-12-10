#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <rp.h>

// Globale Variablen:
float frequency1		= 3575000.0; // Betriebsfrequenz in Hz (max. 60MHz)
float amplitude1_on		= 1.0;	     // Pegel bei Signalausgabe in Vpp (max. 1.0 Vpp)
float amplitude1_off		= 0.0;	     // Pegel bei Ruheausgabe 

int CWSpeed			= 100;	     // Basislänge (Punkt) in ms

//-------------------------------------------------------------------
void sigcode(int Bitinfo)
{
  switch(Bitinfo)
  {
    case '-': 
            {	// long 
            rp_GenAmp(RP_CH_1,amplitude1_on);
            usleep(CWSpeed * 3000);
            rp_GenAmp(RP_CH_1,amplitude1_off);
            usleep(CWSpeed * 1000);
            }
            break;

    case '.': 
            { // short
            rp_GenAmp(RP_CH_1,amplitude1_on);
            usleep(CWSpeed * 1000);
            rp_GenAmp(RP_CH_1,amplitude1_off);
            usleep(CWSpeed * 1000);
            }
            break;

    case ' ': 
            {	// silence
            rp_GenAmp(RP_CH_1,amplitude1_off);
            usleep(CWSpeed * 9000);
            }
            break;
  }
}

//-------------------------------------------------------------------
int main()
{
  int i,n;
  int Bitinfo   = 0; 
  char input	= 0;
  char *aktchar;
  char TextArray[] = "Automatic Generated Red Pitaya Test Bake de NOCALL";
  time_t timestamp;
  struct tm *ts;

  unsigned char *Zeichencode[127];
  
  // . = Kurz ; - = Lang
  char A[]     ={'.','-',0};
  char B[]     ={'-','.','.','.',0};
  char C[]     ={'-','.','-','.',0};
  char D[]     ={'-','.','.',0};
  char E[]     ={'.',0};
  char F[]     ={'.','.','-','.',0};
  char G[]     ={'-','-','.',0};
  char H[]     ={'.','.','.','.',0};
  char I[]     ={'.','.',0};
  char J[]     ={'.','-','-','-',0};
  char K[]     ={'-','.','-',0};
  char L[]     ={'.','-','.','.',0};
  char M[]     ={'-','-',0};
  char N[]     ={'-','.',0};
  char O[]     ={'-','-','-',0};
  char P[]     ={'.','-','-','.',0};
  char Q[]     ={'-','-','.','-',0};
  char R[]     ={'.','-','.',0};
  char S[]     ={'.','.','.',0};
  char T[]     ={'-',0};
  char U[]     ={'.','.','-',0};
  char V[]     ={'.','.','.','-',0};
  char W[]     ={'.','-','-',0};
  char X[]     ={'-','.','.','-',0};
  char Y[]     ={'-','.','-','-',0};
  char Z[]     ={'-','-','.','.',0};
  char Zero[]  ={'-','-','-','-','-',0};
  char Eins[]  ={'.','-','-','-','-',0};
  char Zwei[]  ={'.','.','-','-','-',0};
  char Drei[]  ={'.','.','.','-','-',0};
  char Vier[]  ={'.','.','.','.','-',0};
  char Funf[]  ={'.','.','.','.','.',0};
  char Sechs[] ={'-','.','.','.','.',0};
  char Sieben[]={'-','-','.','.','.',0};
  char Acht[]  ={'-','-','-','.','.',0};
  char Neun[]  ={'-','-','-','-','.',0};

  char Space[]   = {' ',0};
  char Punkt[]   = {'.','-','.','-','.','-',0};
  char Komma[]   = {'-','-','.','.','-','-',0};
  char DPunkt[]  = {'-','-','-','.','.','.',0};
  char Simikol[] = {'-','.','-','.','-','.',0};
  char Frage[]   = {'.','.','-','-','.','.',0};
  char Strich[]  = {'-','.','.','.','.','-',0};
  char Ustrich[] = {'.','.','-','-','.','-',0};
  char KlamAuf[] = {'-','.','-','-','.',0};
  char KlamZu[]  = {'-','.','-','-','.','-',0};
  char Apostro[] = {'.','-','-','-','-','.',0};
  char Gleich[]  = {'-','.','.','.','-',0};
  char Plus[]    = {'.','-','.','-','.',0};
  char Sstrich[] = {'-','.','.','-','.',0};
  char AT[]      = {'.','-','-','.','-','.',0};
  
  // Init Pointer-Array
  for(i=0;i<127;i++)
    Zeichencode[i] = NULL;

  Zeichencode[32] = Space;
  Zeichencode[46] = Punkt;
  Zeichencode[44] = Komma;
  Zeichencode[58] = DPunkt;
  Zeichencode[59] = Simikol;
  Zeichencode[63] = Frage;
  Zeichencode[45] = Strich;
  Zeichencode[95] = Ustrich;
  Zeichencode[40] = KlamAuf;
  Zeichencode[41] = KlamZu;
  Zeichencode[39] = Apostro;
  Zeichencode[61] = Gleich;
  Zeichencode[43] = Plus;
  Zeichencode[47] = Sstrich;
  Zeichencode[64] = AT;
      
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
 
  printf("\n CW-Bake \n");
  
  if(rp_Init() != RP_OK)
  {
    fprintf(stderr, "Init-Funktion failed\n\n");
  }

//-----------------------------------------------
//Generator CH 1
  float phase1			= 0.0;      // ° (-180.0° ... 0.0° ... 180.0°)
  rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(RP_CH_1,frequency1);
  rp_GenPhase(RP_CH_1,phase1);
  rp_GenAmp(RP_CH_1,amplitude1_off); // No Signal

  rp_GenOutEnable(RP_CH_1);

  timestamp = time(NULL);
  ts=localtime(&timestamp);
  printf("Zeit: %02d.%02d.%04d - %02d:%02d:%02d \n",ts->tm_mday,ts->tm_mon+1,ts->tm_year+1900,ts->tm_hour,ts->tm_min,ts->tm_sec);
  
  for(n=0;(n< sizeof(TextArray)) & (TextArray[n] != 0);n++)
  {
    input = TextArray[n];
    aktchar = Zeichencode[input];
    
    if(aktchar != 0)
    {
      for(i=0;(i< sizeof(aktchar)) & (aktchar[i] != 0);i++)
        sigcode(aktchar[i]);
      usleep(CWSpeed * 3000);  // Sleep in char
    }
  }

  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    
  // Funktionen disablen
  rp_Release();
  return(0);
}