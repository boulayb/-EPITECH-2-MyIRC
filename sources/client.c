/*
** client.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Tue May 17 15:04:57 2016 Arnaud Boulay
** Last update Fri May 27 14:18:54 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "myirc.h"

int		rm_client(t_client *client)
{
  if (client->fd != -1)
    {
      if (close(client->fd) == -1)
	{
	  perror("./client: close");
	  return (RET_FAILURE);
	}
    }
  free_tab(client->cmd);
  rm_buff(client->input);
  rm_buff(client->output);
  free(client);
  return (RET_SUCCESS);
}

t_client	*create_client(void)
{
  t_client	*client;

  if ((client = malloc(sizeof(t_client))) == NULL)
    {
      my_putstr(STDERR_FILENO, ERR_MALLOC);
      return (NULL);
    }
  else if ((client->input = create_buff()) == NULL ||
	   (client->output = create_buff()) == NULL)
    return (NULL);
  client->cmd = NULL;
  client->fd = -1;
  return (client);
}

int			client_action(t_client *client, fd_set *fd_read,
				      fd_set *fd_write)
{
  if (FD_ISSET(client->fd, fd_read))
    {
      if (read_server(client) == RET_FAILURE)
	return (RET_FAILURE);
    }
  else if (FD_ISSET(STDIN_FILENO, fd_read))
    {
      if (read_input(client) == RET_FAILURE)
	return (RET_FAILURE);
    }
  else if (FD_ISSET(client->fd, fd_write))
    {
      if (write_server(client) == RET_FAILURE)
	return (RET_FAILURE);
    }
  return (RET_SUCCESS);
}

int		client_loop(t_client *client)
{
  int			fd_max;
  fd_set		fd_read;
  fd_set		fd_write;

  FD_ZERO(&fd_read);
  FD_ZERO(&fd_write);
  FD_SET(STDIN_FILENO, &fd_read);
  fd_max = STDIN_FILENO;
  if (client->fd != -1)
    {
      FD_SET(client->fd, &fd_read);
      FD_SET(client->fd, &fd_write);
      fd_max = client->fd;
    }
  if (select(fd_max + 1, &fd_read, &fd_write, NULL, NULL) == -1)
    {
      perror("./client: select");
      return (RET_FAILURE);
    }
  else if (client_action(client, &fd_read, &fd_write) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int		main(void)
{
  t_client	*client;

  if ((client = create_client()) == NULL)
    return (RET_FAILURE);
  while (TRUE)
    {
      if (client_loop(client) == RET_FAILURE)
	{
	  if (rm_client(client) == RET_FAILURE)
	    return (EXIT_FAILURE);
	  return (EXIT_FAILURE);
	}
    }
  if (rm_client(client) == RET_FAILURE)
    return (EXIT_FAILURE);
  return (EXIT_SUCCESS);
}
