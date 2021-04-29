#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================================================ STRUCTURE DE DONNEES

typedef struct
{
    char* numero;
    char* rue;
    char* ville;
    char* codePostal;

} Adresse;

typedef struct
{
    long duree_execution;
    long nombre_lignes;

} Observation;

//  ||||||||||||||||||||||||||||||||||||    CONSTANTES ET VARIABLES GLOBALES    ||||||||||||||||||||||||||||||||||||||||

// Fichier csv contenant les adresses de France, à mettre dans le répertoire parent de l'exécutable
const char* FICHIER_LECTURE_CSV = "../france.csv";
// Fichier csv où écrire les temps de charagement, à mettre dans le répertoire parent de l'exécutable
const char* FICHIER_ECRITURE_TEMPS_CHARGEMENT_CSV = "../observations_chargement.csv";
// Fichier csv où écrire les temps de tri, à mettre dans le répertoire parent de l'exécutable
const char* FICHIER_ECRITURE_TEMP_TRI_CSV = "../observations_tri.csv";

// Tableau global avec une taille suffisante égale au nombre total des addresses sur le fichier CSV
Adresse tableau_adresses[27381996];

//  ||||||||||||||||||||||||||||||||||| FONCTIONS DE LA PARTIE 1. Lecture de données  ||||||||||||||||||||||||||||||||||

/**
 * La fonction suivante permet d'afficher en console une adresse donnée en paramètre
 * @param adresse : Adresse* l'adresse de type struct Adresse
 */
print_adress(Adresse adresse, unsigned long index)
{
    printf("Adresse num %d : %s %s, %s, %s \n",index, adresse.numero, adresse.rue,adresse.ville,adresse.codePostal);
}

/**
 * La fonction suivante permet d'afficher en cosole tous les élément du tableau d'adresses
 */
print_tableau_adresses(const Adresse *tableau_adresses, unsigned long taille){
    printf("%d \n", sizeof (tableau_adresses));
        for (int i = 0; i < taille ; ++i) {
        print_adress(tableau_adresses[i], i+1);
    }
}

/**
 * Cette fonction permet de séparer une chaine en donnant un séparateur, elle renvoit la première partie de la chaine
 * @param stringp
 * @param delim
 * @return
 */
char *strsep (char **stringp, const char *delim){
    char *begin, *end;
    begin = *stringp;
    if (begin == NULL) return NULL;
    end = begin + strcspn (begin, delim);
    if (*end) {
        *end++ = '\0';
        *stringp = end;
    }
    else *stringp = NULL;
    return begin;
}

read_csv_file(Adresse tableau_adresses, unsigned long limit, FILE input_file_stream, FILE* output_file_stream){

    char ligne[1000]; // pour lire 1000 caractères à chaque ligne

    unsigned long num_ligne = 1;
    unsigned long tab_index = 0;

    // Initialiser le compteur de lecture de données
    clock_t debut = clock(); // nombre de pas processeur par seconde

    // Lecture du fichier csv des adresses ligne par ligne
    while (fgets(ligne, sizeof (ligne), input_file_stream)){
        if (num_ligne > limit){ // Si la limite de ligne est dépassée, alors sortir de la boucle
            break;
        }else{
            if (num_ligne != 1) {
                // Lire une ligne du fichier csv (sous forme d'une chaine)
                char* s = strdup(ligne); // on met dans s la chaine à découper
                int colonne = 1;
                char* token = strsep(&s,","); // on récupère le premier token dans token
                while(token!=NULL){ // tant qu’il reste un token
                    switch (colonne) { // Regarder sur quelle colonne on se trouve
                        case 3: // Récupérer le numero de rue si on est à la colonne 3
                            tableau_adresses[tab_index].numero = token;
                            break;
                        case 4: // Récupérer le nom de la rue si on est à la colonne 4
                            tableau_adresses[tab_index].rue = token;
                            break;
                        case 6: // Récupérer le de la ville si on est à la colonne 6
                            tableau_adresses[tab_index].ville = token;
                            break;
                        case 9: // Récupérer le code postal si on est à la colonne 9
                            tableau_adresses[tab_index].codePostal = token;
                            break;
                        default: // Autre colonne, ne rien récupérer
                            break;
                    }
                    token = strsep(&s,","); // récupération du token suivant qui est la colonne suivante
                    colonne ++;
                }
                // Aller à l'adresse suivante sur le tableau
                tab_index+=1;
            }
            // Aller à la ligne suivate sur le fichier csv des adresses de France
            num_ligne+=1;
        }
    }
    // Horodatage de la fin de lecture
    clock_t fin = clock();

    // Ecriture du temps de chargement sur le fichier d'observation
    int temps = 1000*(fin-debut)/CLOCKS_PER_SEC;
    printf("> Ecriture du temps de chargement pour la lecture de %d lignes, %d ms\n", limit, temps);
    fprintf(output_file_stream,"%d, %d \n", limit, temps);
}

//  ||||||||||||||||||||||||||||||||||||||||||| FONCTIONS DE LA PARTIE 2. TRI BRETON  ||||||||||||||||||||||||||||||||||

breton_sort_2(Adresse *t, Adresse *t1){
    char* num = t->numero;
    char* rue = t->rue;
    char* ville = t->ville;
    char* codePostal = t->codePostal;

    t->numero = t1->numero;
    t->rue = t1->rue;
    t->ville = t1->ville;
    t->codePostal = t1->codePostal;

    t1->numero = num;
    t1->rue = rue;
    t1->ville = ville;
    t1->codePostal = codePostal;
}

breton_sort_1(Adresse *t, unsigned long i){
    Adresse max_postal_code_adress = {.numero = t[0].numero, .rue = t[0].rue, .ville = t[0].ville, .codePostal = t[0].codePostal };

    for (unsigned long j=0; j<=i; j++){
        if (atoi(t[j].codePostal)>atoi(max_postal_code_adress.codePostal)){
            breton_sort_2(&t[i], &t[j]);
        }
    }

}


breton_sort(Adresse t, unsigned long taille, FILE output_file_stream_1){
    // Initialiser le compteur de lecture de données
    clock_t debut = clock(); // nombre de pas processeur par seconde

    printf("breton_sort, taille = %d \n", taille);
    while (taille!=-1){
        for (unsigned long i=0; i<taille; i++){
            breton_sort_1(t, i);
        }
        taille-=1;
    }

    // Horodatage de la fin de lecture
    clock_t fin = clock();

    // Ecriture du temps de chargement sur le fichier d'observation
    int temps = 1000*(fin-debut)/CLOCKS_PER_SEC;
    printf("> Ecriture du temps de chargement pour la lecture de %d lignes, %d ms\n", taille, temps);
    fprintf(output_file_stream_1,"%d, %d \n", taille, temps);

}

//  |||||||||||||||||||||||||||||||||||||||||    PROGRAMME PRINCIPALE     ||||||||||||||||||||||||||||||||||||||||||||||

int main() {

    // =========================================================================   Lecture (chargement des données)

    unsigned long nb_max_ligne = 100000; // Equivalent à 1 fois 10 puissance 5 nombres de lignes à lire

    // Pour lire les données
    FILE* input_file_stream = fopen(FICHIER_LECTURE_CSV, "r");
    if (input_file_stream==NULL){
        perror("Impossible d'ouvrir le fichier csv de lecture !");
        exit(1);
    }

    // Pour écrire les temps de chargement des données
    FILE* output_file_stream = fopen(FICHIER_ECRITURE_TEMPS_CHARGEMENT_CSV, "w+");
    if (output_file_stream==NULL){
        perror("Impossible d'ouvrir le fichier csv d'ecriture de temps de chargement !");
        exit(1);
    }
    fprintf(output_file_stream,"NOMBRE DE LIGNES CHARGEES, TEMPS DE CHARGEMENT EN MILLISECONDES\n");

    // Pour ecrire les temps de tri
    FILE* output_file_stream_1 = fopen(FICHIER_ECRITURE_TEMP_TRI_CSV, "w+");
    if (output_file_stream_1==NULL){
        perror("Impossible d'ouvrir le fichier csv d'ecriture de temps de tri !");
        exit(1);
    }
    fprintf(output_file_stream_1,"NOMBRE DE LIGNES CHARGEES, TEMPS DE CHARGEMENT EN MILLISECONDES\n");

    while (nb_max_ligne<=2000000){ // 27381996 Nombre totale des lignes sur le fichier csv

        // Lire le fichier csv dans le tableau
        printf("> LECTURE DU FICHIER CSV ... \n");
        read_csv_file(&tableau_adresses, nb_max_ligne, input_file_stream, output_file_stream);
        printf("> FIN DE LECTURE DU FICHIER CSV. \n");

        // Tri des données
//        printf("> TRI DES DONNEES ... \n");
//        breton_sort(&tableau_adresses,nb_max_ligne-1, output_file_stream_1); // nb_max_ligne-1 car la première ligne est une entête sur le fichier
//        printf("> TRI DES DONNEES. \n");

        // Décommenter la ligne suivante pour afficher le contenu du tableau sur la console
        // print_tableau_adresses(&tableau_adresses, nb_max_ligne-1);

        nb_max_ligne+=100000; // Augmenter le nombre de lignes à lire depuis le fichier de 1*10^5 lignes
    }

    // Fermer les fichiers csv
    fclose(input_file_stream);
    fclose(output_file_stream);
    fclose(output_file_stream_1);

    return 0;
}