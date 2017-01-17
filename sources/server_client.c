/*
** server_client.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Wed May 18 20:32:54 2016 Arnaud Boulay
** Last update Mon May 30 15:30:43 2016 Arnaud Boulay
*/

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "myirc.h"

static t_cmd		gl_cmd[] =
  {
    {"NICK", &nick_cmd},
    {"USER", &user_cmd},
    {"QUIT", &quit_cmd},
    {"LIST", &list_cmd},
    {"JOIN", &join_cmd},
    {"PART", &part_cmd},
    {"NAMES", &names_cmd},
    {"USERS", &users_cmd},
    {"PRIVMSG", &msg_cmd},
    {"", &no_cmd}
  };

int			get_input(t_user *user)
{
  int			crlf;
  char			buff[BUFF_SIZE + 1];

  read_buff(user->input, buff);
  if ((crlf = find_crlf(buff)) != RET_FAILURE)
    {
      buff[crlf - 1] = '\0';
      buff[MSG_SIZE] = '\0';
      if ((user->cmd = my_strtowordtab(buff, ' ')) == NULL)
	return (RET_FAILURE);
      if ((user->input->start += (crlf + 1)) >= BUFF_SIZE)
	user->input->start -= BUFF_SIZE - 1;
    }
  return (RET_SUCCESS);
}

int			parse_cmd(t_env *env, t_user *user)
{
  int			i;

  i = -1;
  while (strcmp(gl_cmd[++i].cmd, "") != 0)
    if (strcmp(gl_cmd[i].cmd, user->cmd[0]) == 0)
      return (gl_cmd[i].func(env, user));
  return (gl_cmd[i].func(env, user));
}

int			write_client(t_env *env, int fd)
{
  int			ret;
  t_user		*user;
  t_list		*chain;

  ret = 0;
  if ((chain = getuserbyfd(env->users, fd)) == NULL)
    return (RET_FAILURE);
  user = chain->elem;
  if (get_input(user) == RET_FAILURE)
    return (RET_FAILURE);
  else if (user->cmd != NULL && user->cmd[0] != NULL)
    {
      if ((ret = parse_cmd(env, user)) == RET_FAILURE)
	return (RET_FAILURE);
      free_tab(user->cmd);
      user->cmd = NULL;
    }
  if (send_to_user(user) == RET_FAILURE ||
      (ret == RET_QUIT && close_fd(env, fd) == RET_FAILURE))
    return (RET_FAILURE);
  return (RET_SUCCESS);
}

int			read_client(t_env *env, int fd)
{
  int			ret;
  char			buff[BUFF_SIZE + 1];
  t_list		*user;

  if ((ret = read(fd, buff, BUFF_SIZE)) == -1)
    {
      perror("./server: read");
      return (RET_FAILURE);
    }
  else if (ret > 0)
    {
      buff[ret] = '\0';
      if ((user = getuserbyfd(env->users, fd)) == NULL ||
	  write_buff(((t_user *)user->elem)->input, buff) == RET_FAILURE)
	return (RET_FAILURE);
    }
  else if (close_fd(env, fd) == RET_FAILURE)
    return (RET_FAILURE);
  return (RET_SUCCESS);
}
