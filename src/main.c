#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include <mode.h>
#include <parser.h>
#include <cipher.h>


int main(int argc, char *argv[]){
    int opt;                                // |
	int flag_c=0;				            // |
	int flag_d=0;                           // |
	int flag_o=0;                           // |
	int flag_t=0;                           // |  varilables utiliseées  
	int flag_m=0;                           // |  pour le parsing d'argument
	int flag_k=0;                           // |
	int flag_x=0;                           // |
	int flag_x_in=0;                        // |
	int flag_x_out=0;                       // |
	int flag_v=0;                           // |
	char *key_arg=NULL;                     // |
	char *iv_arg=NULL;                      // |
	char *x_arg=NULL;                       // |
    char *fichier_sortie = "cipher.txt";    // |
	char *fichier_entree = NULL;            // |
	unsigned char key[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};  // clé par défaut
	unsigned char IV[16]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};   // IV par défaut
	int lenKey=16;      //  varaibles initialisées pour 
	int Nr=10;         //  faire par défaut du chiffrement 128 bits
	int Nk=4;          //
	
//__________________________________________________________________//

    void afficher_aide() {         //aide qui explique toutes les options
        printf("Usage :\t main [-c FILE|-d FILE|-o FILE|-m ecb/cbc|-k clé|-v IV|-t|-x in/out/all|-h] ...\n");
		printf("-c : chiffre en AES le texte du fichier donné (mode ECB par défault) \n");
		printf("-d : déchiffre en AES le texte du fichier donné (mode ECB par défault) \n");
		printf("-o : écrit le fichier chiffré dans un fichier choisi (l'écrase s'il existe déjà) \n");
		printf("-m : mode de chiffrement (ECB ou CBC) (mode ECB par défault) \n");
		printf("-k : clé à entrée par l'utilisateur pour chiffrer avec celle-ci \n");
		printf("-v : vecteur d'initialisation à entrée par l'utilisateur pour chiffrer en mode CBC \n");
		printf("-t : chiffre 100 fois alice.txt en ECB (par défaut on utilise la clé par défaut) \n");
		printf("-x : permet de lire et écrire les fichiers hexa\n\t '-x in' : pour juste le fichier d'entrée en hexa\n\t '-x out': pour juste le fichier de sortie en hexa \n\t '-x all': pour les deux\n");
		printf("-h : affiche cette aide \n");
    }

//__________________________________________________________________//

    static struct option long_options[] ={                 // construction des options
		{"chiffre FILE", required_argument, NULL, 'c'},
		{"dechiffre FILE", required_argument, NULL, 'd'},
		{"output FILE", required_argument, NULL, 'o'},
		{"mode *char", required_argument, NULL, 'm'},
		{"key *char", required_argument, NULL, 'k'},
		{"vecteur *char", required_argument, NULL, 'v'},
		{"test", no_argument, NULL, 't'},
		{"hexa", required_argument, NULL, 'x'},
		{"help", no_argument, 0,'h'},
		{0,0,0,0}
	};

	
//__________________________________________________________________//

    while ((opt = getopt_long(argc,argv, "c:d:o:m:k:v:tx:h", long_options,NULL)) != -1) {         // parser d'options
		switch (opt) {
			
			case 'c':                                                   
				fichier_entree = optarg;                        //on va chiffrer le fichier entrée dans la commande
				if (fichier_entree==NULL){
					fprintf(stderr,"error fichier entree\n");
					exit(EXIT_FAILURE);
				}
				flag_c=1;
				break;

			case 'd':                                                   
				fichier_entree = optarg;                       //on va déchiffrer le fichier entrée dans la commande
				fichier_sortie = "plain.txt";
				if (fichier_entree==NULL){
					fprintf(stderr,"error fichier entree\n");
					exit(EXIT_FAILURE);
				}
				flag_d=1;
				break;

			case 'o':
				fichier_sortie = optarg;                        //on va écrire le résultat du chiffrement/déchiffrement 
				flag_o=1;
				if (fichier_sortie==NULL){                      // dans le fichier entrée dans la commande
					fprintf(stderr,"error fichier sortie\n");
					exit(EXIT_FAILURE);
				}
				break;
			
			case 'm': 
				if(strcmp(optarg,"ecb")==0 || strcmp(optarg,"ECB")==0){      // on a ici 2 modes possibles
					flag_m=0;												 // soit ECB ou bien CBC 
				}
				if(strcmp(optarg,"cbc")==0 || strcmp(optarg,"CBC")==0){
					flag_m=1;
				}
				break;

			case 'k':
				flag_k=1;                        
				key_arg=optarg;            // on stock l'argument dans un pionteur pour le réutiliser plus tard
				break;

			case 'v':
				flag_v=1;
				iv_arg=optarg;			   // on stock l'argument dans un pionteur pour le réutiliser plus tard
				break;

			case 't':
				fichier_entree="alice.txt";   //option pour le test le fichier qui sera chiffré est alice.txt
				flag_t=1;                    
				break;

			case 'x':
				flag_x=1;                  
				x_arg=optarg;              // on stock l'argument dans un pionteur pour le réutiliser plus tard
				break;
			case 'h' :
				afficher_aide();         //on affiche l'aide
				exit(EXIT_SUCCESS);

			case '?':
				fprintf(stderr, "option inconnue : %s \n", argv[optind-1]);   //si l'option ne fait pas partie des options prévues on renvoie une erreur
				exit(EXIT_FAILURE);
		}
    }
    
//__________________________________________________________________//


	//on vérifie qu'il n'y ait pas plusieurs commandes incompatibes demandées en même temps

	if(flag_c && flag_d){         
		fprintf(stderr, "commandes incompatible : on ne peut pas chiffrer et déchiffrer en même temps\n");
        exit(EXIT_FAILURE);
	}

	if(!(flag_c || flag_d || flag_t)){
		fprintf(stderr, "Il manque une commande il faut soit :\n\tchiffrer : -c FILE\n\tdéchiffrer : -d FILE\n\teffectuer le test : -t\n");
        exit(EXIT_FAILURE);
	}

	if(flag_t && (flag_c || flag_d || flag_m )){
		fprintf(stderr, "commandes incompatible : seul '-t' ou '-t -k clé' sont acceptées\n");
        exit(EXIT_FAILURE);
	}


	//on ne renvoie pas d'erreur mais on préviens d'éventuelle commandes passées inutiles ou de commande non nécessaires oubliées

	if((flag_c || flag_d || flag_v) && !flag_m && (flag_v)){
		printf("Le mode ECB n'utilise pas de vecteur d'initialisation la commande '-v' n'est pas nécessaire\n");
	}

	if(flag_m && !flag_v){
		printf("On chiffre en mode CBC mais aucun vecteur d'initialisation n'a été entré\nOn utilise donc l'IV par défaut : 00112233445566778899aabbccddeeff\n");
	}
	
	if(flag_o && flag_t){
		printf("Dans le mode de test aucun fichier de sortie n'est attendu\n");
	}

	if(flag_x && flag_t){
		printf("Dans le mode de test la commande '-x in/out/all' est inutile et n'a aucun effet\n");
	}

//__________________________________________________________________//
//A partir d'ici on va exécuter les commandes demandées


	if(flag_x){                                                    // on distingue 3 cas :
		flag_x=0;	                                               //
		if(strcmp(x_arg,"in")==0 || strcmp(x_arg,"IN")==0){        //  -on lit le fichier entrée en hexa
			flag_x_in=1;                                           //
		}                                                          //
		if(strcmp(x_arg,"out")==0 || strcmp(x_arg,"OUT")==0){      //  -on écrit dans le fichier de sortie en hexa
			flag_x_out=1;										   //
		}                                                          //
		if(strcmp(x_arg,"all")==0 || strcmp(x_arg,"ALL")==0){      //  -on lit le fichier entrée en hexa
			flag_x=1;                                              //    et on écrit dans le fichier de sortie en hexa
		}
	}

	if(!(flag_x || flag_x_in) && !flag_t){         //on prévient d'un éventuelle oubli de la commande -x
			printf("WARNING : si le fichier d'entrée est en hexadécimal le résultat du chiffrement/déchiffrement ne sera pas le bon\n");
		}

    if(flag_k){
		int lenstr=strlen(key_arg);                              
		if (lenstr%2!=0){                                       // on vérifie que la taille de la clé soit valide
			fprintf(stderr, "taille de clé invalide \n");
        	exit(EXIT_FAILURE);
		}
		if (lenstr/2==16 || lenstr/2==24 || lenstr/2==32 ){
			lenKey=lenstr/2;
		}
		else{
			fprintf(stderr, "taille de clé invalide \n");
        	exit(EXIT_FAILURE);
		}
		if(lenKey == 24){                  // cas où on chiffre/déchiffre avec une clé 192 bits
			Nk=6;
			Nr=12;
		}
		if(lenKey == 32){				   // cas où on chiffre/déchiffre avec une clé 256 bits
			Nk=8;
			Nr=14;
		}
		key[lenKey];
		hex_string_to_hex(key_arg , key , lenstr);    // on trasforme l'argument reçu en clé valide qu'on pourra utilisé plus tard
	}



	if(flag_v){
		int lenstr=strlen(iv_arg);                        // on vérifie que la taille du vecteur d'initialisation soit valide
				if (lenstr%2!=0){
					fprintf(stderr, "taille du vecteur est invalide \n");
        			exit(EXIT_FAILURE);
				}
				if (lenstr/2!=16){
					fprintf(stderr, "taille de vecteur invalide différent de 128 bits \n");
        			exit(EXIT_FAILURE);
				}
				lenstr=lenstr/2;
				unsigned char IV[lenstr];
				hex_string_to_hex(iv_arg , IV , lenstr);  // on trasforme l'argument reçu en vecteur valide qu'on pourra utilisé plus tard
	}
	
	
      
	if(flag_t){                                  // on effectue ici le test qui consiste à chiffrer 100 fois alice.txt en mode ECB
		clock_t start_time , end_time ;          //  et avec la clé par defaut (on peut choisir de le faire avec une autre clé avec -k)
		double cpu_time;
		start_time=clock();
		for (int i=0 ; i<100 ; i++){
			int nbloc;
			unsigned char** txt=file_parser(fichier_entree , &nbloc); 
			modeECBc(txt,key,Nr,Nk,nbloc);
		}
		end_time=clock();                                                               //précision : le temps donner par l'algorithme n'est pas le temps réel 
		cpu_time= ((double)(end_time - start_time))/CLOCKS_PER_SEC;                     // pris pous l'execution de la commande -t mais celui du processeur pour 
		printf("Temps pour chiffrer 100 fois alice.txt : %f secondes\n" , cpu_time );   //  faire 100 fois : - lire le ficher et le chiffrer
		printf("Possibilité d'utiliser la commande 'time ./main -t' pour avoir le temps réel de l'exécution\n");
	}
	

	
    if(flag_c){                                           // partie où on va chiffrer
		int nbloc;
		unsigned char** txt;
		if(flag_x || flag_x_in){
			txt=file_parser_h(fichier_entree , &nbloc);      // on stock les informations du fichier d'entrée 	
		}													 // qui est soit en caractères soit en hexadécimal
		else{
			txt=file_parser(fichier_entree , &nbloc);
		}
		switch (flag_m)               // on va chiffrer en fonction du mode choisi avec la commande -m
		{
		case 0:
			modeECBc(txt , key , Nr ,Nk ,nbloc);
			printf("Un fichier %s a été créé contenant %s chiffré en AES mode ECB\n", fichier_sortie , fichier_entree);
			break;

		case 1:
			modeCBCc(txt , key , IV , Nr ,Nk ,nbloc);
			printf("Un fichier %s a été créé contenant %s chiffré en AES mode CBC\n", fichier_sortie , fichier_entree);
			break;

		default:
			modeECBc(txt , key , Nr ,Nk ,nbloc);
			printf("Un fichier %s a été créé contenant %s chiffré en AES mode ECB\n", fichier_sortie , fichier_entree);
			break;
		}
		if(flag_x || flag_x_out){
			print_in_file_h(fichier_sortie , txt , nbloc);      //on écrit le résultat dans le fichier de sortie 
		}else{                                                  // soit en caractère soit en hexadécimal 
			print_in_file(fichier_sortie , txt , nbloc);
		} 
	    txt_free(txt,nbloc);
    }




	if(flag_d){                    //partie où on va déchiffrer
		int nbloc;
		unsigned char** txt;
		if(flag_x || flag_x_in){
			txt=file_parser_h(fichier_entree , &nbloc);          // on stock les informations du fichier d'entrée
		}														 // qui est soit en caractères soit en hexadécimal
		else{
			txt=file_parser(fichier_entree , &nbloc);
		}
		 
		switch (flag_m)                  // on va déchiffrer en fonction du mode choisi avec la commande -m
		{
		case 0:
			modeECBd(txt , key , Nr ,Nk ,nbloc);
			printf("Un fichier %s a été créé contenant %s déchiffré en AES mode ECB\n", fichier_sortie , fichier_entree);
			break;

		case 1:
			modeCBCd(txt , key , IV , Nr ,Nk ,nbloc);
			printf("Un fichier %s a été créé contenant %s déchiffré en AES mode CBC\n", fichier_sortie , fichier_entree);
			break;

		default:
			modeECBd(txt , key , Nr ,Nk ,nbloc);
			printf("Un fichier %s a été créé contenant %s séchiffré en AES mode ECB\n", fichier_sortie , fichier_entree);
			break;
		}
		if(flag_x || flag_x_out){
			print_in_file_h(fichier_sortie , txt , nbloc);       //on écrit le résultat dans le fichier de sortie 
		}else{                                                   // soit en caractère soit en hexadécimal 
			print_in_file(fichier_sortie , txt , nbloc);
		} 
	    txt_free(txt,nbloc);
    }
	

}