/*
** tools.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Tue May 17 15:10:57 2016 Arnaud Boulay
** Last update Fri May 27 15:17:29 2016 Arnaud Boulay
*/

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "myirc.h"

int			find_crlf(char *str)
{
  int			i;

  i = -1;
  while (str[++i] != '\0')
    if (str[i] == '\n' && i > 1 && str[i - 1] == '\r')
      return (i);
  return (RET_FAILURE);
}

int			my_putstr(int fd, char *str)
{
  if (write(fd, str, strlen(str)) == -1)
    {
      perror("write");
      return (RET_FAILURE);
    }
  return (RET_SUCCESS);
}
