/**
 * \file csv.c
 * \author Audrey Muratore
 * \date 2 Janvier 2018
 * \brief Regroupe les fonctions relatives a la lecture et a l'ecriture des fichiers .csv
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "llist.h"

/**
 * \fn llist* read_csv (char *name)
 * \brief Fonction permettant de stocker les donnees d'un fichier CSV dans une liste chainee. La liste chainee cree est de type llist et prend en compte les lignes et les colonnes du fichier source.
 * \param name est une chaine de caractere indiquant le fichier de lecture.
 * \return Renvoit un pointeur sur le type llist permettant d'acceder a l'ensemble des donnees de la liste chainee.
*/
llist* read_csv(char *name)
{
	FILE *file = fopen(name, "r");

	if (file != NULL)
	{
		llist *l_row = NULL; /* Creation de la liste chainee des lignes */

		/* Buffer temporaire pour lire les lignes */
		char *full_line = malloc(LINE_SIZE);
		memset(full_line, 0, LINE_SIZE);

		/* Tant qu'il y a des lignes */
		while (fgets(full_line, LINE_SIZE, file) != NULL)
		{
			/* Creation de la structure de la ligne */
			row *newrow = malloc(sizeof(row));
			newrow->col = NULL; /* Creation de la liste chainee des colonnes */

			int i = 0, j = 0;
			char *full_cell = malloc(COL_SIZE);
			memset(full_cell, 0, COL_SIZE);

			/* Tant qu'on est pas a la fin du fichier ou la fin de la ligne */
			while  (full_line[i] != '\0' && full_line[i] != '\n' && full_line[i] != '\r')
			{
				if (full_line[i] == ',') /* La cellule est complete */
				{
					/* Creer la structure colonne et l'initialiser a zero, copier full_cell dedans et l'ajouter a la liste chainee */
					col *newcol = malloc(sizeof(col));
					memset(&(newcol->cell), 0, COL_SIZE);
					strcpy(newcol->cell, full_cell);
					add_element(newrow->col, newcol);

					/* Reinitialiser full_cell */
					memset(full_cell, 0, COL_SIZE);
					j = 0;
					i++;
				}
				else if (j != COL_SIZE)
				{
					full_cell[j] = full_line[i];
					j++;
					i++;
				}
			}
			/* Lire le dernier element de la ligne */
			col *newcol = malloc(sizeof(col));
			memset(newcol->cell, 0, COL_SIZE);
			strcpy(newcol->cell, full_cell);
			add_element(newrow->col, newcol);

			add_element(l_row, newrow); /* Ajouter la nouvelle ligne a la liste chainee */
		}

		fclose(file);
		return l_row;
	}
	else
	{
		printf("Impossible d'ouvrir le fichier\n");
		return NULL;
	}
}

/**
 * \fn void write_csv (char *name, llist *tab)
 * \brief Fonction permettant d'ecrire les donnees d'une liste chainee dans un fichier au format CSV. La liste chainee envoyee est de type llist et utilise necessairement les structures col et row comme donnees.
 * \param name est une chaine de caractere indiquant le fichier de lecture.
 * \param tab est un pointeur sur le type llist permettant d'acceder a l'ensemble des donnees de la liste chainee.
*/
void write_csv(char *name, llist *tab)
{
	FILE* file = fopen(name, "w+");
	if (file != NULL)
	{
		for_each_list(tab, tab_row) /* Pour chaque ligne */
		{
			for_each_list(((row*)(tab_row->data))->col, tab_col) /* Pour chaque colonne */
			{
				fputs(((col*)(tab_col->data))->cell, file);
				if (tab_col->next != NULL)
				{
					fputc(',', file);
				}
				else
				{
					fputc('\n', file);
				}
			}
		}
		fclose(file);
	}
	else
	{
		printf("Impossible d'ouvrir le fichier %s pour ecriture\n", name);
	}
}
