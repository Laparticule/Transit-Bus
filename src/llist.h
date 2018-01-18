/**
 * \file llist.h
 * \author Audrey Muratore
 * \date 2 janvier 2018
 * \brief Fichier header de llist.c
 * \details Regroupe les prototypes, defines et structures de donnees relatives a la liste chainee generique
*/

#ifndef _LLIST_H_
#define _LLIST_H_

/**
 * \def add_element(l, ptr)
 * \brief Une macro simplifiant la syntaxe de la fonction _add_element(l, ptr)
*/
#define add_element(l, ptr) l = _add_element(l, ptr)
/**
 * \def delete_element(l, ptr)
 * \brief Une macro simplifiant la syntaxe de la fonction _delete_element(l, ptr)
*/
#define delete_element(l, ptr) l = _delete_element(l, ptr)
/**
 * \def for_each_list(l, tmp) for(llist *tmp = l; tmp != NULL; tmp = tmp->next
 * \brief Une macro simplifiant les boucles de lecture de donnee. l est la liste chainee que l'on souhaite parcourir, tmp est une variable temporaire.
*/
#define for_each_list(l, tmp) for(llist *tmp = l; tmp != NULL; tmp = tmp->next)

/**
 * \struct s_list
 * \brief Definition d'une structure generique pour les listes chainees
*/
/**
 * \var typedef struct s_list {void *data; struct s_list *next;} llist
 * \brief Un type permettant de manipuler plus facilement la structure s_list
*/
typedef struct s_list
{
	void *data; /**< Pointeur vers n'importe quel objet. Il peut notamment servir a pointer vers des structures specifiques a chaque liste.*/
	struct s_list *next; /**< Pointeur vers le prochain element de llist*/
} llist;

/**
 * \fn _add_element(llist *l, void *ptr)
 * \brief Fonction permettant d'ajouter un element pointe par ptr a la fin d'une liste chainee l.
 * \param l est une liste chainee de type llist*.
 * \param ptr est un pointeur sur un element ou structure.
 * \return Renvoit la nouvelle liste.
*/
llist* _add_element(llist *l, void *ptr);

/**
 * \fn _delete_element(llist *l, void *ptr)
 * \brief Fonction permettant de supprimer un element pointe par ptr dans une liste chainee l.
 * \param l est une liste chainee de type llist*.
 * \param ptr est un pointeur sur un element ou structure.
 * \return Renvoit la nouvelle liste chainee.
*/
llist* _delete_element(llist *l, void *ptr);

#endif
