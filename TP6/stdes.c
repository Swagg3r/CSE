#include "stdes.h"
#include "errno.h"

#define TAILLE 4096
#define LECTURE O_RDONLY
#define ECRITURE O_WRONLY | O_CREATE
#define NB_FILE 15

/* STRUCTURE */
typedef struct _ES_FICHIER {
	int filedesc;
	char buffer[TAILLE];
	int pos;
	char mode;
	int index;
} FICHIER;

FICHIER tab_fichier[NB_FILE] = NULL;


/* OUVRIR */
// Ajout d'un struct + open
//Tester si le nom existe deja ou pas.
FICHIER* ouvrir(char *nom, char mode){
	FICHIER *f = malloc(sizeof(f));
	f->pos = 0;
	f->mode = mode;

	if (mode == 'L') {
		f->filedesc = open(nom, LECTURE);
	} else if (mode == 'E') {
		f->filedesc = open(nom, ECRITURE, 0777 );
	} else {
		return NULL;
	}

	int i = 0;
	while(tab_fichier[i] != NULL) {
		i++;
	}
	if (i == NB_FILE) {
		//plus de place dans le tableau
		//placer errno
		return NULL;
	} else {
		f->index = i;
		tab_fichier[i] = f;
	}

	return f;
}



/* FERMER */
// return 0 if success, -1 if error
// TODO Destruction de la struct + IS DONE close
int fermer(FICHIER* f) {
	//Ecrire dans le fichier si il reste des données en mode ecriture dans le buffer
	if (f.mode == 'E') {
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
	//Virer la struct du tableau.
	tab_fichier[f->index] = NULL;
	//Free struct
	free(f);

	return check_c;
}

/* LIRE */
// Buffer <- Read de taille buffer
int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f) {
	int res = read(f->filedesc, f->buffer, TAILLE);
	if (res == 0) {
		//fin de fichier atteinte
	} else if (res == -1) {
		//verifier errno
	}
	//verifier position structure
}

/* ECRIRE */
// Buffer <- elements.
int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f) {

}