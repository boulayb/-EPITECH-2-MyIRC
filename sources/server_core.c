/*
** server_core.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Wed May 18 20:30:34 2016 Arnaud Boulay
** Last update Fri May 27 17:15:26 2016 Arnaud Boulay
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "myirc.h"

int			close_fd(t_env *env, int fd)
{
  t_list		*user;
  t_list		*chain;

  if ((user = getuserbyfd(env->users, fd)) != NULL)
    {
      chain = ((t_user *)user->elem)->chans->next;
      while (chain != ((t_user *)user->elem)->chans)
	{
	  if (quit_chan(env, chain->elem, fd) == RET_FAILURE)
	    return (RET_FAILURE);
	  chain = chain->next;
	}
      rm_user(user->elem);
      rm_chain(user);
    }
  if (close(fd) == -1)
    {
      perror("./server: close");
      return (RET_FAILURE);
    }
  env->fd_type[fd] = FD_FREE;
  return (RET_SUCCESS);
}

int			server_accept(t_env *env, int fd)
{
  int			cs;
  socklen_t		client_sin_len;
  struct sockaddr_in	client_sin;
  t_user		*user;

  client_sin_len = sizeof(client_sin);
  if ((cs = accept(fd, (struct sockaddr *)&client_sin, &client_sin_len)) == -1)
    {
      perror("./server: accept");
      return (RET_FAILURE);
    }
  else if ((user = create_user(cs)) == NULL ||
	   add_chain(env->users, user) == RET_FAILURE ||
	   set_nick(user, cs, NULL) == RET_FAILURE)
    return (RET_FAILURE);
  env->fd_type[cs] = FD_CLIENT;
  env->fct_read[cs] = read_client;
  env->fct_write[cs] = write_client;
  return (RET_SUCCESS);
}

int			server_close(t_env *env)
{
  int			i;

  i = -1;
  while (++i < MAX_FD)
    if (env->fd_type[i] == FD_CLIENT)
      {
	if (close_fd(env, i) == RET_FAILURE)
	  return (RET_FAILURE);
      }
    else if (env->fd_type[i] != FD_FREE)
      {
	if (close(i) == -1)
	  {
	    perror("./server: close");
	    return (RET_FAILURE);
	  }
      }
  rm_list(env->users);
  rm_list(env->chans);
  return (RET_SUCCESS);
}

int			server_action(t_env *env, fd_set *fd_read,
				      fd_set *fd_write)
{
  int			i;

  i = -1;
  while (++i < MAX_FD)
    {
      if (FD_ISSET(i, fd_read))
	{
	  if ((env->fct_read[i](env, i)) == RET_FAILURE)
	    return (RET_FAILURE);
	}
      else if (FD_ISSET(i, fd_write))
	{
	  if ((env->fct_write[i](env, i)) == RET_FAILURE)
	    return (RET_FAILURE);
	}
    }
  return (RET_SUCCESS);
}

int			server_loop(t_env *env)
{
  int			i;
  int			fd_max;
  fd_set		fd_read;
  fd_set		fd_write;

  FD_ZERO(&fd_read);
  FD_ZERO(&fd_write);
  fd_max = 0;
  i = -1;
  while (++i < MAX_FD)
    if (env->fd_type[i] != FD_FREE)
      {
	FD_SET(i, &fd_read);
	FD_SET(i, &fd_write);
	fd_max = i;
      }
  if (select(fd_max + 1, &fd_read, &fd_write, NULL, NULL) == -1)
    {
      perror("./server: select");
      return (RET_FAILURE);
    }
  else if (server_action(env, &fd_read, &fd_write) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}
