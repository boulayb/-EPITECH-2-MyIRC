/*
** cmd.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Sun May 22 16:31:18 2016 Arnaud Boulay
** Last update Sun May 29 17:25:26 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <string.h>
#include "myirc.h"

int		no_cmd(t_env *env, t_user *user)
{
  (void)env;
  if (send_reply(user, SERV_NICK, ANS_NOCMD, user->nick) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int		user_cmd(t_env *env, t_user *user)
{
  (void)env;
  if (send_reply(user, SERV_NICK, ANS_WELCOME, user->nick) == RET_FAILURE ||
      send_reply(user, SERV_NICK, ANS_YOURHOST, user->nick) == RET_FAILURE ||
      send_reply(user, SERV_NICK, ANS_CREATED, user->nick) == RET_FAILURE ||
      send_reply(user, SERV_NICK, ANS_MYINFO, user->nick) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int		quit_cmd(t_env *env, t_user *user)
{
  int		i;
  char		msg[MSG_SIZE];

  (void)env;
  i = 0;
  strcpy(msg, ":");
  strcat(msg, user->nick);
  strcat(msg, " QUIT :");
  if (user->cmd[1] == NULL)
    strcat(msg, user->nick);
  else
    while (user->cmd[++i] != NULL)
      {
	strcat(msg, user->cmd[i]);
	if (user->cmd[i + 1] != NULL)
	  strcat(msg, " ");
      }
  strcat(msg, END_MSG);
  if (send_to_all(user, msg) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_QUIT);
}

int		list_cmd(t_env *env, t_user *user)
{
  char		buff[MSG_SIZE];
  t_list	*chan;

  chan = env->chans->next;
  while (chan != env->chans)
    {
      if (((user->cmd[1] != NULL &&
	    strstr(((t_chan *)chan->elem)->name, user->cmd[1]) != NULL) ||
	   user->cmd[1] == NULL))
	{
	  strcpy(buff, user->nick);
	  strcat(buff, " ");
	  strcat(buff, ((t_chan *)chan->elem)->name);
	  if (send_reply(user, SERV_NICK, ANS_LIST, buff) == RET_FAILURE)
	    return (RET_FAILURE);
	}
      chan = chan->next;
    }
  if (send_reply(user, SERV_NICK, ANS_ENDLIST, user->nick) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}
