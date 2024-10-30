#ifndef MODE_H
#define MODE_H
 
//fonction qui permet de chiffrer les nbloc blocs de txt en mode ECB
void modeECBc(unsigned char **txt , unsigned char *key , int Nr , int Nk , int nbloc);

//fonction qui permet de déchiffrer les nbloc blocs de txt en mode ECB
void modeECBd(unsigned char **txt , unsigned char *key , int Nr , int Nk , int nbloc);

//fonction qui permet de chiffrer les nbloc blocs de txt en mode CBC avec IV en tant que vecteur d'initialisation
void modeCBCc(unsigned char **txt , unsigned char *key , unsigned char *IV, int Nr , int Nk , int nbloc);

//fonction qui permet de déchiffrer les nbloc blocs de txt en mode CBC avec IV en tant que vecteur d'initialisation
void modeCBCd(unsigned char **txt , unsigned char *key , unsigned char *IV, int Nr , int Nk , int nbloc);

#endif