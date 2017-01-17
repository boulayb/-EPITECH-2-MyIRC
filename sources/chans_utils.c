/*
** chans_utils.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Sun May 22 15:22:59 2016 Arnaud Boulay
** Last update Sun May 29 15:19:56 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "myirc.h"

int		quit_chan(t_env *env, t_chan *chan, int fd)
{
  char		msg[MSG_SIZE];
  t_list	*tmp;
  t_list	*user;

  if ((user = getuserbyfd(chan->users, fd)) == NULL)
    return (RET_FAILURE);
  strcpy(msg, ":");
  strcat(msg, ((t_user *)user->elem)->nick);
  strcat(msg, " PART ");
  strcat(msg, chan->name);
  strcat(msg, END_MSG);
  if (send_to_chan(chan, msg) == RET_FAILURE ||
      (tmp = getchanbyname(((t_user *)user->elem)->chans, chan->name)) == NULL)
    return (RET_FAILURE);
  rm_chain(tmp);
  rm_chain(user);
  if (chan->users->next == chan->users)
    {
      if ((tmp = getchanbyname(env->chans, chan->name)) == NULL)
	return (RET_FAILURE);
      rm_chain(tmp);
      rm_chan(chan);
    }
  return (RET_SUCCESS);
}

t_list		*getchanbyname(t_list *list, char *name)
{
  t_list	*chain;

  chain = list->next;
  while (chain != list)
    {
      if (strcmp(((t_chan *)(chain->elem))->name, name) == 0)
	return (chain);
      chain = chain->next;
    }
  return (NULL);
}
