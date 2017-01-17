/*
** buff.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Sun May 22 20:12:01 2016 Arnaud Boulay
** Last update Fri May 27 16:07:16 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "myirc.h"

int		read_buff(t_buff *buff, char *str)
{
  if (buff->end < buff->start)
    {
      memcpy(str, buff->buff + buff->start, BUFF_SIZE - buff->start);
      memcpy(str + (BUFF_SIZE - buff->start), buff->buff, buff->end);
      str[BUFF_SIZE - buff->start + buff->end] = '\0';
    }
  else
    {
      memcpy(str, buff->buff + buff->start, buff->end - buff->start);
      str[buff->end - buff->start] = '\0';
    }
  return (RET_SUCCESS);
}

int		write_buff(t_buff *buff, char *str)
{
  int		len;

  len = strlen(str);
  if (len + buff->end >= BUFF_SIZE)
    {
      len = BUFF_SIZE - buff->end;
      if (buff->start >= buff->end)
	{
	  my_putstr(STDERR_FILENO, ERR_BUFF);
	  return (RET_FAILURE);
	}
      memcpy(buff->buff + buff->end, str, len);
      buff->end = 0;
      str = str + len;
      len = strlen(str);
    }
  memcpy(buff->buff + buff->end, str, len);
  buff->end = buff->end + len;
  return (RET_SUCCESS);
}

t_buff		*create_buff()
{
  t_buff	*buff;

  if ((buff = malloc(sizeof(t_buff))) == NULL)
    {
      my_putstr(STDERR_FILENO, ERR_MALLOC);
      return (NULL);
    }
  buff->start = 0;
  buff->end = 0;
  memset(buff->buff, '\0', BUFF_SIZE);
  return (buff);
}

void		rm_buff(t_buff *buff)
{
  free(buff);
}
