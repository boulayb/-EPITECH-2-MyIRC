/*
** list.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Fri May 20 15:59:41 2016 Arnaud Boulay
** Last update Sat May 21 17:47:46 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <unistd.h>
#include "myirc.h"

t_list		*create_list(void)
{
  t_list	*list;

  if ((list = malloc(sizeof(t_list))) == NULL)
    {
      my_putstr(STDERR_FILENO, ERR_MALLOC);
      return (NULL);
    }
  list->elem = NULL;
  list->back = list;
  list->next = list;
  return (list);
}

int		add_chain(t_list *list, void *elem)
{
  t_list	*new_chain;

  if (elem == NULL)
    {
      my_putstr(STDERR_FILENO, ERR_ELEM);
      return (RET_FAILURE);
    }
  else if ((new_chain = malloc(sizeof(t_list))) == NULL)
    {
      my_putstr(STDERR_FILENO, ERR_MALLOC);
      return (RET_FAILURE);
    }
  new_chain->elem = elem;
  new_chain->back = list->back;
  new_chain->next = list;
  list->back->next = new_chain;
  list->back = new_chain;
  return (RET_SUCCESS);
}

void		rm_chain(t_list *chain)
{
  chain->back->next = chain->next;
  chain->next->back = chain->back;
  free(chain);
}

void		rm_list(t_list *list)
{
  t_list	*chain;
  t_list	*tmp;

  chain = list->next;
  while (chain != list)
    {
      tmp = chain->next;
      rm_chain(chain);
      chain = tmp;
    }
  free(list);
}

void		empty_list(t_list *list)
{
  t_list	*chain;
  t_list	*tmp;

  chain = list->next;
  while (chain != list)
    {
      tmp = chain->next;
      rm_chain(chain);
      chain = tmp;
    }
}
