#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include <DS3231.h>
#define TX 3
#define RX 4
TinyGPS gps;
SoftwareSerial swSerial(RX, TX);
DS3231 rtc;
RTCDateTime Cas;
/*################# konfigurace  ############*/
//Letní nastavení
int letoZapniHodina1 = 5;//17 požadavek byl: v létě svítit 5-6 hod a 20-01 hod. Zima 15:30- 01 a 6-7 hod. Zima 1.11. - 31.3.
int letoZapniMinuta1 = 0;
int letoZapniHodina2 = 20;//17
int letoZapniMinuta2 = 0;

int letoVypniHodina1 = 6; //6
int letoVypniMinuta1 = 0;//0
int letoVypniHodina2 = 1; //6
int letoVypniMinuta2 = 0;//0

//zimní nastavení
int ZimaZapniHodina1 = 6; // požadavek byl: v létě svítit 5-6 hod a 20-01 hod. Zima 15:30- 01 a 6-7 hod. Zima 1.11. - 31.3.
int ZimaZapniMinuta1 = 0;
int ZimaZapniHodina2 = 15;//
int ZimaZapniMinuta2 = 30;

int ZimaVypniHodina1 = 7; //
int ZimaVypniMinuta1 = 0;//
int ZimaVypniHodina2 = 1; //
int ZimaVypniMinuta2 = 0;//

//nastavení kdy začína zima a kdy léto
int letoDen = 1;
int letoMesic = 4;

int zimaDen = 1;
int zimaMesic = 11;






#define Pot1 A0 //obrys
#define Pot2 A1 //vnitrek
#define Pot3 A2 //napis

#define Out1 9 //obrys
#define Out2 10 //vnitrek
#define Out3 11 //napis

#define cervena 6 //power (běží program)
#define zelena 7  //GPS
#define modra 8   //letni rezim

#define prepinac 5
#define rychlost 5

void setup() {
  swSerial.begin(9600);
  Serial.begin(9600);
  rtc.begin();
  
  pinMode(Out1, OUTPUT);
  pinMode(Out2, OUTPUT);
  pinMode(Out3, OUTPUT);
  pinMode(cervena, OUTPUT);
  pinMode(zelena, OUTPUT);
  pinMode(modra, OUTPUT);
  pinMode(prepinac, INPUT_PULLUP);
  for(uint8_t i = 0; i < 255; i++){
    analogWrite(cervena, i);
    delay(30);
  }


  
  for (int i = 0; i < map(analogRead(Pot1), 0, 1023, 0, 255); i++) {
    analogWrite(Out1, i);
    delay(rychlost);
  }
  for (int i = 0; i < map(analogRead(Pot2), 0, 1023, 0, 255); i++) {
    analogWrite(Out2, i);
    delay(rychlost);
  }
  for (int i = 0; i < map(analogRead(Pot3), 0, 1023, 0, 255); i++) {
    analogWrite(Out3, i);
    delay(rychlost);
  }
  digitalWrite(cervena, HIGH); //program jede
}

void loop() {
  Cas = rtc.getDateTime();
    Serial.print("Raw data: ");
  Serial.print(Cas.year);   Serial.print("-");
  Serial.print(Cas.month);  Serial.print("-");
  Serial.print(Cas.day);    Serial.print(" ");
  Serial.print(Cas.hour);   Serial.print(":");
  Serial.print(Cas.minute); Serial.print(":");
  Serial.print(Cas.second); Serial.println("");
/*
if( !digitalRead(prepinac) ){ //kod bezi pri zapnuti auto
  if ( ((Cas.month >= letoMesic + 1 && Cas.month <= zimaMesic - 1) || (Cas.month == letoMesic && Cas.day >= letoDen) || (Cas.month == zimaMesic && Cas.day <= zimaDen))  ) { //leto nebo je zvoleno leto na prepinaci
    digitalWrite(modra, HIGH);
    if( (Cas.hour >= letoZapniHodina+1 && Cas.hour <= letoVypniHodina-1) || (Cas.hour == letoZapniHodina && Cas.minute >= letoZapniMinuta) || (Cas.hour == letoVypniHodina && Cas.minute <= letoVypniMinuta) ){
      zapni();
    }
    else{  //vypnuti ve dne
      vypni();
    }
  }
  else{ //zima
    zapni();
  }

}else{
  
  digitalWrite(modra, LOW);
  zapni();
}
*/
if( !digitalRead(prepinac) ){ //kod bezi pri zapnuti auto
  if ( ((Cas.month == letoMesic && Cas.day >= letoDen) || (Cas.month > letoMesic && Cas.month < zimaMesic) || (Cas.month == zimaMesic && Cas.day <= zimaDen))  ) { //leto nebo je zvoleno leto na prepinaci
    digitalWrite(modra, HIGH);
    if( (Cas.hour == letoZapniHodina1 && Cas.minute >= letozapniMinuta1) || (Cas.hour > letoZapniHodina1 || Cas.hour < letoVypniHodina1) || (Cas.hour == letoVypniHodina1 && Cas.minute <= letoVypniMinuta1) ){
      zapni(); // svítí od 5:00 do 6:00
    }
    else if( (Cas.hour == letoZapniHodina2 && Cas.minute >= letozapniMinuta2) || (Cas.hour > letoZapniHodina2 || Cas.hour < letoVypniHodina2) || (Cas.hour == letoVypniHodina2 && Cas.minute <= letoVypniMinuta2) ){
      zapni(); // svítí od 20:00 do 1:00
    }
    
    else{  //vypnuti v době nečinnosti
      vypni();
    }
  }
  else{ //zima
    if( (Cas.hour == ZimaZapniHodina1 && Cas.minute >= ZimazapniMinuta1) || (Cas.hour > ZimaZapniHodina1 || Cas.hour < ZimaVypniHodina1) || (Cas.hour == ZimaVypniHodina1 && Cas.minute <= ZimaVypniMinuta1) ){
      zapni(); // svítí od 6:00 do 7:00
    }
    else if( (Cas.hour == letoZapniHodina2 && Cas.minute >= letozapniMinuta2) || (Cas.hour > letoZapniHodina2 || Cas.hour < letoVypniHodina2) || (Cas.hour == letoVypniHodina2 && Cas.minute <= letoVypniMinuta2) ){
      zapni(); // svítí od 15:30 do 1:00
    }
    
    else{  //vypnuti v době nečinnosti
      vypni();
    }
  }

}else{  //manualni rezim still on
  
  digitalWrite(modra, LOW);
  zapni();
}



  bool novaData = false;
  unsigned long znaky;
  unsigned short slova, chyby;
  // po dobu jedné vteřiny budeme kontrolovat příjem dat z GPS
  for (unsigned long start = millis(); millis() - start < 1000;) {
    // kontrola aktivity softwarové komunikace
    while (swSerial.available()) {
      // vytvoření proměnné pro uložení načtených dat z GPS
      char c = swSerial.read();
      //Serial.write(c); // pro výpis přijatých dat odkomentujte tento řádek
      // dekódování přijaté zprávy s kontrolou platných dat
      if (gps.encode(c)) {
        // pokud jsou přijatá data platná, nastavíme proměnnou pro tištění dat
        novaData = true;
      }
    }
  }
  // pokud proběhl příjem nových dat, vytiskneme všechny dostupné informace
  if (novaData) {
    digitalWrite(zelena, HIGH);
    // vytvoření dočasných proměnných pro načtení dat z GPS modulu
    float zSirka, zDelka;
    unsigned long stariDat;
    int rok;
    byte mesic, den, hodina, minuta, sekunda, setinaSekundy;
    // načtení GPS pozice do proměnných
    gps.f_get_position(&zSirka, &zDelka, &stariDat);
    // vytištění informací po sériové lince
    Serial.println("::Dostupne GPS udaje::");
    // nejprve zkontrolujeme, jestli máme platné údaje
    // (zSirka == TinyGPS::GPS_INVALID_F_ANGLE),
    // pokud nejsou validní (platné), vytiskneme nulu,
    // v opačném případě vytiskneme obsah proměnné s přesností 6 desetinných míst,
    // podobným způsobem se pracuje i s ostatními údaji
    Serial.print(" Pocet satelitu: ");
    Serial.println(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print("Presnost: ");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    Serial.print(" Stari dat: ");
    Serial.print(stariDat == TinyGPS::GPS_INVALID_AGE ? 0 : stariDat);
    // načtení data a času z GPS modulu do proměnných
    gps.crack_datetime(&rok, &mesic, &den, &hodina, &minuta, &sekunda, &setinaSekundy, &stariDat);
    // kontrola platnosti dat

    if (stariDat == TinyGPS::GPS_INVALID_AGE) {
      Serial.println("Nelze nacist datum a cas.");
      digitalWrite(zelena, LOW);
    } else {
      // vytvoření proměnné pro vytištění data a času
      char datumCas[32];
      Serial.print("Datum a cas: ");
      // poskládání celé zprávy do proměnné datumCas a poté její vytištění,
      // %02d znamená desetinné číslo uvedené za uvozovkami s přesností na 2 číslice
      if (hodina == 0 && minuta == 0 && (den % 2 == 0)) { //každou půlnoc , každý sudý den
        rtc.setDateTime(rok, mesic, den, hodina, minuta, sekunda);
        Serial.println("cas synchronizovan");
        Serial.print("cas: ");
        Serial.print(Cas.hour);
        Serial.print(" : ");
        Serial.println(Cas.minute);

      }
      sprintf(datumCas, "%02d/%02d/%02d %02d:%02d:%02d", mesic, den, rok, hodina, minuta, sekunda);
      Serial.println(datumCas);
    }
  }
  // načtení a vytištění informací o komunikaci s GPS modulem
  gps.stats(&znaky, &slova, &chyby);
  Serial.print("chyby pri kontrole dat: ");
  Serial.println(chyby);
  digitalWrite(zelena, !chyby);
  // kontrola chyb při komunikaci skrze detekci přijatých znaků
  if (znaky == 0) {
    Serial.println("Chyba pri prijmu dat z GPS, zkontrolujte zapojeni!");
  }









  if (novaData) {
    digitalWrite(zelena, HIGH);
  } else {
    digitalWrite(zelena, LOW);
  }

}

void zapni() {

  analogWrite(Out1, map(analogRead(Pot1), 0, 1023, 0, 255)); //nastaveni jasu podle potenciometru
  analogWrite(Out2, map(analogRead(Pot2), 0, 1023, 0, 255));
  analogWrite(Out3, map(analogRead(Pot3), 0, 1023, 0, 255));
}
void vypni() {

  analogWrite(Out1, 0); //nastaveni jasu podle potenciometru
  analogWrite(Out2, 0);
  analogWrite(Out3, 0);
}
