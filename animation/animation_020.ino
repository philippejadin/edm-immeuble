/*************************************************** 
 * Servo control EDM
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver etage1 = Adafruit_PWMServoDriver(0x42);
Adafruit_PWMServoDriver etage2 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver etage3 = Adafruit_PWMServoDriver(0x40);


#define SERVOMIN  120 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  400 // this is the 'maximum' pulse length count (out of 4096)


const int servo_enable_pin = 2;  // numéro d'entrée utilisée pour activer ou désactiver tous les servos (switch général)
const int interval_enable_pin = 6;  // numéro d'entrée utilisée pour activer ou désactiver le système d'interval
int angle;
int val;
boolean actif = true;

void setup() {
  pinMode(servo_enable_pin, INPUT);  // interrupteur qui coupe 
  pinMode(interval_enable_pin, INPUT);  // interrupteur qui coupe 
  // une variable par étage
  etage1.begin();
  etage1.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
  etage2.begin();
  etage2.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
  etage3.begin();
  etage3.setPWMFreq(50);  // Analog servos run at ~50 Hz updates

  Serial.begin(9600);
  randomSeed(analogRead(1) + analogRead(2));

}




/* 
 Permet de controller un servo
 
 Etge :  de 1 à 3 (numéro de la carte I2C)
 Servo : de 0 à 15
 Angle : de 0 à 180 degré
 */

void setservo(int etage, int servo, int angle)
{

  if (digitalRead(servo_enable_pin) == HIGH) // si le switch général qui active les servo est HIGH 
  {
    if (actif) // ET si l'intervalomettre n'a pas décidé qu'on était inactif
    {
      double pulselength; 
      pulselength = map(angle, 0, 180, SERVOMIN, SERVOMAX);

      if (etage==1)
      {
        etage1.setPWM(servo, 0, pulselength);
      }

      if (etage==2)
      {
        etage2.setPWM(servo, 0, pulselength);
      }

      if (etage==3)
      {
        etage3.setPWM(servo, 0, pulselength);
      }
    }
  }
}



unsigned long offset_last = 0;
unsigned long cravate_last = 0;
unsigned long tapeur_last = 0;
unsigned long dessinateur_last = 0;
unsigned long scenariste_last = 0;
unsigned long photocopieuse_last = 0;
unsigned long pigeon_last = 0;
unsigned long interface_last = 0;
unsigned long intervalometre_last = 0;
unsigned long facteur = 100; // facteur de base temporelle, en pourcent.



void loop() 
{


  unsigned long current = millis(); 


  // gestion de l'intervalometre
  if (digitalRead(interval_enable_pin) == HIGH)
  {
    if (actif)
    {
      if(current - intervalometre_last > 1000 * 30) // 30 secondes
      {
        intervalometre_last = current;
        actif = false;
        Serial.println("------------------intervalometre active et animations en pause------------------"); 
      }
    }
    
    if (!actif)
    {
      if(current - intervalometre_last > 1000 * 30) // 1 minutes de pause
      {
        intervalometre_last = current;
        actif = true;
        Serial.println("-----------------intervalometre active et animations en cours------------------"); 
      }
    }
  }
  else
  {
    actif = true;
  }



  // -------- gestion interface utilisateur physique ----------
  // on sample le potentiomètre toutes les 100 msecs


  if(current - interface_last < 500)
  {

  }
  else if(current - interface_last < 1000)
  {
    interface_last = current;
    val = analogRead(0);            // reads the value of the potentiometer (value between 0 and 1023) 
    facteur = map(val, 1023, 0, 30, 100);
    Serial.print("facteur : "); 
    Serial.println(facteur); 
  }


  // -------- etage 1 ----------



  // Machine Ofset

  if(current - offset_last < 22000)
  {

  }
  else if(current - offset_last < 23000)
  {
    setservo(1, 0, 180); // moteur tournant à fond
    Serial.println("offset : demarre");
  }

  else if(current - offset_last < 24000)
  {
    setservo(1, 1, 180); // papier sorti d'un coté
    Serial.println("offset : papier in");
  }

  else if(current - offset_last < 25000)
  {
    setservo(1, 1, 0); // papier sorti de l'autre coté
    Serial.println("offset : papier out");
  }

  // todo plus de va et vient
  else if(current - offset_last < 26000)
  {
    offset_last = current;
    setservo(1, 0, 119); // moteur tournant coupé
    Serial.println("offset : fin de boucle, arret moteur"); 
  }






  // -------- etage 2 ---------- 




  // gars en cravate qui dit non

    if(current - cravate_last < 21000 * facteur / 100)
  {

    //Serial.print("cravate : vitesse : ");
    //Serial.println(21000 * facteur / 100);

  }
  else if(current - cravate_last < 22000 * facteur / 100)
  {
    setservo(2, 2, 180);
    Serial.println("cravate : droite");
  }

  else if(current - cravate_last < 23000 * facteur / 100)
  {
    setservo(2, 2, 40);
    Serial.println("cravate : gauche");
  }

  else if(current - cravate_last < 24000 * facteur / 100)
  {
    setservo(2, 2, 180);
    Serial.println("cravate : droite");
  }

  else if(current - cravate_last < 25000 * facteur / 100)
  {
    setservo(2, 2, 40);
    Serial.println("cravate : gauche");
  }

  else if(current - cravate_last < 26000 * facteur / 100)
  {
    setservo(2, 2, 180);

    Serial.println("cravate : gauche"); 
  }
  else if(current - cravate_last < 27000 * facteur / 100)
  {
    cravate_last = current;
    setservo(2, 2, 100);
    Serial.println("cravate : fin de boucle, retour au centre"); 
  }







  // animation du tapeur à l'ordi 

  if(current - tapeur_last < 10000  * facteur / 100)
  {
    // fait rien pendant 10 secondes
  }
  else if(current - tapeur_last < 11000 * facteur / 100)
  {
    setservo(2, 0, 30); // bras droit
    setservo(2, 1, 50); // bras gauche
    Serial.println("tapeur : tape 1");
  }

  else if(current - tapeur_last < 12000 * facteur / 100)
  {
    setservo(2, 0, 110); // bras droit
    setservo(2, 1, 150); // bras gauche
    Serial.println("tapeur : tape 2");
  }

  else if(current - tapeur_last < 13000 * facteur / 100)
  {
    setservo(2, 0, 30); // bras droit
    setservo(2, 1, 50); // bras gauche
    Serial.println("tapeur : tape 1");
  }

  else if(current - tapeur_last < 14000 * facteur / 100)
  {
    setservo(2, 0, 110); // bras droit
    setservo(2, 1, 150); // bras gauche
    Serial.println("tapeur : tape 2");
  }


  else if(current - tapeur_last < 15000 * facteur / 100)
  {
    setservo(2, 0, 30); // bras droit
    setservo(2, 1, 50); // bras gauche
    Serial.println("tapeur : tape 1");
  }

  else if(current - tapeur_last < 16000 * facteur / 100)
  {
    setservo(2, 0, 110); // bras droit
    setservo(2, 1, 150); // bras gauche
    Serial.println("tapeur : tape 2");
  }


  else if(current - tapeur_last < 17000 * facteur / 100)
  {
    setservo(2, 0, 30); // bras droit
    setservo(2, 1, 50); // bras gauche
    Serial.println("tapeur : tape 1");
  }

  else if(current - tapeur_last < 18000 * facteur / 100)
  {
    setservo(2, 0, 110); // bras droit
    setservo(2, 1, 150); // bras gauche
    Serial.println("tapeur : tape 2");
  }


  else if(current - tapeur_last < 19000 * facteur / 100)
  {
    tapeur_last = current;
    setservo(2, 0, 85); 
    setservo(2, 1, 94);
    Serial.println("tapeur : fin de boucle, retour au centre"); 
  }





  // -------- etage 3 ----------

  // animation du dessinateur :    

  if(current - dessinateur_last < 8000 * facteur / 100)
  {
    // fait rien pendant 10 secondes
  }
  else if(current - dessinateur_last < 9000 * facteur / 100)
  {
    setservo(3, 1, 35); //abaisse le bras
    setservo(3, 0, 50); // bouge le coude
    Serial.println("dessinateur : abaisse bras et bouge coude");
  }

  else if(current - dessinateur_last < 10500 * facteur / 100)
  {
    setservo(3, 0, 0); //extérieur de la planche à dessin
    setservo(3, 1, 27);
    Serial.println("dessinateur : dessine exterieur");
  }

  else if(current - dessinateur_last < 11000 * facteur / 100)
  {
    setservo(3, 0, 60);
    setservo(3, 1, 35);
    Serial.println("dessinateur : dessine interieur");
  }

  else if(current - dessinateur_last < 11500 * facteur / 100)
  {
    setservo(3, 0, 0); //extérieur de la planche à dessin
    setservo(3, 1, 35);
    Serial.println("dessinateur : dessine exterieur");
  }


  else if(current - dessinateur_last < 12000 * facteur / 100)
  {
    setservo(3, 0, 60);
    setservo(3, 1, 35);
    Serial.println("dessinateur : dessine interieur");
  }


  else if(current - dessinateur_last < 12500 * facteur / 100)
  {
    setservo(3, 0, 0); //extérieur de la planche à dessin
    setservo(3, 1, 27);
    Serial.println("dessinateur : dessine exterieur");
  }


  else if(current - dessinateur_last < 13000 * facteur / 100)
  {
    setservo(3, 0, 60);
    setservo(3, 1, 35);
    Serial.println("dessinateur : dessine interieur");
  }


  else if(current - dessinateur_last < 13500 * facteur / 100)
  {
    setservo(3, 0, 0); //extérieur de la planche à dessin
    setservo(3, 1, 27);
    Serial.println("dessinateur : dessine exterieur");
  }


  else if(current - dessinateur_last < 14000 * facteur / 100)
  {
    setservo(3, 0, 60);
    setservo(3, 1, 35);
    Serial.println("dessinateur : dessine interieur");
  }

  else if(current - dessinateur_last < 14500 * facteur / 100)
  {
    setservo(3, 0, 0); //extérieur de la planche à dessin
    setservo(3, 1, 27);
    Serial.println("dessinateur : dessine exterieur");
  }


  else if(current - dessinateur_last < 15000 * facteur / 100)
  {
    setservo(3, 0, 60);
    setservo(3, 1, 35);
    Serial.println("dessinateur : dessine interieur");
  }

  else if(current - dessinateur_last < 15500 * facteur / 100)
  {
    setservo(3, 0, 0); //extérieur de la planche à dessin
    setservo(3, 1, 27);
    Serial.println("dessinateur : dessine exterieur");
  }


  else if(current - dessinateur_last < 16000 * facteur / 100)
  {
    setservo(3, 0, 60);
    setservo(3, 1, 35);
    Serial.println("dessinateur : dessine interieur");
  }

  else if(current - dessinateur_last < 16500 * facteur / 100)
  {
    setservo(3, 0, 30); // crayon en bouche
    setservo(3, 1, 60); //lève le bras
    Serial.println("dessinateur : crayon en bouche et fin de boucle");
    dessinateur_last = current;
  }






  // animation du scénariste



  if(current - scenariste_last < 10000 * facteur / 100)
  {
    // fait rien pendant 10 secondes
  }
  else if(current - scenariste_last < 10800 * facteur / 100)
  {
    setservo(3, 2, 16);
    Serial.println("scenariste : pas 1");
  }

  else if(current - scenariste_last < 11600 * facteur / 100)
  {
    setservo(3, 2, 40);
    Serial.println("scenariste : pas 2");
  }

  else if(current - scenariste_last < 12400 * facteur / 100)
  {
    setservo(3, 2, 80);
    Serial.println("scenariste : pas 3");
  }

  else if(current - scenariste_last < 13200 * facteur / 100)
  {
    setservo(3, 2, 110);
    Serial.println("scenariste : pas 4");
  }


  else if(current - scenariste_last < 14000 * facteur / 100)
  {
    setservo(3, 2, 156);
    Serial.println("scenariste : pas 5");
  }


  else if(current - scenariste_last < 14800 * facteur / 100)
  {
    setservo(3, 2, 110);
    Serial.println("scenariste : pas 6");
  }


  else if(current - scenariste_last < 15600 * facteur / 100)
  {
    setservo(3, 2, 80);
    Serial.println("scenariste : pas 7");
  }


  else if(current - scenariste_last < 16200 * facteur / 100)
  {
    setservo(3, 2, 40);
    Serial.println("scenariste : pas 8");
  }


  else if(current - scenariste_last < 17000 * facteur / 100)
  {

    Serial.println("scénariste : fin de boucle");
    scenariste_last = current;
  }





  // animation de la photocopieuse

  if(current - photocopieuse_last < 14000 * facteur / 100)
  {

  }
  else if(current - photocopieuse_last < 15000 * facteur / 100)
  {
    setservo(3, 3, 180);
    Serial.println("photocopieuse : haut");
  }

  else if(current - photocopieuse_last < 16000 * facteur / 100)
  {
    setservo(3, 3, 20);
    Serial.println("photocopieuse : bas");
  }
  else if(current - photocopieuse_last < 17000 * facteur / 100)
  {
    setservo(3, 3, 180);
    Serial.println("photocopieuse : haut");
  }

  else if(current - photocopieuse_last < 18000 * facteur / 100)
  {
    setservo(3, 3, 20);
    Serial.println("photocopieuse : bas");
  }
  else if(current - photocopieuse_last < 19000 * facteur / 100)
  {
    setservo(3, 3, 180);
    Serial.println("photocopieuse : haut");
  }

  else if(current - photocopieuse_last < 20000 * facteur / 100)
  {
    setservo(3, 3, 20);
    Serial.println("photocopieuse : fin de boucle, retour en bas"); 
    photocopieuse_last = current;
  }







  // -------- etage toit (relié au 3) ----------
  // pigeons

  if(current - pigeon_last < 6000 * facteur / 100)
  {

  }
  else if(current - pigeon_last < 7000 * facteur / 100)
  {
    setservo(3, 4, 110); 
    Serial.println("pigeon : haut");
  }

  else if(current - pigeon_last < 8000 * facteur / 100)
  {
    setservo(3, 4, 20); 
    Serial.println("pigeon : bas");
  }
  else if(current - pigeon_last < 9000 * facteur / 100)
  {
    setservo(3, 4, 110); 
    Serial.println("pigeon : haut");
  }

  else if(current - pigeon_last < 10000 * facteur / 100)
  {
    setservo(3, 4, 20); 
    Serial.println("pigeon : bas");
  }
  else if(current - pigeon_last < 11000 * facteur / 100)
  {
    setservo(3, 4, 110); 
    Serial.println("pigeon : haut");
  }

  else if(current - pigeon_last < 12000 * facteur / 100)
  {
    setservo(3, 4, 20); 
    Serial.println("pigeon : bas");
  }

  else if(current - pigeon_last < 13000 * facteur / 100)
  {

    Serial.println("pigeon : reset");
    pigeon_last = current;
  }







  // delay(500);



  /*
  
   // -------- etage 1 ----------
   
   
   //machine offset
   setservo(1, 0, 180); // moteur tournant à fond
   delay(1000);
   
   
   setservo(1, 1, 180); // papier sorti d'un coté
   delay(600);
   setservo(1, 1, 0); // papier sorti de l'autre coté
   delay(600);
   setservo(1, 1, 180); // papier sorti d'un coté
   delay(600);
   setservo(1, 1, 0); // papier sorti de l'autre coté
   delay(600);
   
   
   
   setservo(1, 0, 119); // moteur tournant coupé
   delay(1000);
   */


  /*

   // -------- etage 2 ---------- 
   
   
   
   
   // gars en cravate qui dit non
   setservo(2, 2, 60);
   delay(1000);
   setservo(2, 2, 180); 
   delay(1000);
   
   
   
   // animation du tapeur à l'ordi :
   setservo(2, 0, 38); // bras 1
   setservo(2, 1, 134); // bras 2
   delay(1000);
   setservo(2, 0, 110); 
   setservo(2, 1, 54);
   delay(1000);
   
   
   // -------- etage 3 ----------
   
   // animation du dessinateur :      
   setservo(3, 1, 30); //abaisse le bras
   setservo(3, 0, 50); // bouge le coude
   delay(1000);
   setservo(3, 0, 0); //extérieur de la planche à dessin
   setservo(3, 1, 27);
   delay(500);
   setservo(3, 0, 60);
   setservo(3, 1, 30);
   delay(500);
   setservo(3, 0, 0);
   setservo(3, 1, 27);
   delay(500);
   setservo(3, 0, 60);
   setservo(3, 1, 30);
   delay(1000);
   setservo(3, 0, 30); // crayon en bouche
   setservo(3, 1, 75); //lève le bras
   delay(1000);
   
   
   
   // animation du scénariste
   setservo(3, 2, 16); 
   delay(800);
   setservo(3, 2, 40); 
   delay(800);
   setservo(3, 2, 80); 
   delay(300);
   setservo(3, 2, 110); 
   delay(800);
   setservo(3, 2, 156); 
   delay(800);
   setservo(3, 2, 110); 
   delay(800);
   setservo(3, 2, 80); 
   delay(800);
   setservo(3, 2, 40); 
   delay(800);
   
   
   // animation de la photocopieuse
   setservo(3, 3, 180); // en haut
   delay(500);
   setservo(3, 3, 20); // cachée
   delay(500);
   setservo(3, 3, 180); 
   delay(500);
   setservo(3, 3, 20); 
   delay(500);
   
   
   
   // -------- etage toit (relié au 3) ----------
   // pigeons
   
   
   
   setservo(3, 4, 110); 
   delay(1000);
   setservo(3, 4, 20); 
   delay(1000);
   
   
   */


  // lecture continues du potentiometre sur A5







  /*
  setservo(1, 15, angle);  
   setservo(1, 14, angle);
   setservo(2, 15, angle);  
   setservo(2, 14, angle);
   setservo(3, 15, angle);  
   setservo(3, 14, angle);
   */
  //Serial.print("angle servo : ");
  //Serial.println(angle);



  //delay(50);





}































