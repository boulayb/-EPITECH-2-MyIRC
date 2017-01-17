/*
** server_msg.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Sun May 29 17:26:43 2016 Arnaud Boulay
** Last update Sun May 29 19:17:11 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <string.h>
#include "myirc.h"

int		msg_cat(t_user *user, char *msg, char *dest)
{
  int		i;

  i = 1;
  strcpy(msg, ":");
  strcat(msg, user->nick);
  strcat(msg, " PRIVMSG ");
  strcat(msg, dest);
  strcat(msg, " ");
  while (user->cmd[++i] != NULL)
    {
      strcat(msg, user->cmd[i]);
      if (user->cmd[i + 1] != NULL)
	strcat(msg, " ");
    }
  strcat(msg, END_MSG);
  return (RET_SUCCESS);
}

int		msg_user(t_env *env, t_user *user)
{
  char		msg[MSG_SIZE];
  t_list	*dest;

  if (strlen(user->cmd[1]) > 9 ||
      (dest = getuserbynick(env->users, user->cmd[1])) == NULL)
    return (send_reply(user, SERV_NICK, ANS_NONICK, user->nick));
  msg_cat(user, msg, user->cmd[1]);
  return (write_buff(((t_user *)dest->elem)->output, msg));
}

int		msg_chan(t_env *env, t_user *user)
{
  char		msg[MSG_SIZE];
  t_list	*chan;

  if (user->cmd[1][0] != '#' || strchr(user->cmd[1], ' ') != NULL ||
      strchr(user->cmd[1], 7) != NULL ||
      strchr(user->cmd[1], ',') != NULL ||
      strchr(user->cmd[1], ':') != NULL || strlen(user->cmd[1]) > 50 ||
      (chan = getchanbyname(env->chans, user->cmd[1])) == NULL)
    return (send_reply(user, SERV_NICK, ANS_BADCHAN, user->nick));
  else if (getuserbyfd(((t_chan *)chan->elem)->users, user->fd) == NULL)
    return (send_reply(user, SERV_NICK, ANS_CANTSEND, user->nick));
  msg_cat(user, msg, user->cmd[1]);
  return (send_to_other(user, chan->elem, msg));
}

int		msg_cmd(t_env *env, t_user *user)
{
  if (user->cmd[1] == NULL || user->cmd[2] == NULL)
    return (send_reply(user, SERV_NICK, ANS_NOTEXT, user->nick));
  else if (user->cmd[1][0] == '#' || user->cmd[1][0] == '&' ||
	   user->cmd[1][0] == '!' || user->cmd[1][0] == '+')
    return (msg_chan(env, user));
  return (msg_user(env, user));
}
