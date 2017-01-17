##
## Makefile for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
##
## Made by Arnaud Boulay
## Login   <boulay_b@epitech.net>
##
## Started on  Tue May 17 14:55:57 2016 Arnaud Boulay
## Last update Sun May 29 19:39:18 2016 Arnaud Boulay
##

NAME1	= server

NAME2	= client

SRCSFO	= sources/

INCSFO	= includes/

SRCSSER	= $(SRCSFO)server.c \
	  $(SRCSFO)server_core.c \
	  $(SRCSFO)server_client.c \
	  $(SRCSFO)server_setup.c \
	  $(SRCSFO)chans.c \
	  $(SRCSFO)chans_utils.c \
	  $(SRCSFO)list.c \
	  $(SRCSFO)buff.c \
	  $(SRCSFO)users.c \
	  $(SRCSFO)nick.c \
	  $(SRCSFO)cmd_server.c \
	  $(SRCSFO)send.c \
	  $(SRCSFO)server_names.c \
	  $(SRCSFO)server_join.c \
	  $(SRCSFO)server_msg.c \
	  $(SRCSFO)strtowordtab.c \
	  $(SRCSFO)tools.c

SRCSCLI	= $(SRCSFO)client.c \
	  $(SRCSFO)buff.c \
	  $(SRCSFO)client_connect.c \
	  $(SRCSFO)client_server.c \
	  $(SRCSFO)client_input.c \
	  $(SRCSFO)client_msg.c \
	  $(SRCSFO)cmd_client.c \
	  $(SRCSFO)cmd_client2.c \
	  $(SRCSFO)strtowordtab.c \
	  $(SRCSFO)tools.c

OBJSSER	= $(SRCSSER:.c=.o)

OBJSCLI	= $(SRCSCLI:.c=.o)

GCC	= gcc -o

RM	= rm -rf

CFLAGS	= -Wall -Wextra -Werror -I$(INCSFO)

all: $(NAME1) $(NAME2)

$(NAME1): $(OBJSSER)
	$(GCC) $(NAME1) $(OBJSSER) $(CFLAGS)

$(NAME2): $(OBJSCLI)
	$(GCC) $(NAME2) $(OBJSCLI) $(CFLAGS)

debug: CFLAGS = -Wall -Wextra -g3 -I$(INCSFO)
debug: all

clean:
	$(RM) $(OBJSSER)
	$(RM) $(OBJSCLI)

fclean: clean
	$(RM) $(NAME1)
	$(RM) $(NAME2)

re: fclean all

.PHONY: all clean fclean re
