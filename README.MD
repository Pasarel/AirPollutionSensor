# Smart Pollution Sensor Newtork
Rețea de senzori de poluare care permite controlarea ferestrei de acasă și verificarea în timp real a stării aerului din jurul unui anumit punct.

## Dependințe
Cerințele de sistem și bibliotecile de care e nevoie pentru rularea și compilarea acestui program sunt:
```
Ubuntu 20.04 LTS
libboost v1.71
libpistache v0.0.002
libpoco v1.9.2
```

Toate dependințele se instalează automat, rulând scriptul ./install_dependencies:

```
chmod +x install_dependencies.sh
sudo ./install_dependencies
```

## Compilare și rulare
Programul are două componente: serverul propriu-zis și un crawler care preia date live de la senzori de pe internet. Ambele componente se pot compila rulând:

```
make all
```

Alternativ, se poate rula:

```
make server // compilează doar serverul
make crawler // compilează doar crawlerul
```

Pentru funcționarea crawlerului, e necesară pornirea serverului:

```
./server
./crawler
```

Serverul se poate porni și fără crawler:

```
./server
```

Dar atunci este necesară popularea din alta sursă a fișierului data.json, prin requesturi trimise la ruta webhookului. (Vezi mai jos.)

## Ce face programul?
![Sample image](sample.png?raw=true "Exemplu de output PNG")

Ascultă după notificări primite de la senzori de aer din teren și permite utilizatorului să controleze starea ferestrei lui de acasă (deschis/închis). Utilizatorul poate verifica în timp real nivelul de poluare al senzorilor de lângă casa lui, răspunsul primindu-l fie text, fie în format .PNG (codificat base64). Există și funcționalitate minimală de închidere automată a ferestrei și notificare prin email.

Cum niciunul din membrii acestei echipe nu dispune de vreo rețea personală de senzori de aer, notificările de la senzori sunt simulate de un crawler care agregă date de la waqi.info și aerlive.ro.

Exemple de requesturi (întreaga specificație se găsește în docs/IO specs.json):

```
curl http://localhost:9080/open_window/:id
curl http://localhost:9080/change_window_coords/:id/:coords
curl http://localhost:9080/get_aq/:coords // primește date în format text despre calitatea aerului la coordonatele date
curl http://localhost:9080/img/get/:id    // primește o imagine .png (base64) cu starea aerului în jurul geamului dat
curl -X POST  http://localhost:9080/action1/controlul_ferestrei/automat // schimbă modul ferestrei din manual în automat
curl -X POST http://localhost:9080/webhook --data [json] // trimite manual (fără crawler) date despre aer către webhook
```

## Au interpretat actorii
Andrei-Alexandru Oltean, Bogdan Ioan Pintilie, Ioan Christian Tana, Sorin Alexandru Dobre, Mihai Alexandru Gheorghe.