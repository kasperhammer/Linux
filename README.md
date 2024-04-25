
StartUpService.c Dokumentation
Formål
Formålet med StartUpService.c-filen er at implementere et program, der starter op ved systemets opstart og kører i baggrunden som en daemon. Programmet opsætter og styrer tråde til at indsamle og opdatere tid og temperaturinformation, som derefter vises på en skærm. Derudover publiceres temperaturinformationen til en MQTT-broker.

Funktioner
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

Implementering af Daemon
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

Bemærkninger
Sørg for at have de nødvendige tilladelser til at køre StartUpService som en daemon, og at alle afhængigheder er opfyldt.
Du kan justere servicekonfigurationen efter behov, f.eks. for at angive en anden eksekverbar sti eller tilføje yderligere startparametre.
