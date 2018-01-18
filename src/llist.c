/**
 * \file llist.c
 * \author Audrey Muratore
 * \date 2 Janvier 2018
 * \brief Regroupe les fonctions relatives a la liste chainee generique
**/

#include <stdlib.h>
#include <stdio.h>
#include "llist.h"

/**
 * \fn _add_element(llist *l, void *ptr)
 * \brief Fonction permettant d'ajouter un element pointe par ptr a la fin d'une liste chainee l.
 * \param l est une liste chainee de type llist*.
 * \param ptr est un pointeur sur un element ou structure.
 * \return Renvoit la nouvelle liste.
*/
llist* _add_element(llist *l, void *ptr)
{
    llist *newchild = malloc(sizeof(llist));
    newchild->next = NULL;
    newchild->data = ptr;
    if (l == NULL)
    {
        return newchild;
    }
    else
    {
        llist *tmp = l;
        while (tmp->next != NULL)
        {
            tmp =  tmp->next;
        }
        tmp->next = newchild;
        return l;
    }
}

/**
 * \fn _delete_element(llist *l, void *ptr)
 * \brief Fonction permettant de supprimer un element pointe par ptr dans une liste chainee l.
 * \param l est une liste chainee de type llist*.
 * \param ptr est un pointeur sur un element ou structure.
 * \return Renvoit la nouvelle liste chainee.
*/
llist* _delete_element(llist *l, void *ptr)
{
	if (ptr == NULL)
		printf("Erreur: Impossible de supprimer l'element. L'element n'existe pas\n");
    if (l->data == ptr)
    {
        return l->next;
    }
    else
    {
        llist *tmp = l;
        while (tmp->next->data != ptr)
        {
            tmp = tmp->next;
        }
        free(tmp->next);
        tmp->next = tmp->next->next;
    }
    return l;
}

