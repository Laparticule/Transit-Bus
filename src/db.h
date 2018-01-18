/**
 * \file db.c
 * \author Audrey Muratore
 * \date 2 Janvier  2018
 * \brief Fichier header de db.c
 * \details Regroupe les prototypes, defines et structures de donnees relatives aux fonctions permettant de stocker en memoire et reecrire la base de donnee.
*/

#ifndef _DB_H_
#define _DB_H_
#include "llist.h"
#include "csv.h"

/**
 * \struct s_lines
 * \brief Definition d'une structure de donnee permettant de stocker en memoire les lignes de bus.
*/
/**
 * \var typedef struct s_lines {int num; llist *directions;} lines
 * \brief un type permettant de manipuler plus facilement la structure s_lines.
*/
typedef struct s_lines
{
	int num; /**< Entier permettant de stocker le numero d'une ligne*/
	llist *directions; /**< Pointeur sur le type llist. directions est une liste chainee des directions possibles pour une ligne.*/
} lines;

/**
 * \struct s_destinations
 * \brief Definition d'une structure de donnee permettant de stocker en memoire les destinations possibles d'une ligne.
*/
/**
 * \var typedef s_destinations {char name[COL_SIZE]; llist *stops;} destinations.
 * \brief Un type permettant de manipuler plus facilement la structure s_destinations.
*/
typedef struct s_destinations
{
	char name[COL_SIZE]; /**< Chaine de caracteres permettant de stocker le nom d'une destination.*/
	llist *stops; /**< Pointeur sur le type llist. stops est une lsite chainee d'arrets de bus.*/
} destinations;

/**
 * \struct s_stops
 * \brief Definition d'une structure de donnee permettant de stocker en memoire les arrets de bus pour une  destination donnee.
*/
/**
 * \var typedef struct s_stops {char name[COL_SIZE]; llist *schedule;} stops
 * \brief Un type permettant de manipuler plus facilement la structure s_stops
*/
typedef struct s_stops
{
	char name[COL_SIZE]; /**< Chaine de caractere permettant de stocker le nom d'un arret.*/
	llist *schedule; /**< Pointeur sur le type llist. schedule est une liste chainee d'horaires.*/
} stops;

/**
 * \struct s_schedule
 * \brief Definition d'une structure de donnee permettant de stocker en memoire les horaires d'un arret.
*/
/**
 * \var typedef struct s_schedule {char time[5];} schedule
 * \brief Un type permettant de manipuler plus facilement la structure s_schedule.
*/
typedef struct s_schedule
{
	char time[6]; /**< Chaine de caractere permettant de stocker un horaire.*/
} schedule;


/**
 * \fn llist* read_db (void)
 * \brief Stocke l'ensemble de la base de donnee (fichiers .csv) dans une structure en memoire.
 * \details Dans le principe, cette fonction lit un fichier main.csv et en deduit les destinations possibles pour chaque ligne. Ensuite, on lit des fichiers .csv (format numligne_nom_de_la_dest.csv) et les stocke en memoire.
 * \return Renvoit un pointeur sur llist. A partir de ce pointeur, il est possible de retrouver l'ensemble des donnees de la TCAT (lignes, destinations associees, arrets, horaires).
*/
llist* read_db(void);

/**
 * \fn void write_db (llist *l)
 * \brief Ecrase et reecrit l'ensemble des donnees en memoire dans des fichiers au format .csv.
 * \details Dans le principe, cette fonction ecrit un fichier main.csv resumant les lignes et leurs destinations possibles. Ensuite, on ecrit les fichiers .csv (format numligne_nom_de_la_dest.csv) independamment en fonction des arrets et de leurs horaires.
 * \param l est un pointeur sur llist. A partir de ce pointeur, il est possible de retrouver l'ensemble des donnees de la TCAT (lignes, destinations associees, arrets, horaires) stockees en memoire.
*/
void write_db(llist *l);

/**
 * \fn char* space_to_underscore(char *str)
 * \brief Fonction remplaçant les espaces par des underscores
 * \details Aide notamment a formater le nom des fichiers csv en utilisant le nom de la destination (qui comprend des espaces).
 * \param str est une chaine de caracteres pouvant contenir des espaces
 * \return Renvoit une chaine de caractere ne comprenant aucun espace
*/
char* space_to_underscore(char* str);

/**
 * \fn check_line_existence(llist *l, int nb_line)
 * \brief Fonction permettant de rechercher si une ligne de bus donnee existe en memoire.
 * \param l est un pointeur sur llist. Permet d'acceder a toutes les informations des lignes.
 * \param nb_line est le numero de la ligne recherchee
 * \return Renvoit 1 en cas de succes (ligne trouvee), 0 sinon
*/
int check_line_existence(llist *l, int nb_line);

/**
 * \fn check_dest_existence(llist *l, int nb_line, char *dest_name)
 * \brief Fonction permettant de rechercher si une destination existe pour une ligne donnee.
 * \param l est un pointeur sur llist. Permet d'acceder a toutes les informations des lignes.
 * \param nb_line est le numero de la ligne recherchee.
 * \param dest_name est le nom de la destination.
 * \return Renvoit 1 en cas de succes (destination trouvee), 0 sinon.
*/
int check_dest_existence(llist *l, int nb_line, char *dest_name);

/**
 * \fn check_stop_existence(llist *l, int nb_line, char *dest_name, char *stop_name)
 * \brief Fonction permettant de rechercher si un arret de bus donne existe pour une ligne et une direction donnee.
 * \param l est un pointeur sur llist. Permet d'acceder a toutes les informations des lignes.
 * \param nb_line est le numero de la ligne recherchee.
 * \param dest_name est le nom de la destination.
 * \param stop_name est le nom de l'arret de bus.
 * \return Renvoit 1 en cas de succes (arret trouve), 0 sinon.
*/
int check_stop_existence(llist *l, int nb_line, char *dest_name, char *stop_name);

/**
 * \fn check_sched_existence(llist *l, int nb_line, char *dest_name, char *stop_name, char *sched)
 * \brief Fonction permettant de rechercher si un horaire precis existe pour un arret donne d'une ligne.
 * \param l est un pointeur sur llist. Permet d'acceder a toutes les informations des lignes.
 * \param nb_line est le numero de la ligne recherchee.
 * \param dest_name est le nom de la destination.
 * \param stop_name est le nom de l'arret de bus.
 * \param sched est un horaire.
 * \return Renvoit 1 en cas de succes (horaire trouve), 0 sinon.
*/
int check_sched_existence(llist *l, int nb_line, char *dest_name, char *stop_name, char *sched);

/**
 * \fn void update_sched(llist *l, int nb_line, char *dest_name, char *stop_name, char *oldsched, char *newsched)
 * \brief Fonction modifiant un horaire donne dans la base de donnee en memoire.
 * \param l est un pointeur sur llist. Permet d'acceder a toutes les informations des lignes.
 * \param nb_line est le numero de la ligne recherchee.
 * \param dest_name est le nom de la destination.
 * \param stop_name est le nom de l'arret de bus.
 * \param oldsched correspond a l'horaire a modifier.
 * \param newsched correspond au nouvel horaire.
*/
void update_sched_db(llist *l, int nb_line, char *dest_name, char *stop_name, char *oldsched, char *newsched);

/**
 * \fn llist* delete_stop_db(llist *l, int nb_line, char *dest_name, char *stop_name);
 * \brief Fonction supprimant un arret dans la base de donnee
 * \param l est un pointeur sur llist. Permet d'acceder a toutes les informations des lignes.
 * \param nb_line est le numero de la ligne recherchee
 * \param dest_name est le nom de la destination
 * \param stop_name est le nom de l'arret de bus
 * \return Pointeur sur llist.
*/
llist* delete_stop_db(llist *l, int nb_line, char *dest_name, char *stop_name);

/**
 * \fn void update_stop_db(llist *l, int nb_line, char *old_stop_name, char *new_stop_name)
 * \brief Fonction permettant de renommer un arret pour toutes les destinations
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param old_stop_name est l'ancien nom de l'arret
 * \param new_stop_name est le nouveau nom de l'arret
*/
void update_stop_db(llist *l, char *old_stop_name, char *new_stop_name);

/**
 * \fn llist* delete_dest_db(llist *l, int nb_line, char *dest_name);
 * \brief Fonction permettant de supprimer une destination donnee
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param dest_name est le nom de la destination
 * \return Renvoit un pointeur llist
*/
llist* delete_dest_db(llist *l, int nb_line, char *dest_name);

/**
 * \fn llist* delete_line_db(llist *l, int nb_line);
 * \brief Fonction permettant de supprimer une ligne de la base de donnee
 * \param l est un pointeur sur llist.
 * \param nb_line est le numero de la ligne
 * \return Renvoit un pointeur sur llist.
*/
llist* delete_line_db(llist *l, int nb_line);

/**
 * \fn void update_line_db(llist *l, int old_line, int new_line);
 * \brief Fonction permettant de modifier une ligne
 * \param l est un pointeur sur llist
 * \param old_line est l'ancien nom de la ligne
 * \param new_line est le nouveau nom que l'on veut donner a la ligne
*/
void update_line_db(llist *l, int old_line, int new_line);

/**
 * \fn void update_dest_db(llist *l, int nb_line, char *old_name, char *new_name);
 * \brief Fonction permettant de modifier le nom d'une destination
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param old_name est l'ancien nom de la destination
 * \param new_name est le nouveau nom que l'on veut doner a la destination
*/
void update_dest_db(llist *l, int nb_line, char *old_name, char *new_name);

/**
 * \fn void add_line_db(llist *l, int nb_line);
 * \brief Fonction permettant d'ajouter une nouvelle ligne
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
*/
void add_line_db(llist *l, int nb_line);

/**
 * \fn llist* add_dest_db(llist *l, int nb_line, char *dest_name);
 * \brief Fonction permettant d'ajouter une nouvelle destination
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param dest_name est le nom de la destination
 * \return Renvoit un pointeur sur llist
*/
llist* add_dest_db(llist *l, int nb_line, char *dest_name);

/**
 * \fn llist* add_stop_db(llist *l, int nb_line, char *dest_name, char *stop_name);
 * \brief Fonction permettant d'ajouter un nouvel arret
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param dest_name est le nom de la destination
 * \param stop_name est le nom de l'arret
 * \return Renvoit un pointeur sur llist
*/
llist* add_stop_db(llist *l, int nb_line, char *dest_name, char *stop_name);

/**
 * \fn llist* add_sched_db(llist *l, int nb_line, char *dest_name, char *stop_name, char *new_sched);
 * \brief Fonction permettant d'ajouter un horaire
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param dest_name est le nom de la destination
 * \param stop_name est le nom de l'arret
 * \return renvoit un pointeur sur llist.
*/
llist* add_sched_db(llist *l, int nb_line, char *dest_name, char *stop_name, char *new_sched);

/**
 * \fn llist* delete_sched_db(llist *l, int nb_line, char *dest_name, char *stop_name, char *sched);
 * \brief Fonction permettant de supprimer un horaire
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param dest_name est le nom de la destination
 * \param stop_name est le nom de l'arret
 * \return Renvoit un pointeur sur llist
*/
llist* delete_sched_db(llist *l, int nb_line, char *dest_name, char *stop_name, char *sched);

#endif
