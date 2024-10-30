#include <mode.h>
#include<cipher.h>
#include <stdio.h>
#include <stdlib.h>



void modeECBc(unsigned char **txt , unsigned char *key , int Nr , int Nk , int nbloc){
    int size_ek=16*(Nr+1);                // taille de expandedKey en fonction de si on opère en 128 ou 192 ou 256 bits
    unsigned char expandedKey[size_ek];
    unsigned char state[16];
    int i;
    int j;
    expandKey(key , expandedKey , Nk , size_ek);   // On calcule expandedKey
    for(i=0 ; i<nbloc ; i++){
        for (j=0 ; j<16 ; j++){                
            state[j]=txt[i][j];               
        }
        cipher(state , expandedKey , Nr );     // on chiffre chaque bloc indépendamment 
        for (j=0 ; j<16 ; j++){
            txt[i][j]=state[j];
        }
    }
}

void modeECBd(unsigned char **txt , unsigned char *key , int Nr , int Nk , int nbloc){
    int size_ek=16*(Nr+1);             // taille de expandedKey en fonction de si on opère en 128 ou 192 ou 256 bits
    unsigned char expandedKey[size_ek];
    unsigned char state[16];
    int i;
    int j;
    expandKey(key , expandedKey , Nk , size_ek);   // On calcule expandedKey
    for(i=0 ; i<nbloc ; i++){
        for (j=0 ; j<16 ; j++){
            state[j]=txt[i][j];
        }
        invCipher(state , expandedKey , Nr );    // on déchiffre chaque bloc indépendamment 
        for (j=0 ; j<16 ; j++){
            txt[i][j]=state[j];
        }
    }
}


void modeCBCc(unsigned char **txt , unsigned char *key , unsigned char *IV, int Nr , int Nk , int nbloc){
    int size_ek=16*(Nr+1);                // taille de expandedKey en fonction de si on opère en 128 ou 192 ou 256 bits
    unsigned char expandedKey[size_ek];
    unsigned char state[16];
    unsigned char IVtemp[16];
    int i;
    int j;
    for (i =0 ; i<16 ; i++){
        IVtemp[i]=IV[i];            
    }
    expandKey(key , expandedKey , Nk , size_ek);  // On calcule expandedKey
    for(i=0 ; i<nbloc ; i++){
        for (j=0 ; j<16 ; j++){
            state[j]=txt[i][j] ^ IVtemp[j];    // on XOR le bloc de plaintext avec le bloc de ciphertext précédent (avec IV pour le premier bloc)
        }
        cipher(state , expandedKey , Nr );    // on chiffre le bloc 
        for (j=0 ; j<16 ; j++){
            txt[i][j]=state[j];
            IVtemp[j]=state[j];             
        }
    }
}

void modeCBCd(unsigned char **txt , unsigned char *key , unsigned char *IV, int Nr , int Nk , int nbloc){
    int size_ek=16*(Nr+1);                   // taille de expandedKey en fonction de si on opère en 128 ou 192 ou 256 bits
    unsigned char expandedKey[size_ek];
    unsigned char state[16];
    unsigned char IVtemp[16];
    unsigned char IVtemp2[16];
    int i;
    int j;
    for (i =0 ; i<16 ; i++){
        IVtemp[i]=IV[i];
    }
    expandKey(key , expandedKey , Nk , size_ek);  // On calcule expandedKey
    for(i=0 ; i<nbloc ; i++){
        for (j=0 ; j<16 ; j++){
            state[j]=txt[i][j];
            if(i%2==0){
                IVtemp[j]=txt[i][j];              
            }
            if(i%2==1){
                IVtemp2[j]=txt[i][j];
            }
        }
        invCipher(state , expandedKey , Nr );
        for (j=0 ; j<16 ; j++){
            if(i==0){
                txt[i][j]=state[j] ^ IV[j];                // on XOR le ciphertxt passé dans invCipher avec le plaintext précédent
            }                                              // pour retrouver le bon plaintext(on XOR avec IV pour le premier bloc)
            if(i%2==0 && i!=0){
                txt[i][j]=state[j] ^ IVtemp2[j];
            }
            if(i%2==1){
                txt[i][j]=state[j] ^ IVtemp[j];
            }
        }
    }
}
