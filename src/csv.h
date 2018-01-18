/**
 * \file csv.h
 * \author Audrey Muratore
 * \date 2 janvier 2018
 * \brief Fichier header de csv.c
 * \details Regroupe les prototypes, defines et structures de donnees relatives a la liste chainee generique
*/

#ifndef _CSV_H_
#define _CSV_H_
#include "llist.h"

/**
 * \def LINE_SIZE
 * \brief Defini la taille maximale d'une ligne pouvant être lue dans un fichier
*/
#define LINE_SIZE 4096

/**
 * \def ROW_SIZE
 * \brief Defini la taille maximale d'une colonne pouvant être lue dans un fichier
*/
#define COL_SIZE 64

/**
 * \struct s_row
 * \brief Definition d'une structure de donnee permettant de stocker en memoire les lignes d'un fichier.
*/
/**
 * \var typedef struct s_row {llist *col;} llist
 * \brief Un type permettant de manipuler plus facilement la structure s_row
*/
typedef struct s_row
{
	llist *col; /**< Pointeur sur le type llist. col est une liste chainee de colonnes.*/
} row;

/**
 * \struct s_col
 * \brief Definition d'une structure de donnee permettant de stocker en memoire les colonnes issues d'une ligne isolee.
*/
/**
 * \var typedef struct s_col {char cell[COL_SIZE];} col
 * \brief Un type permettant de manipuler plus facilement la structure s_col.
*/
typedef struct s_col
{
	char cell[COL_SIZE]; /**< Chaine de caractere permettant de stocker une cellule du fichier csv.*/
} col;

/**
 * \fn llist* read_csv (char *name)
 * \brief Fonction permettant de stocker les donnees d'un fichier CSV dans une liste chainee. La liste chainee cree est de type llist et prend en compte les lignes et les colonnes du fichier source.
 * \param name est une chaine de caractere indiquant le fichier de lecture.
 * \return Renvoit un pointeur sur le type llist permettant d'acceder a l'ensemble des donnees de la liste chainee.
*/
llist* read_csv(char *name);



/**
 * \fn void write_csv (char *name, llist *tab)
 * \brief Fonction permettant d'ecrire les donnees d'une liste chainee dans un fichier au format CSV. La liste chainee envoyee est de type llist et utilise necessairement les structures col et row comme donnees.
 * \param name est une chaine de caractere indiquant le fichier de lecture.
 * \param tab est un pointeur sur le type llist permettant d'acceder a l'ensemble des donnees de la liste chainee.
*/
void write_csv(char *name, llist *tab);

#endif
