/**
 * \file db.c
 * \author Audrey Muratore
 * \date 2 Janvier 2018
 * \brief Regroupe les fonctions permettant de stocker en memoire la base de donnee et de la reecrire entierement dans des fichiers csv.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "llist.h"
#include "csv.h"

/**
 * \fn llist* read_db (void)
 * \brief Stocke l'ensemble de la base de donnee (fichiers .csv) dans une structure en memoire.
 * \details Dans le principe, cette fonction lit un fichier main.csv et en deduit les destinations possibles pour chaque ligne. Ensuite, on lit des fichiers .csv (format numligne_nom_de_la_dest.csv) et les stocke en memoire.
 * \return Renvoit un pointeur sur llist. A partir de ce pointeur, il est possible de retrouver l'ensemble des donnees de la TCAT (lignes, destinations associees, arrets, horaires).
*/
llist* read_db (void)
{
	llist *all_lines = NULL;
	char *file_name = malloc(sizeof(char)*50);

	/* Lecture des differentes lignes et des directions possibles */
	llist *main_file = NULL;
	main_file = read_csv("csv/main.csv");
	if (main_file == NULL)
	{
		printf("ERREUR: Aucun fichier n'a pu etre lu\n");
		return NULL;
	}

	// Pour chaque ligne dans main_file
	for_each_list(main_file, tmp_main_file)
	{
		// Stocker les lignes et leurs colonnes
		row *tmprow = tmp_main_file->data;
		llist *tmpcol = tmprow->col;

		// Creer et initialiser une nouvelle ligne de bus
		lines *newline = malloc(sizeof(lines));
		newline->num = atoi(((col*)tmpcol->data)->cell);
		newline->directions = NULL;

		// Pour chaque colonne de la ligne creer une destination
		for_each_list(tmpcol->next, tmp_tmpcol)
		{
			col *temp = tmp_tmpcol->data;
			destinations *newdest = malloc(sizeof(destinations));
			strcpy(newdest->name, temp->cell);
			newdest->stops = NULL;

			// Lire fichier CSV associe
			sprintf(file_name, "csv/line-%d-%s.csv", newline->num, newdest->name);
			file_name = space_to_underscore(file_name);
			llist *current_file = NULL;
			current_file = read_csv(file_name);

			// Pour chaque ligne du fichier
			for_each_list(current_file, each_row)
			{
				// Creer arret avec la premiere colonne de la ligne
				row *tmp_row = each_row->data;
				llist *tmp_col = tmp_row->col;

				stops *newstop = malloc(sizeof(stops));
				strcpy(newstop->name, ((col*)tmp_col->data)->cell);
				newstop->schedule = NULL;

				// Pour toutes les autres colonnes de la ligne creer les horaires associes a l'arret
				for_each_list(tmp_col->next, each_col)
				{
					col *temp_col = each_col->data;
					schedule *newschedule = malloc(sizeof(schedule));
					memset(newschedule->time, 0, 6);
					strncpy(newschedule->time, temp_col->cell, 5);
					add_element(newstop->schedule, newschedule); /* Ajout des horaires */
				}
				add_element(newdest->stops, newstop); /* Ajout des stops */
			}
			add_element(newline->directions, newdest); /* Ajout des destinations possibles */
		}
		add_element(all_lines, newline); /* Ajout des lignes de bus */
	}
	return all_lines;
}


/**
 * \fn void write_db (llist *l)
 * \brief Ecrase et reecrit l'ensemble des donnees en memoire dans des fichiers au format .csv.
 * \details Dans le principe, cette fonction ecrit un fichier main.csv resumant les lignes et leurs destinations possibles. Ensuite, on ecrit les fichiers .csv (format numligne_nom_de_la_dest.csv) independamment en fonction des arrets et de leurs horaires.
 * \param l est un pointeur sur llist. A partir de ce pointeur, il est possible de retrouver l'ensemble des donnees de la TCAT (lignes, destinations associees, arrets, horaires) stockees en memoire.
*/
void write_db(llist *l)
{
	llist *all_lines = l;

	llist *main_file = NULL;
	llist *current_file = NULL;

	char *file_name = malloc(sizeof(char)*50);


	// Ecriture du fichier main.csv
	for_each_list(all_lines, each_line)
	{
		lines *newline = each_line->data;
		llist *all_destinations = newline->directions;

		llist *all_col = NULL;

		// Ecriture du numero de ligne
		col *tmpcol = malloc(sizeof(col));
		char busline[2]; sprintf(busline, "%d", newline->num);
		strcpy(tmpcol->cell, busline);
		add_element(all_col, tmpcol);

		// Ecriture des destinations
		for_each_list(all_destinations, each_dest)
		{
			destinations *newdest = each_dest->data;

			col *tmpcol = malloc(sizeof(col));
			strcpy(tmpcol->cell, newdest->name);
			add_element(all_col, tmpcol);
		}
		row *tmprow = malloc(sizeof(row));
	    tmprow->col = all_col;
		add_element(main_file, tmprow);
	}
	write_csv("csv/main.csv", main_file);

	// Ecriture des autres fichiers
	for_each_list(all_lines, each_line)
	{
		lines *newline = each_line->data;
		llist *all_destinations = newline->directions;

		// Pour chaque fichier
		for_each_list(all_destinations, each_dest)
		{
			destinations *newdest = each_dest->data;
			llist *all_stops = newdest->stops;

			current_file = NULL;

			// Pour chaque ligne
			for_each_list(all_stops, each_stop)
			{
				stops *newstop = each_stop->data;
				llist *all_schedule = newstop->schedule;

				llist *all_col = NULL;

				// Ecrire l'arret
				col *tmpcol = malloc(sizeof(col));
				strcpy(tmpcol->cell, newstop->name);
				add_element(all_col, tmpcol);
				// Ecrire les horaires
				for_each_list(all_schedule, each_schedule)
				{
					schedule *newschedule = each_schedule->data;

					col *tmpcol = malloc(sizeof(col));
					strcpy(tmpcol->cell, newschedule->time);
					add_element(all_col, tmpcol);
				}
				row *tmprow = malloc(sizeof(row));
	   			tmprow->col = all_col;
				add_element(current_file, tmprow);
			}

			sprintf(file_name, "csv/line-%d-%s.csv", newline->num, newdest->name);
			file_name = space_to_underscore(file_name);
			write_csv(file_name, current_file);
		}
	}
}

/**
 * \fn char* space_to_underscore(char *str)
 * \brief Fonction remplaçant les espaces par des underscores
 * \details Aide notamment a formater le nom des fichiers csv en utilisant le nom de la destination (qui comprend des espaces).
 * \param str est une chaine de caracteres pouvant contenir des espaces
 * \return Renvoit une chaine de caractere ne comprenant aucun espace
*/
char* space_to_underscore(char *str)
{
	int i = 0;
	while(str[i])
	{
		if (str[i] == ' ')
		{
			str[i] = '_';
		}
		i++;
	}
	return str;
}

/**
 * \fn check_line_existence(llist *l, int nb_line)
 * \brief Fonction permettant de rechercher si une ligne de bus donnee existe en memoire.
 * \param l est un pointeur sur llist. Permet d'acceder a toutes les informations des lignes.
 * \param nb_line est le numero de la ligne recherchee
 * \return Renvoit 1 en cas de succes (ligne trouvee), 0 sinon
*/
int check_line_existence(llist *l, int nb_line)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			return 1;
		}
	}
	return 0;
}

/**
 * \fn check_dest_existence(llist *l, int nb_line, char *dest_name)
 * \brief Fonction permettant de rechercher si une destination existe pour une ligne donnee.
 * \param l est un pointeur sur llist. Permet d'acceder a toutes les informations des lignes.
 * \param nb_line est le numero de la ligne recherchee.
 * \param dest_name est le nom de la destination.
 * \return Renvoit 1 en cas de succes (destination trouvee), 0 sinon.
*/
int check_dest_existence(llist *l, int nb_line, char *dest_name)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			llist *all_destinations = tmpline->directions;
			for_each_list(all_destinations, tmp_all_dest)
			{
				destinations *tmpdest = tmp_all_dest->data;
				if (strcmp(tmpdest->name, dest_name) ==0)
					return 1;
			}
		}
	}
	return 0;
}


/**
 * \fn check_stop_existence(llist *l, int nb_line, char *dest_name, char *stop_name)
 * \brief Fonction permettant de rechercher si un arret de bus donne existe pour une ligne et une direction donnee.
 * \param l est un pointeur sur llist. Permet d'acceder a toutes les informations des lignes.
 * \param nb_line est le numero de la ligne recherchee.
 * \param dest_name est le nom de la destination.
 * \param stop_name est le nom de l'arret de bus.
 * \return Renvoit 1 en cas de succes (arret trouve), 0 sinon.
*/
int check_stop_existence(llist *l, int nb_line, char *dest_name, char *stop_name)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			llist *all_destinations = tmpline->directions;
			for_each_list(all_destinations, tmp_all_dest)
			{
				destinations *tmpdest = tmp_all_dest->data;
				if (strcmp(tmpdest->name, dest_name) ==0)
				{
					llist *all_stops = tmpdest->stops;
					for_each_list(all_stops, tmp_all_stops)
					{
						stops *tmpstop = tmp_all_stops->data;
						if (strcmp(tmpstop->name, stop_name) == 0)
								return 1;
					}
				}
			}
		}
	}
	return 0;
}


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
int check_sched_existence(llist *l, int nb_line, char *dest_name, char *stop_name, char *sched)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			llist *all_destinations = tmpline->directions;
			for_each_list(all_destinations, tmp_all_dest)
			{
				destinations *tmpdest = tmp_all_dest->data;
				if (strcmp(tmpdest->name, dest_name) ==0)
				{
					llist *all_stops = tmpdest->stops;
					for_each_list(all_stops, tmp_all_stops)
					{
						stops *tmpstop = tmp_all_stops->data;
						if (strcmp(tmpstop->name, stop_name) == 0)
						{
							llist *all_schedule = tmpstop->schedule;
							for_each_list(all_schedule, tmp_all_schedule)
							{
								schedule *tmpschedule = tmp_all_schedule->data;
								if (strcmp(tmpschedule->time, sched) == 0)
								{
									return 1;
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

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
void update_sched_db(llist *l, int nb_line, char *dest_name, char *stop_name, char *oldsched, char *newsched)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			llist *all_destinations = tmpline->directions;
			for_each_list(all_destinations, tmp_all_dest)
			{
				destinations *tmpdest = tmp_all_dest->data;
				if (strcmp(tmpdest->name, dest_name) ==0)
				{
					llist *all_stops = tmpdest->stops;
					for_each_list(all_stops, tmp_all_stops)
					{
						stops *tmpstop = tmp_all_stops->data;
						if (strcmp(tmpstop->name, stop_name) == 0)
						{
							llist *all_schedule = tmpstop->schedule;
							for_each_list(all_schedule, tmp_all_schedule)
							{
								schedule *tmpschedule = tmp_all_schedule->data;
								if (strcmp(tmpschedule->time, oldsched) == 0)
								{
									strcpy(tmpschedule->time, newsched);
								}
							}
						}
					}
				}
			}
		}
	}
}

/**
 * \fn void update_stop_db(llist *l, int nb_line, char *old_stop_name, char *new_stop_name)
 * \brief Fonction permettant de renommer un arret pour toutes les destinations
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param old_stop_name est l'ancien nom de l'arret
 * \param new_stop_name est le nouveau nom de l'arret
*/
void update_stop_db(llist *l, char *old_stop_name, char *new_stop_name)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		llist *all_destinations = tmpline->directions;
		for_each_list(all_destinations, tmp_all_dest)
		{
			destinations *tmpdest = tmp_all_dest->data;
			llist *all_stops = tmpdest->stops;
			for_each_list(all_stops, tmp_all_stops)
			{
				stops *tmpstop = tmp_all_stops->data;
				if (strcmp(tmpstop->name, old_stop_name) == 0)
				{
					memset(tmpstop->name, 0, COL_SIZE);
					strcpy(tmpstop->name, new_stop_name);
				}
			}
		}
	}
}

/**
 * \fn llist* add_sched_db(llist *l, int nb_line, char *dest_name, char *stop_name, char *new_sched);
 * \brief Fonction permettant d'ajouter un horaire
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param dest_name est le nom de la destination
 * \param stop_name est le nom de l'arret
 * \return Renvoit un pointeur sur llist.
*/
llist* add_sched_db(llist *l, int nb_line, char *dest_name, char *stop_name, char *new_sched)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			llist *all_destinations = tmpline->directions;
			for_each_list(all_destinations, tmp_all_dest)
			{
				destinations *tmpdest = tmp_all_dest->data;
				if (strcmp(tmpdest->name, dest_name) ==0)
				{
					llist *all_stops = tmpdest->stops;
					for_each_list(all_stops, tmp_all_stops)
					{
						stops *tmpstop = tmp_all_stops->data;
						if (strcmp(tmpstop->name, stop_name) == 0)
						{
							llist *all_schedule = tmpstop->schedule;
							schedule *tmpschedule = malloc(sizeof(schedule));
							strcpy(tmpschedule->time, new_sched);
							add_element(all_schedule, tmpschedule);
							tmpstop->schedule = all_schedule;
						}
					}
					tmpdest->stops = all_stops;
				}
			}
			tmpline->directions = all_destinations;
		}
	}
	return l;
}


/**
 * \fn llist* delete_sched_db(llist *l, int nb_line, char *dest_name, char *stop_name, char *sched);
 * \brief Fonction permettant de supprimer un horaire
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param dest_name est le nom de la destination
 * \param stop_name est le nom de l'arret
 * \return Renvoit un pointeur sur llist.
*/
llist* delete_sched_db(llist *l, int nb_line, char *dest_name, char *stop_name, char *sched)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			llist *all_destinations = tmpline->directions;
			for_each_list(all_destinations, tmp_all_dest)
			{
				destinations *tmpdest = tmp_all_dest->data;
				if (strcmp(tmpdest->name, dest_name) ==0)
				{
					llist *all_stops = tmpdest->stops;
					for_each_list(all_stops, tmp_all_stops)
					{
						stops *tmpstop = tmp_all_stops->data;
						if (strcmp(tmpstop->name, stop_name) == 0)
						{
							llist *all_schedule = tmpstop->schedule;
							for_each_list(all_schedule, tmp_all_sched)
							{
								schedule *oldsched = tmp_all_sched->data;
								if (strcmp(oldsched->time, sched) == 0)
									delete_element(all_schedule, oldsched);
							}
							tmpstop->schedule = all_schedule;
						}
					}
					tmpdest->stops = all_stops;
				}
			}
			tmpline->directions = all_destinations;
		}
	}
	return l;
}

/**
 * \fn llist* delete_stop_db(llist *l, int nb_line, char *dename, char *stop_name);
 * \brief Fonction supprimant un arret dans la base de donnees. 
 * \param l est un pointeur sur llist.
 * \param nb_line est le numero de la ligne recherchee
 * \param dest_name est le nom de la destination
 * \param stop_name est le nom de l'arret de bus
 * \return Pointeur sur llist.
*/
llist* delete_stop_db(llist *l, int nb_line, char *dest_name, char *stop_name)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			llist *all_destinations = tmpline->directions;
			for_each_list(all_destinations, tmp_all_dest)
			{
				destinations *tmpdest = tmp_all_dest->data;
				if (strcmp(tmpdest->name, dest_name) ==0)
				{
					llist *all_stops = tmpdest->stops;
					for_each_list(all_stops, tmp_all_stops)
					{
						stops *tmpstop = tmp_all_stops->data;
						if (strcmp(tmpstop->name, stop_name) == 0)
						{
							delete_element(all_stops, tmpstop);
						}
					}
					tmpdest->stops = all_stops;
				}
			}
			tmpline->directions = all_destinations;
		}
	}
	return l;
}


/**
 * \fn llist* delete_dest_db(llist *l, int nb_line, char *dest_name);
 * \brief Fonction permettant de supprimer une destination donnee
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param dest_name est le nom de la destination
 * \return Renvoit un pointeur llist
*/
llist* delete_dest_db(llist *l, int nb_line, char *dest_name)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			llist *all_destinations = tmpline->directions;
			for_each_list(all_destinations, tmp_all_dest)
			{
				destinations *tmpdest = tmp_all_dest->data;
				if (strcmp(tmpdest->name, dest_name) ==0)
				{
					delete_element(all_destinations, tmpdest);
				}
			}
			tmpline->directions = all_destinations;
		}
	}
	return l;
}

/**
 * \fn llist* delete_line_db(llist *l, int nb_line);
 * \brief Fonction permettant de supprimer une ligne de la base de donnee
 * \param l est un pointeur sur llist.
 * \param nb_line est le numero de la ligne
 * \return Renvoit un pointeur sur llist.
*/
llist* delete_line_db(llist *l, int nb_line)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			delete_element(l, tmpline);
			return l;
		}
	}
}

void update_line_db(llist *l, int old_line, int new_line)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == old_line)
		{
			tmpline->num = new_line;
		}
	}
}

void update_dest_db(llist *l, int nb_line, char *old_name, char *new_name)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			llist *all_destinations = tmpline->directions;
			for_each_list(all_destinations, tmp_all_dest)
			{
				destinations *tmpdest = tmp_all_dest->data;
				if (strcmp(tmpdest->name, old_name) ==0)
				{
					strcpy(tmpdest->name, new_name);
				}
			}
		}
	}
}


/**
 * \fn llist* add_dest_db(llist *l, int nb_line, char *dest_name);
 * \brief Fonction permettant d'ajouter une nouvelle destination
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param dest_name est le nom de la destination
 * \return Renvoit un pointeur sur llist
*/
llist* add_dest_db(llist *l, int nb_line, char *dest_name)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			llist *all_destinations = tmpline->directions;
			destinations *tmpdest = malloc(sizeof(destinations));
			strcpy(tmpdest->name, dest_name);
			tmpdest->stops = NULL;
			add_element(all_destinations, tmpdest);
			tmpline->directions = all_destinations;
		}
	}
	return l;
}

/**
 * \fn void add_line_db(llist *l, int nb_line);
 * \brief Fonction permettant d'ajouter une nouvelle ligne
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
*/
void add_line_db(llist *l, int nb_line)
{
	lines *newline = malloc(sizeof(lines));

	newline->num = nb_line;
	newline->directions = NULL;

	add_element(l, newline);
}

/**
 * \fn llist* add_stop_db(llist *l, int nb_line, char *dest_name, char *stop_name);
 * \brief Fonction permettant d'ajouter un nouvel arret
 * \param l est un pointeur sur llist
 * \param nb_line est le numero de la ligne
 * \param dest_name est le nom de la destination
 * \param stop_name est le nom de l'arret
 * \return Renvoit un pointeur sur llist
*/
llist* add_stop_db(llist *l, int nb_line, char *dest_name, char *stop_name)
{
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;
		if (tmpline->num == nb_line)
		{
			llist *all_destinations = tmpline->directions;
			for_each_list(all_destinations, tmp_all_dest)
			{
				destinations *tmpdest = tmp_all_dest->data;
				if (strcmp(tmpdest->name, dest_name) ==0)
				{
					llist *all_stops = tmpdest->stops;

					stops *tmpstop = malloc(sizeof(stops));
					strcpy(tmpstop->name, stop_name);
					tmpstop->schedule = NULL;

					add_element(all_stops, tmpstop);

					tmpdest->stops = all_stops;
				}
			}
			tmpline->directions = all_destinations;
		}
	}
	return l;
}


