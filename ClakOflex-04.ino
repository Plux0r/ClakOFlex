#include <TM1637Display.h>      // lib pour afficheur 7 segments
#include <Adafruit_NeoPixel.h>  // lib pour anneau led
#include "pitches.h"            // lib correspondance nom de note > frequence note pour utilisation de tone(pin,freq) 
#include "avr/pgmspace.h"       // lib pour ecriture/lecture en flash
#define CLK A4          // CLK  TM1637
#define DIO A5          // DIO  TM1637
#define Mushroom 2              // bouton champignon sur entrée avec interruption
#define BUTTON_LEFT 4           // bouton gauche
#define BUTTON_RIGHT 5          // bouton droit
#define BUZZER 3                // sortie Buzzer avec PWM possible 
// #define BUZZER 13            // un test

#define NEOPIXEL_PIN 10         // pin de l'anneau LED
#define NUMPIXELS 12            // nombre de LED sur l'anneau

TM1637Display display(CLK, DIO);  
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);


// variables gloabales parce que c'est plus facile
volatile bool interruptFlag = false;    // flag d'interruption initialise a faux
uint16_t startTime;
int duration;
int note;
int mel, dur;
uint16_t reactionTime;
// composantes couleur pour l'anneau LED
uint8_t REDcolor;
uint8_t GREENcolor;
uint8_t BLUEcolor;


void setup() 
{
  pinMode(Mushroom, INPUT_PULLUP);      // le BP tire à la masse la patte 2, on y met donc une pull-up pour lire un "1" au repos
  pinMode(BUTTON_LEFT, INPUT_PULLUP);   // le BP tire à la masse la patte 2, on y met donc une pull-up pour lire un "1" au repos
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);  // le BP tire à la masse la patte 2, on y met donc une pull-up pour lire un "1" au repos
  pinMode(BUZZER, OUTPUT);              // configure la pin du buzzer en sortie
  digitalWrite(BUZZER, HIGH);           // on force la sortie à 1 pour éviter de tuer le buzzer
  Serial.begin(9600);                   // initialisation de la liaison série à 9600BPs, c'est pratique pour le debug
  display.setBrightness(0x0f);          // luminosite de l'afficheur
  display.showNumberDec(0);             // on affiche "0" pour valider que l'afficheur fonctionne 

  strip.begin();                        // on initialise l'anneau led
  strip.setBrightness(32);              // on baisse beaucoup la luminosité sinon c'est pénible
  strip.show();                         // on envoie cette config à l'anneau

  // initialisation de l'interruption du BP champignon
  // handleInterrupt est la fonction qui sera appelé chaque fois que la pin "Mushroom" verra un front descendant "FALLING"
  attachInterrupt(digitalPinToInterrupt(Mushroom), handleInterrupt, FALLING);


}

void loop() 
{
  // PlayMusic();
  // couleur aléatoire sur l'anneau led
  REDcolor = random(0,0xFF);
  GREENcolor = random(0,0xFF);
  BLUEcolor = random(0,0xFF);
  //tempo aléatoire entre 1 et 5S
  delay(random(1000, 5000));
  // allumage de l'anneau LED
  setNeoPixelColor(strip.Color(REDcolor, GREENcolor, BLUEcolor));
  // on démarre le tiemr
  startTime = millis();
  interruptFlag = false;
  // tant qu'il n'y a pas eu d'appui BP on est bloquant ici ! On aurait pu le faire simplement avec un while(digitalRead(Mushroom)==HIGH), mais l'idée ici est d'utiliser une interruption
  while (!interruptFlag) {
  }
  //calcul du temps de réaction
  reactionTime = millis() - startTime;
  // on éteint l'anneau led
  setNeoPixelColor(strip.Color(0, 0, 0)); 
  // on affiche le temps de réaction en mS
  display.showNumberDec(reactionTime);
  // un petite LA pendant 200mS
  Beep(440, 200);
  
  digitalWrite(BUZZER, HIGH);
  
  delay(2000);
  //et on recommence
}


// fonction appelé à l'interruption
void handleInterrupt() 
{
  // on change l'état du flag d'interruption
  // il faut faire du code ultra court et non bloquant en interruption
  interruptFlag = true;
}

void Beep(int frequency, int duration) {
  // tone(BUZZER, frequency, duration);
  tone(BUZZER, frequency);
  delay(duration);
  noTone(BUZZER);
  digitalWrite(BUZZER, HIGH);
}

void setNeoPixelColor(uint32_t color) {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}


// là où est stoqué la musique de "Doom's gate"
// sans entrer dans les détails, il faut déclarer le tableau avec les préfixes "stati const PROGMEM"
// la lecture se fait avec la fonction pgm_read_byte() : pgm_read_byte(melody+index) : le paramètre est l'adresse de la variable que l'on souhaite. 
// voir la fonction PlayMusic()
static const  PROGMEM int melody[] = {
  82,82,165,82,82,147,82,82,
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,
  
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,
  
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,
  
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
  NOTE_FS3, NOTE_D3, NOTE_B2, NOTE_A3, NOTE_FS3, NOTE_B2, NOTE_D3, NOTE_FS3, NOTE_A3, NOTE_FS3, NOTE_D3, NOTE_B2,
  
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,
  
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
  NOTE_B3, NOTE_G3, NOTE_E3, NOTE_G3, NOTE_B3, NOTE_E4, NOTE_G3, NOTE_B3, NOTE_E4, NOTE_B3, NOTE_G4, NOTE_B4,
  
  NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2, 
  NOTE_F3, NOTE_A2, NOTE_A2, NOTE_DS3, NOTE_A2, NOTE_A2, NOTE_E3, NOTE_F3,
  NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2,
  NOTE_F3, NOTE_A2, NOTE_A2, NOTE_DS3,
  
  NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2, 
  NOTE_F3, NOTE_A2, NOTE_A2, NOTE_DS3, NOTE_A2, NOTE_A2, NOTE_E3, NOTE_F3,
  NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2,
  NOTE_A3, NOTE_F3, NOTE_D3, NOTE_A3, NOTE_F3, NOTE_D3, NOTE_C4, NOTE_A3, NOTE_F3, NOTE_A3, NOTE_F3, NOTE_D3,
  
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,
  
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, 
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
  NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
  NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,
};

static const  PROGMEM int durations[] = {
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,  
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 2,
  
  8, 8, 8, 8, 8, 8, 8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
};


// faire delta ce qui écrit et ce qui est lu
void PlayMusic()
{
    const int size = sizeof(durations) / sizeof(int);
    Serial.println("Size="); Serial.print(size);
  //int size = 1500 ; 
  for ( note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
     duration = 1000 / pgm_read_byte(durations+note);
     mel = pgm_read_byte(melody+note);
    tone(BUZZER,mel , duration);
    Serial.println("Note n°"); Serial.print(note); Serial.print(":"); Serial.print(mel);
    Serial.print(" ");
    Serial.print("duration n°"); Serial.print(note); Serial.print(":"); Serial.print(duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    
    //stop the tone playing:
    noTone(BUZZER);
  }
}