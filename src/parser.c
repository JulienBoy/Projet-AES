#include <parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 16

int valid_hex_str( char *str , int size_str){
    for (int i=0 ; i<size_str ; i++){
        if(!(valid_hex(str[i]))){            // on vérifie que chaque caractère est bien valide pour représenter  
            return 0;                        // un chaine d'hexadécimale
        }
    }
    return 1;
}


int valid_hex(unsigned char c){
    if (c >= 65 && c <= 70){   //entre A et F
        return 1;
    }
    if (c >= 97 && c <= 102){   //entre a et f
        return 1;
    }
    if (c >= 48 && c <= 57){   //entre 0 et 9
        return 1;
    }
    return 0; // c ne correspond pas à un caractère valide pour de l'hexa
}

void hex_string_to_hex( char *hex_string , unsigned char *key , int lenstr){
    if(!(valid_hex_str(hex_string , lenstr))){                                //on vérifie que la chine de caractère est bien en hexa
        fprintf(stderr, "clé non valide : la clé est demandée en hexa \n");
        exit(EXIT_FAILURE);
    }
    for(int i=0 ; i<lenstr/2 ; i++){                                        //on transforme 2 caractères de la liste en un caractère qui a  
        const char byte_str[3]={hex_string[2*i] , hex_string[2*i+1],'\0'};  //  une représentation en hexa qui correspond aux 2 caractères de la liste de caractère entrée
        key[i] = (unsigned char)strtol(byte_str , NULL ,16);                //  hexa:4bits   char:8bits donc chaque char est représenté par 2 hexa
    }
}

void txt_free(unsigned char **txt,int nbloc){
    for(int i =0 ; i<nbloc ; i++){
        free(txt[i]);
    }
    free(txt);
}

void print_in_file(char *filename , unsigned char **txt, int nbloc){
    FILE *new_file= fopen(filename,"w");                            //on ouvre le fichier "filename" en mode écriture 
    for(int i=0 ; i<nbloc ; i++){                                   // si il existe déjà le fichier est écrasé et on écrit par dessus
        for(int j=0;j<16;j++){
            fprintf(new_file,"%c",txt[i][j]);
        }
    }
    fclose(new_file);                                         
}

void print_in_file_h(char *filename , unsigned char **txt, int nbloc){
    FILE *new_file= fopen(filename,"w");                    //on ouvre le fichier "filename" en mode écriture
    for(int i=0 ; i<nbloc ; i++){                           // si il existe déjà le fichier est écrasé et on écrit par dessus
        for(int j=0;j<16;j++){
            fprintf(new_file,"%02X",txt[i][j]);             // on écrit le contenu du fichier dnas le format hexadécimal
        }
    }
    fclose(new_file);
}


unsigned char** file_parser(char* filename, int* nbloc) {
    FILE *file;
    file = fopen(filename, "r");              // on ouvre le fichier en mode lecture
    unsigned char** txt;                      // on initialise le tableau de tableau de char
    int i;
    int j;
    
    if (file == NULL) {                        //on vérifie que le fichier existe
        fprintf(stderr, "erreur lors de l'ouverture  du fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fseek(file,0,SEEK_END);
    int taille_fichier = ftell(file);         //calcul de la taille du fichier 
    fseek(file,0,SEEK_SET);
    if (taille_fichier%N==0){
        *nbloc = taille_fichier/N;            // taille du tableau de tableau de 16 char (128bits)
    }else{
        *nbloc = taille_fichier/N +1;           
    }
    txt =(unsigned char**)malloc( *nbloc * sizeof(unsigned char*));         //on alloue la mémoire de txt
    if(txt == NULL) {
        fprintf(stderr, "la mémoire n'a pas été allouée\n");
        fclose(file);
        return NULL;
    }

    for(i = 0; i < *nbloc; i++) {                                          
        txt[i] = (unsigned char*)malloc((N+1)* sizeof(unsigned char));
        if(txt[i] == NULL) {
            fprintf(stderr, "la mémoire n'a pas été allouée\n");
            fclose(file);
            return NULL;
        }
    }
    
    unsigned char c;                           // on remplit txt avec le contenu du fichier
    for(i = 0; i < *nbloc - 1; i++) {          // par bloc de 16 char
        for(j = 0; j < N; j++) {
            c = fgetc(file);
            txt[i][j] = c;
        }
        txt[i][N] = '\0';
    }
    int pad=taille_fichier % N;       //on initialse la taille du padding
    if (pad!=0){                      
        for (j=0 ; j<pad ; j++){      //on padd avec des  si besoin
            c = fgetc(file);
            txt[*nbloc-1][j] = c;
        }
        for (j=pad ; j<N ;j++){
            txt[*nbloc-1][j] = '0';
        }
        txt[*nbloc-1][N] = '\0';
    }else{
       for (j=0 ; j<N ; j++){
            c = fgetc(file);
            txt[*nbloc-1][j] = c;
        } 
        txt[*nbloc-1][N] = '\0';
    }
    
    fclose(file);
    return txt;

}

unsigned char** file_parser_h(char* filename, int* nbloc) {
    FILE *file;
    file = fopen(filename, "r");                   // on ouvre le fichier en mode lecture
    unsigned char** txt;                           // on initialise le tableau de tableau de char
    int i;
    int j;
    
    if (file == NULL) {                          //on vérifie que le fichier existe
        fprintf(stderr, "erreur lors de l'ouverture  du fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fseek(file,0,SEEK_END);
    int taille_fichier_h = ftell(file);             //on calcule la taille du fichier
    if(taille_fichier_h%2!=0){                        //on renvoie une erreur si la taille n'est pas cohérente (divisible par 2)
        fprintf(stderr, "fichier invalide la taille de %s ne correspond pas à un fichier en hexa \n", filename);
        exit(EXIT_FAILURE);
    }
    fseek(file,0,SEEK_SET);
    int taille_fichier =taille_fichier_h/2;            //comme un caractère est représentée par 2 hexa
                                                       // la taille du fichier si il était écrit en char serait 2 fois plus petite
    if (taille_fichier%N==0){
        *nbloc = taille_fichier/N;                     // calcul de la taille de bloc de 16 char
    }else{
        *nbloc = taille_fichier/N +1;
    }
    txt =(unsigned char**)malloc( *nbloc * sizeof(unsigned char*));      // on alloue la mémoire pour que txt puisse contenir 
    if(txt == NULL) {                                                    //  le contenu du fichier "filname"
        fprintf(stderr, "la mémoire n'a pas été allouée\n");
        fclose(file);
        return NULL;
    }

    for(i = 0; i < *nbloc; i++) {
        txt[i] = (unsigned char*)malloc((N+1)* sizeof(unsigned char));
        if(txt[i] == NULL) {
            fprintf(stderr, "la mémoire n'a pas été allouée\n");
            fclose(file);
            return NULL;
        }
    }
    
    unsigned char c1;
    unsigned char c2;
    unsigned char h;
    for(i = 0; i < *nbloc - 1; i++) {        // on remplie txt avec le contenu du fichier (les char correspondant aux couple d'hexa)
        for(j = 0; j < N; j++) {                   
            c1 = fgetc(file);
            c2 = fgetc(file);
            const char byte_str[3]={c1 , c2,'\0'};
            h = (unsigned char)strtol(byte_str , NULL ,16);
            txt[i][j] = h;
        }
        txt[i][N] = '\0';
    }
    int pad=(taille_fichier % N) ;         // on calcul la taille du padding
    if(pad!=0){
        for (j=0 ; j<pad ; j++){            // et puis on padd si besoin
            c1 = fgetc(file);
            c2 = fgetc(file);
            const char byte_str[3]={c1 , c2,'\0'};
            h = (unsigned char)strtol(byte_str , NULL ,16);
            txt[*nbloc -1 ][j] = h;
        }
        for (j=pad ; j<N ;j++){
            txt[*nbloc-1][j] = '0';
        }
        txt[*nbloc-1][N] = '\0';
    }else{
        for (j=0 ; j<N ; j++){
            c1 = fgetc(file);
            c2 = fgetc(file);
            const char byte_str[3]={c1 , c2,'\0'};
            h = (unsigned char)strtol(byte_str , NULL ,16);
            txt[*nbloc -1 ][j] = h;
        }
        txt[*nbloc-1][N] = '\0';
    }
    
    fclose(file);
    return txt;

}
