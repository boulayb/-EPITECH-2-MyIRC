/*
** cmd_client.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Thu May 26 15:22:32 2016 Arnaud Boulay
** Last update Sun May 29 19:38:58 2016 Arnaud Boulay
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "myirc.h"

int			list_cli(t_client *client)
{
  char			msg[MSG_SIZE];

  strcpy(msg, CMD_LIST);
  if (client->cmd[1] != NULL)
    {
      if (strlen(client->cmd[1]) >= MSG_SIZE - strlen(CMD_LIST) - 3)
	{
	  my_putstr(STDOUT_FILENO, MSG_MSGLEN);
	  return (RET_SUCCESS);
	}
      strcat(msg, client->cmd[1]);
    }
  strcat(msg, END_MSG);
  if (write_buff(client->output, msg) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int			nick_cli(t_client *client)
{
  char			msg[MSG_SIZE];

  if (client->cmd[1] == NULL)
    my_putstr(STDOUT_FILENO, MSG_NONICK);
  else if (strlen(client->cmd[1]) > 9)
    my_putstr(STDOUT_FILENO, MSG_NICKSIZE);
  else
    {
      strcpy(msg, CMD_NICK);
      strcat(msg, client->cmd[1]);
      strcat(msg, END_MSG);
      if (write_buff(client->output, msg) == RET_FAILURE)
	return (RET_FAILURE);
    }
  return (RET_SUCCESS);
}

int			server_cli(t_client *client)
{
  int			port;

  if (client->cmd[1] == NULL)
    my_putstr(STDOUT_FILENO, MSG_ARG);
  else if (client->fd == -1)
    {
      port = parse_ip(client->cmd[1]);
      if (write_buff(client->output, CMD_SERVER) == RET_FAILURE ||
	  prepare_connection(client, port) == RET_FAILURE)
	return (RET_FAILURE);
    }
  else if (client->fd != -1)
    my_putstr(STDOUT_FILENO, MSG_CONNECTED);
  return (RET_SUCCESS);
}

int			nocmd_cli(t_client *client)
{
  my_putstr(STDOUT_FILENO, client->cmd[0]);
  my_putstr(STDOUT_FILENO, MSG_CMD);
  return (RET_SUCCESS);
}
