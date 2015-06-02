##
## boxin.ino for boxin
## 
## Made by elominp
## Login   <pirou_g@epitech.net>
## 
## Started on  Tue Jun  2 17:32:55 2015 elominp
## Last update Tue Jun  2 17:33:09 2015 elominp
##

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <stdlib.h>
#include <EEPROM.h>

#define MAX_NB_DESSIN 32

#define NBLEDSPARFACE 9
#define NBFACES 4

#define OPTIONS NEO_GRB + NEO_KHZ800

#define FACE1 3
#define FACE2 5
#define FACE3 6
#define FACE4 9
#define COUVERCLE 10

#define TAILLE_TAMPON_SERIE 200

#define NB_DESSINS 14

typedef struct pixel
{
  unsigned char rouge;
  unsigned char vert;
  unsigned char bleu;
} pixel;

Adafruit_NeoPixel faces[NBFACES + 1] = { Adafruit_NeoPixel(9, FACE1, OPTIONS),
                                         Adafruit_NeoPixel(9, FACE2, OPTIONS),
                                         Adafruit_NeoPixel(9, FACE3, OPTIONS),
                                         Adafruit_NeoPixel(9, FACE4, OPTIONS),
                                         Adafruit_NeoPixel(1, COUVERCLE, OPTIONS) };
                               
String tampon = "";
bool status_tampon = false;
unsigned char lumiere = 64;

/*pixel dessins[NB_DESSINS][9] = { { { 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }, { 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }, { 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 } },
                                 { { 255, 255, 0 }, { 0, 255, 255 }, { 255, 0, 255 }, { 255, 255, 0 }, { 0, 255, 255 }, { 255, 0, 255 }, { 255, 255, 0 }, { 0, 255, 255 }, { 255, 0, 255 } },
                                 { { 120, 180, 255 }, { 180, 120, 255 }, { 255, 120, 180 }, { 120, 180, 255 }, { 180, 120, 255 }, { 255, 120, 180 }, { 120, 180, 255 }, { 180, 120, 255 }, { 255, 120, 180 } },
                                 { { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 255 } },
                                 { { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 } },
                                 { { 0, 255, 0 }, { 0, 255, 0 }, { 0, 255, 0 }, { 0, 255, 0 }, { 0, 255, 0 }, { 0, 255, 0 }, { 0, 255, 0 }, { 0, 255, 0 }, { 0, 255, 0 } },
                                 { { 0, 0, 255 }, { 0, 0, 255 }, { 0, 0, 255 }, { 0, 0, 255 }, { 0, 0, 255 }, { 0, 0, 255 }, { 0, 0, 255 }, { 0, 0, 255 }, { 0, 0, 255 } },
                                 { { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 } },
                                 { { 0, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 } },
                                 { { 0, 0, 0 }, { 255, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 } },
                                 { { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 } },
                                 { { 255, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 } },
                                 { { 0, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 255, 0, 0 }, { 0, 0, 0 } },
                                 { { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 }, { 255, 0, 0 } } };

void eeprom_update(unsigned int adress, unsigned char data)
{
  if (adress >= 1024 || EEPROM.read(adress) == data)
    return;
  EEPROM.write(adress, data);
}

void ecrire_dessin(unsigned int numero, void *pointeur)
{
  pixel *dessin = (pixel *)pointeur;
  if (numero > MAX_NB_DESSIN)
    return;
  for (char i = 0; i < 9; i++)
  {
    eeprom_update((i * 3) + (numero * 9 * 3), dessin[i].rouge);
    eeprom_update((i * 3) + (numero * 9 * 3) + 1, dessin[i].vert);
    eeprom_update((i * 3) + (numero * 9 * 3) + 2, dessin[i].bleu);
  }
}

void ecrire_tous_les_dessins()
{
  for (int i = 0; i < NB_DESSINS; i++)
    ecrire_dessin(i, dessins[i]);
}*/

void *lire_dessin(unsigned int numero)
{
  static pixel *tableau = (pixel *)malloc(sizeof(pixel) * 9);
  
  if (numero > MAX_NB_DESSIN)
    return (NULL);
  for (char i = 0; i < 9; i++)
  {
    tableau[i].rouge = EEPROM.read((numero * 9 * 3) + (i * 3));
    tableau[i].vert = EEPROM.read((numero * 9 * 3) + (i * 3)  + 1);
    tableau[i].bleu = EEPROM.read((numero * 9 * 3) + (i * 3) + 2);
  }
  return (tableau);
}

void initialise_faces()
{
  for (int i = 0; i < NBFACES; i++)
  {
    faces[i].begin();
    faces[i].show();
  }
}

void reinitialise_leds(char face)
{
  if (face < 0 || face >= NBFACES)
    return;
  for (int i = 0; i < NBLEDSPARFACE; i++)
    faces[face].setPixelColor(i, 0, 0, 0);
  faces[face].show();
}

void setup() {
  // put your setup code here, to run once:
  initialise_faces();
  //ecrire_tous_les_dessins();
  Serial.begin(9600);
  tampon.reserve(TAILLE_TAMPON_SERIE);
  status_tampon = false;
  Serial.println("L'initialisation de boxin est terminée");
}

void loop() {
  const char *tableau;
  int dessin = 0;
  char face = 0;
  // put your main code here, to run repeatedly:
  if (status_tampon)
  {
    if (tampon.length() >= 3 && (tableau = tampon.c_str()) != NULL)
    {
      face = (tableau[0] - 48);
      lumiere = (unsigned char)pow(2, tableau[1] - 48);
      if (tampon.length() >= 4)
        dessine(face, dessin = atoi(tableau + 2));
      faces[face].setBrightness(lumiere);
      faces[face].show();
    }
    else if (tampon.length() == 2 && (tableau = tampon.c_str()) != NULL)
      reinitialise_leds(tableau[0] - 48);
    tampon = "";
    status_tampon = false;
  }
}

void dessine(char face, int dessin)
{
  pixel *tableau;
  
  if (face < 0 || face > COUVERCLE || dessin < 0 || dessin >= NB_DESSINS ||
      (tableau = (pixel *)lire_dessin(dessin)) == NULL)
    return;
  for (int i = 0; i < NBLEDSPARFACE; i++)
  {
    faces[face].setPixelColor(i, tableau[i].rouge,
                                 tableau[i].vert,
                                 tableau[i].bleu);
  }
}

void serialEvent()
{
  char c;
  
  while (Serial.available())
  {
    c = (char)Serial.read();
    tampon += c;
    if (c == '\n')
      status_tampon = true;
    if (tampon.length() == (TAILLE_TAMPON_SERIE - 1))
    {
      tampon += '\n';
      status_tampon = true;
    }
  }
}
