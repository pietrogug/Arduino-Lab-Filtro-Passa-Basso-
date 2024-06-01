
// RICORDARSI DI MODIFICARE I VALORI SOTTOSTANTI IN BASE A COME SI È COLLEGATA LA SCHEDA ARDUINO AL CIRCUITO SULLA BREADBORD, 

// QUELLI PREIMPOSTATI VANNO BENE SE IL CIRCUITO È STATO COLLEGATO ALLA STESSA MANIERA MOSTRATA SU TINKERCAD DURANTE IL VIDEO

#define POTENZIOMETRO A0 // Pin collegato al potenziometro

#define OUT_ONDE_QUADRE 3 // Pin collegato al generatore di segnale periodico

#define IN A2 // Pin misura segnale d'ingresso

#define OUT A1 // Pin misura segnale d'uscita

#define FREQUENZA_MAX 500  // frequenza massima di 500 Hz

#define FREQUENZA_MIN 20 // frequenza minima di 20 Hz

#define ITERAZIONI 50 // max 88



unsigned long time;
unsigned long delta_time;
char buff[1500] = {0}; // buffer usato per inviare i dati a python
char temp_str[100] = {0}; // stringa che salva le misure singole da aggiungere al buffer
int i = 0;
int frequenza;

void setup() {

  pinMode(OUT_ONDE_QUADRE, OUTPUT); // impostiamo il pin 3 come uscita
  time = micros();

  Serial.begin(38400); // inizializzazione della comunicazione seriale con python
  Serial.setTimeout(1);

}

void loop() { // Ciclo calcolo e acquisizione dati

  frequenza = FREQUENZA_MAX * (float) analogRead(POTENZIOMETRO)/1023.0 + FREQUENZA_MIN; // Frequenza in base al potenziometro
  
  tone(OUT_ONDE_QUADRE, frequenza); // Uscita onda quadra alla frequenza impostata

  delta_time = micros() - time; // misura il tempo trascorso dall'ultima misura

  sprintf(temp_str, "%lu,%d,%d;", delta_time, analogRead(IN), analogRead(OUT)); // Acquisizione dei dati

  strcat(buff, temp_str); // Aggiunta dati alla lista di tutti i punti

  if(i > ITERAZIONI){ // Dopo aver acquisito il numero scelto di dati, arduino li invia a python e reimposta le variabili alle condizioni iniziale per il nuovo ciclo

    Serial.println(buff); 

    i = 0;
    time = micros();
    memset(buff, 0, 1024);
    delay(20);

  }

  // Quando si fa il run dal codice python dei dati aquisiti da questo codice può capitare che le prime volte dia un errore a causa di dati "spazzatura",
  // rifare il run da python un paio di volte e dovrebbe partire, questa cosa è mostrata anche nel video
  
  i++;
}