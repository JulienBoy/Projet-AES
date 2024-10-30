#ifndef PARSER_H
#define PARSER_H

//fonction qui renvoie 1 si la chaine de caractère str est de l'hexadécimal, 0 sinon
int valid_hex_str( char *str , int size_str);

//fonction qui renvoie 1 si le caractère c est de l'hexadécimal, 0 sinon
int valid_hex(unsigned char c);

//fonction qui prend une chaine de caractère d'hexa hex_string et qui modifie la chaine de caractère key pour qu'elle corresponde en la chaine d'hexa hex_string 
//exemple : hex_string = "303030" alors la fonction renvoie key = "000"
void hex_string_to_hex( char *hex_string , unsigned char *key , int lenstr);

//fonction qui permet de désallouer la mémoire pour txt
void txt_free(unsigned char **txt,int nbloc);

//fonction qui permet d'écrire en caractère txt dans le fichier "filename" 
void print_in_file(char *filename , unsigned char **txt, int nbloc);

//fonction qui permet d'écrire en hexadécimal txt dans le fichier "filename" 
void print_in_file_h(char *filename , unsigned char **txt, int nbloc);

//fonction qui permet stocker dans un tableau de tableau de caractère le contenu du fichier "filname" (contenu en caractère)
//et modifie la valeur de nbloc en le nombre de bloc de 128 bits de ce contenu
unsigned char** file_parser(char* filename, int* nbloc);

//fonction qui permet stocker dans un tableau de tableau de caractère le contenu du fichier "filname" (contenu en hexa)
//et modifie la valeur de nbloc en le nombre de bloc de 128 bits de ce contenu
unsigned char** file_parser_h(char* filename, int* nbloc);


#endif