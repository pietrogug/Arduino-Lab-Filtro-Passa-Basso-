// codice per plot del diagramma di Bode

// RICORDARSI DI MODIFICARE I VALORI SOTTOSTANTI IN BASE A COME SI È COLLEGATA LA SCHEDA ARDUINO AL CIRCUITO SULLA BREADBORD, 

// QUELLI PREIMPOSTATI VANNO BENE SE IL CIRCUITO È STATO COLLEGATO ALLA STESSA MANIERA MOSTRATA SU TINKERCAD DURANTE IL VIDEO


#define POTENZIOMETRO A0 // Pin collegato al potenziometro

#define OUT_ONDE_QUADRE 3 // Pin collegato al generatore di onde quadre

#define IN A2 // Pin misura segnale di ingresso

#define OUT A1 // Pin misura segnale di uscita

#define FREQUENZA_MAX 10000  // Frequenza massima di 10000 Hz

#define FREQUENZA_MIN 2 // Frequenza minima di 2 Hz


char buff[1500] = {0}; // buffer usato per inviare i dati a python
char temp_str[100] = {0}; // stringa che salva le misure singole da aggiungere al buffer
int Vmin, Vmax;
int ampiezza;
int i = 0, j = 0;

void setup() {
  pinMode(OUT_ONDE_QUADRE, OUTPUT); // impostiamo il pin 3 come uscita

  Serial.begin(38400); // inizializzazione della comunicazione seriale con python
  Serial.setTimeout(1);
}

void loop() {

  for (i = 0; i <= 88; i++){ // Non aumentare il valore di punti oltre 88 la memoria della scheda Arduino non lo permette

    int frequenza = FREQUENZA_MAX * (float) i/100.0 + FREQUENZA_MIN;

    tone(OUT_ONDE_QUADRE, frequenza); // Uscita onda quadra alla frequenza impostata

    Vmin = analogRead(OUT);

    Vmax = Vmin;

    for(j = 0; j < 1000; j++){ // Ciclo per trovare il massimo e il minimo valore del segnale di uscita

      int temp = analogRead(OUT);
      if (temp > Vmax){
        Vmax = temp;
      }      
      if (temp < Vmin){
        Vmin = temp;
      }
    }
    
    ampiezza = (int) ((float) (Vmax - Vmin) / 1023.0 * 1000.0); // Calcolo ampiezza pico-pico del segnale di uscita

    sprintf(temp_str, "%d,%d,%d;", frequenza, 1023, ampiezza); // Aquisizione dati, il valore di ingrsso è fissato a 1023 (5V) come riferimento

    strcat(buff, temp_str); // Aggiunta dati alla lista di tutti i punti
  
  }

  Serial.println(buff);
  i = 0;
  j = 0;
  memset(buff, 0, 1024);
  delay(20);

// Quando si effettua il run dal codice python dei dati inviati da questo codice è normale dover aspettare qualche secondo per visualizzare i dati sul grafico  

}
