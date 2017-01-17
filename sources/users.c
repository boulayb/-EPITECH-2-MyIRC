/*
** users.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Sat May 21 16:40:33 2016 Arnaud Boulay
** Last update Sun May 29 17:23:52 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "myirc.h"

t_list		*getuserbyfd(t_list *list, int fd)
{
  t_list	*chain;

  chain = list->next;
  while (chain != list)
    {
      if (((t_user *)(chain->elem))->fd == fd)
	return (chain);
      chain = chain->next;
    }
  return (NULL);
}

t_list		*getuserbynick(t_list *list, char *nick)
{
  t_list	*chain;

  chain = list->next;
  while (chain != list)
    {
      if (strcmp(((t_user *)(chain->elem))->nick, nick) == 0)
	return (chain);
      chain = chain->next;
    }
  return (NULL);
}

t_user		*create_user(int fd)
{
  t_user	*user;

  if ((user = malloc(sizeof(t_user))) == NULL)
    {
      my_putstr(STDERR_FILENO, ERR_MALLOC);
      return (NULL);
    }
  else if ((user->input = create_buff()) == NULL ||
	   (user->output = create_buff()) == NULL ||
	   (user->chans = create_list()) == NULL)
    return (NULL);
  user->cmd = NULL;
  user->fd = fd;
  memset(user->nick, '\0', NICK_SIZE);
  return (user);
}

void		rm_user(t_user *user)
{
  free_tab(user->cmd);
  rm_buff(user->input);
  rm_buff(user->output);
  rm_list(user->chans);
  free(user);
}

void		rm_users(t_list *list)
{
  t_list	*chain;
  t_list	*tmp;

  chain = list->next;
  while (chain != list)
    {
      tmp = chain->next;
      rm_user(chain->elem);
      rm_chain(chain);
      chain = tmp;
    }
  free(list);
}
