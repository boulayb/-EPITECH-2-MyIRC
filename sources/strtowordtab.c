/*
** strtowordtab.c for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Sat May 21 17:54:57 2016 Arnaud Boulay
** Last update Sun May 22 16:35:48 2016 Arnaud Boulay
*/

#include <stdlib.h>
#include "myirc.h"

int		count_words(char *str, char sep)
{
  int		words;
  int		i;

  i = -1;
  words = 0;
  while (str[++i] != '\0')
    if ((str[i] != sep && str[i] != '\t') &&
	(str[i + 1] == sep || str[i + 1] == '\t' || str[i + 1] == '\0'))
      ++words;
  return (words);
}

int		wordlen(char *str, int *i, char sep)
{
  int		j;

  j = 0;
  while (str[++*i] != '\0')
    if (str[*i] != sep && str[*i] != '\t')
      {
	++j;
	if (str[*i + 1] == sep || str[*i + 1] == '\t' || str[*i + 1] == '\0')
	  return (j);
      }
  return (-1);
}

char		*my_wordcpy(char *tab, char *str, int *i, char sep)
{
  int		j;

  j = -1;
  while (str[++*i] != '\0')
    if (str[*i] != sep && str[*i] != '\t')
      {
	tab[++j] = str[*i];
	if (str[*i + 1] == sep || str[*i + 1] == '\t' || str[*i + 1] == '\0')
	  {
	    tab[++j] = '\0';
	    return (tab);
	  }
      }
  return (NULL);
}

char		**my_strtowordtab(char *str, char sep)
{
  char		**tab;
  int		words;
  int		i;
  int		j;
  int		k;

  i = -1;
  j = -1;
  k = -1;
  if (str == NULL)
    return (NULL);
  words = count_words(str, sep);
  if ((tab = malloc(sizeof(char *) * (words + 1))) == NULL)
    return (NULL);
  while (++i < words)
    {
      if ((tab[i] = malloc(sizeof(char) * (wordlen(str, &j, sep) + 1))) == NULL)
	{
	  free_tab(tab);
	  return (NULL);
	}
      tab[i] = my_wordcpy(tab[i], str, &k, sep);
    }
  tab[i] = NULL;
  return (tab);
}

void		free_tab(char **tab)
{
  int		i;

  i = -1;
  if (tab != NULL)
    {
      while (tab[++i] != NULL)
	free(tab[i]);
      free(tab[i]);
    }
  free(tab);
  tab = NULL;
}
