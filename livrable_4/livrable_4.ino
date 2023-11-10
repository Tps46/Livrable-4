#include <string.h>

const int ledPin = 13;

int modeleCarte;
boolean VFVerif = false;

//Les leds rouges
int led1=12;
int led2=11;
int led3=10;
int led4=9;

//La led verte
int ledverte=8;

//Les boutons
int bouton1=7;
int bouton2=6;
int bouton3=5;
int bouton4=4;

void setup() {

  // Configuration de la broche comme une sortie
  pinMode(ledPin, OUTPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  pinMode(ledverte, OUTPUT);

  pinMode(bouton1,INPUT);
  pinMode(bouton2,INPUT);
  pinMode(bouton3,INPUT);
  pinMode(bouton4,INPUT);
 
  Serial.begin(9600);
  while (!Serial) {
    ; // Attendre que le port série soit prêt
  }
  Serial.println("\n");
  Serial.println("Saisi Modèle de la carte");
  while (!Serial.available()) {
    // Attendre que l'utilisateur entre une valeur
    delay(100);
   
  }

//-----------------Attribution du modèle de carte ---------------------
  modeleCarte = Serial.parseInt();
  if (modeleCarte > 0 && modeleCarte < 6) {
    if (modeleCarte == 1) {
      modele1();
    } else if (modeleCarte == 2) {
      modele2();
    } else if (modeleCarte == 3) {
      modele3();
    } else if (modeleCarte == 4) {
      modele4();
    } else {
      modele5();
    }
  } else {
    Serial.println("Erreur, le modèle saisi n'existe pas");
    fin();
  }
  Serial.println("Vous avez validé votre niveau de sécurité");
  VFVerif = true;

}

//-----------------Les différents modèles------------------------------
// Dans notre cas nous avons le modèle 1, selon le modèle les sécurités ne sont pas les mêmes
 
void modele1() {
  boolean demandeMA1 = true;
  mA1(demandeMA1);


  char x = auth();
  mA3(String(x));
}


void modele2() {
  boolean demandeMA1 = true;
  mA1(demandeMA1);
 
  char x = auth();
  mA4(String(x));
}
void modele3() {


  char x = auth();
  mA2(String(x));
  mA5(String(x));
}


void modele4() {
 
  char x = auth();
  mA2(String(x));
  mA3(String(x));
  mA4(String(x));
}


void modele5() {
  boolean demandeMA1 = true;
  mA1(demandeMA1);


  char x = auth();
  mA2(String(x));
  mA3(String(x));
  mA5(String(x));
}

//------------------------Authentification-----------------------------
char auth() {
  char x;
    Serial.begin(9600);


    Serial.println("Identifiez-vous"); //Pour les modèles 3, 4 et 5 une identification est nécessaire car nous utilisons le nom de l'agent
  while (Serial.available()==0);
  while (Serial.available()!=0){
   
    x=Serial.read();
    delay(100);}
  return x;
}

//-----------------Différentes sécurités-------------------------------

///////////////////////////////////////////////////////////////////////
//MA1 : Authentification par questions/réponses
boolean mA1(boolean demandeMA1) {
  int step = 0;
  Serial.println("Entrez la bonne combinaison de boutons");

  while (demandeMA1 == true) {

  if (digitalRead(bouton1)==1 && step==0)
  {
    digitalWrite(led1,HIGH);
    step=1;
    delay(200);
  }
  else if (step==0 && ( digitalRead(bouton2)==1 || digitalRead(bouton3)==1 || digitalRead(bouton4)==1 ))
  {
   step=0;
   Erreur();
   delay(200);
  }
  if (digitalRead(bouton2)==1 && step==1)
  {
   step=2;
   digitalWrite(led2,HIGH);
   delay(200);
  }
  else if (step==1 && (digitalRead(bouton1)==1 || digitalRead(bouton3)==1 || digitalRead(bouton4)==1))
  {
   step=0;
   Erreur();
   delay(200);
  }
  if (digitalRead(bouton3)==1 && step==2)
  {
   step=3;
   digitalWrite(led3,HIGH);
   delay(200);
  }
  else if (step==2 && (digitalRead(bouton1)==1 || digitalRead(bouton2)==1 || digitalRead(bouton4)==1))
  {
   step=0;
   Erreur();
   delay(200);
  }
  if (digitalRead(bouton4)==1 && step==3)
  {
   step=4;
   digitalWrite(led4,HIGH);
   delay(200);
  }
  else if (step==3 && (digitalRead(bouton1)==1 || digitalRead(bouton2)==1 || digitalRead(bouton3)==1))
  {
   step=0;
   Erreur();
   delay(200);
  }
  if(step==4)
  {
   digitalWrite(ledverte,HIGH);
   return true;
  }

}

}

//------------------------------
void Erreur()
{
  for(int i=0;i<3;i++)//boucle pour i, sert à faire clignoter les LEDs
 {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  delay(200);


  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  delay(200);
 }
 fin();
}

///////////////////////////////////////////////////////////////////////
//MA2 : Authentification par un code qui change au cours du temps
boolean mA2(String x) {
  boolean mA2Verif = false;
  int e, m, mP;
  long c;
  m = random(2881);
  e = cleAgent(x);
  c = chiffrer(m, e);

  Serial.println("Message à déchiffrer :");
  Serial.println(c);

  while (!Serial.available()) {
    // Attendre que l'utilisateur entre une valeur
    delay(100);
  }
  mP = Serial.parseInt();
 
  if (m == mP) {
    Serial.println("Mécanisme MA2 Vérifié");
    mA2Verif = true;
  } else {
    Serial.println("Erreur, le mécanisme MA2 a échoué");
    Serial.println(c);
    fin();
  }

  return mA2Verif;
}

//------------------------------
//attribue la clé publique de l'utilisateur concerné
int cleAgent(String x) {
  int Ce = 0;
  int i;
  String lettre[16] = {"A", "B", "C", "D", "E", "F", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q"};
  int valeur[16] = {601, 619, 631, 641, 647, 653, 661, 673, 691, 701, 733, 739, 751, 797, 809, 811};


  for (i = 0; i < 16; i++) {
    if (x == lettre[i]) {
      Ce = valeur[i];
    }
  }
  if (Ce == 0) {
    Serial.println("Erreur, agent non reconnu");
    fin();
  }
  return Ce;
}

//------------------------------
int chiffrer(int m, int e) {
  int n;
  long c2;
  n = 43 * 67; // Utilisation d'une valeur n arbitraire


  c2 = modexp(m, e, n);
 
  return c2;
}

//------------------------------
//Renvoie l'exponentiation modulaire, a^e mod n
int modexp(int a, int e, int n) {
    long r;


    if (e < 0) {
        Serial.println("unhandled case");
        exit(-1);
    }
    if (a == 0 || n == 1) {
        r = 0;
    } else {
        r = 1;
        while (e > 0) {
            r = r * (a % n) % n;
            e = e - 1;
        }
    }


    return r;
}

///////////////////////////////////////////////////////////////////////
//MA3: Scan rétinien (pour notre prototype, il sera simulé par un message)
boolean mA3(String x) {
  int mA3Saisie;


  Serial.println("Présentez votre oeil devant le capteur");
  while (!Serial.available()) {
    // Attendre que l'utilisateur entre une valeur
    delay(100);
  }
  mA3Saisie = Serial.parseInt();


  int localisationLettre = 0;
  int i;
  boolean mA3Verif = false;
  int listeID[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}; //Ici les valeurs sont volontairement simples nous pouvons le changer facilement dans le modèle réel
  String lettre[16] = {"A", "B", "C", "D", "E", "F", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q"};


  for (i = 0; i < 16; i++) {
    if (x == lettre[i]) {
      localisationLettre = i;
    }
  }
  if (listeID[localisationLettre] == mA3Saisie) {
    Serial.println("Mécanisme MA3 Vérifié");
    mA3Verif = true;
  } else {
    Serial.println("Erreur, l'oeil de l'utilisateur ne correspond pas");
    fin();
  }


  return mA3Verif;
}

///////////////////////////////////////////////////////////////////////
//MA4 : Scan digital (pour notre prototype, il sera simulé par un message)
boolean mA4(String x) {
  int mA4Saisie;


  Serial.println("Entrez votre empreinte digitale");
  while (!Serial.available()) {
    // Attendre que l'utilisateur entre une valeur
    delay(100);
  }
  mA4Saisie = Serial.parseInt();


  int localisationLettre = 0;
  int i;
  boolean mA4Verif = false;
  int listeID[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}; //Ici les valeurs sont volontairement simples nous pouvons le changer facilement dans le modèle réel
  String lettre[16] = {"A", "B", "C", "D", "E", "F", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q"};


  for (i = 0; i < 16; i++) {
    if (x == lettre[i]) {
      localisationLettre = i;
    }
  }
  if (listeID[localisationLettre] == mA4Saisie) {
    Serial.println("Mécanisme MA4 Vérifié");
    mA4Verif = true;
  } else {
    Serial.println("Erreur, l'empreinte renseigné ne correspond pas");
    fin();
  }


  return mA4Verif;
}

///////////////////////////////////////////////////////////////////////
//MMA5 : CARD_ID. L'utilisateur a déjà renseigné sa lettre d'agent, il ne manque plus que son CARD_ID
boolean mA5(String x) {
  int mA5Saisie;


  Serial.println("Entrez l'identifiant CARTE-ID");
  while (!Serial.available()) {
    // Attendre que l'utilisateur entre une valeur
    delay(100);
  }
  mA5Saisie = Serial.parseInt();


  int localisationLettre = 0;
  int i;
  boolean mA5Verif = false;
  int listeID[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}; //Ici les valeurs sont volontairement simples nous pouvons le changer facilement dans le modèle réel
  String lettre[16] = {"A", "B", "C", "D", "E", "F", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q"};


  for (i = 0; i < 16; i++) {
    if (x == lettre[i]) {
      localisationLettre = i;
    }
  }
  if (listeID[localisationLettre] == mA5Saisie) {
    Serial.println("Mécanisme MA5 Vérifié");
    mA5Verif = true;
  } else {
    Serial.println("Erreur, l'ID renseigné n'est pas correct");
    fin();
  }


  return mA5Verif;
}

//-----------------Mesure de la tension--------------------------------
void loop() {
  if (VFVerif == true) {
    // Lit la tension d'entrée sur la broche A0
    int valeurAnalogique = analogRead(A0);


    // Convertit la valeur brute en tension en millivolts
    float tensionMillivolts = (valeurAnalogique / 1023.0) * 5000.0; // 5000 mV pour une alimentation de 5V


    // Affiche la tension en millivolts sur le moniteur série
    Serial.print("Tension en millivolts lue sur A0 : ");
    Serial.print(tensionMillivolts);
    Serial.println(" mV");


    if (tensionMillivolts < 900 && tensionMillivolts > 850){ //Si la tension est comprise entre 850 et 900 mV cela correspond à la bonne combinaison de levier
      digitalWrite(ledPin, HIGH);                            //Nous pouvons alors allumer la Led d'ouverture du coffre
        Serial.println("Ouverture du coffre");
        fin();
       


    }
    else {
      digitalWrite(ledPin, LOW);
      Serial.println("Erreur, la combinaison de levier n'est pas la bonne");
      fin();
  }

}

}

//-----------------Fonction fin----------------------------------------
// Elle sert à stopper le système de tel façon qu'il devient impossible de continuer à moins de relancer le programme


void fin() {
  int valeur_impossible_a_atteindre = 1;
  Serial.println("Fin du programme.");
  while (valeur_impossible_a_atteindre == 1) {
  }
}

