#ifndef CIPHER_H
#define CIPHER_H

//fonction qui renvoie un char qui correspond à la transformation par la S-box du char entré
unsigned char getsbox(unsigned char c);

//fonction qui renvoie un char qui correspond à la transformation par l'inverse de la S-box du char entré
unsigned char getinv_sbox(unsigned char c);

//fonction qui prends un mot (4 char) et le modifie en appliquant la S-box sur chaque char
void subWord(unsigned char* word);

//fontion applique une permutation sur les élément du mot entré (i -> (i+1)%4 )
void RotWord(unsigned char* word);

//fonction qui calcul la clé étendu pour un clé entrée 
void expandKey(unsigned char *key , unsigned char *expandedKey , int Nk , int size_ek);

//fonction qui applique la transormation SubByte à chaque char de l'état (un état = 16 char)
void subBytes(unsigned char* state);

//fonction qui applique la transformation inverse de SubByte à chaque char de l'état
void invSubBytes(unsigned char* state);

//fonction qui applique la transformtion shiftRows à l'état entré
void shiftRows(unsigned char* state);

//fonction qui applique la transformtion inverse de shiftRows à l'état entré
void invShiftRows(unsigned char *state);

//fonction qui permet de faire une multiplication dans le corps F2^8
unsigned char galois_mult(unsigned char a , unsigned char b);

//fonction qui applique la transformation mixColums à l'état entré
void mixColumns(unsigned char *state);

//fonction qui applique la transformation inverse de mixColums à l'état entré
void invMixColumns(unsigned char *state);

//fonction qui applique la transformation addRoundKey à l'état entré
void addRoundKey(unsigned char *state ,unsigned char *roundKey);

//fonction qui permet d'a partir de la clé étendu de d'avoir la clé de ronde choisie stocké dans roundKey
void getRoundKey(unsigned char *expandedKey , unsigned char *roundKey ,int nbrRound);

//fonction qui chiffre un état (bloc de 128 bit) en appliquant les différentes opérations réalisées en AES
void cipher(unsigned char *state , unsigned char *expandedKey,int Nr);

//fonction qui déchiffre un état (bloc de 128 bit) en appliquant les différentes opérations réalisées en AES
void invCipher(unsigned char *state , unsigned char *expandedKey,int Nr);

#endif