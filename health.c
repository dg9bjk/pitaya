#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <rp.h>

#define MAXPATH  256
#define MAXVALUE 16

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

int main()
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
  
  if(rp_Init() != RP_OK)
  {
    fprintf(stderr, "\n\nInit-Funktion failed\n\n");
    return(-1);
  }

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
    sleep(10);
  }  
  rp_release();
  return(0);
}