#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <rp.h>

// Globale Variablen:
float frequency1		= 3575000.0; // Betriebsfrequenz (Mittenfrequenz) in Hz (max. 60MHz)
float shift			= 170.0;     // Shift in Hz	
float amplitude1		= 1.0;	     // Pegel bei Signalausgabe in Vpp (max. 1.0 Vpp)

int speed			= 6;	     // Zeichenlänge in  ms 
// RTTY  45 =  45.45 Bd = Speed 
// RTTY  50 =  50.00 Bd = Speed 
// RTTY  75 =  75.00 Bd = Speed 
// RTTY 100 = 100.00 Bd = Speed 
//-------------------------------------------------------------------
void sigcode(int Bitinfo)
{
  switch(Bitinfo)
  {
    case '0': 
            {	// Space
            rp_GenFreq(RP_CH_1,frequency1 - shift);
            usleep(speed * 1000);
            }
            break;

    case '1': 
            { // Mark
            rp_GenFreq(RP_CH_1,frequency1);
            usleep(speed * 1000);
            }
            break;
    case '>': 
            {	// Space - start
            rp_GenFreq(RP_CH_1,frequency1 - shift);
            usleep(speed * 1000);
            }
            break;

    case '<': 
            { // Mark - stop
            rp_GenFreq(RP_CH_1,frequency1);
            usleep(speed * 1000 * 4);
            }
            break;
  }
}

//-------------------------------------------------------------------
int main()
{
  int i,n;
  int Bitinfo     = 0; 
  int Zeichenmode = 0;
  int Zahlenmode  = 0;
  char input	  = 0;
  char *aktchar;
  char *st_char;
  char TextArray[] = "ryryryryryry Automatic Generated Red Pitaya Test Bake de DG9BJK ryryryryry";
//  char TextArray[] = "ryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryryry";
  time_t timestamp;
  struct tm *ts;

  unsigned char *Zeichencode[127];
  
  // 1=Mark, 0=Space
  char A[]       ={'B','>','1','1','0','0','0','<',0};
  char B[]       ={'B','>','1','0','0','1','1','<',0};
  char C[]       ={'B','>','0','1','1','1','0','<',0};
  char D[]       ={'B','>','1','0','0','1','0','<',0};
  char E[]       ={'B','>','1','0','0','0','0','<',0};
  char F[]       ={'B','>','1','0','1','1','0','<',0};
  char G[]       ={'B','>','0','1','0','1','1','<',0};
  char H[]       ={'B','>','0','0','1','0','1','<',0};
  char I[]       ={'B','>','0','1','1','0','0','<',0};
  char J[]       ={'B','>','1','1','0','1','0','<',0};
  char K[]       ={'B','>','1','1','1','1','0','<',0};
  char L[]       ={'B','>','0','1','0','0','1','<',0};
  char M[]       ={'B','>','0','0','1','1','1','<',0};
  char N[]       ={'B','>','0','0','1','1','0','<',0};
  char O[]       ={'B','>','0','0','0','1','1','<',0};
  char P[]       ={'B','>','0','1','1','0','1','<',0};
  char Q[]       ={'B','>','1','1','1','0','1','<',0};
  char R[]       ={'B','>','0','1','0','1','0','<',0};
  char S[]       ={'B','>','1','0','1','0','0','<',0};
  char T[]       ={'B','>','0','0','0','0','1','<',0};
  char U[]       ={'B','>','1','1','1','0','0','<',0};
  char V[]       ={'B','>','0','1','1','1','1','<',0};
  char W[]       ={'B','>','1','1','0','0','1','<',0};
  char X[]       ={'B','>','1','0','1','1','1','<',0};
  char Y[]       ={'B','>','1','0','1','0','1','<',0};
  char Z[]       ={'B','>','1','0','0','0','1','<',0};

  char Zero[]    ={'Z','>','0','1','1','0','1','<',0};
  char Eins[]    ={'Z','>','1','1','1','0','1','<',0};
  char Zwei[]    ={'Z','>','1','1','0','0','1','<',0};
  char Drei[]    ={'Z','>','1','0','0','0','0','<',0};
  char Vier[]    ={'Z','>','0','1','0','1','0','<',0};
  char Funf[]    ={'Z','>','0','0','0','0','1','<',0};
  char Sechs[]   ={'Z','>','1','0','1','0','1','<',0};
  char Sieben[]  ={'Z','>','1','1','1','0','0','<',0};
  char Acht[]    ={'Z','>','0','1','1','0','0','<',0};
  char Neun[]    ={'Z','>','0','0','0','1','1','<',0};
  char Punkt[]   ={'Z','>','0','0','1','1','1','<',0};
  char Komma[]   ={'Z','>','0','0','1','1','0','<',0};
  char DPunkt[]  ={'Z','>','0','1','1','1','0','<',0};
  char Frage[]   ={'Z','>','1','0','0','1','1','<',0};
  char Strich[]  ={'Z','>','1','1','0','0','0','<',0};
  char Sstrich[] ={'Z','>','1','0','1','1','1','<',0};
  char KlamAuf[] ={'Z','>','1','1','1','1','0','<',0};
  char KlamZu[]  ={'Z','>','0','1','0','0','1','<',0};
  char Apostro[] ={'Z','>','1','0','1','0','0','<',0};
  char Gleich[]  ={'Z','>','0','1','1','1','1','<',0};
  char Plus[]    ={'Z','>','1','0','0','0','1','<',0};
  char Bell[]    ={'Z','>','1','1','0','1','0','<',0};
  char WerDa[]   ={'Z','>','1','0','0','1','0','<',0};

  char Space[]   ={'N','>','0','0','1','0','0','<',0};
  char St_CR[]   ={'N','>','0','0','0','1','0','<',0};
  char St_NL[]   ={'N','>','0','1','0','0','0','<',0};
  char St__B[]   ={'N','>','1','1','1','1','1','<',0};
  char St__Z[]   ={'N','>','1','1','0','1','1','<',0};
  char St__K[]   ={'N','>','0','0','0','0','0','<',0};
  
  // Init Pointer-Array
  for(i=0;i<127;i++)
    Zeichencode[i] = NULL;

  Zeichencode[32] = Space;
  Zeichencode[13] = St_CR;
  Zeichencode[10] = St_NL;
  Zeichencode[07] = Bell;
  Zeichencode[22] = WerDa;

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
  Zeichencode[46] = Punkt;
  Zeichencode[44] = Komma;
  Zeichencode[58] = DPunkt;
  Zeichencode[63] = Frage;
  Zeichencode[45] = Strich;
  Zeichencode[47] = Sstrich;
  Zeichencode[40] = KlamAuf;
  Zeichencode[41] = KlamZu;
  Zeichencode[39] = Apostro;
  Zeichencode[61] = Gleich;
  Zeichencode[43] = Plus;

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
 
  printf("\n RTTY-Bake \n");
  
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

  rp_GenOutEnable(RP_CH_1);
  sleep(1);
//while(1)
{  
  timestamp = time(NULL);
  ts=localtime(&timestamp);
  printf("Zeit: %02d.%02d.%04d - %02d:%02d:%02d \n",ts->tm_mday,ts->tm_mon+1,ts->tm_year+1900,ts->tm_hour,ts->tm_min,ts->tm_sec);
  
  for(n=0;(n< strlen(TextArray)) & (TextArray[n] != 0);n++)
  {
    input = TextArray[n];
    aktchar = Zeichencode[input];
    
    if(aktchar != 0)
    {
      if((((!Zahlenmode) & (!Zeichenmode)) | Zahlenmode) & (aktchar[0] == 'B'))
      {
//        printf("Debug Bmode\n");
        st_char = St__B;
        for(i=1;(i< strlen(st_char)) & (st_char[i] != 0);i++)
        {
          sigcode(st_char[i]); // ST__B
        }
        Zahlenmode  = 0;
        Zeichenmode = 1;
      }
      
      if((((!Zahlenmode) & (!Zeichenmode)) | Zeichenmode) & (aktchar[0] == 'Z'))
      {
//        printf("Debug Zmode\n");
        st_char = St__Z;
        for(i=1;(i< strlen(st_char)) & (st_char[i] != 0);i++)
        {
          sigcode(st_char[i]); // ST__Z
        }
        Zahlenmode  = 1;
        Zeichenmode = 0;
      }
      
      for(i=1;(i< strlen(aktchar)) & (aktchar[i] != 0);i++)
      {
//        printf("Debug: %c\n",aktchar[i]);
        sigcode(aktchar[i]);
      }
    }
  }
  sleep(1);
}  
  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    
  // Funktionen disablen
  rp_Release();
  return(0);
}