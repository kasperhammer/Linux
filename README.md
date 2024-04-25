
# StartUpService.c Dokumentation
## Formål
Formålet med StartUpService.c-filen er at implementere et program, der starter op ved systemets opstart og kører i baggrunden som en daemon. Programmet opsætter og styrer tråde til at indsamle og opdatere tid og temperaturinformation, som derefter vises på en skærm. Derudover publiceres temperaturinformationen til en MQTT-broker.

## Funktioner
1. int BitPrint(int bits)
Denne funktion udskriver en binær repræsentation af et heltal til konsollen. Det bruges til fejlfinding eller diagnosticering.

2. void InterProlateAndPrint()
Denne funktion kombinerer temperatur- og tidsinformation og viser den kombinerede streng på en skærm. Det opdaterer løbende skærmen med de seneste oplysninger.

3. void *getTime(void *arg)
Denne trådfunktion kontinuerligt indhenter den aktuelle tid og opdaterer en global variabel med tidsoplysningerne. Derefter kaldes InterProlateAndPrint() for at opdatere skærmen.

4. void *getTemp(void *arg)
Denne trådfunktion kontinuerligt indhenter den aktuelle temperatur og opdaterer en global variabel med temperaturoplysningerne. Derefter kaldes InterProlateAndPrint() for at opdatere skærmen.

5. void *publishTemp(void *arg)
Denne trådfunktion kontinuerligt offentliggør temperaturoplysningerne til en MQTT-broker.

6. int main(void)
Denne funktion er programmets indgangspunkt. Den initialiserer systemet, opretter trådene til tid, temperatur og MQTT-publicering, og afslutter derefter programmet.

## Implementering af Daemon
For at køre StartUpService.c som en daemon ved systemets opstart, kan du følge disse trin:

Kompilér koden: Kompilér din StartUpService.c-fil til en eksekverbar fil. For eksempel kan du bruge GCC-kommandoen:

```gcc StartUpService.c -o StartUpService -lpthread
Kopier eksekverbar fil til bin-mappe: Kopier den kompilerede eksekverbare fil (StartUpService) til /usr/local/bin eller et andet passende sted.

sudo cp StartUpService /usr/local/bin
Opret en systemd-servicefil: Opret en systemd-servicefil (f.eks. startup.service) i /etc/systemd/system/-mappen med følgende indhold:
plaintext

[Unit]
Description=Startup Service
After=network.target

[Service]
Type=simple
ExecStart=/usr/local/bin/StartUpService

[Install]
WantedBy=multi-user.target
Genindlæs systemd-dæmoner: Genindlæs systemd-dæmonerne for at opdatere servicekonfigurationerne.

sudo systemctl daemon-reload
Aktivér og start servicen: Aktivér og start den nye service.

sudo systemctl enable startup.service
sudo systemctl start startup.service ```

Nu vil StartUpService køre som en daemon ved systemets opstart og fortsætte med at køre i baggrunden, indsamle tid og temperaturinformation, opdatere skærmen og offentliggøre temperaturinformation til MQTT-brokeren.

## Bemærkninger
Sørg for at have de nødvendige tilladelser til at køre StartUpService som en daemon, og at alle afhængigheder er opfyldt.
Du kan justere servicekonfigurationen efter behov, f.eks. for at angive en anden eksekverbar sti eller tilføje yderligere startparametre.

# IpGet.c Dokumentation
## Formål
Formålet med IpGet.c-filen er at indeholde funktioner til at hente den lokale IP-adresse fra systemet. Dette er nyttigt, når du skal oprette forbindelse til en lokal netværksenhed eller når du har brug for at vide den lokale IP-adresse til andre formål.

## Funktioner
1. char *GetLocalIp()
Denne funktion returnerer en streng, der repræsenterer den lokale IP-adresse for systemet. Funktionen opretter en socket, henter IP-adressen fra den ønskede netværksgrænseflade og formatterer derefter IP-adressen som en streng.

## Implementering
For at bruge IpGet.c i dit projekt skal du følge disse trin:

Inkludér headerfilen: Tilføj følgende linje øverst i din kildefil for at inkludere IpGet.h:
```
#include "IpGet.h"
Kald funktionen: Brug GetLocalIp()-funktionen i din kode, hvor du har brug for at hente den lokale IP-adresse. Eksempel:

char *ipAddress = GetLocalIp();
printf("Local IP Address: %s\n", ipAddress);
free(ipAddress); // Husk at frigøre hukommelsen efter brug```

Kompilér kildefilerne: Når du kompilerer dit projekt, skal du sikre dig, at IpGet.c er inkluderet i kommandoen.
## Bemærkninger
Funktionen GetLocalIp() kan returnere NULL, hvis der opstår en fejl under hentning af IP-adressen. Det anbefales at kontrollere for NULL-værdien, når du bruger funktionen for at undgå uønskede fejl.
Sørg for at frigøre den allokerede hukommelse ved hjælp af free() efter brug af den returnerede streng for at undgå hukommelseslækager.

# mqtt.c Dokumentation
## Formål
Formålet med mqtt.c-filen er at indeholde funktioner til at oprette forbindelse til en MQTT-broker, sende og modtage MQTT-beskeder og håndtere MQTT-forbindelseshændelser.

## Funktioner
1. void publish_message(struct mosquitto *mosq, const char *payload)
Denne funktion bruges til at offentliggøre en besked til en MQTT-broker. Den tager en pointer til en mosquitto-struktur og en streng som input og offentliggør derefter beskeden til det angivne emne.

2. void on_connect1(struct mosquitto *mosq, void *obj, int result)
Denne funktion fungerer som en tilbagekaldsfunktion, der bliver kaldt, når der oprettes forbindelse til en MQTT-broker. Det kan anvendes til at foretage yderligere handlinger efter en succesfuld forbindelse.

3. void on_message1(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
Denne funktion fungerer som en tilbagekaldsfunktion, der bliver kaldt, når en ny besked modtages fra en MQTT-broker. Den modtager en pointer til en mosquitto_message struktur, der indeholder information om den modtagne besked, og kan anvendes til at behandle beskeden.

4. struct mosquitto* GetMqtt()
Denne funktion initialiserer en ny mosquitto-struktur og returnerer en pointer til den initialiserede struktur. Det er nødvendigt at kalde denne funktion, før du bruger andre MQTT-relaterede funktioner.

5. int DestroyMQTT(struct mosquitto *mosq1)
Denne funktion frigiver ressourcer, der er allokeret til en mosquitto-struktur. Det er vigtigt at kalde denne funktion, når MQTT-objektet ikke længere er i brug for at undgå hukommelseslækager.

6. int InitMqtt(struct mosquitto *mosq1)
Denne funktion initialiserer MQTT-forbindelsen ved at oprette forbindelse til en MQTT-broker med den angivne IP-adresse og portnummer.

## Implementering
For at bruge mqtt.c i dit projekt skal du følge disse trin:

Inkludér headerfilen: Tilføj følgende linje øverst i din kildefil for at inkludere mqtt.h:

```
#include "mqtt.h"
Initialiser MQTT: Brug GetMqtt()-funktionen til at initialisere en MQTT-struktur og InitMqtt()-funktionen til at oprette forbindelse til en MQTT-broker.


struct mosquitto *mosq1 = GetMqtt();
InitMqtt(mosq1);
Brug MQTT-funktioner: Brug de forskellige MQTT-funktioner som f.eks. publish_message() til at interagere med MQTT-brokeren.
Afslut MQTT-forbindelse: Når du er færdig med at bruge MQTT, skal du frigøre ressourcerne ved at kalde DestroyMQTT().


DestroyMQTT(mosq1);```

## Bemærkninger
Før du bruger MQTT-funktionerne, skal du sørge for at have inkluderet mosquitto.h og have konfigureret dit projekt til at linke med Mosquitto-biblioteket.
Det anbefales at håndtere eventuelle fejl, der kan opstå under MQTT-operationerne, for at sikre en robust applikation.

# screen.c Dokumentation
## Formål
Formålet med screen.c-filen er at implementere funktioner til styring af en LCD-skærm via I2C-protokollen. Denne fil indeholder funktioner til at skrive tekst på skærmen og indstille baggrundsfarven på skærmbelysningen.

## Funktioner
1. void i2c_write_byte(int file, unsigned char reg, unsigned char value)
Denne funktion skriver en byte til en bestemt registreringsadresse på en I2C-enhed. Det bruges til at sende kommandoer og data til LCD-skærmen.

2. void write_string_to_lcd(int file, const char *str)
Denne funktion skriver en streng til LCD-skærmen. Hvis strengen er længere end 16 tegn, skifter den til næste linje automatisk.

3. void SetScreen(char *inputString)
Denne funktion initialiserer LCD-skærmen og viser en given streng på skærmen. Det bruges til at starte skærmen og vise en initial besked.

4. void SetColor(int color)
Denne funktion indstiller farven på LCD-skærmens baggrundsbelysning baseret på den modtagne farveparameter. Farvevalg inkluderer blå, grøn, rød eller ingen farve.

5. void InitScreen()
Denne funktion initialiserer LCD-skærmens indstillinger, så den er klar til brug. Det opsætter skærmens grundlæggende konfigurationer som to-linje-tilstand, blinkkontrol osv.

6. void WriteTopLine(char *str)
Denne funktion skriver en streng til LCD-skærmens øverste linje.

7. void WriteBottomLine(char *str)
Denne funktion skriver en streng til LCD-skærmens nederste linje.

## Implementering
For at bruge funktionerne i screen.c-filen skal du følge disse trin:

Inkludér headerfilen: I din hovedfil eller det relevante program, inkluder screen.h-headerfilen.
 ```
#include "screen.h"
Initialisér skærmen: Kald InitScreen()-funktionen for at initialisere LCD-skærmen.

InitScreen();
Skriver tekst på skærmen: Brug write_string_to_lcd()-funktionen til at skrive tekst på skærmen.

write_string_to_lcd(file, "Hello, World!");
Indstil baggrundsfarve: Brug SetColor()-funktionen til at indstille baggrundsfarven på skærmbelysningen.

SetColor(0); // Blå baggrund
Vis på skærmen: Brug WriteTopLine() og WriteBottomLine() til at skrive tekst på henholdsvis øverste og nederste linje af skærmen.

WriteTopLine("Temperature:");
WriteBottomLine("25°C"); ```

## Bemærkninger
Sørg for at have de nødvendige tilladelser til at bruge I2C-bussen, og at skærmen er tilsluttet korrekt til din enhed.
Funktionerne i screen.c-filen kan tilpasses efter behov, f.eks. for at ændre skærmens konfigurationer eller tilføje yderligere visningsmuligheder.

# temp.c Dokumentation
Formål
Formålet med temp.c-filen er at implementere funktioner til at interagere med en MCP9808 digital temperatursensor via I2C-protokollen. Denne fil indeholder funktioner til initialisering af sensoren og aflæsning af den aktuelle temperatur.

## Funktioner
1. int i2c_init(char *bus, unsigned int address)
Denne funktion initialiserer kommunikationen med en I2C-enhed ved at åbne den tilsvarende I2C-bus og sætte enhedens adresse. Den returnerer en filbeskrivelse, som bruges til at kommunikere med enheden.

2. int init()
Denne funktion initialiserer MCP9808-temperaturens sensorenhed og returnerer den aktuelle temperatur som en flydende punktværdi i Celsius.

3. char* GetTemp()
Denne funktion returnerer den aktuelle temperatur som en streng.

## Implementering
For at bruge funktionerne i temp.c-filen skal du følge disse trin:

Inkludér nødvendige headerfiler: I din hovedfil eller det relevante program, inkluder temp.h-headerfilen.
```
#include "temp.h"
Initialisér sensor: Kald init()-funktionen for at initialisere MCP9808-sensoren og få den aktuelle temperatur.

double temperature = init();
Aflæs temperatur: Brug GetTemp()-funktionen til at få den aktuelle temperatur som en streng.

char *temperatureString = GetTemp();```

## Bemærkninger
Sørg for at have de nødvendige tilladelser til at bruge I2C-bussen, og at sensoren er tilsluttet korrekt til din enhed.
Funktionerne i temp.c-filen kan tilpasses efter behov, f.eks. for at ændre sensorens konfigurationer eller tilføje yderligere funktionalitet.

# TimeGet.c Dokumentation
## Formål
Formålet med TimeGet.c-filen er at implementere funktioner til at hente den aktuelle tid i timer og minutterformat. Dette er nyttigt i situationer, hvor applikationen har brug for at arbejde med tidsstempeldata.

## Funktioner
1. int getHour()
Denne funktion returnerer den aktuelle time som en heltalsværdi.

2. int getMin()
Denne funktion returnerer den aktuelle minut som en heltalsværdi.

3. char* GetTime(int collon)
Denne funktion returnerer den aktuelle tid i formatet "HH:MM" eller "HH MM", afhængigt af værdien af parameteren collon. Den returnerede streng indeholder tidspunktet som en tegnstreng.

## Implementering
For at bruge funktionerne i TimeGet.c-filen skal du følge disse trin:

Inkludér nødvendige headerfiler: I din hovedfil eller det relevante program, inkluder TimeGet.h-headerfilen.
```
#include "TimeGet.h"
Hent time og minut: Brug getHour() og getMin()-funktionerne til at hente den aktuelle time og minut.

int currentHour = getHour();
int currentMin = getMin();
Hent tid som streng: Brug GetTime()-funktionen til at få den aktuelle tid som en streng.

char *timeString = GetTime(1); // Returnerer tid i formatet "HH MM" ```

## Bemærkninger
Funktionerne i TimeGet.c-filen returnerer tidspunkter baseret på den lokale tidszone.
GetTime()-funktionen giver mulighed for at vælge mellem to formater for tidspunktet ved at angive værdien af collon-parameteren (0 for "HH:MM" og 1 for "HH MM").
