#include "stdes.h"
#include "errno.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>

#define TAILLE 4096
#define LECTURE O_RDONLY
#define ECRITURE O_WRONLY | O_CREAT
#define NB_FILE 15

/* STRUCTURE */
typedef struct _ES_FICHIER {
	int filedesc;
	char buffer[TAILLE];
	int pos;
	char mode;
	int index;
	int pos_max;
} FICHIER;

FICHIER tab_fichier[NB_FILE];
int tab_verif[NB_FILE];


/* OUVRIR */
// Return l'adresse de la structure ou NULL si probleme.
FICHIER* ouvrir(char *nom, char mode){

	printf("nom=%s\n", nom);

	//Creation de la structure dans le TAS.
	FICHIER *f = malloc(sizeof(FICHIER));
	//Initialisation des champs de le structure.
	f->pos = 0;
	f->pos_max = 0;
	f->mode = mode;
	//Initialisation du buffer.
	// int j = 0;
	// for (j = 0; j < TAILLE; j++){
	// 	f->buffer[j] = '0';
	// }

	//Test du mode d'ouverture pour l'AS open.
	if (mode == 'L') {
		f->filedesc = open(nom, LECTURE);
	} else if (mode == 'E') {
		f->filedesc = open(nom, ECRITURE , 0666 );
	} else {
		//Ouverture impossible : renvoi NULL
		return NULL;
	}

	//Recherche d'une place dans le tableau de FICHIER
	int i = 0;
	while(tab_verif[i] != 0) {
		i++;
	}
	if (i == NB_FILE) {
		//plus de place dans le tableau
		//placer errno
		return NULL;
	} else {
		//La place i est libre.
		f->index = i;
		tab_fichier[i] = *f;
		tab_verif[i] = 1;
	}

	return f;
}

/* FERMER */
// return 0 if success, -1 if error
int fermer(FICHIER* f) {
	printf("bonjour je veux fermer. buffer=\"%s\"\n", f->buffer);
	//Ecrire dans le fichier si il reste des données dans le buffer. En mode ecriture.
	if (f->mode == 'E') {
		//todo : gestion d'erreur
		int check_w = write(f->filedesc, f->buffer, f->pos);
		if (check_w == -1) {
			perror("erreur write");
			// if(errno == EINTR) {
			// 	check_w = write(f->filedesc, f->buffer, f->pos);
			// }
		} else if (check_w != f->pos) {
			check_w = write(f->filedesc, f->buffer + check_w, f->pos - check_w);
		}
	}
	int check_c = close(f->filedesc);
	//Libere le tableau de la struct.
	tab_verif[f->index] = 0;
	//Free de la struct.
	free(f);
	return check_c;
}

/* LIRE */
// return le nombre d'octets lus (dans le buffer) si succes, -1 si erreur.
int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f) {
	int res;
	if (f->mode == 'L'){
		//On est dans le bon mode.
		//Chargement initial du buffer.
		if (f->pos == 0 || f->pos >= 4096){
			res = read(f->filedesc, f->buffer, TAILLE);
			if (res <= 4096){
				//Mettre un champ pos max pour simuler une fin de fichier.
				f->pos_max = res;
				f->pos = 0;
			} else {
				//printf("Erreur relecture.\n");
			}
		}
		//On place en memoire (pointé par l'utilisateur) la lecture demandée.
		//Il en faut lire dans le buffer que la longeur du fichier disponible.
		if (f->pos < f->pos_max){
			//On place a l'endroit voulu par l'utilisateur sa lecture.
			strncpy((char*) p, f->buffer+f->pos, taille*nbelem);
			//On decale la posotion de lecture dans le buffer.
			f->pos += (taille * nbelem);
			return nbelem*taille;
		} else {
			//Il n'y a plus rien a lire : return -1
			return -1;
		}
	} else {
		//Pas dans le bon mode: return -1
		return -1;
	}
}

/* ECRIRE */
//return le nombre d'octets ecrit (dans le buffer).
int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f) {
	if (f->mode == 'E'){
		//On est dans le bon mode.
		//Ajout dans le buffer du contenu pointé par p.
		strncpy(f->buffer+f->pos, (char*) p, taille*nbelem);
		printf("buffer=\"%s\"\n", f->buffer);
		//Verification du remplissage du buffer.
		f->pos += (taille * nbelem);
		//Si le buffer est plein : 
		if (f->pos >= 4096){
			//On ecrit réellement sur le disque.
			int ecrire = write(f->filedesc, f->buffer, 4096);
			if (ecrire != 4096){
				printf("erreur ecriture des 4096 caracteres.\n");
			}
			//Replacement de la variable position au debut.
			f->pos = 0;
			//Verifier le debordement du buffer.

		}
		//Return de la taille ecrite dans le buffer.
		return (taille * nbelem);
	}
	return -1;
}

/* FECRIREF */
// equivalent maison de fprintf (version gitan 2.4)
int fecriref(FICHIER* f, char* format, ...) {
	va_list ap;	//structure interne au compilateur pour savoir ou il en est.
	va_start(ap, format); //Il faut lui donner le dernier parametre connu avant les "..." a savoir format.

	int i = 0;
	while(*(format+i) != '\0') {
		//printf("loop=%d\n", i);
		if (*(format+i) == '%') {
			i++;
			switch(*(format+i)) {
				case 'c' : {
					int c = va_arg(ap, int);
					ecrire(&c, sizeof(char), 1, f);
					break;
				}
				case 'd' : {
					int d = va_arg(ap, int);
					printf("int detected (value=%d)\n", d);
					ecrire(&d, sizeof(int), 1, f);
					break;
				}
				case 's' : {
					char* s = va_arg(ap, char*);
					ecrire(s, strlen(s), 1, f);
					break;
				}
			}
		} else { 
			ecrire(format+i, sizeof(char), 1, f);
		}
		i++;
	}
	va_end(ap);

	return 0;
}

/* FLIREF */
// . . . . . . . . . . . . . . .
int fliref(FICHIER* f,char* format, ...) {
	return 0;
}