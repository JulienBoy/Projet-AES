#include <cipher.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 16

int Nk=4;
int Nb=4;
unsigned char sbox[256] = {
    // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,  // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,  // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,  // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,  // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,  // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,  // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,  // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,  // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,  // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,  // A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,  // B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,  // C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,  // D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,  // E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}; // F

unsigned char inv_sbox[256] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D};

unsigned char Rcon[255] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d,
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab,
    0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d,
    0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25,
    0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01,
    0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d,
    0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa,
    0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a,
    0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02,
    0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f,
    0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5,
    0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33,
    0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb};




unsigned char getsbox(unsigned char c){
    return sbox[c];
}

unsigned char getinv_sbox(unsigned char c){
    return inv_sbox[c];
}



void subWord(unsigned char* word){
    for( int i=0 ; i<4 ; i++){
        word[i]=getsbox(word[i]);
    }
}

void RotWord(unsigned char* word){
    unsigned char tmp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = tmp;
}


void expandKey(unsigned char* key , unsigned char* expandedKey , int Nk , int size_ek){
    int i;
    int rcon_it=1;       //indice qui permet de savoir quelle valeur de Rcon on doit avoir
    int csize_ek=0;      //taille en cours de expandedKey
    unsigned char temp[4];  //word temporaire
    for(i=0 ; i<4*Nk ; i++){
        expandedKey[i]=key[i];    //le premier word de la clé étendu est la clé elle-même
        csize_ek++;
    }
    while(csize_ek < size_ek){      
        for(i=0 ; i<4 ; i++){
            temp[i]=expandedKey[csize_ek-4+i];       // on donne à temp la valeur du word d'avant
        }
        if(csize_ek%(4*Nk)==0){
            RotWord(temp);                //on applque les transformations à temp
            subWord(temp);
            temp[0] = temp[0] ^ Rcon[rcon_it] ;    
            rcon_it++;
        }else if(Nk > 6  && csize_ek%(4*Nk) == 16){
            subWord(temp);
        }
        for(i=0 ; i<4 ; i++){                                           //et on xor le résultat de temp après les transformations 
            expandedKey[csize_ek]= expandedKey[csize_ek-4*Nk] ^ temp[i];// avec le word précédent pour avoir le nouveau word
            csize_ek++;
        }
    }
    
}

void subBytes(unsigned char* state){
    for(int i=0 ; i<16 ; i++){
        state[i]=getsbox(state[i]);    //on applique à chaque cellule de l'état la transformation de la S-box
    }
}

void invSubBytes(unsigned char* state){
    for(int i=0 ; i<16 ; i++){
        state[i]=getinv_sbox(state[i]);  //on applique à chaque cellule de l'état la transformation inverse de la S-box
    }
}

void shiftRows(unsigned char* state){
    unsigned char tmp = state[1];
                                        //les celleules d'indices 0,4,8,12 de l'état sont inchangées (1ere ligne de l'état)
    state[1]=state[5];
    state[5]=state[9];                  //les celleules d'indices 1,5,9,13 de l'état sont décalées de 1 vers la gauche (2e ligne de l'état)
    state[9]=state[13];
    state[13]=tmp;

    tmp=state[2];                       //les celleules d'indices 2,6,10,14 de l'état sont décalées de 2 vers la gauche (3e ligne de l'état)
    state[2]=state[10];
    state[10]=tmp;
    tmp=state[6];
    state[6]=state[14];
    state[14]=tmp;

    tmp=state[3];                       //les celleules d'indices 3,7,11,15 de l'état sont décalées de 3 vers la gauche (4e ligne de l'état)
    state[3]=state[15];
    state[15]=state[11];
    state[11]=state[7];
    state[7]=tmp;
}

void invShiftRows(unsigned char *state){
    unsigned char tmp = state[1];
                                        //les celleules d'indices 0,4,8,12 de l'état sont inchangées (1ere ligne de l'état)
    state[1]=state[13];
    state[13]=state[9];                 //les celleules d'indices 1,5,9,13 de l'état sont décalées de 1 vers la droite (2e ligne de l'état)
    state[9]=state[5];
    state[5]=tmp;

    tmp=state[2];
    state[2]=state[10];                 //les celleules d'indices 2,6,10,14 de l'état sont décalées de 2 vers la droite (3e ligne de l'état)
    state[10]=tmp;
    tmp=state[6];
    state[6]=state[14];
    state[14]=tmp;

    tmp=state[3];                       //les celleules d'indices 3,7,11,15 de l'état sont décalées de 3 vers la droite (4e ligne de l'état)
    state[3]=state[7];
    state[7]=state[11];
    state[11]=state[15];
    state[15]=tmp;
}

unsigned char galois_mult(unsigned char a , unsigned char b){
    unsigned char result=0;
    unsigned char high_bit_set;
    for(int i=0 ; i<8 ; i++){   // on décompose la multiplication de en plusieurs multiplication
        if(b & 1){             
            result ^= a;      
        }
        high_bit_set= a & 0x80;    //coeff de poids le plus fort de a (0x80 = 1000 0000) en faisant le xor on récupère bien le bit de poids fort de a
        a<<=1;                     // on regarde le bit suivant (on part du bit de poids le plus fort vers le plus faible)

        if(high_bit_set){          // si le bit de a qu'on regarde est 1 alors on xor avex 0x1b (0001 1011) car dans F2^8 x^8=x^4+x^3+x+1
            a^=0x1b;
        }

        b>>=1;
    }
    return result;
}

void mixColumns(unsigned char *state){
    unsigned char temp[4];           // temp stock les char d'une colonne de l'état 
    int j;
    for(int i =0 ; i<4 ;i++){          // pour chaque colonne 
        for(j=0 ; j<4 ;j++){
            temp[j]=state[i*4+j];       // on stock chaque char dans temp
        }
        state[i*4]=galois_mult(temp[0],0x02) ^ galois_mult(temp[1],0x03) ^ temp[2] ^temp[3] ;    // et on la multiplication par le vecteur qu'il faut
        state [i*4+1]=galois_mult(temp[1],0x02) ^ galois_mult(temp[2],0x03) ^ temp[0] ^temp[3] ; // de la matrice : (2 3 1 1)
        state [i*4+2]=galois_mult(temp[2],0x02) ^ galois_mult(temp[3],0x03) ^ temp[0] ^temp[1] ; //                 (1 2 3 1)
        state [i*4+3]=galois_mult(temp[3],0x02) ^ galois_mult(temp[0],0x03) ^ temp[1] ^temp[2] ; //                 (1 1 2 3)
    }                                                                                            //                 (3 1 1 2)
}                                                                                                //et on affecte ces résultats dans l'état


void invMixColumns(unsigned char *state){
    unsigned char temp[4];      // temp stock les char d'une colonne de l'état 
    int j;
    for(int i =0 ; i<4 ;i++){   // pour chaque colonne
        for(j=0 ; j<4 ;j++){
            temp[j]=state[i*4+j];  // on stock chaque char dans temp
        }
        state[i*4]=galois_mult(temp[0],0x0e) ^ galois_mult(temp[1],0x0b) ^ galois_mult(temp[2],0x0d) ^ galois_mult(temp[3],0x09);    // et on la multiplication par le vecteur qu'il faut
        state [i*4+1]=galois_mult(temp[0],0x09) ^ galois_mult(temp[1],0x0e) ^ galois_mult(temp[2],0x0b) ^ galois_mult(temp[3],0x0d); // de la matrice : (0e 0b 0d 09)
        state [i*4+2]=galois_mult(temp[0],0x0d) ^ galois_mult(temp[1],0x09) ^ galois_mult(temp[2],0x0e) ^ galois_mult(temp[3],0x0b); //                 (09 0e 0b 0d)
        state [i*4+3]=galois_mult(temp[0],0x0b) ^ galois_mult(temp[1],0x0d) ^ galois_mult(temp[2],0x09) ^ galois_mult(temp[3],0x0e); //                 (0d 09 0e 0b)
    }                                                                                                                                //                 (0b 0d 09 0e)
}                                                                                                                                    //et on affecte ces résultats dans l'état



void addRoundKey(unsigned char *state ,unsigned char *roundKey){
    for(int i =0 ; i<16 ;i++){
        state[i]= state[i] ^ roundKey[i];     // xor chaque char de l'état avec le char de la clé de ronde correspondante
    }
}

void getRoundKey(unsigned char *expandedKey , unsigned char *roundKey ,int nbrRound){
    for(int i=0 ; i<16 ; i++){
        roundKey[i] = expandedKey[i + 16*nbrRound];   // récupère la clé de ronde du nbrRound tour de ronde
        
    }
}

void cipher(unsigned char *state , unsigned char *expandedKey,int Nr){
    int nbrRound=0;
    unsigned char roundKey[16];
    getRoundKey(expandedKey,roundKey,nbrRound);       // on récupère la première clé de ronde
    nbrRound++;
    addRoundKey(state,roundKey);                      // et on applique addRoundKey avec
	
    for(int i =0 ; i<Nr-1 ; i++){                     // pour Nr-1 tour ronde on applique les 4 transformation de l'AES
        getRoundKey(expandedKey,roundKey,nbrRound);
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state , roundKey);
        nbrRound++;
    }
    getRoundKey(expandedKey,roundKey,nbrRound);     // pour le dernier tour on ne fait pas mixColums
    subBytes(state);
    shiftRows(state);
    addRoundKey(state , roundKey);
}


void invCipher(unsigned char *state , unsigned char *expandedKey,int Nr){
    int nbrRound=Nr;
    unsigned char roundKey[16];                      //fonctionne de manière analogue à cipher mais en prenant comme première clé de ronde la dernière de la clé étendu
    getRoundKey(expandedKey,roundKey,nbrRound);      // et en inversant l'ordre de invShiftRows et invSubBytes par rapport à shiftRows et subBytes
    nbrRound--;                                      //      et de addRoundKey et InvMixColumns par rapport addRoundKey et mixColumns
    addRoundKey(state,roundKey);
    
	
    for(int i =0 ; i<Nr-1 ; i++){
        getRoundKey(expandedKey,roundKey,nbrRound);
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state , roundKey);
        invMixColumns(state);
        
        nbrRound--;
    }
    getRoundKey(expandedKey,roundKey,nbrRound);
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state , roundKey);
}
