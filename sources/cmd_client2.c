/*
** cmd_client2.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Sun May 29 19:38:39 2016 Arnaud Boulay
** Last update Sun May 29 19:38:42 2016 Arnaud Boulay
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "myirc.h"

int			users_cli(t_client *client)
{
  char			msg[MSG_SIZE];

  strcpy(msg, CMD_USERS);
  strcat(msg, END_MSG);
  if (write_buff(client->output, msg) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int			names_cli(t_client *client)
{
  char			msg[MSG_SIZE];

  strcpy(msg, CMD_NAMES);
  if (client->cmd[1] == NULL)
    {
      my_putstr(STDOUT_FILENO, MSG_ARG);
      return (RET_SUCCESS);
    }
  else if (strlen(client->cmd[1]) >= MSG_SIZE - strlen(CMD_NAMES) - 3)
    {
      my_putstr(STDOUT_FILENO, MSG_MSGLEN);
      return (RET_SUCCESS);
    }
  strcat(msg, client->cmd[1]);
  strcat(msg, END_MSG);
  if (write_buff(client->output, msg) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int			part_cli(t_client *client)
{
  char			msg[MSG_SIZE];

  strcpy(msg, CMD_PART);
  if (client->cmd[1] == NULL)
    {
      my_putstr(STDOUT_FILENO, MSG_ARG);
      return (RET_SUCCESS);
    }
  else if (strlen(client->cmd[1]) >= MSG_SIZE - strlen(CMD_PART) - 3)
    {
      my_putstr(STDOUT_FILENO, MSG_MSGLEN);
      return (RET_SUCCESS);
    }
  strcat(msg, client->cmd[1]);
  strcat(msg, END_MSG);
  if (write_buff(client->output, msg) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int			join_cli(t_client *client)
{
  char			msg[MSG_SIZE];

  strcpy(msg, CMD_JOIN);
  if (client->cmd[1] == NULL)
    {
      my_putstr(STDOUT_FILENO, MSG_ARG);
      return (RET_SUCCESS);
    }
  else if (strlen(client->cmd[1]) >= MSG_SIZE - strlen(CMD_JOIN) - 3)
    {
      my_putstr(STDOUT_FILENO, MSG_MSGLEN);
      return (RET_SUCCESS);
    }
  strcat(msg, client->cmd[1]);
  strcat(msg, END_MSG);
  if (write_buff(client->output, msg) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int			quit_cli(t_client *client)
{
  char			msg[MSG_SIZE];

  strcpy(msg, CMD_QUIT);
  if (client->cmd[1] == NULL)
    strcat(msg, MSG_DEFQUIT);
  else if (client->cmd[1] != NULL)
    {
      if (strlen(client->cmd[1]) >= MSG_SIZE - strlen(CMD_QUIT) - 3)
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
