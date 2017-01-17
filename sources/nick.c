/*
** nick.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Mon May 23 15:19:52 2016 Arnaud Boulay
** Last update Sun May 29 17:20:36 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "myirc.h"

char		*set_nick_answer(t_user *user, char *msg)
{
  strcpy(msg, ":");
  strcat(msg, user->nick);
  strcat(msg, " NICK :");
  strcat(msg, user->cmd[1]);
  strcat(msg, END_MSG);
  return (msg);
}

int		check_nick_syntax(char *nick)
{
  int		i;

  i = -1;
  while (nick[++i] != '\0')
    if ((nick[i] != '-' && nick[i] != '[' && nick[i] != ']' &&
	 nick[i] != '\\' && nick[i] != '{' && nick[i] != '}' &&
	 nick[i] != '`' && nick[i] != '^' && nick[i] != '_' &&
	 (nick[i] < '0' || (nick[i] > '9' && nick[i] < 'A') ||
	  (nick[i] > 'Z' && nick[i] < 'a') || nick[i] > 'z')) ||
	(i == 0 && nick[i] >= '0' && nick[i] <= '9'))
      return (RET_FAILURE);
  return (RET_SUCCESS);
}

int		set_nick(t_user *user, int fd, char *nick)
{
  if (nick == NULL)
    {
      if (snprintf(user->nick, NICK_SIZE, "%s%d", DEFAULT_NICK, fd) < 0)
	{
	  my_putstr(STDERR_FILENO, ERR_PRINTF);
	  return (RET_FAILURE);
	}
    }
  else
    {
      memcpy(user->nick, nick, strlen(nick));
      user->nick[strlen(nick)] = '\0';
    }
  return (RET_SUCCESS);
}

int		nick_cmd(t_env *env, t_user *user)
{
  char		msg[MSG_SIZE];

  msg[0] = '\0';
  if (user->cmd[1] == NULL)
    strcpy(msg, ANS_NICK_NULL);
  else if ((strlen(user->cmd[1]) > 9 ||
	    check_nick_syntax(user->cmd[1]) == RET_FAILURE))
    strcpy(msg, ANS_NICK_ERR);
  else if (getuserbynick(env->users, user->cmd[1]) != NULL)
    strcpy(msg, ANS_NICK_INUSE);
  if (msg[0] != '\0' &&
      send_reply(user, SERV_NICK, msg, user->nick) == RET_FAILURE)
    return (RET_FAILURE);
  else if (msg[0] == '\0')
    {
      if (((user->chans->next == user->chans) &&
	   send_reply(user, strcat(strcat(msg, ":"), user->nick),
		      " NICK :", user->cmd[1]) == RET_FAILURE) ||
	  set_nick_answer(user, msg) == NULL ||
	  set_nick(user, user->fd, user->cmd[1]) == RET_FAILURE ||
	  send_to_all(user, msg) == RET_FAILURE)
	return (RET_FAILURE);
    }
  return (RET_SUCCESS);
}
