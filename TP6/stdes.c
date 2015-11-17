#include "stdes.h"
#include "errno.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


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
} FICHIER;

FICHIER tab_fichier[NB_FILE];
int tab_verif[NB_FILE];


/* OUVRIR */
// Return l'adresse de la structure ou NULL si probleme.
FICHIER* ouvrir(char *nom, char mode){
	//Creation de la structure dans le TAS.
	FICHIER *f = malloc(sizeof(f));
	//Initialisation des champs de le structure.
	f->pos = 0;
	f->mode = mode;

	//Test du mode d'ouverture pour l'AS open.
	if (mode == 'L') {
		f->filedesc = open(nom, LECTURE);
	} else if (mode == 'E') {
		f->filedesc = open(nom, ECRITURE , 0777 );
	} else {
		//Ouverture impossible : renvoi NULL
		return NULL;
	}

	//Recherche d'une place dans le tableau de FICHIER
	int i = 0;
	/*while(tab_fichier[i].index != 0) {
		i++;
	}*/
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
	printf("fonction de fermeture\n");
	//Ecrire dans le fichier si il reste des données dans le buffer. En mode ecriture.
	if (f->mode == 'E') {
		int check_w = write(f->filedesc, f->buffer, f->pos);
		if (check_w == -1) {
			if(errno == EINTR) {
				check_w = write(f->filedesc, f->buffer, f->pos);
			}
		} else if (check_w != f->pos) {
			check_w = write(f->filedesc, f->buffer + check_w, f->pos - check_w);
		}
	}
	int check_c = close(f->filedesc);
	//Libere le tableau de la struct.
	tab_verif[f->index] = 0;
	//tab_fichier[f->index] = -1;
	//Free de la struct.
	free(f);
	printf("exit fermeture\n");
	return check_c;
}

/* LIRE */
// return le nombre d'octets lus si succes, -1 si erreur.
int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f) {
	printf("fonction lire\n");
	int res;
	if (f->mode == 'L'){
		//On est dans le bon mode.
		//Chargement initial du buffer.
		if (f->pos == 0 || f->pos >= 4096){
			res = read(f->filedesc, f->buffer, TAILLE);
			if (res <= 4096){
				//Mettre un champ pos max pour simuler une fin de fichier.
				f->pos = 0;
			} else {
				//printf("Erreur relecture.\n");
			}
		}
		//On place en memoire (pointé par l'utilisateur) la lecture demandée.

		//*(char*)p = f->buffer[f->pos];
		//*(char*)p = *((char *)f->buffer)[f->pos];
		printf("buffer = %s\n",f->buffer+f->pos);
		strncpy((char*) p, f->buffer+f->pos, taille*nbelem);
		//memcpy ( p, f->buffer[f->pos], taille * nbelem);
		//On decale la posotion de lecture dans le buffer.
		printf("pos = %d\n", f->pos);
		f->pos += (taille * nbelem);
		return nbelem*taille;
		//Si l'utilisateur a lut tout le buffer on le recharge.
		//verifier position structure
	} else {
		return -1;
	}
}

/* ECRIRE */
//return le nombre d'octets ecrit.
int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f) {
	printf("fonction ecrire\n");
	if (f->mode == 'E'){
		printf("mode ok E\n");
		//On est dans le bon mode.
		//Ajout dans le buffer du contenu pointé par p.
		//strcpy(f->buffer[f->pos+1],p);
		strncpy(f->buffer+f->pos, (char*) p, taille*nbelem);
		//memcpy ( f->buffer, p, taille * nbelem);
		//Verification du remplissage du buffer.
		f->pos += (taille * nbelem);
		if (f->pos >= 4096){
			//Si buffer rempli, on ecrit réellement sur le disque.
			write(f->filedesc, f->buffer, 4096);
			//Replacement de la variable position au debut.
			f->pos = 0;
			//Verifier le debordement du buffer.

			//Return de la taille ecrite dans le buffer.
		}
		printf("return %d \n", taille*nbelem);
		return (taille * nbelem);
	}
	return -1;
}