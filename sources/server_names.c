/*
** server_names.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Sat May 28 15:01:40 2016 Arnaud Boulay
** Last update Sat May 28 17:30:43 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <string.h>
#include "myirc.h"

int		names_chan(t_chan *chan, t_user *user, int *first)
{
  char		msg[MSG_SIZE];
  t_list	*chain;

  chain = chan->users->next;
  strcpy(msg, user->nick);
  strcat(msg, " = ");
  strcat(msg, chan->name);
  while (chain != chan->users)
    {
      if (*first == FALSE &&
	  send_reply(user, SERV_NICK, ANS_NAMRPLY, msg) == RET_FAILURE)
	return (RET_FAILURE);
      strcpy(msg, ((t_user *)chain->elem)->nick);
      strcat(msg, END_MSG);
      if (write_buff(user->output, msg) == RET_FAILURE)
	return (RET_FAILURE);
      *first = TRUE;
      chain = chain->next;
    }
  *first = FALSE;
  return (RET_SUCCESS);
}

int		names_user(t_env *env, t_user *user, int *first)
{
  char		msg[MSG_SIZE];
  t_list	*chain;

  chain = env->users->next;
  strcpy(msg, user->nick);
  strcat(msg, " = ");
  strcat(msg, SERV_NICK);
  while (chain != env->users)
    {
      if (((t_user *)chain->elem)->chans->next ==
	  ((t_user *)chain->elem)->chans)
	{
	  if (*first == FALSE &&
	      send_reply(user, SERV_NICK, ANS_NAMRPLY, msg) == RET_FAILURE)
	    return (RET_FAILURE);
	  strcpy(msg, ((t_user *)chain->elem)->nick);
	  strcat(msg, END_MSG);
	  if (write_buff(user->output, msg) == RET_FAILURE)
	    return (RET_FAILURE);
	  *first = TRUE;
	}
      chain = chain->next;
    }
  *first = FALSE;
  return (RET_SUCCESS);
}

int		names_cmd(t_env *env, t_user *user)
{
  int		first;
  t_list	*chan;

  first = FALSE;
  if (user->cmd[1] != NULL &&
      (chan = getchanbyname(env->chans, user->cmd[1])) != NULL &&
      names_chan(chan->elem, user, &first) == RET_FAILURE)
    return (RET_FAILURE);
  else if (user->cmd[1] == NULL)
    {
      chan = env->chans->next;
      while (chan != env->chans)
	{
	  if (names_chan(chan->elem, user, &first) == RET_FAILURE)
	    return (RET_FAILURE);
	  chan = chan->next;
	}
      if (names_user(env, user, &first) == RET_FAILURE)
	return (RET_FAILURE);
    }
  if (send_reply(user, SERV_NICK, ANS_ENDNAME, user->nick) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int		users_cmd(t_env *env, t_user *user)
{
  int		first;
  t_list	*chan;

  first = FALSE;
  chan = env->chans->next;
  while (chan != env->chans)
    {
      if (names_chan(chan->elem, user, &first) == RET_FAILURE)
	return (RET_FAILURE);
      chan = chan->next;
    }
  if (names_user(env, user, &first) == RET_FAILURE)
    return (RET_FAILURE);
  if (send_reply(user, SERV_NICK, ANS_ENDNAME, user->nick) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}
