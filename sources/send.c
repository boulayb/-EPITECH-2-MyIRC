/*
** send.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Tue May 24 23:36:57 2016 Arnaud Boulay
** Last update Sun May 29 19:16:31 2016 Arnaud Boulay
*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "myirc.h"

int			send_to_user(t_user *user)
{
  int			ret;
  char			buff[BUFF_SIZE + 1];

  read_buff(user->output, buff);
  if ((ret = write(user->fd, buff, strlen(buff))) == -1)
    {
      perror("./server: write");
      return (RET_FAILURE);
    }
  else
    user->output->start += ret;
  return (RET_SUCCESS);
}

int			send_to_other(t_user *user, t_chan *chan, char *msg)
{
  t_list		*dest;

  dest = chan->users->next;
  while (dest != chan->users)
    {
      if (user != ((t_user *)dest->elem))
	if (write_buff(((t_user *)dest->elem)->output, msg) == RET_FAILURE)
	  return (RET_FAILURE);
      dest = dest->next;
    }
  return (RET_SUCCESS);
}

int			send_to_chan(t_chan *chan, char *msg)
{
  t_list		*user;

  user = chan->users->next;
  while (user != chan->users)
    {
      if (write_buff(((t_user *)user->elem)->output, msg) == RET_FAILURE)
	return (RET_FAILURE);
      user = user->next;
    }
  return (RET_SUCCESS);
}

int			send_to_all(t_user *user, char *msg)
{
  t_list		*chan;

  chan = user->chans->next;
  while (chan != user->chans)
    {
      if (send_to_chan(chan->elem, msg) == RET_FAILURE)
	return (RET_FAILURE);
      chan = chan->next;
    }
  return (RET_SUCCESS);
}

int			send_reply(t_user *user, char *src,
				   char *code, char *msg)
{
  char			buff[MSG_SIZE];

  strcpy(buff, src);
  strcat(buff, code);
  strcat(buff, msg);
  strcat(buff, END_MSG);
  if (write_buff(user->output, buff) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}
