/*
** server_setup.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Wed May 18 23:35:26 2016 Arnaud Boulay
** Last update Fri May 27 17:15:55 2016 Arnaud Boulay
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "myirc.h"

int			server_setup_chans(t_env *env)
{
  if ((env->chans = create_list()) == NULL ||
      (env->users = create_list()) == NULL)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int			server_setup_core(int *sock,
					  struct sockaddr_in *sin,
					  struct protoent *proto)
{
  if ((*sock = socket(AF_INET, SOCK_STREAM, proto->p_proto)) == -1)
    {
      perror("./server: socket");
      return (RET_FAILURE);
    }
  else if (bind(*sock, (struct sockaddr*)sin, sizeof(*sin)) == -1)
    {
      perror("./server: bind");
      return (RET_FAILURE);
    }
  else if (listen(*sock, LISTEN_BACKLOG) == -1)
    {
      perror("./server: listen");
      return (RET_FAILURE);
    }
  else if (*sock >= MAX_FD)
    {
      my_putstr(STDERR_FILENO, ERR_FD);
      return (RET_FAILURE);
    }
  return (RET_SUCCESS);
}

int			server_setup(t_env *env, int port)
{
  int			sock;
  struct protoent	*proto;
  struct sockaddr_in	sin;

  memset(env->fd_type, FD_FREE, MAX_FD);
  if ((proto = getprotobyname(PROTOCOL)) == NULL)
    {
      my_putstr(STDERR_FILENO, ERR_PROTO);
      return (RET_FAILURE);
    }
  else if (server_setup_chans(env) == RET_FAILURE)
    return (RET_FAILURE);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  if (server_setup_core(&sock, &sin, proto) == RET_FAILURE)
    return (RET_FAILURE);
  env->fd_type[sock] = FD_SERVER;
  env->fct_read[sock] = server_accept;
  env->fct_write[sock] = NULL;
  return (RET_SUCCESS);
}
