/**
 * \file main.c
 * \author Audrey Muratore
 * \date 2 Janvier 2018
 * \brief Fichier principal du projet. Regroupe les fonctions relatives a l'interface et a l'affichage.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "llist.h"
#include "db.h"
#include "csv.h"

/**
 * \def color(param) printf("\033[%sm",param)
 * \brief Une macro pour changer la couleur de l'affichage
*/
#define color(param) printf("\033[%sm",param)
/**
 * \def clean_buffer while (getchar() !) '\n');
 * \brief Une macro permettant d'effacer le '\n' du buffer pour eviter qu'il ne soit lu par la suite
*/
#define clean_buffer while (getchar() != '\n')
/**
 * \def scanff(str) scanf("%[^\n]%*c", str)
 * \brief Permet de lire une chaine de caractere comportant des espcaces contrairement au %s classique.
*/
#define scanff(str) scanf("%[^\n]%*c", str)
/**
 * \def user_menu printf("1 - Lister les prochains bus pour mon arret\n2 - Consulter les horaires d'un arret\n3 - Consulter les horaires d'une ligne\n4 - Telecharger les horaires d'une ligne\n5 - Options avancees\n6 - Quitter\n")
 * \brief Affichage du menu pour utilisateur simple
*/
#define user_menu printf("1 - Lister les prochains bus pour mon arret\n2 - Consulter les horaires d'un arret\n3 - Consulter les horaires d'une ligne\n4 - Telecharger les horaires d'une ligne\n5 - Options avancees\n6 - Quitter\n\nVotre choix: ")
/**
 * \def admin_menu printf("1 - Creer une ligne\n2 - Modifier une ligne\n3 - Supprimer une ligne\n4 - Creer une destination\n5 - Modifier une destination\n6 - Supprimer une destination\n7 - Ajouter un arret\n8 - Modifier un arret\n9 - Supprimer un arret\n10 - Ajouter un horaire\n11 - Modifier un horaire\n12 - Supprimer un horaire\n13 - Charger la configuration d'une ligne\n14 - Quitter le mode administrateur\n")
 * \brief Affichage du menu pour administrateur
*/
#define admin_menu printf("1 - Creer une ligne\n2 - Modifier une ligne\n3 - Supprimer une ligne\n4 - Creer une destination\n5 - Modifier une destination\n6 - Supprimer une destination\n7 - Ajouter un arret\n8 - Modifier un arret\n9 - Supprimer un arret\n10 - Ajouter un horaire\n11 - Modifier un horaire\n12 - Supprimer un horaire\n13 - Charger la configuration d'une ligne\n14 - Quitter le mode administrateur\n\nVotre choix: ")

void print_line(llist *l, int nb, char dest_name[]);
void print_stop(llist *l, char stop_name[]);
void print_stop_next_sched(llist *l, char stop_name[]);

void download_line(llist *l, int nb, char *dest_name, char *file_name);
void load_line(llist *l);

void update_sched(llist *l);
void add_sched(llist *l);
void delete_sched(llist *l);

void delete_stop(llist *l);
void add_stop(llist *l);
void update_stop(llist *l);

void delete_dest(llist *l);
void add_dest(llist *l);
void update_dest(llist *l);

void delete_line(llist *l);
void add_line(llist *l);
void update_line(llist *l);


/**
 * \fn int main(int argc, char *argv[])
 * \brief Fonction
 * \param argc est un entier comptant le nombre d'arguments
 * \param argv est un tableau de caractere listant les arguments
 * \return Renvoit EXIT_SUCCESS ou EXIT_FAILURE.
*/
int main(int argc, char *argv[])
{
	llist *all_data = read_db();

	int option_choice = 0;
	int admin_choice = 0;

	int nb_line;
	char stop_name[COL_SIZE];
	char dest_name[COL_SIZE];
	char file_name[COL_SIZE];
	char password[30];
	memset(password, 0, 30);
	memset(stop_name, 0, COL_SIZE);
	memset(dest_name, 0, COL_SIZE);
	memset(file_name, 0, COL_SIZE);

	printf("\nBienvenue sur l'application de la TCAT !\n\n");



	while (option_choice != 6)
	{
		color("36");
		user_menu;
		color("0");
		scanf("%d", &option_choice);
		clean_buffer;

		switch (option_choice)
		{
			case 1:
				printf("Lister les prochains bus pour mon arret\n\n");
				printf("Choix de l'arret de bus: ");
				scanff(stop_name);
				printf("\n");
				print_stop_next_sched(all_data, stop_name);
				break;
			case 2:
				printf("Consulter les horaires d'un arret\n\n");
				printf("Choix de l'arret de bus: ");
				scanff(stop_name);
				printf("\n");
				print_stop(all_data, stop_name);
				break;
			case 3:
				printf("Consulter les horaires d'une ligne\n\n");
				printf("Choix de la ligne: ");
				scanf("%d", &nb_line);
				clean_buffer;
				printf("Choix de la destination: ");
				scanff(dest_name);
				printf("\n");
				print_line(all_data, nb_line, dest_name);
				break;
			case 4:
				printf("Telecharger les horaires d'une ligne\n\n");
				printf("Choix de la ligne: ");
				scanf("%d", &nb_line);
				clean_buffer;
				printf("Choix de la destination: ");
				scanff(dest_name);
				printf("Ecrire dans le fichier: ");
				scanff(file_name);
				printf("\n");
				download_line(all_data, nb_line, dest_name, file_name);
				break;
			case 5:
				printf("Options avancees\n\n");
				admin_choice = 0;
				printf("Mot de passe administrateur: ");
				scanff(password);
				if (strcmp(password, "admin") == 0)
				{
				while (admin_choice != 14)
				{
					color("36");
					admin_menu;
					color("0");
					scanf("%d", &admin_choice);
					clean_buffer;

					switch(admin_choice)
					{
						case 1:
							printf("Creer une ligne\n\n");
							add_line(all_data);
							break;
						case 2:
							printf("Modifier une ligne\n\n");
							update_line(all_data);
							break;
						case 3:
							printf("Supprimer une ligne\n\n");
							delete_line(all_data);
							break;
						case 4:
							printf("Creer une destination\n\n");
							add_dest(all_data);
							break;
						case 5:
							printf("Modifier une destination\n\n");
							update_dest(all_data);
							break;
						case 6:
							printf("Supprimer une destination\n\n");
							delete_dest(all_data);
							break;
						case 7:
							printf("Ajouter un arret\n\n");
							add_stop(all_data);
							break;
						case 8:
							printf("Modifier un arret\n\n");
							update_stop(all_data);
							break;
						case 9:
							printf("Supprimer un arret\n\n");
							delete_stop(all_data);
							break;
						case 10:
							printf("Ajouter un horaire\n\n");
							add_sched(all_data);
							break;
						case 11:
							printf("Modifier un horaire\n\n");
							update_sched(all_data);
							break;
						case 12:
							printf("Supprimer un horaire\n\n");
							delete_sched(all_data);
							break;
						case 13:
							printf("Charger la configuration d'une ligne\n\n");
							load_line(all_data);
							break;
						case 14:
							break;
						default:
							printf("Choix non valide\n\n");
					}
					printf("\n");
				}
				break;
				}
				else
				{
				color("31");
				printf("Acces refuse\n");
				color("0");
				break;
				}
			case 6:
				printf("A bientot !\n");
				break;
			default:
				printf("Choix non valide\n");

		}
		printf("\n");
	}

	return 0;
}

/**
 * \fn void update_sched(llist *l)
 * \brief Fonction permettant de modifier un horaire a partir de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist. Contient toutes les donnees relatives aux lignes.
*/
void update_sched(llist *l)
{
	int nb_line;
	char *dest_name = malloc(COL_SIZE);
	char *stop_name = malloc(COL_SIZE);
	char *old_sched = malloc(6);
	char *new_sched = malloc(6);

	memset(dest_name, 0, COL_SIZE);
	memset(stop_name, 0, COL_SIZE);
	memset(old_sched, 0, 6);
	memset(new_sched, 0, 6);

	int i = 0;

	printf("Saisir la ligne pour laquelle vous voulez faire une modification: ");
	while (i == 0)
	{
		scanf("%d", &nb_line);
		clean_buffer;
		i = check_line_existence(l, nb_line);
		if (i == 0)
			printf("La ligne %d n'existe pas. Reessayez: ", nb_line);
	}

	i = 0;
	printf("Saisir sa destination: ");
	while (i == 0)
	{
		scanff(dest_name);
		i = check_dest_existence(l, nb_line, dest_name);
		if (i == 0)
			printf("La destination %s pour la liste %d n'existe pas. Reessayez: ", dest_name, nb_line);
	}

	i = 0;
	printf("Saisir l'arret: ");
	while (i == 0)
	{
		scanff(stop_name);
		i = check_stop_existence(l, nb_line, dest_name, stop_name);

		if (i == 0)
			printf("L'arret %s n'existe pas pour cette destination. Reessayez: ", stop_name);
	}

	i = 0;
	printf("Saisir l'horaire a modifier: ");
	while (i == 0)
	{
		scanff(old_sched);
		i = check_sched_existence(l, nb_line, dest_name, stop_name, old_sched);

		if (i == 0)
			printf("L'horaire %s n'existe pas pour cet arret. Reessayez: ", old_sched);
	}

	printf("Saisir le nouvel horaire: ");
	scanff(new_sched);

	update_sched_db(l, nb_line, dest_name, stop_name, old_sched, new_sched);

	write_db(l);
}

/**
 * \fn void add_sched(llist *l)
 * \brief Fonction permettant d'ajouter un horaire a partir de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist.
*/
void add_sched(llist *l)
{
	int nb_line;
	char *dest_name = malloc(COL_SIZE);
	char *stop_name = malloc(COL_SIZE);
	char *new_sched = malloc(6);

	memset(dest_name, 0, COL_SIZE);
	memset(stop_name, 0, COL_SIZE);
	memset(new_sched, 0, 6);

	int i = 0;

	printf("Saisir la ligne pour laquelle vous voulez faire une modification: ");
	while (i == 0)
	{
		scanf("%d", &nb_line);
		clean_buffer;
		i = check_line_existence(l, nb_line);
		if (i == 0)
			printf("La ligne %d n'existe pas. Reessayez: ", nb_line);
	}

	i = 0;
	printf("Saisir sa destination: ");
	while (i == 0)
	{
		scanff(dest_name);
		i = check_dest_existence(l, nb_line, dest_name);
		if (i == 0)
			printf("La destination %s pour la liste %d n'existe pas. Reessayez: ", dest_name, nb_line);
	}

	i = 0;
	printf("Saisir l'arret: ");
	while (i == 0)
	{
		scanff(stop_name);
		i = check_stop_existence(l, nb_line, dest_name, stop_name);

		if (i == 0)
			printf("L'arret %s n'existe pas pour cette destination. Reessayez: ", stop_name);
	}

	i = 1;
	printf("Saisir le nouvel horaire: ");
	while (i == 1)
	{
		scanff(new_sched);
		i = check_sched_existence(l, nb_line, dest_name, stop_name, new_sched);

		if (i == 1)
			printf("L'horaire %s existe deja pour cet arret. Reessayez: ", new_sched);
	}

	l = add_sched_db(l, nb_line, dest_name, stop_name, new_sched);

	write_db(l);
}

/**
 * \fn void delete_sched(llist *l)
 * \brief Fonction permettant de supprimer un horaire en fonction de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist.
*/
void delete_sched(llist *l)
{
	int nb_line;
	char *dest_name = malloc(COL_SIZE);
	char *stop_name = malloc(COL_SIZE);
	char *sched = malloc(6);

	memset(dest_name, 0, COL_SIZE);
	memset(stop_name, 0, COL_SIZE);
	memset(sched, 0, 6);

	int i = 0;

	printf("Saisir la ligne pour laquelle vous voulez faire une modification: ");
	while (i == 0)
	{
		scanf("%d", &nb_line);
		clean_buffer;
		i = check_line_existence(l, nb_line);
		if (i == 0)
			printf("La ligne %d n'existe pas. Reessayez: ", nb_line);
	}

	i = 0;
	printf("Saisir sa destination: ");
	while (i == 0)
	{
		scanff(dest_name);
		i = check_dest_existence(l, nb_line, dest_name);
		if (i == 0)
			printf("La destination %s pour la liste %d n'existe pas. Reessayez: ", dest_name, nb_line);
	}

	i = 0;
	printf("Saisir l'arret: ");
	while (i == 0)
	{
		scanff(stop_name);
		i = check_stop_existence(l, nb_line, dest_name, stop_name);

		if (i == 0)
			printf("L'arret %s n'existe pas pour cette destination. Reessayez: ", stop_name);
	}

	i = 0;
	printf("Saisir l'horaire a supprimer: ");
	while (i == 0)
	{
		scanff(sched);
		i = check_sched_existence(l, nb_line, dest_name, stop_name, sched);

		if (i == 0)
			printf("L'horaire %s n'existe pas. Reessayez: ", sched);
	}

	l = delete_sched_db(l, nb_line, dest_name, stop_name, sched);

	write_db(l);
}

/**
 * \fn void delete_stop(llist *l)
 * \brief Fonction permettant de supprimer un arret en fonction de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist
*/
void delete_stop(llist *l)
{
	int nb_line;
	char *dest_name = malloc(COL_SIZE);
	char *stop_name = malloc(COL_SIZE);

	memset(dest_name, 0, COL_SIZE);
	memset(stop_name, 0, COL_SIZE);

	int i = 0;

	printf("Saisir la ligne pour laquelle vous voulez faire une modification: ");
	while (i == 0)
	{
		scanf("%d", &nb_line);
		clean_buffer;
		i = check_line_existence(l, nb_line);
		if (i == 0)
			printf("La ligne %d n'existe pas. Reessayez: ", nb_line);
	}

	i = 0;
	printf("Saisir sa destination: ");
	while (i == 0)
	{
		scanff(dest_name);
		i = check_dest_existence(l, nb_line, dest_name);
		if (i == 0)
			printf("La destination %s pour la liste %d n'existe pas. Reessayez: ", dest_name, nb_line);
	}

	i = 0;
	printf("Saisir l'arret de bus a supprimer : ");
	while (i == 0)
	{
		scanff(stop_name);
		i = check_stop_existence(l, nb_line, dest_name, stop_name);

		if (i == 0)
			printf("L'arret %s n'existe pas pour cette destination. Reessayez: ", stop_name);
	}

	l = delete_stop_db(l, nb_line, dest_name, stop_name);

	write_db(l);
}

/**
 * \fn void delete_dest(llist *l)
 * \brief Fonction permettant de supprimer une destination en fonction de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist
*/
void delete_dest(llist *l)
{
	int nb_line;
	char *dest_name = malloc(COL_SIZE);

	memset(dest_name, 0, COL_SIZE);

	int i = 0;

	printf("Saisir la ligne pour laquelle vous voulez faire une modification: ");
	while (i == 0)
	{
		scanf("%d", &nb_line);
		clean_buffer;
		i = check_line_existence(l, nb_line);
		if (i == 0)
			printf("La ligne %d n'existe pas. Reessayez: ", nb_line);
	}

	i = 0;
	printf("Saisir la destination a supprimer: ");
	while (i == 0)
	{
		scanff(dest_name);
		i = check_dest_existence(l, nb_line, dest_name);
		if (i == 0)
			printf("La destination %s pour la liste %d n'existe pas. Reessayez: ", dest_name, nb_line);
	}

	l = delete_dest_db(l, nb_line, dest_name);

	write_db(l);
}

/**
 * \fn void delete_line(llist *l)
 * \brief Fonction permettant de supprimer une ligne en fonction de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist
*/
void delete_line(llist *l)
{
	int nb_line;
	int i = 0;

	printf("Saisir la ligne a supprimer: ");
	while (i == 0)
	{
		scanf("%d", &nb_line);
		clean_buffer;
		i = check_line_existence(l, nb_line);
		if (i == 0)
			printf("La ligne %d n'existe pas. Reessayez: ", nb_line);
	}

	l = delete_line_db(l, nb_line);

	write_db(l);

}

/**
 * \fn void update_line(llist *l)
 * \brief Fonction permettant de modifier le numero d'une ligne en fonction de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist
*/
void update_line(llist *l)
{
	int nb_line_old;
	int nb_line_new;

	int i = 0;
	printf("Saisir la ligne a modifier: ");
	while (i == 0)
	{
		scanf("%d", &nb_line_old);
		clean_buffer;
		i = check_line_existence(l, nb_line_old);
		if (i == 0)
			printf("La ligne %d n'existe pas. Reessayez: ", nb_line_old);;
	}

	i = 1;
	printf("Saisir le nouveau nom de la ligne: ");
	while (i == 1)
	{
		scanf("%d", &nb_line_new);
		clean_buffer;
		i = check_line_existence(l, nb_line_new);
		if (i == 1)
			printf("La ligne %d existe deja. Reessayez: ", nb_line_new);
	}

	update_line_db(l, nb_line_old, nb_line_new);

	write_db(l);

}

/**
 * \fn void update_dest(llist *l)
 * \brief Fonction permettant de modifier une destination en fonction de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist
*/
void update_dest(llist *l)
{
	int nb_line;
	char *old_name = malloc(COL_SIZE);
	char *new_name = malloc(COL_SIZE);

	memset(old_name, 0, COL_SIZE);
	memset(new_name, 0, COL_SIZE);

	int i = 0;
	printf("Saisir la ligne pour laquelle vous voulez faire une modification: ");
	while (i == 0)
	{
		scanf("%d", &nb_line);
		clean_buffer;
		i = check_line_existence(l, nb_line);
		if (i == 0)
			printf("La ligne %d n'existe pas. Reessayez: ", nb_line);
	}

	i = 0;
	printf("Saisir la destination a modifier: ");
	while (i == 0)
	{
		scanff(old_name);
		i = check_dest_existence(l, nb_line, old_name);
		if (i == 0)
			printf("La destination %s pour la liste %d n'existe pas. Reessayez: ", old_name, nb_line);
	}

	i = 1;
	printf("Saisir le nouveau nom de la destination: ");
	while (i == 1)
	{
		scanff(new_name);
		i = check_dest_existence(l, nb_line, new_name);
		if (i == 1)
			printf("La destination %s pour la liste %d existe deja. Reessayez: ", old_name, nb_line);
	}

	update_dest_db(l, nb_line, old_name, new_name);

	write_db(l);
}

/**
 * \fn void add_line(llist *l)
 * \brief Fonction permettant d'ajouter une ligne en fonction de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist
*/
void add_line(llist *l)
{
	int nb_line;

	int i = 1;
	printf("Saisir la nouvelle ligne: ");
	while (i == 1)
	{
		scanf("%d", &nb_line);
		clean_buffer;
		i = check_line_existence(l, nb_line);
		if (i == 1)
			printf("La ligne %d existe deja. Reessayez: ", nb_line);;
	}

	add_line_db(l, nb_line);

	write_db(l);
}

/**
 * \fn void add_dest(llist *l)
 * \brief Fonction permettant d'ajouter une destination en fonction de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist
*/
void add_dest(llist *l)
{
	int nb_line;
	char *dest_name = malloc(COL_SIZE);

	memset(dest_name, 0, COL_SIZE);

	int i = 0;

	printf("Saisir la ligne pour laquelle vous voulez faire une modification: ");
	while (i == 0)
	{
		scanf("%d", &nb_line);
		clean_buffer;
		i = check_line_existence(l, nb_line);
		if (i == 0)
			printf("La ligne %d n'existe pas. Reessayez: ", nb_line);
	}

	i = 1;
	printf("Saisir la nouvelle destination: ");
	while (i == 1)
	{
		scanff(dest_name);
		i = check_dest_existence(l, nb_line, dest_name);
		if (i == 1)
			printf("La destination %s pour la liste %d n'existe pas. Reessayez: ", dest_name, nb_line);
	}

	l = add_dest_db(l, nb_line, dest_name);

	write_db(l);
}

/**
 * \fn void add_stop(llist *l)
 * \brief Fonction permettant de supprimer un arret en fonction de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist
*/
void add_stop(llist *l)
{
	int nb_line;
	char *dest_name = malloc(COL_SIZE);
	char *stop_name = malloc(COL_SIZE);

	memset(dest_name, 0, COL_SIZE);
	memset(stop_name, 0, COL_SIZE);

	int i = 0;

	printf("Saisir la ligne pour laquelle vous voulez faire une modification: ");
	while (i == 0)
	{
		scanf("%d", &nb_line);
		clean_buffer;
		i = check_line_existence(l, nb_line);
		if (i == 0)
			printf("La ligne %d n'existe pas. Reessayez: ", nb_line);
	}

	i = 0;
	printf("Saisir sa destination: ");
	while (i == 0)
	{
		scanff(dest_name);
		i = check_dest_existence(l, nb_line, dest_name);
		if (i == 0)
			printf("La destination %s pour la liste %d n'existe pas. Reessayez: ", dest_name, nb_line);
	}

	i = 1;
	printf("Saisir le nouvel arret de bus: ");
	while (i == 1)
	{
		scanff(stop_name);
		i = check_stop_existence(l, nb_line, dest_name, stop_name);

		if (i == 1)
			printf("L'arret %s existe deja pour cette destination. Reessayez: ", stop_name);
	}

	l = add_stop_db(l, nb_line, dest_name, stop_name);

	write_db(l);
}


/**
 * \fn void update_stop(llist *l)
 * \brief Fonction permettant de modifier un arret en fonction de diverses informations demandees a l'utilisateur
 * \param l est un pointeur sur llist
*/
void update_stop(llist *l)
{
	int nb_line;
	char *old_stop_name = malloc(COL_SIZE);
	char *new_stop_name = malloc(COL_SIZE);

	memset(old_stop_name, 0, COL_SIZE);
	memset(new_stop_name, 0, COL_SIZE);

	printf("Saisir l'arret de bus a modifier: ");
	scanff(old_stop_name);

	printf("Saisir son nouveau nom: ");
	scanff(new_stop_name);

	update_stop_db(l, old_stop_name, new_stop_name);

	write_db(l);
}

/**
 * \fn print_line(llist *l, int nb, char dest_name[])
 * \brief Fonction permettant d'afficher tous les horaires d'une ligne (pour une destination de ligne donnee)
 * \param l est un pointeur sur llist permettant d'acceder a l'ensemble des donnees des lignes.
 * \param nb est le numero de la ligne recherchee
 * \param dest_name est le nom de la destination recherchee
*/
void print_line(llist *l, int nb, char dest_name[])
{
	short found = 0;

	// On parcourt chaque ligne
	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;

		// Si la ligne existe on continue
		if (tmpline->num == nb)
		{
			llist *all_destinations = tmpline->directions;

			for_each_list(all_destinations, tmp_all_dest)
			{
				destinations *tmpdest = tmp_all_dest->data;

				// Si la destination existe on continue
				if (strcmp(tmpdest->name, dest_name) == 0)
				{
					found = 1;
					llist *all_stops = tmpdest->stops;
					for_each_list(all_stops, tmp_all_stops)
					{
						stops *tmpstop = tmp_all_stops->data;
						color("34");
						printf("%s\n", tmpstop->name);
						color("0");
						llist *all_schedule = tmpstop->schedule;
						for_each_list(all_schedule, tmp_all_sched)
						{
							schedule *tmpsched = tmp_all_sched->data;
							printf("%s ", tmpsched->time);
						}
						printf("\n\n");
					}
				}
			}
		}
	}
	if (found == 0)
	{
		color("31");
		printf("Aucune destination associee a cette ligne n'a ete trouvee.\n");
		color("0");
	}
}

/**
 * \fn print_stop(llist *l, char stop_name[])
 * \brief Fonction permettant d'afficher tous les horaires des lignes pour un arret donne.
 * \param l est un pointeur sur llist permettant d'acceder a l'ensemble des donnees des lignes.
 * \param stop_name est le nom de l'arret de bus recherche
*/
void print_stop(llist *l, char stop_name[])
{
	short found = 0;

	// On parcourt chaque ligne
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
				if (strcmp(tmpstop->name, stop_name) == 0)
				{
					found = 1;
					color("34");
					printf("Ligne %d -> Direction %s\n", tmpline->num, tmpdest->name);
					color("0");
					llist *all_schedule = tmpstop->schedule;
					for_each_list(all_schedule, tmp_all_sched)
					{
						schedule *tmpsched = tmp_all_sched->data;
						printf("%s ", tmpsched->time);
					}
					printf("\n\n");
				}
			}
		}
	}
	if (found == 0)
	{
		color("31");
		printf("Aucune destination associee a cette ligne n'a ete trouvee.\n");
		color("0");
	}
}


/**
 * \fn print_stop_next_sched(llist *l, char *stop_name)
 * \brief Fonction permettant d'afficher le prochain arret pour un arret donne. La notion de prochain arret est calculee en fonction de l'heure locale.
 * \param l est un pointeur sur llist permettant d'acceder a l'ensemble des donnees des lignes.
 * \param stop_name est le nom de l'arret de bus
*/
void print_stop_next_sched(llist *l, char *stop_name)
{
	short found = 0;

	time_t current_time, closest_time;
	time(&current_time); // current_time prend la date actuelle
	closest_time = 0;

	char buff[20];

	struct tm *tme = localtime(&current_time);

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
				if (strcmp(tmpstop->name, stop_name) == 0)
				{
					found = 1;
					color("34");
					printf("Ligne %d -> Direction %s\n", tmpline->num, tmpdest->name);
					color("0");
					llist *all_schedule = tmpstop->schedule;
					closest_time = 0;
					for_each_list(all_schedule, tmp_all_sched)
					{
						schedule *tmpsched = tmp_all_sched->data;

						strptime(tmpsched->time, "%H:%M", tme);
						time_t sched_time = mktime(tme); // Timestamp de l'horaire considere

						if (sched_time < current_time) // Le bus est deja passe
							continue;

						if (closest_time == 0)
							closest_time = sched_time;

						if (sched_time < closest_time)
							closest_time = sched_time;
					}
					strftime(buff, 20, "%H:%M", localtime(&closest_time)); // Permet de convertir un Timestamp
					printf("prochain bus a %s\n", buff);

					printf("\n\n");
				}
			}
		}
	}
	if (found == 0)
	{
		color("31");
		printf("Aucune destination associee a cette ligne n'a ete trouvee.\n");
		color("0");
	}
}

/**
 * \fn void download_line(llist *l, int nb, char *dest_name, char *file_name)
 * \brief Fonction permettant de sauvegarder les informations d'une ligne pour une destination dans un fichier
 * \param l est un pointeur sur llist
 * \param dest_name est le nom de la destination
 * \param nb est le numero de la ligne
 * \param file_name est le nom du fichier
*/
void download_line(llist *l, int nb, char *dest_name, char *file_name)
{
	llist *file = NULL;

	for_each_list(l, tmp_l)
	{
		lines *tmpline = tmp_l->data;

		if (tmpline->num == nb)
		{
			llist *all_destinations = tmpline->directions;
			for_each_list(all_destinations, tmp_all_destinations)
			{
				destinations *tmpdest = tmp_all_destinations->data;
				if (strcmp(tmpdest->name, dest_name) == 0)
				{
					llist *all_stops = tmpdest->stops;
					file = NULL;

					// Pour chaque ligne du fichier
					for_each_list(all_stops, tmp_all_stops)
					{
						stops *tmpstop = tmp_all_stops->data;
						llist *all_schedule = tmpstop->schedule;

						llist *all_col = NULL;

						// Ecrire l'arret
						col *tmpcol = malloc(sizeof(col));
						strcpy(tmpcol->cell, tmpstop->name);
						add_element(all_col, tmpcol);

						// Ecrire les horaires
						for_each_list(all_schedule, tmp_all_sched)
						{
							schedule *newschedule = tmp_all_sched->data;
							col *tmpcol = malloc(sizeof(col));
							strcpy(tmpcol->cell, newschedule->time);
							add_element(all_col, tmpcol);
						}
						row *tmprow = malloc(sizeof(row));
						tmprow->col = all_col;
						add_element(file, tmprow);
					}
					write_csv(file_name, file);
				}
			}
		}
	}
}

/**
 * \fn void load_line(llist *l)
 * \brief Fonction permettant de charger les informations d'une ligne pour une destination a partir d'un fichier
 * \param l est un pointeur sur llist
*/
void load_line(llist *l)
{
	int nb_line;
	char *dest_name = malloc(COL_SIZE);
	char *file_name = malloc(COL_SIZE);

	memset(dest_name, 0, COL_SIZE);
	memset(file_name, 0, COL_SIZE);

	int i = 0;

	printf("Saisir la ligne pour laquelle vous voulez charger la configuration: ");
	while (i == 0)
	{
		scanf("%d", &nb_line);
		clean_buffer;
		i = check_line_existence(l, nb_line);
		if (i == 0)
			printf("La ligne %d n'existe pas. Reessayez: ", nb_line);
	}

	i = 0;
	printf("Saisir sa destination: ");
	while (i == 0)
	{
		scanff(dest_name);
		i = check_dest_existence(l, nb_line, dest_name);
		if (i == 0)
			printf("La destination %s pour la liste %d n'existe pas. Reessayez: ", dest_name, nb_line);
	}

	printf("Saisir le nom du fichier: ");
	scanff(file_name);


	llist *file = NULL;
	file = read_csv(file_name);

	llist *all_stops = NULL;
	for_each_list(file, tmp_file)
	{
		row *tmp_row = tmp_file->data;
		llist *tmp_col = tmp_row->col;

		stops *newstop  = malloc(sizeof(stops));
		strcpy(newstop->name, ((col*)tmp_col->data)->cell);
		newstop->schedule = NULL;

		for_each_list(tmp_col->next, each_col)
		{
			col *temp_col = each_col->data;
			schedule *newschedule = malloc(sizeof(schedule));
			memset(newschedule->time, 0, 6);
			strncpy(newschedule->time, temp_col->cell, 5);
			add_element(newstop->schedule, newschedule);
		}
		add_element(all_stops, newstop);
	}

	for_each_list(l, tmp_l)
	{
		lines *newline = tmp_l->data;
		if (newline->num == nb_line)
		{
			llist *all_dest = newline->directions;
			for_each_list(all_dest, tmp_all_dest)
			{
				destinations *tmp_dest = tmp_all_dest->data;
				if (strcmp(tmp_dest->name, dest_name) == 0)
				{
					tmp_dest->stops = all_stops;
				}
			}
		}
	}

	write_db(l);
}

