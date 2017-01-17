/*
** chans.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Sat May 21 16:20:21 2016 Arnaud Boulay
** Last update Mon May 23 15:16:03 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "myirc.h"

t_chan		*create_chan(char *name)
{
  t_chan	*chan;

  if ((chan = malloc(sizeof(t_chan))) == NULL ||
      (chan->name = strdup(name)) == NULL)
    {
      my_putstr(STDERR_FILENO, ERR_MALLOC);
      return (NULL);
    }
  else if ((chan->users = create_list()) == NULL)
    return (NULL);
  return (chan);
}

void		rm_chan(t_chan *chan)
{
  free(chan->name);
  rm_list(chan->users);
  free(chan);
}

void		rm_chans(t_list *list)
{
  t_list	*chain;
  t_list	*tmp;

  chain = list->next;
  while (chain != list)
    {
      tmp = chain->next;
      rm_chan(chain->elem);
      rm_chain(chain);
      chain = tmp;
    }
  free(list);
}
