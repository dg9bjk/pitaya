#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <rp.h>

// Globale Variablen:
float frequency1		= 3575000.0; // Betriebsfrequenz (Mittenfrequenz) in Hz (max. 60MHz)
float amplitude1		= 1.0;	     // Pegel bei Signalausgabe in Vpp (max. 1.0 Vpp)
float shift			= 170.0	     // Frequenzversatz in Hz	
int speed			= 32;	     // Zeichenlänge in  ms 
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
            rp_GenFreq(RP_CH_1,frequency1 + shift);
            usleep(speed * 1000);
            }
            break;
  }
}

//-------------------------------------------------------------------
int main()
{
  int i,n;
  int Bitinfo     = 0; 
  char input	  = 0;
  char *aktchar;
  char TextArray[] = "Automatic Generated Red Pitaya Test Bake de DG9BJK";
  time_t timestamp;
  struct tm *ts;

  unsigned char *Zeichencode[127];
  
  // 1=Mark, 0=Space
  char St_NUL[]   ={'1','0','1','0','1','0','1','0','1','1',0}; // 1010 1010 11
  char St_SOH[]   ={'1','0','1','1','0','1','1','0','1','1',0}; // 1011 0110 11
  char St_STX[]   ={'1','0','1','1','1','0','1','1','0','1',0}; // 1011 1011 01
  char St_ETX[]   ={'1','1','0','1','1','1','0','1','1','1',0}; // 1101 1101 11
  char St_EOT[]   ={'1','0','1','1','1','0','1','0','1','1',0}; // 1011 1010 11
  char St_ENQ[]   ={'1','1','0','1','0','1','1','1','1','1',0}; // 1101 0111 11
  char St_ACK[]   ={'1','0','1','1','1','0','1','1','1','1',0}; // 1011 1011 11
  char St_BEL[]   ={'1','0','1','1','1','1','1','1','0','1',0}; // 1011 1111 01
  char St_BS[]    ={'1','0','1','1','1','1','1','1','1','1',0}; // 1011 1111 11
  char St_HT[]    ={'1','1','1','0','1','1','1','1',0}; // 1110 1111
  char St_LF[]    ={'1','1','1','0','1',0}; // 1110 1
  char St_VT[]    ={'1','1','0','1','1','0','1','1','1','1',0}; // 1101 1011 11
  char St_FF[]    ={'1','0','1','1','0','1','1','1','0','1',0}; // 1011 0111 01
  char St_CR[]    ={'1','1','1','1','1',0}; // 1111 1
  char St_SO[]    ={'1','1','0','1','1','1','0','1','0','1',0}; // 1101 1101 01
  char St_SI[]    ={'1','1','1','0','1','0','1','0','1','1',0}; // 1110 1010 11
  char St_DLE[]   ={'1','0','1','1','1','1','0','1','1','1',0}; // 1011 1101 11
  char St_DC1[]   ={'1','0','1','1','1','1','0','1','0','1',0}; // 1011 1101 01
  char St_DC2[]   ={'1','1','1','0','1','0','1','1','0','1',0}; // 1110 1011 01
  char St_DC3[]   ={'1','1','1','0','1','0','1','1','1','1',0}; // 1110 1011 11
  char St_DC4[]   ={'1','1','0','1','0','1','1','0','1','1',0}; // 1101 0110 11
  char St_NAK[]   ={'1','1','0','1','1','0','1','0','1','1',0}; // 1101 1010 11
  char St_SYN[]   ={'1','1','0','1','1','0','1','1','0','1',0}; // 1101 1011 01
  char St_ETB[]   ={'1','1','0','1','0','1','0','1','1','1',0}; // 1101 0101 11
  char St_CAN[]   ={'1','1','0','1','1','1','1','0','1','1',0}; // 1101 1110 11
  char St_EM[]    ={'1','1','0','1','1','1','1','1','0','1',0}; // 1101 1111 01
  char St_SUB[]   ={'1','1','1','0','1','1','0','1','1','1',0}; // 1110 1101 11
  char St_ESC[]   ={'1','1','0','1','0','1','0','1','0','1',0}; // 1101 0101 01
  char St_FS[]    ={'1','1','0','1','0','1','1','1','0','1',0}; // 1101 0111 01
  char St_GS[]    ={'1','1','1','0','1','1','1','0','1','1',0}; // 1110 1110 11
  char St_RS[]    ={'1','0','1','1','1','1','1','0','1','1',0}; // 1011 1110 11
  char St_US[]    ={'1','1','0','1','1','1','1','1','1','1',0}; // 1101 1111 11
  char Space[]    ={'1',0};		    // 1
  char Ausruf[]   ={'1','1','1','1','1','1','1','1','1',0}; // 1111 1111 1
  char DApostro[] ={'1','0','1','0','1','1','1','1','1',0}; // 1010 1111 1
  char Raute[]    ={'1','1','1','1','1','0','1','0','1',0}; // 1111 1010 1
  char Dollar[]   ={'1','1','1','0','1','1','0','1','1',0}; // 1110 1101 1
  char Prozent[]  ={'1','0','1','1','0','1','0','1','0','1',0}; // 1011 0101 01
  char Kaufund[]  ={'1','0','1','0','1','1','1','0','1','1',0}; // 1010 1110 11
  char Apostro[]  ={'1','0','1','1','1','1','1','1','1',0}; // 1011 1111 1
  char KlamAuf[]  ={'1','1','1','1','1','0','1','1',0}; // 1111 1011
  char KlamZu[]   ={'1','1','1','1','0','1','1','1',0}; // 1111 0111
  char Stern[]    ={'1','0','1','1','0','1','1','1','1',0}; // 1011 0111 1
  char Plus[]     ={'1','1','1','0','1','1','1','1','1',0}; // 1110 1111 1
  char Komma[]    ={'1','1','1','0','1','0','1',0}; // 1110 101
  char Strich[]   ={'1','1','0','1','0','1',0}; // 1101 01
  char Punkt[]    ={'1','0','1','0','1','1','1',0}; // 1010 111
  char Sstrich[]  ={'1','1','0','1','0','1','1','1','1',0}; // 1101 0111 1
  char Zero[]     ={'1','0','1','1','0','1','1','1',0}; // 1011 0111
  char Eins[]     ={'1','0','1','1','1','1','0','1',0}; // 1011 1101
  char Zwei[]     ={'1','1','1','0','1','1','0','1',0}; // 1110 1101
  char Drei[]     ={'1','1','1','1','1','1','1','1',0}; // 1111 1111
  char Vier[]     ={'1','0','1','1','1','0','1','1','1',0}; // 1011 1011 1
  char Funf[]     ={'1','0','1','0','1','1','0','1','1',0}; // 1010 1101 1
  char Sechs[]    ={'1','0','1','1','0','1','0','1','1',0}; // 1011 0101 1
  char Sieben[]   ={'1','1','0','1','0','1','1','0','1',0}; // 1101 0110 1
  char Acht[]     ={'1','1','0','1','0','1','0','1','1',0}; // 1101 0101 1
  char Neun[]     ={'1','1','0','1','1','0','1','1','1',0}; // 1101 1011 1
  char DPunkt[]   ={'1','1','1','1','0','1','0','1',0}; // 1111 0101
  char Simikol[]  ={'1','1','0','1','1','1','1','0','1',0}; // 1101 1110 1
  char PfeilAuf[] ={'1','1','1','1','0','1','1','0','1',0}; // 1111 0110 1
  char Gleich[]   ={'1','0','1','0','1','0','1',0}; // 1010 101
  char PfeilZu[]  ={'1','1','1','0','1','0','1','1','1',0}; // 1110 1011 1
  char Frage[]    ={'1','0','1','0','1','0','1','1','1','1',0}; // 1010 1011 11
  char AT[]       ={'1','0','1','0','1','1','1','1','0','1',0}; // 1010 1111 01
  char gA[]       ={'1','1','1','1','1','0','1',0}; // 1111 101
  char gB[]       ={'1','1','1','0','1','0','1','1',0}; // 1110 1011
  char gC[]       ={'1','0','1','0','1','1','0','1',0}; // 1010 1101
  char gD[]       ={'1','0','1','1','0','1','0','1',0}; // 1011 0101
  char gE[]       ={'1','1','1','0','1','1','1',0}; // 1110 111
  char gF[]       ={'1','1','0','1','1','0','1','1',0}; // 1101 1011
  char gG[]       ={'1','1','1','1','1','1','0','1',0}; // 1111 1101
  char gH[]       ={'1','0','1','0','1','0','1','0','1',0}; // 1010 1010 1
  char gI[]       ={'1','1','1','1','1','1','1',0}; // 1111 111
  char gJ[]       ={'1','1','1','1','1','1','1','0','1',0}; // 1111 1110 1
  char gK[]       ={'1','0','1','1','1','1','1','0','1',0}; // 1011 1110 1
  char gL[]       ={'1','1','0','1','0','1','1','1',0}; // 1101 0111
  char gM[]       ={'1','0','1','1','1','0','1','1',0}; // 1011 1011
  char gN[]       ={'1','1','0','1','1','1','0','1',0}; // 1101 1101
  char gO[]       ={'1','0','1','0','1','0','1','1',0}; // 1010 1011
  char gP[]       ={'1','1','0','1','0','1','0','1',0}; // 1101 0101
  char gQ[]       ={'1','1','1','0','1','1','1','0','1',0}; // 1110 1110 1
  char gR[]       ={'1','0','1','0','1','1','1','1',0}; // 1010 1111
  char gS[]       ={'1','1','0','1','1','1','1',0}; // 1101 111
  char gT[]       ={'1','1','0','1','1','0','1',0}; // 1101 101
  char gU[]       ={'1','0','1','0','1','0','1','1','1',0}; // 1010 1011 1
  char gV[]       ={'1','1','0','1','1','0','1','0','1',0}; // 1101 1010 1
  char gW[]       ={'1','0','1','0','1','1','1','0','1',0}; // 1010 1110 1
  char gX[]       ={'1','0','1','1','1','0','1','0','1',0}; // 1011 1010 1
  char gY[]       ={'1','0','1','1','1','1','0','1','1',0}; // 1011 1101 1
  char gZ[]       ={'1','0','1','0','1','0','1','1','0','1',0}; // 1010 1011 01
  char EKlamAuf[] ={'1','1','1','1','1','0','1','1','1',0}; // 1111 1011 1
  char BackSlach[]={'1','1','1','1','0','1','1','1','1',0}; // 1111 0111 1
  char EKlamZu[]  ={'1','1','1','1','1','1','0','1','1',0}; // 1111 1101 1
  char Dach[]     ={'1','0','1','0','1','1','1','1','1','1',0}; // 1010 1111 11
  char UStrich[]  ={'1','0','1','1','0','1','1','0','1',0}; // 1011 0110 1
  char BackApro[] ={'1','0','1','1','0','1','1','1','1','1',0}; // 1011 0111 11
  char ka[]       ={'1','0','1','1',0}; // 1011
  char kb[]       ={'1','0','1','1','1','1','1',0}; // 1011 111
  char kc[]       ={'1','0','1','1','1','1',0}; // 1011 11
  char kd[]       ={'1','0','1','1','0','1',0}; // 1011 01
  char ke[]       ={'1','1',0}; // 11
  char kf[]       ={'1','1','1','1','0','1',0}; // 1111 01
  char kg[]       ={'1','0','1','1','0','1','1',0}; // 1011 011
  char kh[]       ={'1','0','1','0','1','1',0}; // 1010 11
  char ki[]       ={'1','1','0','1',0}; // 1101
  char kj[]       ={'1','1','1','1','0','1','0','1','1',0}; // 1111 0101 1
  char kk[]       ={'1','0','1','1','1','1','1','1',0}; // 1011 1111
  char kl[]       ={'1','1','0','1','1',0}; // 1101 1
  char km[]       ={'1','1','1','0','1','1',0}; // 1110 11
  char kn[]       ={'1','1','1','1',0}; // 1111
  char ko[]       ={'1','1','1',0}; // 111
  char kp[]       ={'1','1','1','1','1','1',0}; // 1111 11
  char kq[]       ={'1','1','0','1','1','1','1','1','1',0}; // 1101 1111 1
  char kr[]       ={'1','0','1','0','1',0}; // 1010 1
  char ks[]       ={'1','0','1','1','1',0}; // 1011 1
  char kt[]       ={'1','0','1',0}; // 101
  char ku[]       ={'1','1','0','1','1','1',0}; // 1101 11
  char kv[]       ={'1','1','1','1','0','1','1',0}; // 1111 011
  char kw[]       ={'1','1','0','1','0','1','1',0}; // 1101 011
  char kx[]       ={'1','1','0','1','1','1','1','1',0}; // 1101 1111
  char ky[]       ={'1','0','1','1','1','0','1',0}; // 1011 101
  char kz[]       ={'1','1','1','0','1','0','1','0','1',0}; // 1110 1010 1
  char SwKlamAuf[]={'1','0','1','0','1','1','0','1','1','1',0}; // 1010 1101 11
  char Pipe[]     ={'1','1','0','1','1','1','0','1','1',0}; // 1101 1101 1
  char SwKlamZu[] ={'1','0','1','0','1','1','0','1','0','1',0}; // 1010 1101 01
  char Tilde[]    ={'1','0','1','1','0','1','0','1','1','1',0}; // 1011 0101 11
  
  // Init Pointer-Array
  for(i=0;i<127;i++)
    Zeichencode[i] = NULL;

  Zeichencode[0]   = St_NUL;
  Zeichencode[1]   = St_SOH;
  Zeichencode[2]   = St_STX;
  Zeichencode[3]   = St_ETX;
  Zeichencode[4]   = St_EOT;
  Zeichencode[5]   = St_ENQ;
  Zeichencode[6]   = St_ACK;
  Zeichencode[7]   = St_BEL;
  Zeichencode[8]   = St_BS ;
  Zeichencode[9]   = St_HT ;
  Zeichencode[10]  = St_LF ;
  Zeichencode[11]  = St_VT ;
  Zeichencode[12]  = St_FF ;
  Zeichencode[13]  = St_CR ;
  Zeichencode[14]  = St_SO ;
  Zeichencode[15]  = St_SI ;
  Zeichencode[16]  = St_DLE;
  Zeichencode[17]  = St_DC1;
  Zeichencode[18]  = St_DC2;
  Zeichencode[19]  = St_DC3;
  Zeichencode[20]  = St_DC4;
  Zeichencode[21]  = St_NAK;
  Zeichencode[22]  = St_SYN;
  Zeichencode[23]  = St_ETB;
  Zeichencode[24]  = St_CAN;
  Zeichencode[25]  = St_EM ;
  Zeichencode[26]  = St_SUB;
  Zeichencode[27]  = St_ESC;
  Zeichencode[28]  = St_FS ;
  Zeichencode[29]  = St_GS ;
  Zeichencode[30]  = St_RS ;
  Zeichencode[31]  = St_US ;
  Zeichencode[32]  = Space ;
  Zeichencode[33]  = Ausruf;
  Zeichencode[34]  = DApostro;
  Zeichencode[35]  = Raute;
  Zeichencode[36]  = Dollar;
  Zeichencode[37]  = Prozent;
  Zeichencode[38]  = Kaufund;
  Zeichencode[39]  = Apostro;
  Zeichencode[40]  = KlamAuf;
  Zeichencode[41]  = KlamZu;
  Zeichencode[42]  = Stern;
  Zeichencode[43]  = Plus;
  Zeichencode[44]  = Komma;
  Zeichencode[45]  = Strich;
  Zeichencode[46]  = Punkt;
  Zeichencode[47]  = Sstrich;
  Zeichencode[48]  = Zero;
  Zeichencode[49]  = Eins;
  Zeichencode[50]  = Zwei;
  Zeichencode[51]  = Drei;
  Zeichencode[52]  = Vier;
  Zeichencode[53]  = Funf;
  Zeichencode[54]  = Sechs;
  Zeichencode[55]  = Sieben;
  Zeichencode[56]  = Acht;
  Zeichencode[57]  = Neun;
  Zeichencode[58]  = DPunkt;
  Zeichencode[59]  = Simikol;
  Zeichencode[60]  = PfeilAuf;
  Zeichencode[61]  = Gleich;
  Zeichencode[62]  = PfeilZu;
  Zeichencode[63]  = Frage;
  Zeichencode[64]  = AT;
  Zeichencode[65]  = gA;
  Zeichencode[66]  = gB;
  Zeichencode[67]  = gC;
  Zeichencode[68]  = gD;
  Zeichencode[69]  = gE;
  Zeichencode[70]  = gF;
  Zeichencode[71]  = gG;
  Zeichencode[72]  = gH;
  Zeichencode[73]  = gI;
  Zeichencode[74]  = gJ;
  Zeichencode[75]  = gK;
  Zeichencode[76]  = gL;
  Zeichencode[77]  = gM;
  Zeichencode[78]  = gN;
  Zeichencode[79]  = gO;
  Zeichencode[80]  = gP;
  Zeichencode[81]  = gQ;
  Zeichencode[82]  = gR;
  Zeichencode[83]  = gS;
  Zeichencode[84]  = gT;
  Zeichencode[85]  = gU;
  Zeichencode[86]  = gV;
  Zeichencode[87]  = gW;
  Zeichencode[88]  = gX;
  Zeichencode[89]  = gY;
  Zeichencode[90]  = gZ;
  Zeichencode[91]  = EKlamAuf;
  Zeichencode[92]  = BackSlach;
  Zeichencode[93]  = EKlamZu;
  Zeichencode[94]  = Dach;
  Zeichencode[95]  = UStrich;
  Zeichencode[96]  = BackApro;
  Zeichencode[97]  = ka;
  Zeichencode[98]  = kb;
  Zeichencode[99]  = kc;
  Zeichencode[100] = kd;
  Zeichencode[101] = ke;
  Zeichencode[102] = kf;
  Zeichencode[103] = kg;
  Zeichencode[104] = kh;
  Zeichencode[105] = ki;
  Zeichencode[106] = kj;
  Zeichencode[107] = kk;
  Zeichencode[108] = kl;
  Zeichencode[109] = km;
  Zeichencode[110] = kn;
  Zeichencode[111] = ko;
  Zeichencode[112] = kp;
  Zeichencode[113] = kq;
  Zeichencode[114] = kr;
  Zeichencode[115] = ks;
  Zeichencode[116] = kt;
  Zeichencode[117] = ku;
  Zeichencode[118] = kv;
  Zeichencode[119] = kw;
  Zeichencode[120] = kx;
  Zeichencode[121] = ky;
  Zeichencode[122] = kz;
  Zeichencode[123] = SwKlamAuf;
  Zeichencode[124] = Pipe;
  Zeichencode[125] = SwKlamZu;
  Zeichencode[126] = Tilde;
  Zeichencode[127] = NULL;
 
  printf("\n Packet-Bake \n");
  
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

  // Preamble for Sync
  for(i=0;i<10;i++)
  {
    sugcode(1);
    sigcode(0);
  }
  
  timestamp = time(NULL);
  ts=localtime(&timestamp);
  printf("Zeit: %02d.%02d.%04d - %02d:%02d:%02d \n",ts->tm_mday,ts->tm_mon+1,ts->tm_year+1900,ts->tm_hour,ts->tm_min,ts->tm_sec);
  
  for(n=0;(n< strlen(TextArray)) & (TextArray[n] != 0);n++)
  {
    input = TextArray[n];
    aktchar = Zeichencode[input];
    
    if(aktchar != 0)
    {
      for(i=1;(i< strlen(aktchar)) & (aktchar[i] != 0);i++)
      {
        printf("Debug: %c\n",aktchar[i]);
        sigcode(aktchar[i]);
      }
      
      for(i=0;i<2;i++)
        sigcode(0);	// Zeichentrenner 2x '0';
    }
  }
  
  // Postamble
  for(i=0;i<10;i++)
  {
    sugcode(1);
    sigcode(0);
  }

  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    
  // Funktionen disablen
  rp_Release();
  return(0);
}