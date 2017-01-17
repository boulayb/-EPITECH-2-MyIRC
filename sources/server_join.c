/*
** server_join.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Sat May 28 15:02:16 2016 Arnaud Boulay
** Last update Sun May 29 15:21:03 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <string.h>
#include "myirc.h"

int		join_msg(t_chan *chan, t_user *user)
{
  char		msg[MSG_SIZE];
  int		first;

  first = FALSE;
  strcpy(msg, ":");
  strcat(msg, user->nick);
  strcat(msg, " JOIN :");
  strcat(msg, chan->name);
  strcat(msg, END_MSG);
  if (add_chain(user->chans, chan) == RET_FAILURE ||
      add_chain(chan->users, user) == RET_FAILURE ||
      send_to_chan(chan, msg) == RET_FAILURE ||
      send_reply(user, SERV_NICK, ANS_NOTOPIC, user->nick) == RET_FAILURE ||
      names_chan(chan, user, &first) == RET_FAILURE ||
      send_reply(user, SERV_NICK, ANS_ENDNAME, user->nick) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int		join_core(t_env *env, t_user *user)
{
  t_list	*chan;
  t_chan	*new;

  if ((chan = getchanbyname(env->chans, user->cmd[1])) == NULL)
    {
      if ((new = create_chan(user->cmd[1])) == NULL ||
	  add_chain(env->chans, new) == RET_FAILURE)
	return (RET_FAILURE);
    }
  else
    new = chan->elem;
  if (getuserbyfd(new->users, user->fd) != NULL)
    return (RET_SUCCESS);
  else if (join_msg(new, user) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int		join_cmd(t_env *env, t_user *user)
{
  char		msg[MSG_SIZE];

  msg[0] = '\0';
  if (user->cmd[1] == NULL)
    strcpy(msg, ANS_NOPARAMS);
  else if (user->cmd[1][0] != '#' || strchr(user->cmd[1], ' ') != NULL ||
	   strchr(user->cmd[1], 7) != NULL ||
	   strchr(user->cmd[1], ',') != NULL ||
	   strchr(user->cmd[1], ':') != NULL || strlen(user->cmd[1]) > 50)
    strcpy(msg, ANS_BADCHAN);
  if (msg[0] != '\0' &&
      send_reply(user, SERV_NICK, msg, user->nick) == RET_FAILURE)
    return (RET_FAILURE);
  else if (msg[0] == '\0')
    if (join_core(env, user) == RET_FAILURE)
      return (RET_FAILURE);
  return (RET_SUCCESS);
}

int		part_cmd(t_env *env, t_user *user)
{
  char		msg[MSG_SIZE];
  t_list	*chan;

  msg[0] = '\0';
  if (user->cmd[1] == NULL)
    strcpy(msg, ANS_NOPARAMS);
  else if (user->cmd[1][0] != '#' || strchr(user->cmd[1], ' ') != NULL ||
	   strchr(user->cmd[1], 7) != NULL ||
	   strchr(user->cmd[1], ',') != NULL ||
	   strchr(user->cmd[1], ':') != NULL || strlen(user->cmd[1]) > 50 ||
	   (chan = getchanbyname(env->chans, user->cmd[1])) == NULL)
    strcpy(msg, ANS_NOCHAN);
  else if (getuserbyfd(((t_chan *)chan->elem)->users, user->fd) == NULL)
    strcpy(msg, ANS_NOTONCHAN);
  if (msg[0] != '\0' &&
      send_reply(user, SERV_NICK, msg, user->nick) == RET_FAILURE)
    return (RET_FAILURE);
  else if (msg[0] == '\0' &&
	   quit_chan(env, chan->elem, user->fd) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}
