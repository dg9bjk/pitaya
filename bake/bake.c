/*********************************************************
*      Software Defined Radio  -  Red Pitaya Bake        *
*                                                        *
*                                                        *
*                                                        *
* DG9BJK, Jens (Germany)                                 *
*                                                        *
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <rp.h>

#define MAXFREQ		20
#define MAXDIGIOUT	8
#define MAXPATH		256
#define MAXVALUE	16
#define MAXTEXT		120

struct ConfigArray
{
  char TextArray[MAXTEXT];
  float Frequency[MAXFREQ];		// The Base-Frequency for sending the Information
  float Amplitude[MAXFREQ];		// The Base-Frequency-Pegel for sending the Information
  float Phase    [MAXFREQ];
  int 	Output   [MAXFREQ];		// The Output for sending the Information
  int   FilterLowFlag [MAXDIGIOUT];	// Area of the Filter-Outputs
  int   FilterHighFlag[MAXDIGIOUT];
  int   LoopDelay;			// Wait for Restart the Loop
  int   WaitLoop;			// Wait between the Modes
  int   TimeOut;			// Max. TX operation
  int   CWSpeed;			// MainLaength (Dot) in ms
  float RttyShift;
  int   RttySpeed;
};

//-------------------------------------------------------------------
void sigcodecw(int Bitinfo,struct ConfigArray *Config,int txloop)
{
  rp_channel_t Channel;
  switch(Config->Output[txloop])
  {
    case 1: Channel = RP_CH_1;
          break;
    case 2: Channel = RP_CH_2;
          break;
    default: Channel = RP_CH_1;
          break;
  };  
  
  float amplitude_on  = Config->Amplitude[txloop];
  float amplitude_off = 0.0;
  int cwspeed         = Config->CWSpeed;

  switch(Bitinfo)
  {
    case '-': 
            {	// long 
            rp_GenAmp(Channel,amplitude_on);
            usleep(cwspeed * 3000);
            rp_GenAmp(Channel,amplitude_off);
            usleep(cwspeed * 1000);
            }
            break;

    case '.': 
            { // short
            rp_GenAmp(Channel,amplitude_on);
            usleep(cwspeed * 1000);
            rp_GenAmp(Channel,amplitude_off);
            usleep(cwspeed * 1000);
            }
            break;

    case ' ': 
            {	// silence
            rp_GenAmp(Channel,amplitude_off);
            usleep(cwspeed * 9000);
            }
            break;
  }
}

//-------------------------------------------------------------------
int cw(struct ConfigArray *Config,int txloop)
{
  int i,n;
  int Bitinfo          = 0; 
  int cwspeed          = Config->CWSpeed;
  char input	       = 0;
  float frequency      = Config->Frequency[txloop];
  float phase          = Config->Phase[txloop];
  float amplitude_off = 0.0;
  char *aktchar;
  time_t timestamp;
  struct tm *ts;

  rp_channel_t Channel;
  switch(Config->Output[txloop])
  {
    case 1: Channel = RP_CH_1;
          break;
    case 2: Channel = RP_CH_2;
          break;
    default: Channel = RP_CH_1;
          break;
  };  

  char TextArray[MAXTEXT];
  strcpy(TextArray,Config->TextArray);
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

  rp_GenMode(Channel, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(Channel, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(Channel,frequency);
  rp_GenPhase(Channel,phase);
  rp_GenAmp(Channel,amplitude_off); // No Signal
  rp_GenOutEnable(Channel);

  timestamp = time(NULL);
  ts=localtime(&timestamp);
  printf("Zeit: %02d.%02d.%04d - %02d:%02d:%02d \n",ts->tm_mday,ts->tm_mon+1,ts->tm_year+1900,ts->tm_hour,ts->tm_min,ts->tm_sec);
  
  for(n=0;(n< strlen(TextArray)) & (TextArray[n] != 0);n++)
  {
    input = TextArray[n];
    aktchar = Zeichencode[input];
    
    if(aktchar != 0)
    {
      for(i=0;(i< strlen(aktchar)) & (aktchar[i] != 0);i++)
        sigcodecw(aktchar[i],Config,txloop);
      usleep(cwspeed * 3000);  // Sleep in char
    }
  }

  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);
  return(0);
}

//-------------------------------------------------------------------
void sigcodertty(int Bitinfo,struct ConfigArray *Config,int txloop)
{
  rp_channel_t Channel;
  switch(Config->Output[txloop])
  {
    case 1: Channel = RP_CH_1;
          break;
    case 2: Channel = RP_CH_2;
          break;
    default: Channel = RP_CH_1;
          break;
  };  

  float frequency      = Config->Frequency[txloop];
  float rttyshift      = Config->RttyShift;
  int   rttyspeed      = Config->RttySpeed;

  switch(Bitinfo)
  {
    case '0': 
            {	// Space
            rp_GenFreq(Channel,frequency - rttyshift);
            usleep(rttyspeed * 1000);
            }
            break;

    case '1': 
            { // Mark
            rp_GenFreq(Channel,frequency);
            usleep(rttyspeed * 1000);
            }
            break;
    case '>': 
            {	// Space - start
            rp_GenFreq(Channel,frequency - rttyshift);
            usleep(rttyspeed * 1000);
            }
            break;

    case '<': 
            { // Mark - stop
            rp_GenFreq(Channel,frequency);
            usleep(rttyspeed * 1000 * 4);
            }
            break;
  }
}

//-------------------------------------------------------------------
int rtty(struct ConfigArray *Config,int txloop)
{
  int i,n;
  int Bitinfo     = 0; 
  int Zeichenmode = 0;
  int Zahlenmode  = 0;
  char input	  = 0;
  char *aktchar;
  char *st_char;
  time_t timestamp;
  struct tm *ts;

  float frequency      = Config->Frequency[txloop];
  float phase          = Config->Phase[txloop];
  float amplitude_on  = Config->Amplitude[txloop];

  rp_channel_t Channel;
  switch(Config->Output[txloop])
  {
    case 1: Channel = RP_CH_1;
          break;
    case 2: Channel = RP_CH_2;
          break;
    default: Channel = RP_CH_1;
          break;
  };  

  char TextArray[MAXTEXT];
  strcpy(TextArray,Config->TextArray);
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
  
  rp_GenMode(Channel, RP_GEN_MODE_CONTINUOUS); // Kontinuierlich
  rp_GenWaveform(Channel, RP_WAVEFORM_SINE);   // Sinus
  rp_GenFreq(Channel,frequency);
  rp_GenPhase(Channel,phase);
  rp_GenAmp(Channel,amplitude_on); 

  rp_GenOutEnable(Channel);
  sleep(1);

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
        st_char = St__B;
        for(i=1;(i< strlen(st_char)) & (st_char[i] != 0);i++)
        {
          sigcodertty(st_char[i],Config,txloop); // ST__B
        }
        Zahlenmode  = 0;
        Zeichenmode = 1;
      }
      
      if((((!Zahlenmode) & (!Zeichenmode)) | Zeichenmode) & (aktchar[0] == 'Z'))
      {
        st_char = St__Z;
        for(i=1;(i< strlen(st_char)) & (st_char[i] != 0);i++)
        {
          sigcodertty(st_char[i],Config,txloop); // ST__Z
        }
        Zahlenmode  = 1;
        Zeichenmode = 0;
      }
      
      for(i=1;(i< strlen(aktchar)) & (aktchar[i] != 0);i++)
      {
        sigcodertty(aktchar[i],Config,txloop);
      }
    }
  }
  sleep(1);

  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    
  return(0);
}

//-------------------------------------------------------------------
float readvalue(char *mainpath, char *value, char *scale, char *Corr)
{
  char path[MAXPATH] = "";
  int  fp;
  char filevalue[MAXVALUE] = "";
  float val1  = 0.0;
  float val2  = 0.0;
  float val3  = 0.0;
  
  if(value)
  {
    memset(path,0x0,sizeof(path));
    memset(filevalue,0x0,sizeof(filevalue));
    strcpy(path,mainpath);
    strcat(path,value);
    fp=open(path,O_RDONLY);
    read(fp,filevalue,sizeof(filevalue));
    val1 = atof(filevalue);
    close(fp);
  }
  else
  {
    val1 = 0.0;
  }

  if(scale)
  {
    memset(path,0x0,sizeof(path));
    memset(filevalue,0x0,sizeof(filevalue));
    strcpy(path,mainpath);
    strcat(path,scale);
    fp=open(path,O_RDONLY);
    read(fp,filevalue,sizeof(filevalue));
    val2 = atof(filevalue);
    close(fp);
  }
  else
  {
    val2 = 0.0;
  }

  if(Corr)
  {
    memset(path,0x0,sizeof(path));
    memset(filevalue,0x0,sizeof(filevalue));
    strcpy(path,mainpath);
    strcat(path,Corr);
    fp=open(path,O_RDONLY);
    read(fp,filevalue,sizeof(filevalue));
    val3 = atof(filevalue);
    close(fp);
  }
  else
  {
    val3 = 0.0;
  }

  if(val2)
    return((val1 - val3) / val2); // (Value - Korr) / Scale
  else
    return(val1 - val3);
}

//-------------------------------------------------------------------
int health()
{
  int i;

  float temperatur_FPGA; // Temperatur FPGA
  float voltage_PINT;    // Spannung 3 PINT
  float voltage_PAUX;    // Spannung 4 PAUX
  float voltage_BRAM;    // Spannung 2 BRAM
  float voltage_INT;     // Spannung 0 INT
  float voltage_AUX;     // Spannung 1 AUX
  float voltage_DDR;     // Spannung 5 DDR
  float voltage_REFp;    // Spannung 6 REFp
  float voltage_REFn;    // Spannung 7 REFn
  float voltage_8;       // Spannung 8
  float voltage_9;       // Spannung 9
  float voltage_10;      // Spannung 10
  float voltage_11;      // Spannung 11
  float voltage_12;      // Spannung 12

  char mainpath[]     = "/sys/devices/soc0/amba_pl/83c00000.xadc_wiz/iio:device1/";
  char temp_value[]   = "in_temp0_raw";
  char temp_korr []   = "in_temp0_offset";
  char temp_scale[]   = "in_temp0_scale";
  char vol_0_value[]  = "in_voltage0_vccint_raw";
  char vol_0_scale[]  = "in_voltage0_vccint_scale";
  char vol_1_value[]  = "in_voltage1_vccaux_raw";
  char vol_1_scale[]  = "in_voltage1_vccaux_scale";
  char vol_2_value[]  = "in_voltage2_vccbram_raw";
  char vol_2_scale[]  = "in_voltage2_vccbram_scale";
  char vol_3_value[]  = "in_voltage3_vccpint_raw";
  char vol_3_scale[]  = "in_voltage3_vccpint_scale";
  char vol_4_value[]  = "in_voltage4_vccpaux_raw";
  char vol_4_scale[]  = "in_voltage4_vccpaux_scale";
  char vol_5_value[]  = "in_voltage5_vccoddr_raw";
  char vol_5_scale[]  = "in_voltage5_vccoddr_scale";
  char vol_6_value[]  = "in_voltage6_vrefp_raw";
  char vol_6_scale[]  = "in_voltage6_vrefp_scale";
  char vol_7_value[]  = "in_voltage7_vrefn_raw";
  char vol_7_scale[]  = "in_voltage7_vrefn_scale";
  char vol_8_value[]  = "in_voltage8_raw";
  char vol_8_scale[]  = "in_voltage8_scale";
  char vol_9_value[]  = "in_voltage9_raw";
  char vol_9_scale[]  = "in_voltage9_scale";
  char vol_10_value[] = "in_voltage10_raw";
  char vol_10_scale[] = "in_voltage10_scale";
  char vol_11_value[] = "in_voltage11_raw";
  char vol_11_scale[] = "in_voltage11_scale";
  char vol_12_value[] = "in_voltage12_raw";
  char vol_12_scale[] = "in_voltage12_scale";

  const char *version;
  uint32_t fpga_id;
  uint32_t *pfpga_id;
  uint64_t fpga_dna;
  uint64_t *pfpga_dna;

  rp_calib_params_t caldata;
    
  printf("\nHealth-Parameter \n");
  
  version = rp_GetVersion();
  printf("RP-Lib Version %s\n",version);

  pfpga_id = &fpga_id;
  rp_IdGetID(pfpga_id);
  printf("FPGA-Version: %d\n",fpga_id);
  pfpga_dna = &fpga_dna;
  rp_IdGetDNA(pfpga_dna);
  printf("FPGA-Serial: %ld\n",fpga_dna);

  caldata = rp_GetCalibrationSettings();
  printf("Kalibrierung\n");
  printf("High-Gain Ch. A: %d\n",caldata.fe_ch1_fs_g_hi);
  printf("High-Gain Ch. B: %d\n",caldata.fe_ch2_fs_g_hi);
  printf("Low-Gain Ch. A: %d\n",caldata.fe_ch1_fs_g_lo);
  printf("Low-Gain Ch. B: %d\n",caldata.fe_ch2_fs_g_lo);
  printf("Low DC-Offset Ch. A: %d\n",caldata.fe_ch1_lo_offs);
  printf("Low DC-Offset Ch. B: %d\n",caldata.fe_ch2_lo_offs);
  printf("High DC-Offset Ch. A: %d\n",caldata.fe_ch1_hi_offs);
  printf("High DC-Offset Ch. B: %d\n",caldata.fe_ch2_hi_offs);
  printf("Back End Scale Voltage Ch. A: %d\n",caldata.be_ch1_fs);
  printf("Back End Scale Voltage Ch. B: %d\n",caldata.be_ch2_fs);
  printf("Back End DC-Offset Ch. A: %d\n",caldata.be_ch1_dc_offs);
  printf("Back End DC-Offset Ch. B: %d\n",caldata.be_ch2_dc_offs);
  printf("Magic Number: %x\n",caldata.magic);

  printf("\n\n");
  while(1)
  {
    temperatur_FPGA = readvalue(mainpath, temp_value, temp_scale, temp_korr);
    voltage_PINT    = readvalue(mainpath, vol_0_value, vol_0_scale, NULL) /1000.0;
    voltage_PAUX    = readvalue(mainpath, vol_1_value, vol_1_scale, NULL) /1000.0;
    voltage_BRAM    = readvalue(mainpath, vol_2_value, vol_2_scale, NULL) /1000.0;
    voltage_INT     = readvalue(mainpath, vol_3_value, vol_3_scale, NULL) /1000.0;
    voltage_AUX     = readvalue(mainpath, vol_4_value, vol_4_scale, NULL) /1000.0;
    voltage_DDR     = readvalue(mainpath, vol_5_value, vol_5_scale, NULL) /1000.0;
    voltage_REFp    = readvalue(mainpath, vol_6_value, vol_6_scale, NULL) /1000.0;
    voltage_REFn    = readvalue(mainpath, vol_7_value, vol_7_scale, NULL) /1000.0;
    voltage_8       = readvalue(mainpath, vol_8_value, vol_8_scale, NULL) /1000.0;
    voltage_9       = readvalue(mainpath, vol_9_value, vol_9_scale, NULL) /1000.0;
    voltage_10      = readvalue(mainpath, vol_10_value,vol_10_scale,NULL) /1000.0;
    voltage_11      = readvalue(mainpath, vol_11_value,vol_11_scale,NULL) /1000.0;
    voltage_12      = readvalue(mainpath, vol_12_value,vol_12_scale,NULL) /1000.0;

    printf("Temperatur FPGA: %.1f°C \n",temperatur_FPGA);
    printf("Spannung   PINT: %.2fV \n",voltage_PINT);
    printf("Spannung   PAUX: %.2fV \n",voltage_PAUX);
    printf("Spannung   BRAM: %.2fV \n",voltage_BRAM);
    printf("Spannung    INT: %.2fV \n",voltage_INT);
    printf("Spannung    AUX: %.2fV \n",voltage_AUX);
    printf("Spannung    DDR: %.2fV \n",voltage_DDR);
    printf("Spannung   REFp: %.2fV \n",voltage_REFp);
    printf("Spannung   REFn: %.2fV \n",voltage_REFn);
    printf("Spannung   Spg8: %.2fV \n",voltage_8);
    printf("Spannung   Spg9: %.2fV \n",voltage_9);
    printf("Spannung  Spg10: %.2fV \n",voltage_10);
    printf("Spannung  Spg11: %.2fV \n",voltage_11);
    printf("Spannung  Spg12: %.2fV\n",voltage_12);

    printf("\n");
  }  
  return(0);
}

//-------------------------------------------------------------------
int configread(struct ConfigArray *Config)
{
//  Config->TextArray[120];
//  Config->Frequency[MAXFREQ];
//  Config->Amplitude[MAXFREQ];
//  Config->Output   [MAXFREQ];
//  Config->FilterLowFlag [MAXDIGIOUT];
//  Config->FilterHighFlag[MAXDIGIOUT];
//  Config->LoopDelay;
//  Config->WaitLoop;
//  Config->TimeOut;
;
}

//-------------------------------------------------------------------
int main()
{
  int i;

  // later via config-file
  int txloop    = 0;
  int delayloop = 0;
  int waitloop  = 0;
  struct ConfigArray Config;
  
  // Startup the Red Pitaya API
  if(rp_Init() != RP_OK)
  {
    fprintf(stderr, "Init-Funktion failed\n\n");
    return(-1);
  }
  // Init Textarray (Default)
  strcpy(Config.TextArray,"Red Pitaya Test Bake de NOCALL");

  // Init Frequency-Array -> 0.0 = not used
  for(i=0;i<MAXFREQ;i++)
  {
    Config.Frequency[i] = 0.0;
    Config.Amplitude[i] = 0.0;
    Config.Output   [i] = 1;
  }
  
  // Filterfrequency for choose Filteroption
  // Init Frequency-Array -> 0.0 = not used
  for(i=0;i<MAXDIGIOUT;i++)
  {
    Config.FilterLowFlag [i] = 0.0;
    Config.FilterHighFlag[i] = 0.0;
  }
  // Default-Values  
  Config.LoopDelay = 600;
  Config.WaitLoop  = 10;
  Config.TimeOut   = 60;
  Config.CWSpeed   = 90;
  Config.RttySpeed = 6;
  Config.RttyShift = 170.0;

  configread(&Config);
  
  // Main Loop
  while(1)
  {
    txloop = 0;
    // Für jede Frequenz ein Durchlauf
    while(txloop < MAXFREQ)
    {
      if(Config.Frequency[txloop] != 0.0)
      {
        health();
        cw(&Config,txloop);
        sleep(Config.WaitLoop);
        rtty(&Config,txloop);
        sleep(Config.WaitLoop);

        // Signalgenerator aus
        rp_GenOutDisable(RP_CH_1);
        rp_GenOutDisable(RP_CH_2);
      }
      txloop++;
    }
    sleep(Config.LoopDelay);
  }

  // Signalgenerator aus
  rp_GenOutDisable(RP_CH_1);    
  rp_GenOutDisable(RP_CH_2);    
  // Shotdown the Red Pitaya API
  rp_Release();
  return(0);
}
