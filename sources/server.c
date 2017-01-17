/*
** server.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Tue May 17 15:03:38 2016 Arnaud Boulay
** Last update Mon May 23 00:57:19 2016 Arnaud Boulay
*/

#include <unistd.h>
#include <stdlib.h>
#include "myirc.h"

int			main(int ac, char **av)
{
  int			port;
  t_env			env;

  if (ac != PARAM_NUMBER)
    {
      my_putstr(STDERR_FILENO, ERR_USAGE);
      return (EXIT_FAILURE);
    }
  else if ((port = atoi(av[PARAM_PORT])) <= PORT_MIN || port > PORT_MAX)
    {
      my_putstr(STDERR_FILENO, ERR_PORT);
      return (EXIT_FAILURE);
    }
  else if (server_setup(&env, port) == RET_FAILURE)
    {
      server_close(&env);
      return (EXIT_FAILURE);
    }
  while (TRUE)
    if (server_loop(&env) == RET_FAILURE)
      {
	server_close(&env);
	return (EXIT_FAILURE);
      }
  return (EXIT_SUCCESS);
}
