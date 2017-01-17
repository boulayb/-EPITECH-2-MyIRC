/*
** client_server.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Thu May 26 17:33:31 2016 Arnaud Boulay
** Last update Fri May 27 19:22:41 2016 Arnaud Boulay
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "myirc.h"

int			get_input(t_client *client)
{
  int			crlf;
  char			buff[BUFF_SIZE + 1];

  read_buff(client->input, buff);
  if ((crlf = find_crlf(buff)) != RET_FAILURE)
    {
      buff[crlf - 1] = '\0';
      my_putstr(STDOUT_FILENO, buff);
      my_putstr(STDOUT_FILENO, "\n");
      if ((client->input->start += (crlf + 1)) >= BUFF_SIZE)
	client->input->start -= BUFF_SIZE - 1;
    }
  return (RET_SUCCESS);
}

int			write_server(t_client *client)
{
  int			ret;
  int			crlf;
  char			buff[BUFF_SIZE + 1];

  read_buff(client->output, buff);
  if ((crlf = find_crlf(buff)) != RET_FAILURE)
    {
      buff[crlf + 1] = '\0';
      buff[MSG_SIZE] = '\0';
      if ((ret = write(client->fd, buff, strlen(buff))) == -1)
	{
	  perror("./client: write");
	  return (RET_FAILURE);
	}
      else
	client->output->start += ret;
    }
  get_input(client);
  return (RET_SUCCESS);
}

int			read_server(t_client *client)
{
  int			ret;
  char			buff[BUFF_SIZE + 1];

  if ((ret = read(client->fd, buff, BUFF_SIZE)) == -1)
    {
      perror("./client: read");
      return (RET_FAILURE);
    }
  else if (ret > 0)
    {
      buff[ret] = '\0';
      if (write_buff(client->input, buff) == RET_FAILURE ||
	  get_input(client) == RET_FAILURE)
	return (RET_FAILURE);
    }
  else
    if (close_connect(client) == RET_FAILURE)
      return (RET_FAILURE);
  return (RET_SUCCESS);
}
