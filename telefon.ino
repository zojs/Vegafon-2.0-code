/*eeprom legenda
0 -ozadje
1 -zvonenje
2 - tihi nacin
8- yn switch
9 - tipke
10-22 tipke
*/
//test
#include <EEPROM.h>

#include <Adafruit_GFX.h>

#include <SPI.h>

#include <Adafruit_PCD8544.h>

#include <SoftwareSerial.h>
SoftwareSerial ser(7, 5);
Adafruit_PCD8544 ekran = Adafruit_PCD8544(8, 9, 10, 11, 12);
char omriz[7];
short batlvl = 10;
short siglvl= 0;
bool dela_modul=false;
bool noter=false;
bool debug_izpis;
bool tihi_nacin=false;
bool quickboot=false;
byte stevilka_ozadja=0;
byte stevilka_zvonenja=0;
bool masms=0;
short kontrast=65;
float foffset=0.5;

const char quote1[] PROGMEM = "Prihodnost je negotova, zato";
const char quote2[] PROGMEM = "je njen";
const char quote3[] PROGMEM = "potencial";
const char quote4[] PROGMEM = "neskoncen.";

const char* const quote_tabela[] PROGMEM = {quote1,quote2,quote3,quote4};

const char menu_1[] PROGMEM = "1 - Imenik";
const char menu_2[] PROGMEM = "2 - SMS";
const char menu_3[] PROGMEM = "3 - Nastavi";
const char menu_4[] PROGMEM = "4 - Piskaj";
const char menu_9[] PROGMEM = "9 - Razhrosci";

const char* const menu_tabela[] PROGMEM = {menu_1,menu_2,menu_3,menu_4,menu_9};

const char sms_menu_1[] PROGMEM = "1 - Novo";
const char sms_menu_2[] PROGMEM = "2 - Beri";
const char sms_menu_3[] PROGMEM = "3 - Vsi";

const char* const sms_menu_tabela[] PROGMEM ={sms_menu_1,sms_menu_2,sms_menu_3};

const char ozd_menu_1[] PROGMEM = "1 - Vegaphone";
const char ozd_menu_2[] PROGMEM = "2 - Avto";
const char ozd_menu_3[] PROGMEM = "3 - Nevemse";

const char* const ozd_menu_tabela[] PROGMEM ={ozd_menu_1,ozd_menu_2,ozd_menu_3};

const char zagon_1[] PROGMEM = "AT+CMIC=0,11";
const char zagon_2[] PROGMEM = "AT+CLVL=65";
const char zagon_3[] PROGMEM = "AT+CRSL=0";
const char zagon_4[] PROGMEM = "AT+CBAND=EGSM_PCS_MODE";
const char zagon_5[] PROGMEM = "AT+CMGF=1";
const char zagon_6[] PROGMEM = "";

const char* const zagon_tabela[] PROGMEM ={zagon_1,zagon_2,zagon_3,zagon_4,zagon_5,zagon_6};

const char zvonenje_melodija_1[] PROGMEM= "fffaaaa  fafhhhaaa hahiii ikihhh hpmmmm        x";
const char zvonenje_melodija_2[] PROGMEM= "abababcbcbcbcdcdcddedede   x";

const char prizig_melodija[] PROGMEM= "cc ddgff x";

short tempo_tabela[]={45,200};

const char* const melodije_zv[] PROGMEM = {zvonenje_melodija_1,zvonenje_melodija_2};

const char* const melodije[] PROGMEM = {zvonenje_melodija_1,prizig_melodija}; //TODO: Porihtaj to
//const char zvon_lestvica[] PROGMEM = "cdefgahc";
//byte zvon_lestvica_t=150;

static const unsigned char PROGMEM zaklep_ikona[]={
   0x3c, 0x00, 0x66, 0x00, 0x42, 0x00, 0x42, 0x00, 0x42, 0x00, 0xff, 0x00,
   0x81, 0x00, 0x99, 0x00, 0x99, 0x00, 0x99, 0x00, 0x81, 0x00, 0xff, 0x00 };

static const unsigned char PROGMEM tiho_ikona[] = {
   0x00, 0x00, 0x00, 0x06, 0x18, 0x00, 0x0c, 0x18, 0x00, 0x18, 0x1c, 0x00,
   0x30, 0x1e, 0x00, 0xe0, 0x1f, 0x00, 0xc0, 0x1f, 0x00, 0x98, 0x1f, 0x00,
   0x38, 0x1f, 0x00, 0x78, 0x1e, 0x00, 0xf8, 0x1c, 0x00, 0xf8, 0x19, 0x00,
   0xf8, 0x33, 0x00, 0xc0, 0x67, 0x00, 0x80, 0xcf, 0x00, 0x00, 0x9f, 0x01,
   0x00, 0x1c, 0x03, 0x00, 0x1c, 0x06, 0x00, 0x18, 0x0c, 0x00, 0x00, 0x00 };

static const unsigned char PROGMEM vegova_spodaj[] = {
   0x06, 0xfb, 0xe1, 0x83, 0xc1, 0x87, 0x20, 0x7c, 0x18, 0xc6, 0x07, 0x04,
   0x19, 0x30, 0x84, 0xc3, 0x88, 0x20, 0xc6, 0x38, 0x66, 0x00, 0x0c, 0x19,
   0x18, 0x80, 0xc2, 0x98, 0x20, 0x82, 0x79, 0x66, 0x00, 0x8c, 0x19, 0x08,
   0xc0, 0xc6, 0x98, 0x20, 0x03, 0x59, 0x66, 0x00, 0x88, 0xf8, 0x88, 0x43,
   0xc6, 0x88, 0x3f, 0x03, 0xd9, 0xe6, 0x03, 0x98, 0x18, 0x8c, 0x47, 0xc4,
   0x87, 0x30, 0x03, 0x99, 0x66, 0x00, 0xd0, 0x18, 0x08, 0xe6, 0xcf, 0x80,
   0x20, 0x03, 0x99, 0x67, 0x00, 0x50, 0x18, 0x18, 0x66, 0xcc, 0x80, 0x20,
   0x82, 0x19, 0x67, 0x00, 0x70, 0x18, 0x30, 0x26, 0xc8, 0x80, 0x20, 0xc6,
   0x18, 0x67, 0x00, 0x60, 0xf8, 0xe1, 0x33, 0xd8, 0x80, 0x20, 0x7c, 0x18,
   0xc6, 0x07 };

static const unsigned char PROGMEM sms_bitmap[] = {
   0xff, 0x01, 0x83, 0x01, 0x45, 0x01, 0x29, 0x01, 0x11, 0x01, 0x01, 0x01,
   0x01, 0x01, 0xff, 0x01 };


static const unsigned char PROGMEM vegova_zgoraj[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x03, 0x0c, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xf0, 0x9f, 0xc1, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xfc, 0xff, 0xf0, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x3c, 0x78, 0xf8, 0xe0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x1c, 0x30, 0x7c, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e,
   0x00, 0x1e, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00,
   0x0f, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x80, 0x07,
   0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xc0, 0x07, 0x80,
   0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xe0, 0x03, 0x80, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xf0, 0x61, 0xc0, 0x03, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xf0, 0xff, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xf8, 0x3f, 0xe0, 0xff, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xe0, 0x0f, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static const unsigned char PROGMEM avto_ozadje[]=
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a,
   0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
   0xdf, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0xa0, 0xef,
   0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x7f, 0xca, 0x23, 0xfa, 0x0f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xbc, 0xfa, 0x8c, 0xa9, 0xf0, 0x0f, 0x00,
   0x00, 0x00, 0x00, 0x10, 0x18, 0xf8, 0x1c, 0xf9, 0x02, 0x0f, 0x00, 0x00,
   0x00, 0x40, 0x1e, 0x00, 0xf0, 0x84, 0x09, 0x0f, 0x00, 0x00, 0x00, 0x00,
   0x20, 0x7f, 0x00, 0x78, 0x1c, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0xa8,
   0x5e, 0x88, 0x28, 0x4d, 0x88, 0x24, 0x01, 0x00, 0x00, 0x54, 0x61, 0x00,
   0x11, 0x41, 0x8a, 0x12, 0x41, 0x00, 0x04, 0xc0, 0x1f, 0xa0, 0x10, 0x00,
   0xc8, 0x26, 0x24, 0x14, 0x05, 0x0e, 0xff, 0x24, 0x82, 0x01, 0x00, 0x80,
   0xc1, 0xa4, 0x00, 0x00, 0x0f, 0xed, 0x37, 0x40, 0x20, 0x22, 0x00, 0x51,
   0xbc, 0x70, 0x0d, 0x2f, 0xc9, 0x6f, 0xd5, 0xa5, 0x4a, 0xab, 0x2b, 0xfe,
   0x62, 0x08, 0x80, 0xfe, 0xdf, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0x19,
   0x09, 0x10, 0xfa, 0xbf, 0xc9, 0x95, 0x54, 0x92, 0x9b, 0xff, 0x07, 0x02,
   0x42, 0xfd, 0xff, 0x92, 0x22, 0x82, 0xc8, 0xd8, 0xff, 0x67, 0x09, 0x1e,
   0xfc, 0xff, 0x03, 0x45, 0x10, 0x91, 0xd9, 0xff, 0x6f, 0x00, 0x5e, 0xfd,
   0xff, 0xaa, 0x03, 0x45, 0xc4, 0xfa, 0xff, 0x77, 0x01, 0xf0, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0x07, 0x00, 0x00, 0x7c, 0xff, 0xff,
   0xbf, 0xaa, 0xfa, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xfe, 0xff, 0xad, 0x1b,
   0x00, 0x40, 0xeb, 0xf7, 0x0f, 0x00, 0x00, 0x7e, 0x7f, 0x00, 0x00, 0x00,
   0x00, 0xe0, 0xff, 0x07, 0x00, 0x00, 0xec, 0xed, 0x00, 0x00, 0x00, 0x00,
   0xa0, 0xad, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
bool venizlock = true;
bool zamenjajyn=false;
char mtoplay[100];
float mtempo;
bool mrun;
int mpos=0;
int tipke_v[12];
char key()
{
 int vr;
  vr = analogRead(A4);
  if (vr > 656 && vr < 662) return '1';
  if (vr > 681 && vr < 690) return '2';
  if (vr > 729 && vr < 734) return '3';
  if (vr > 635 && vr < 643) return '4';
  if (vr > 665 && vr < 673) return '5';
  if (vr > 715 && vr < 720) return '6';
  if (vr > 129 && vr < 135) return '7';
  if (vr > 270 && vr < 278) return '8';
  if (vr > 460 && vr < 470) return '9';
  if (vr >= 0 && vr < 10) return '*';
  if (vr > 179 && vr < 185) return '0';
  if (vr > 412 && vr < 423) return '#';
  if (digitalRead(2))  return 'y';
  if (digitalRead(3)) return 'n';
  return 'x';
  /*if (vr > tipke_v[10+1]-5 && vr < tipke_v[10+1]+5) return '1';
  if (vr > tipke_v[10+2]-5 && vr < tipke_v[10+2]+5) return '2';
  if (vr > tipke_v[10+3]-5 && vr < tipke_v[10+3]+5) return '3';
  if (vr > tipke_v[10+4]-5 && vr < tipke_v[10+4]+5) return '4';
  if (vr > tipke_v[10+5]-5 && vr < tipke_v[10+5]+5) return '5';
  if (vr > tipke_v[10+6]-5 && vr < tipke_v[10+6]+5) return '6';
  if (vr > tipke_v[10+7]-5 && vr < tipke_v[10+7]+5) return '7';
  if (vr > tipke_v[10+8]-5 && vr < tipke_v[10+8]+5) return '8';
  if (vr > tipke_v[10+9]-5 && vr < tipke_v[10+9]+5) return '9';
  if (vr >=tipke_v[10+10]-5 && vr < tipke_v[10+10]+5) return '*';
  if (vr > tipke_v[10+0]-5 && vr < tipke_v[10+0]+5) return '0';
  if (vr > tipke_v[10+11]-5 && vr < tipke_v[10+11]+5) return '#';
  if(!zamenjajyn)
  {
  if (digitalRead(2))  return 'y';
  if (digitalRead(3)) return 'n';
  }
  else
  {
      if (digitalRead(3))  return 'y';
      if (digitalRead(2)) return 'n';
  }*/
  return 'x';


}
String do_narekovaja(bool prepisi)
{
  char bajt='x';
  String ven="";
  while(bajt!='"'&&ser.available())
  {
    bajt=ser.read();
    if(prepisi) ven+=bajt;
  }
}
String precitaj_serial()
{
  String vm="";
  Serial.print('s');
  while(ser.available()){
    char vmc;
    vmc=ser.read(); delay(3); Serial.println(vm.length());
    if(vm.length()>100) break;
    vm+=vmc;}
  Serial.print('f');
  return vm;
}
int najdi_string(String &seno, String &igla)
{
  int i,j;
  i=0;
  j=0;
  if(igla.length()==0) return false;
  for(i=0;i<seno.length();i++)
  {
    if(seno[i]==igla[j])
    {
      j++;
    }
    else j=0;
    if(j==igla.length()-1)return i-igla.length();
    
  }
  return -1;
}
int najdi_string(String seno, String igla, int rpt)
{
  int i,j,st;
  i=0;
  j=0;
  st=0;
  if(igla.length()==0) return false;
  for(i=0;i<seno.length();i++)
  {
    if(seno[i]==igla[j])
    {
      j++;
    }
    else j=0;
    if(j==igla.length()-1)rpt++;
    if(rpt==st)return i-igla.length()+1;
    
  }
  return -1;
}
void eeprom_zapis(int mesto, byte vrednost)
{
  int i;
 for( i=0;i<20;i++)
  {
   if(EEPROM.read(mesto)==vrednost) break;
   EEPROM.write(mesto,vrednost);
  }
  if(debug_izpis) 
  {
    //Serial.print("eeprom a:"); Serial.print(mesto); Serial.print(" v:"); Serial.print(vrednost); Serial.print(" i:"); Serial.println(i);
  }
}
void pogrunti_tipke()
{
  {
    int i;
  for(i=0;i<12;i++)
  {
  ekran.clearDisplay();
  ekran.setCursor(0, 0);
  ekran.print("prititsni: "); ekran.print(i);
  ekran.display();
  while(analogRead(A4)>1022); eeprom_zapis(10+i,analogRead(A4)/4);
  delay(350);
  }
  ekran.clearDisplay();
  ekran.setCursor(0, 0);
  ekran.print("pritisni yes");
  ekran.display();
  ekran.display();
  while(!digitalRead(2)||!digitalRead(3))
  if(digitalRead(3))eeprom_zapis(8,1);
  }
}
void bootsplash()
{
  strcpy_P(mtoplay,(char*)pgm_read_word(&(melodije[1])));
  mtempo=5;
  mrun=true;
  int i;
  for(i=30;i>=0;i-=2)
  {
    ekran.clearDisplay();
    ekran.drawXBitmap(0,5-i,vegova_zgoraj,84,30,1);
    ekran.drawXBitmap(0,35+i,vegova_spodaj,84,10,1);
    ekran.display();
    zamik(30);
  }
  delay(800);
   mrun=false;
   madv();
}
void stringiraj(char vhod[], char kam[], int stevilo)
{
  short i;
  for (i = 0; i < stevilo; i++)
  {
    kam[i] = vhod[i];
  }
  for (; i < 20; i++) kam[i] = ' ';

}
void merilec()
{
  
  ekran.clearDisplay();
  Serial.begin(9600);
  ekran.println("Merilec");
  delay(500);
  ekran.println("Povezujem");
  while(!Serial.available()&&Serial.read()!=5||1)
  {
    Serial.write(5);
    if(key()=='n') return;
  }
  ekran.println("Uspeh!");

  
  
}
void madv()
{
  int freq[]={523,554,587,622,659,698,739,783,830,880,932,987,1046,1108,1174,1244,1318,1396};
  static long stej=0;
  static bool has=0;
  if(mrun&&!tihi_nacin)
  {
    if(mtoplay[mpos]=='x'){has=0; mpos=0;}
    else
    if(mtoplay[mpos]==' ') { noTone(4); 
    if(millis()-stej>1000/mtempo)
       {
        mpos++;
        stej=millis();
       }
    }
    else  {
    if(!has){tone(4,freq[mtoplay[mpos]-'a']*foffset);has=1;}
    if(millis()-stej>1000/mtempo)
       {
        mpos++;
        stej=millis();
        has=0;
       }
    }
  }else{
    noTone(4);
    
    stej=millis();
    mpos=0;
    has=0;
  }
}
void ser_clear()
{
  while (ser.available())
    ser.read();
}
void beri_nastavitve()
{
  stevilka_ozadja=EEPROM.read(0); //stevilka ozadja
  stevilka_zvonenja=EEPROM.read(1);//stevilka zvonenja
  tihi_nacin=EEPROM.read(2);
  if(debug_izpis)
  {
  Serial.print("st ozadja:");
  Serial.println(stevilka_ozadja);
  Serial.print("st zvonenja:");
  Serial.println(stevilka_zvonenja);
  }
  zamenjajyn=EEPROM.read(8);
  for(int i=0;i<12;i++) tipke_v[i]=EEPROM.read(i+10);
  eeprom_zapis(9,1);
}
void pocakaj_spust()
{
  while(key()!='x')
  {
    scronloop();
    delay(5);
  }
}
void init_modul()
{
  ser.println("ATE0");
  delay(10);
  ser.println("AT");
  delay(20);
  
  while(ser.available())
  {
    if(ser.read()=='O'&&ser.read()=='K') dela_modul=true;
    delay(1);

  }
 /* ser.println("AT+CMIC=0,9");
  delay(20);
  ser.println("AT+CLVL=100");
  delay(50);
  ser.println("AT+CRSL=0");
  delay(20);
  ser.println("AT+CBAND=EGSM_PCS_MODE");
  delay(20);
  ser.println("AT+CMGF=1");*/
 for(int i=0;i<6;i++)
  {
    char medp[32];
    strcpy_P(medp,(char*)pgm_read_word(&(zagon_tabela[i])));
    ser.println(medp);
    delay(40);
  }
  ser_clear();
       for(int i=0;i<5;i++)
  {
    char medp[32];
    strcpy_P(medp,(char*)pgm_read_word(&(zagon_tabela[i])));
    ser.println(medp);
    delay(40);
  }
       for(int i=0;i<5;i++)
  {
    char medp[32];
    strcpy_P(medp,(char*)pgm_read_word(&(zagon_tabela[i])));
    ser.println(medp);
    delay(40);
  }
  ser_clear();
  delay(100);
}
void zamik(int ms)
{
  long zac=millis();
  while(millis()-zac<ms)
  {
    scronloop();
    madv();
  }
}

void melody(char pond[],float temp)
{

  noTone(4);
  zamik(30);
  int freq[]={523,554,587,622,659,698,739,783,830,880,932,987,1046,1108,1174,1244,1318,1396};
  for(int i=0;1;i++)
  {
    if(pond[i]=='x')break;
    if(pond[i]==' ') { noTone(4); zamik(1000/temp);}
    else  {
    if(!tihi_nacin)tone(4,freq[pond[i]-'a']*foffset);
     zamik(1000/temp);}
  }
  noTone(4);
}
void setup() {
  
  char medp[25];
  pinMode(6, OUTPUT);
  pinMode(13,OUTPUT);
  ekran.begin();
  ekran.setContrast(kontrast);
  ekran.setTextSize(1);
  ekran.setTextColor(BLACK);
  ekran.clearDisplay();
    osvet(1);

  delay(20);
  digitalWrite(13,LOW);
  delay(20);
  digitalWrite(13,HIGH);
  delay(300);
  digitalWrite(13,LOW);
  ser.begin(19200);
  Serial.begin(19200);


  beri_nastavitve();
  if(quickboot==false)bootsplash();
  if(quickboot==false&&0)
  {
  strcpy_P(medp,(char*)pgm_read_word(&(quote_tabela[0])));
  ekran.print(medp);
  ekran.display();
  strcpy_P(medp,(char*)pgm_read_word(&(melodije[0])));
  melody(medp,5);
  
  strcpy_P(medp,(char*)pgm_read_word(&(quote_tabela[1])));
  ekran.println(medp); 
  ekran.display();
  strcpy_P(medp,(char*)pgm_read_word(&(melodije[1])));
  melody(medp,5);
  
  strcpy_P(medp,(char*)pgm_read_word(&(quote_tabela[2])));
  ekran.println(medp);
  ekran.display();
  strcpy_P(medp,(char*)pgm_read_word(&(melodije[0])));
  melody(medp,5);
    
  strcpy_P(medp,(char*)pgm_read_word(&(quote_tabela[3])));
  ekran.println(medp);
  ekran.display();
  strcpy_P(medp,(char*)pgm_read_word(&(melodije[1])));
  melody(medp,5);

  delay(800);
  }
  
int i;


  for(i=0;i<200&&key()=='x';i++) delay(1);


  
  pinMode(A6,INPUT);


  pinMode(4, OUTPUT);
  //digitalWrite(6,HIGH);

 //ser.println("ATZ0");
  //delay(200);
  delay(100);
  init_modul();

  ekran.clearDisplay();
  //ekran.print("Razhroscevalni izpis?");
  //ekran.display();
  /*while(1)
  {
   if(key()=='y'){ debug_izpis=1; break;}
   if(key()=='n'){ debug_izpis=0; break;}
  }*/
  delay(50); 
  beri_nastavitve();
}
void osvet(int vred)
{
  static short sv=2;
  if(vred==true)
  {
    if(analogRead(A0)>400) 
    
    {analogWrite(6, LOW);
    ekran.setContrast(kontrast);
    }
    else
    {
    analogWrite(6, ((250)-analogRead(A0)/2)/2);
    
    ekran.setContrast(kontrast+5-analogRead(A0)/100-5);
    }
    sv=1;
  }else if(sv!=0&&vred==false)
  {
    digitalWrite(6, LOW);
    if(batlvl>80) ekran.setContrast(kontrast);
    ekran.setContrast(kontrast);
    if(batlvl<30) ekran.setContrast(kontrast);
    if(!dela_modul==0) ekran.setContrast(kontrast);
    sv=0;
  }
  sv=vred;
}
void imenik_lookup()
 {
  
 }
void imenik_ind(int ind, String &ime, String &stevilka)
{
  ser_clear();
  stevilka="";
  ser.print("AT+CPBR=");
  ser.println(ind);
  zamik(50);
  pocaki_odgovor(50);
  char bajt='x';
  //Serial.println("kle");
  while(bajt!='"'&&ser.available())
  {
    bajt=ser.read();
    //Serial.print(bajt);

  }
  bajt='x';
  while(bajt!='"'&&ser.available())
  {
    bajt=ser.read();
    stevilka+=bajt;
   // Serial.print(bajt);
  }
    bajt='x';

    while(bajt!='"'&&ser.available())
    {
    bajt=ser.read();
        //Serial.print(bajt);

    }
  bajt='x';

    while(bajt!='"'&&ser.available())
  {
    bajt=ser.read();
    ime+=bajt;
     Serial.print(bajt);

  }  
}
void imenik_izbira(String &stevilka)
{
  int ind=1;
  int indprej=-1;
  String ime="";
  while(1)
  {
    if(ind!=indprej)
    {
      ime="";
      imenik_ind(ind,ime,stevilka);
      indprej=ind;
      stevilka.remove(stevilka.length()-1,1);
      ime.remove(ime.length()-1,1);
    }
  ekran.clearDisplay();
  ekran.setCursor(0,0);
  ekran.print(ind);
  ekran.print(".");
  ekran.setCursor(0,20);
  ekran.print(ime);
  ekran.display();
  if(key()=='y'){pocakaj_spust(); return;}
  if(key()=='1'&&ind>1){ ind--; pocakaj_spust();}
  if(key()=='3'){ ind++;  pocakaj_spust();}
  if(key()=='6'){ ind+=5;  pocakaj_spust();}
  if(key()=='4'){ ind-=5;  pocakaj_spust();}

  scronloop();
  delay(5);
  }
}

void v_klicu(String* st)
{
  ser_clear();
  delay(20);
  osvet(2);
  analogWrite(6,50);
  String serin;
    while (1)
  {
    ekran.clearDisplay();
    statusbar();
    ekran.setCursor(0, 48 / 3);
    ekran.print("V klicu:");
    ekran.setCursor(0, 48 / 2);
    ekran.print(*st);
    ekran.display();
    delay(50);
    //Serial.println("vklicu");
    //scronloop();
    serin=precitaj_serial();
    String igla="NO CARRIER";
    if (pritisnjena('n')||najdi_string(serin,igla)!=-1) {
      ser.println("ATH");
      delay(50);
      ser_clear();
      osvet(1);
      return;
    }
  }
}

String  textnot(bool &acc)
{

  String crke_t[9];
  crke_t[0]=".?!";
  crke_t[1]="abc";
  crke_t[2]="def";
  crke_t[3]="ghi";
  crke_t[4]="jkl";
  crke_t[5]="mno";
  crke_t[6]="pqrs";
  crke_t[7]="tuv";
  crke_t[8]="wxyz";
  String tekst="";
  int pavza=0;
  int poz=0;
  char dod='_';
  char last='x';
  bool noter=true;
  while(1)
  {
    scronloop();
    ekran.clearDisplay();
    statusbar();
    ekran.setCursor(0,12);
    ekran.print("Besedilo:");
    ekran.setCursor(0,20);
    ekran.print(tekst+dod);
  if(pavza==0&&dod!='_')
  {poz=0;
  tekst+=dod;
  dod='_';
  last='x';
  }
  ekran.display();
  char tipka;
  if(tipka=='n'){acc=false; return"";}
  else
  if(tipka=='y'){acc=true; return tekst;}
  tipka=key();
  if(tipka=='*'&&noter==false&&tekst.length()>0) 
  {
    tekst.remove(tekst.length()-1,1);
    noter=true;
  }
  if(tipka=='x') noter=false;
  else
  if(tipka>='0'&&tipka<='9'&&noter==false)
  {
    if(last=='x') last=tipka;
    if(last!=tipka) {poz=0;tekst+=dod;}
    //Serial.println((int)(tipka-'0'));
    //Serial.println(crke_t[(int)(tipka-'0')][poz]);
    //Serial.println();
    noter=true;
    pavza=15;
    if(tipka!='0')dod=crke_t[(int)(tipka-'0')-1][poz];
    else if(tipka=='0'){dod=' ';poz=0; pavza=1;}
    poz++;
    if(poz==crke_t[(int)(tipka-'0')-1].length()){ poz=0;}
    
    
    
    last=tipka;
  }
  
 if(pavza>0) pavza--;
  Serial.println(pavza);
  delay(10);

  
  }
  
}
void usbmelody()
{
  double temprej=0;
  pocakaj_spust();
  delay(100);
  mtempo=5;
  mrun=false;
  mpos=0;
  while(key()!='n')
  {
  madv();
  if(temprej!=mtempo)
  {
  ekran.clearDisplay();
  ekran.println("USBm");
  ekran.println(mtempo);
  ekran.display();
  temprej=mtempo;
  }
  
  mrun=true;
  if(key()=='3')
  {
    mtempo+=0.1;
    delay(50);
  }
  if(key()=='1'&&mtempo>0.5)
  {
    mtempo-=0.1;
    delay(50);
  }
  if(Serial.available())
  {
    mrun=true;
    int i=0;
    String prazen="";
    prazen.toCharArray(mtoplay,0);
    delay(100);
    Serial.println();
    while(Serial.available())
    {
      
      mtoplay[i]=Serial.read();
      Serial.print(mtoplay[i]);
      i++;
    }
  }
  delay(1);
  }
  
}
void beepit()
{

   while(key()!='n')
   {
      ekran.clearDisplay();
    ekran.println("Beeper");
    ekran.display();
    if(!tihi_nacin)tone(4,analogRead(A0)*2+foffset);
    scronloop();
    delay(5);
   }
}
void debugmenu()
{
  pocakaj_spust();
  ekran.clearDisplay();
  ekran.println("1-USBa");
  ekran.println("2-USBm");
  ekran.println("3-Sim. klica");
  ekran.println("4-USBi");
  ekran.display();
  char tipka;
  while(1)
  {
    tipka=key();
    if(tipka=='n') return;
    if(tipka=='1') {pc_debug(true); return;}
    if(tipka=='2') {pc_debug(false); return;}
    if(tipka=='3'){poklican(1);return;}
    if(tipka=='4'){usbmelody();return;}

    delay(20);
  }
}
void pc_debug(bool arduino)
{
   ekran.clearDisplay();
   ekran.println("Razhrosi SIM");
   ekran.display();
   while(key()!='n')
   {
         if(ser.available())
  {
    Serial.write(ser.read());
  }
  if(Serial.available())
  {
    while(Serial.available())
    {
    ser.write(Serial.read());
    }
    if(arduino)ser.println();
  }
   }
}
void vnos_st(String  &stevilka,bool &cez, bool noter)
{
  stevilka = "";
  static int i;
  i = 0;
  //noter = false;
  
  bool ven = false;
  while (ven == false)
  {  
    delay(5);
    scronloop();
    char tipka;
    tipka = key();
    if (tipka == 'y'&&noter==false) {
      
      break;
    }
    else if (tipka == 'n') {
     cez=false; break;
    }
    else if (tipka != 'x' && noter == false&& tipka!='*'&&tipka!='#')
    {
      stevilka += tipka;
      i++;
      noter = true;
      if(!tihi_nacin)tone(4,500+(tipka-'0')*40);
    }
    if (tipka == 'x'){noter = false;noTone(4);}
    if (tipka == '*'&&noter==false){ stevilka.remove(stevilka.length()-1,1);noter=true;}
    if(tipka=='#') imenik_izbira(stevilka);
    ekran.clearDisplay();
    statusbar();
    ekran.setCursor(0, 48/2);
   // char ven[9]="Stevilka";
    ekran.println("Stevilka:");
    ekran.setCursor(0, 48/2+ 48 / 3);
    ekran.print(stevilka);
    ekran.display();
  }

}
bool pocaki_odgovor(int ms)
{
  int i=0;
  while(i<ms)
  {
    delay(1);
    i++;
    if(ser.available())
    {delay(5);
    break;
    }
  }
  
  return false;
}
int st_sms()
{
    int dokje=70,ind; //ugotovi kateri so neprebani
  while(1)
  {
    
    ser.print("AT+CMGR=");
    ser.println(dokje);
    //Serial.print("AT+CMGR=");
    Serial.println(dokje);
    String noter="";
    char bajt='x';
    int i=0;
    pocaki_odgovor(100);

    noter="";
    while(ser.available())
    {
      noter+=(char)ser.read();
      
     
    }
    Serial.println(noter);
    if(noter[noter.indexOf('O')+1]=='K'&&noter.length()<5) dokje--;
     else break;
  }
  //Serial.print("st sms:");
  Serial.println(dokje);

}
void nalozi_sms(String &bes,String &stevilka,String &ime,int ind)
{
  char bajt;
  bes="";
  ser_clear();
  pocaki_odgovor(20);
  int pon=0;
  ser.println("AT+CMGF=1");
  pocaki_odgovor(20);
  ser_clear();
  ser.print("AT+CMGR=");
  ser.println(ind);
  //Serial.print("AT+CMGR=");
  short brk=0;
  pocaki_odgovor(200);
  delay(50);
  while(pon<3)
  {
    bajt=ser.read();
    if(bajt=='"') pon++;
    //Serial.println("BRGbrrejohrei");
  }
  pon=0;
  while(bajt!='"')
  {
    bajt=ser.read();
    stevilka+=bajt;
  }
   while(bajt!='"')
  {
    bajt=ser.read();
  }
 while(bajt!='"')
  {
    bajt=ser.read();
    ime+=bajt;
  }
     while(bajt!='"')
  {
    bajt=ser.read();
  }
  //Serial.print("pon:");
  //Serial.println(pon);
  while (ser.available())
  {
    bes+=(char)ser.read();
    delayMicroseconds(500);
    //if(debug_izpis) Serial.print("b");
  }
  //Serial.println(bes);
}
void sms_bralnik()
{
  masms=0;
  int ind=1;
  int indprej=-1;
  //ind=st_sms();
  int pon;
  String bes="";
  String st="";
  String ime="";
while(1)
{
  if(ind!=indprej)
  {
  indprej=ind;
  nalozi_sms(bes,st,ime,ind);
  ekran.clearDisplay();
  ekran.setCursor(0,0);
  ekran.print(ind);
  ekran.println('.');
  ekran.print(st);
  ekran.setCursor(0,10);
  ekran.print(bes);
  ekran.display();
  delay(20);
  Serial.println("we");
  }
  delay(10);
  if(key()=='3') ind++;
  if(key()=='1'&&ind>1) ind--;
  if(key()=='n') break;
}
  
}
void sms()
{
  bool cez=true;
  String kaj;
  String kam;
  
  kaj=textnot(cez);
  
  if(cez==false) return;
  Serial.println("kle");
  vnos_st(kam,cez,true);
  if(debug_izpis){ Serial.print("po sms:"); Serial.println((int)cez);}
  if(cez==false) return;
  ser.print("AT+CMGS=");
  ser.print((char)(34));
  ser.print(kam);
  ser.println((char)(34));
  delay(100);
  ser.print(kaj);
  ser.write(26);
  if(debug_izpis)
  {
  Serial.print("AT+CMGS=");
  Serial.print((char)(34));
  Serial.print(kam);
  Serial.println((byte)(34));
  }
  delay(20);
  Serial.print(kaj);
  delay(100);
  
  while(ser.available()&&debug_izpis)
  {
    Serial.write(ser.read());
  }
  ser_clear();
  return;
}
void imenikmenu()
{
  pocakaj_spust();
  ekran.clearDisplay();
  //ekran.println("1 - Kontakti");
  //ekran.println("2 - Dodaj");
  //ekran.println("3 - Brisi");
  ekran.display();
  while(1)
  {
    if(key()=='n') return;
    if(key()=='1') {sms(); return;}

    delay(20);
  }
}
void smsmenu()
{
  pocakaj_spust();
  delay(50);
  pocakaj_spust();
  ekran.clearDisplay();
  
  for(int i=0;i<3;i++)
  {
    char medp[20];
    strcpy_P(medp,(char*)pgm_read_word(&(sms_menu_tabela[i])));
    ekran.println(medp);
  }
  ekran.display();
  char tipka;
  while(1)
  {
    tipka=key();
    if(tipka=='n') return;
    if(tipka=='1') {sms(); return;}
    if(tipka=='2') {sms_bralnik(); return;}

    delay(20);
  }
}
void nastavi_zvonenje(byte zvon)
{
  stevilka_zvonenja=zvon;
  eeprom_zapis(1,zvon);
}
void zvonenje_menu()
{
  pocakaj_spust();
       ekran.clearDisplay();
  ekran.println("1 - S;G");
  ekran.println("2 - bucke");
  ekran.display();
  char tipka;
  while(1)
  {
    tipka=key();
    if(tipka=='n') return;
    if(tipka=='1') {nastavi_zvonenje(0); return;}
    if(tipka=='2') {nastavi_zvonenje(1); return;}
    

    delay(20);
  }
}
void nastavi_ozadje(byte st)
{
  stevilka_ozadja=st;
  eeprom_zapis(0,st);

  
  
}

void ozadje_menu()
{
  pocakaj_spust();
      ekran.clearDisplay();
  int zavihek=0;
  
  for(int i=0;i<3;i++)
  {
    char medp[20];
    strcpy_P(medp,(char*)pgm_read_word(&(ozd_menu_tabela[i])));
    ekran.println(medp);
  
  }
  ekran.display();
  char tipka;
  while(1)
  {
    tipka=key();
    if(tipka=='n') return;
    if(tipka=='1') {nastavi_ozadje(0); return;}
    if(tipka=='2') {nastavi_ozadje(1); return;}
    if(tipka=='3') {nastavi_ozadje(2); return;}
    if(tipka=='#') 
    {zavihek++;
       while(key()=='#'){
      scronloop();
      delay(10);
    }
    if(zavihek==0) zavihek++;
      
      }
             if(key()=='*') 
    {zavihek++;
       while(key()=='*'){
      scronloop();
      delay(10);
    }
    if(zavihek!=0) zavihek--;
      
      }
      delay(20);
  }
    

    

}
void nastavitve_menu()
{
    ekran.clearDisplay();
  ekran.println("1 - Ozadje");
  ekran.println("2 - Zvon");
  ekran.display();
  char tipka;
  while(1)
  {
    tipka=key();
    if(tipka=='n') return;
    if(tipka=='1') {ozadje_menu(); return;}
    if(tipka=='2') {zvonenje_menu(); return;}
    

    delay(20);
  }
}
void meni()
{
  int i;
  ekran.clearDisplay();
  char medp[15];
  for(i=0;i<4;i++)
  {
  strcpy_P(medp,(char*)pgm_read_word(&(menu_tabela[i])));
  ekran.println(medp);
  }
  ekran.display();
  char tipka;
  while(1)
  {
    //scronloop();
    tipka=key();
    if(tipka=='n') return;
    if(tipka=='1');
    if(tipka=='2') {smsmenu(); return;}
    if(tipka=='3') {nastavitve_menu(); return;}
    if(tipka=='4') {beepit(); return;}
    if(tipka=='9') {debugmenu(); return;}
    if(tipka=='#') {tihi_nacin=!tihi_nacin;
    if(tihi_nacin)
    {
      eeprom_zapis(2,1);
    } else eeprom_zapis(2,0);
    return;}

    delay(20);
  }
}
void poklican(bool dbg)
{
  delay(125);
  if(analogRead(A6)>30&&dbg==0){masms=1; return;}
  strcpy_P(mtoplay,(char*)pgm_read_word(&(melodije_zv[stevilka_zvonenja])));
  madv();
  mtempo=tempo_tabela[stevilka_zvonenja]/10;
  mpos=0;
  mrun=true;
  String stev;
  String ime;
  stev="";
  ime="";
  int ton=0;
  bool smer=1;
  int val=0;
  osvet(2);
  ekran.setContrast(kontrast);
  ekran.clearDisplay();
  ekran.setTextSize(2);
  ekran.setCursor(0,48/5);
  ekran.println("Klic:");
  ekran.display();
  while(1)
  {



  if(stev==""&&dbg==0){

      while(ser.available())
      {
        if(ser.read()=='C'&&ser.read()=='L'&&ser.read()=='C'&&ser.read()=='C')
        {
          char bajt;
           /*while(ser.available()&&bajt!='"')
           {
              bajt=ser.read();
              //Serial.print(bajt);
           }*/
           do_narekovaja(false);
           bajt='x';
           /*while(ser.available()&&bajt!='"')
           {
            bajt=ser.read();
            stev+=bajt;
           }*/
           stev+=do_narekovaja(true);
           bajt='x';
         /* while(ser.available()&&bajt!='"')
           {
              bajt=ser.read();
           }*/
           do_narekovaja(false);
           bajt='x';
           /*
          while(ser.available()&&bajt!='"')
           {
            bajt=ser.read();
            ime+=bajt;
           }*/
           ime+=do_narekovaja(true);
           stev.remove(stev.length()-1);
           ime.remove(ime.length()-1);

        }
      }
          ekran.setTextSize(1);
          ekran.setCursor(0,48/3+48/5);
          ekran.print(stev);
          ekran.setCursor(0,48/2+48/4.5);
          ekran.print(ime);
          ekran.display();   
         // ser_clear();   
  }
  if(smer==1)val++;
  else if(smer==0)val--;
  if(val==128)smer=0;
  if(val==0)smer=1;
  analogWrite(6,val);
  if(analogRead(A6)>30&&dbg==0){ noTone(4); osvet(1); return;}
  if(!tihi_nacin)
  {
    //tone(4,800);
    madv();
  }
  delay(10);
  if(key()=='y') 
  {
    osvet(1);
    mrun=false;
    noTone(4);
  ser.println("ATA");
  String nic="";
  v_klicu(&stev);
  }
  if(key()=='n')
  {
    osvet(1);
    mrun=false;
    ser.println("ATH");
    noTone(4);
    if(dbg)return;
  }
  
  }
  
}

void scronloop()
{
  static int antistall=0;
  static unsigned long scronloop_timing=0;
  static short lock = 0,lockn=0;
  static short pon = 0;
  //madv();
  if(key()=='x')lockn=0;
  //if(debug_izpis){ Serial.print("Pred scrl: ms= "); Serial.print(millis()); Serial.print("prej");  Serial.println(scronloop_timing);}
  if(millis()-scronloop_timing>=40)
  {
    antistall=0;
   if(debug_izpis){}
   scronloop_timing=millis();
  //Serial.println(analogRead(A6));
  if(analogRead(A6)<30&&batlvl>5&&omriz[0]!=' ') poklican(0);

  if (key()=='n') lockn++;
  else
  if (key() != 'x') {lock = 0;lockn=0;}

      if (lock >= 20||lockn >= 12) {
      lockloop();
      noter=true;
      lock = 0;
      lockn=0;
      venizlock = true;
      osvet(1);
    }
    
  if (pon % 20 == 0)
  {

    if (key() == 'x') {lock += 2;}
    osvet(1);

    
  }
  if (pon % 100 == 0)
  {
    getbat();
    getsig();
    getnet();
  }
  if (pon > 319) pon = 0;
  pon++;
  
  }
  else
  {
  delayMicroseconds(500);
  }
}
void clrchra(char noter[], int st)
{
  short i;
  for (i = 0; i < st; i++)
  {
    noter[i] = ' ';
  }
  

}
void print_array(char noter[],short dolz)
{
  short i;
  for(i=0;i<+dolz;i++)
  ekran.print(noter[i]);
}
void statusbar()
{
  int i;
  //static int baterija;
    ekran.setTextSize(1);

    ekran.fillRect(0,0,30,12,0);
    ekran.fillRect(0,0,84,3,0);
    ekran.setCursor(60,7);
    if(tihi_nacin) ekran.drawXBitmap(60,4,tiho_ikona,20,20,1);
    ekran.setCursor(0, 0);
    

    print_array(omriz,7);
    int vso=siglvl;
    for(i=0;i<5;i++)
    {
      ekran.fillRect(50+3*i,6,2,1,1);
      if(vso>5) ekran.fillRect(50+3*0,4,2,2,1);
      if(vso>10) ekran.fillRect(50+3*1,3,2,3,1);
      if(vso>15) ekran.fillRect(50+3*2,2,2,4,1);
      if(vso>20) ekran.fillRect(50+3*3,1,2,5,1);
      if(vso>25) ekran.fillRect(50+3*3,0,2,6,1);


    }
    ekran.fillRect(66, 0, (float)13*((float)batlvl/100), 4, BLACK);
    ekran.drawRect(66,0,13,4,BLACK);
    ekran.drawRect(79,1,2,2,BLACK);
    //ekran.setCursor(84/2+84/3,0);
    ekran.setCursor(25,30);
    if(masms) ekran.drawBitmap(15,12,sms_bitmap,8,8,1);
    
}

void getbat()
{
  if(dela_modul==true)
  {
  int i=0,j;
  //char omr[30];
  String lvl="";
  ser_clear();
  ser.println("AT+CBC");
  delay(30);
  i=0;
  while (ser.available()&&ser.read() != ':' && i < 30) i++;
  for(i=0;i<3&&ser.available();i++){ ser.read();}
  char bajt;
  while (ser.available()) {
    bajt=ser.read();
    if(bajt==',')break;
    lvl+=bajt;i++;}
  batlvl=lvl.toInt();
  //Serial.print("lvl:");
  //Serial.println(lvl);
  }
  //Serial.println(omr);
  //Serial.println(batlvl);
   
}
void getsig()
{
  if(dela_modul==true)
  {
  int i=0,j;
  //char omr[30];
  String lvl="";
  ser_clear();
  ser.println("AT+CSQ");
  delay(30);
  i=0;
  while (ser.available()&&ser.read() != ':' && i < 30) i++;
  for(i=0;i<1&&ser.available();i++){ ser.read();}
  char bajt;
  while (ser.available()) {
    bajt=ser.read();
    if(bajt==',')break;
    lvl+=bajt;i++;}
  siglvl=lvl.toInt();
  //Serial.print("lvl:");
  //Serial.println(lvl);
  }
  //Serial.println(omr);
  //Serial.println(batlvl);
   
}
void getnet()
{
  if(dela_modul==true)
  {
  //omriz = "";
  ser_clear();
  ser.println("AT+COPS?");
  delay(30);
  short i = 0;
  char bajt;
  //Serial.println("kle");
  while (ser.available())
  {
    bajt = ser.read();
    if(bajt==':'||i>40) break;
    i++;
  }
  String nini="Iscem";
 ser.read();
  ser.read();
  
  if(ser.read()=='0'&&ser.read()!=',')  nini.toCharArray(omriz,6);
  else
  {

   ser.read();
   ser.read();
   ser.read();
    //ser.read();
   // ser.read();
    bajt=ser.read();
    for(i=0;i<7;i++) omriz[i]=' ';
    for(i=0;i<7&&bajt!=(char)(34);i++)
    {
      omriz[i]=bajt;
      bajt=ser.read();
      Serial.print(i);
    }
     if(omriz[0]=='O'&&omriz[1]=='K') nini.toCharArray(omriz,6);

  }
  }
  else {omriz[0]=' '; omriz[1]=' '; init_modul();}
  //Serial.print("S");
  //Serial.println(omriz.length());
}
bool pritisnjena(char n)
{
  if (key() == n) return 1;
  return 0;
}
void napaka(String kaj)
{
  
}
void klici(String* st)
{
  int i;
  ser.print("ATD");
  ser.print(*st);
  ser.println(";");
  String odg="";
  delay(15);
  while (ser.available())
  {
    delay(1);
    odg+=Serial.read();
    if (i > 50) break;
  }
  if(odg.indexOf("K")!=-1); //ok
  //if(ser.read()!='0'&&ser.read()!='K') return;
  if(odg.indexOf("C")!=-1) napaka("Ni omrezja"); //no carrier
  if(odg.indexOf("E")!=-1) napaka("Napaka");
  v_klicu(st);
}
void ozadje()
{
  //narisi_bitmap(0,0,vegova_logo);
  if(stevilka_ozadja==0){ekran.drawXBitmap(0,5,vegova_zgoraj,84,30,1); ekran.drawXBitmap(0,35,vegova_spodaj,84,10,1);}
  if(stevilka_ozadja==1)ekran.drawXBitmap(0,0,avto_ozadje,84,48,1);
  if(debug_izpis)Serial.println("Ris OZADJE!");
}

void homeloop()
{
  getnet();
  osvet(1);
  unsigned long prej=0;

  while (1)
  {
    madv();
    mrun=false;
    char tipka = key();
    if (tipka == 'x'){ venizlock = false;}
    //Serial.println(tipka);
    if (tipka != 'x' && tipka != 'y' && tipka != 'n' && venizlock == false) {
      bool cez;
      String kam;
      cez=true;
      vnos_st(kam,cez,false);
      if(cez)
      klici(&kam);
    }
    if(tipka=='y'){meni(); pocakaj_spust();}  
    
    
    
    if(millis()-prej>=150)
    {
      
    ekran.clearDisplay();
    
    ozadje();
    
    
    }
    statusbar();

    ekran.display();
    prej=millis();
    scronloop();
    delay(5);
  }
}


void lockloop()
{
  noTone(4);
  int ven = 0;
  int pon = 30;
  bool held=false;
  if(key()=='n') held=true;
  while (1)
  {
    if(key()=='x') held=false;
    ekran.clearDisplay();
    ozadje();
   ekran.drawBitmap(1,12,zaklep_ikona,15,12,1);
    osvet(0);
    if(key()!='*'&&key()!='x'&&held==false)
    {
      osvet(1);
      ekran.clearDisplay();
      ekran.setCursor(0,25);
      ekran.println("Pridrzite *");
      ekran.print("za odklepanje");
      ekran.display();
      int i;
      for(i=0;i<50;i++)
      {
        if(key()!='x')i=0;
        if(key()=='*') break;
        delay(50);
      }
    }
    if(pritisnjena('*'))ven++;  
    else ven = 0;
    if (ven > 4) return;
    int i = 0;
    
    statusbar();
    ekran.setCursor(0, 0);
    if(analogRead(A6)<30&&batlvl>5&&omriz[0]!=' ')poklican(0);
    ekran.setCursor(0, 48 / 2 + 48 / 4);
   
    osvet(0);
    ekran.display();
    if (pon > 30) {
      
      getnet();
      getbat();
      pon = 0;
      
    }
    delay(10);
    pon++;
  }
}

void loop() {
  homeloop();
}
