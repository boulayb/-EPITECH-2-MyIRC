/*
** client_msg.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Sun May 29 19:29:25 2016 Arnaud Boulay
** Last update Sun May 29 19:36:41 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "myirc.h"

int			msg_len(t_client *client)
{
  int			i;
  int			len;

  i = 1;
  len = strlen(client->cmd[1]) + 4 + strlen(CMD_MSG);
  while (client->cmd[++i] != NULL)
    len += strlen(client->cmd[i]) + 1;
  if (len >= MSG_SIZE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int			msg_send(t_client *client)
{
  char			msg[MSG_SIZE];
  int			i;

  strcpy(msg, CMD_MSG);
  strcat(msg, client->cmd[1]);
  strcat(msg, " ");
  i = 1;
  while (client->cmd[++i] != NULL)
    {
      strcat(msg, client->cmd[i]);
      if (client->cmd[i + 1] != NULL)
	strcat(msg, " ");
    }
  strcat(msg, END_MSG);
  if (write_buff(client->output, msg) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int			msg_cli(t_client *client)
{
  if (client->cmd[1] == NULL || client->cmd[2] == NULL)
    my_putstr(STDOUT_FILENO, MSG_ARG);
  else if (msg_len(client) == RET_FAILURE)
    my_putstr(STDOUT_FILENO, MSG_MSGLEN);
  else
    if (msg_send(client) == RET_FAILURE)
      return (RET_FAILURE);
  return (RET_SUCCESS);
}
