/*
** client_input.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Thu May 26 17:25:10 2016 Arnaud Boulay
** Last update Fri May 27 20:30:58 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "myirc.h"

static t_cmd		gl_cmd[] =
  {
    {"/server", &server_cli},
    {"/nick", &nick_cli},
    {"/list", &list_cli},
    {"/quit", &quit_cli},
    {"/join", &join_cli},
    {"/part", &part_cli},
    {"/names", &names_cli},
    {"/users", &users_cli},
    {"/msg", &msg_cli},
    {"", &nocmd_cli}
  };

int			check_cmd(t_client *client)
{
  int			i;

  i = -1;
  if (strcmp(client->cmd[0], "/server") != 0 && client->fd == -1)
    {
      my_putstr(STDOUT_FILENO, MSG_NOCONNECT);
      return (RET_SUCCESS);
    }
  while (strcmp(gl_cmd[++i].cmd, "") != 0)
    if (strcmp(gl_cmd[i].cmd, client->cmd[0]) == 0)
      return (gl_cmd[i].func(client));
  return (gl_cmd[i].func(client));
}

int			read_input(t_client *client)
{
  int			ret;
  char			buff[BUFF_SIZE + 1];

  if ((ret = read(STDIN_FILENO, buff, BUFF_SIZE)) == -1)
    {
      perror("./client: read");
      return (RET_FAILURE);
    }
  if (ret > 0)
    buff[ret - 1] = '\0';
  buff[ret] = '\0';
  if ((client->cmd = my_strtowordtab(buff, ' ')) == NULL ||
      (client->cmd[0] != NULL && check_cmd(client) == RET_FAILURE))
    return (RET_FAILURE);
  free_tab(client->cmd);
  client->cmd = NULL;
  return (RET_SUCCESS);
}
