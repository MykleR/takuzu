# Takuzu 02i
Ce projet est une implémentation du jeu Takuzu en C.
le jeu consiste à remplir avec des cases blanches ou noires une grille de taille NxM en respectant certaines règles de placement.


## Prérequis :
 - Cmake
 - Make
 - Un compilateur C

## Compilation :
- téléchargez/clonez le répertoire et placez vous dans ce dernier
- puis effectuez les commandes suivantes:
```
mkdir build; cd build
cmake ..
make
```

## Exécution :
 - placez vous dans le répertoire de compilation 
 - lancez l'executable "game_text" avec `./game_text`
La sélection des options du jeu n'est pas encore disponible mais le sera dans une version future.

## Règles du jeu :
La règle de ce jeu est simple !
En effet, il suffit de remplir la grille avec des W et des B...
Les contraintes sont les suivantes :
Il ne peut y avoir plus de deux W ou de deux B consécutifs.
Il y a le même nombre de W et de B sur chaque ligne et chaque colonne.
Si vous trouvez la solution, un message vous indiquera "Congratulations"

## Contribution


## Autheurs
 - Michael ROUVES
 - Hugo BERNARD
 - Corentin TINNES

