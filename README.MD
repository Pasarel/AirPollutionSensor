# Smart Pollution Sensor Newtork
Acesta este proiectul nostru la IP. Sperăm să obținem măcar nota 5!

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


## Rute
  =====action1========
 =====action1======== 
 =====action1========
 curl -X POST  http://localhost:9080/action1/controlul_ferestrei/manual
curl -X POST  http://localhost:9080/action1/controlul_ferestrei/automat
curl  http://localhost:9080/action1/controlul_ferestrei/

  =====action========
 =====action======== 
 =====action========

 curl -X POST  http://localhost:9080/action/e_mail/add_mail
 curl  http://localhost:9080/action/e_mail


  =====action2========
 =====action2======== 
 =====action2========

curl -X POST  http://localhost:9080/action2/e_mail/MAIL@yubi.com

curl  http://localhost:9080/action2/e_mail/

## Au interpretat actorii
Andrei-Alexandru Oltean, Bogdan Ioan Pintilie, Ioan Christian Tana, Sorin Alexandru Dobre, Mihai Alexandru Gheorghe.