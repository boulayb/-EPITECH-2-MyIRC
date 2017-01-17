/*
** client_connect.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Thu May 26 15:53:36 2016 Arnaud Boulay
** Last update Sat May 28 19:04:18 2016 Arnaud Boulay
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "myirc.h"

int			close_connect(t_client *client)
{
  if (close(client->fd) == -1)
    {
      perror("./client: close");
      return (RET_FAILURE);
    }
  client->fd = -1;
  my_putstr(STDOUT_FILENO, MSG_DROPPED);
  return (RET_SUCCESS);
}

int			parse_ip(char *ip)
{
  char			*pos;
  int			port;

  if ((pos = strrchr(ip, ':')) == NULL)
    return (DEFAULT_PORT);
  port = atoi(pos + 1);
  *pos = '\0';
  return (port);
}

int			connect_client(t_client *client,
				       struct sockaddr_in *s_in, int port)
{
  char			error[MSG_SIZE];

  if (connect(client->fd, (struct sockaddr *)s_in, sizeof(*s_in)) == -1)
    {
      if (close(client->fd) == -1)
	{
	  perror("./client: close");
	  return (RET_FAILURE);
	}
      client->fd = -1;
      client->output->start = client->output->end;
      sprintf(error, "%s%s:%d\n", MSG_ERRCONNECT, client->cmd[1], port);
      my_putstr(STDOUT_FILENO, error);
      return (RET_SUCCESS);
    }
  return (RET_SUCCESS);
}

int			prepare_connection(t_client *client, int port)
{
  struct protoent	*proto;
  struct sockaddr_in	s_in;

  if ((proto = getprotobyname(PROTOCOL)) == NULL)
    {
      my_putstr(STDERR_FILENO, ERR_PROTOCLI);
      return (RET_FAILURE);
    }
  else if ((s_in.sin_addr.s_addr = inet_addr(client->cmd[1])) == INADDR_NONE)
    {
      my_putstr(STDOUT_FILENO, MSG_WRONGIP);
      return (RET_SUCCESS);
    }
  s_in.sin_family = AF_INET;
  s_in.sin_port = htons(port);
  if ((client->fd = socket(AF_INET, SOCK_STREAM, proto->p_proto)) == -1)
    {
      perror("./client: socket");
      return (RET_FAILURE);
    }
  else if (connect_client(client, &s_in, port) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}
