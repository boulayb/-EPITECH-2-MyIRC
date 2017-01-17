/*
** myirc.h for myirc in /home/boulay_b/Rendu/SE4/PSU/PSU_2015_myirc
**
** Made by Arnaud Boulay
** Login   <boulay_b@epitech.net>
**
** Started on  Tue May 17 15:05:23 2016 Arnaud Boulay
** Last update Sun May 29 19:16:13 2016 Arnaud Boulay
*/

#ifndef	MYIRC_H_
# define MYIRC_H_

# include <sys/select.h>

# define TRUE		1
# define FALSE		0
# define RET_SUCCESS	0
# define RET_QUIT	1
# define RET_FAILURE	-1
# define BUFF_SIZE	16384
# define PARAM_NUMBER	2
# define PARAM_PORT	1
# define PORT_MAX	65535
# define PORT_MIN	0
# define PROTOCOL	"TCP"
# define LISTEN_BACKLOG	42
# define FD_FREE	0
# define FD_CLIENT	1
# define FD_SERVER	2
# define MAX_FD		255
# define MSG_SIZE	512
# define NICK_SIZE	10
# define DEFAULT_PORT	6667
# define END_MSG	"\r\n"
# define DEFAULT_NICK	"Anon"
# define SERV_NICK	":127.0.0.1 "
# define CMD_SERVER	"NICK Anon\r\nUSER nobody unknown unknown :noname\r\n"
# define CMD_NICK	"NICK "
# define CMD_LIST	"LIST "
# define CMD_QUIT	"QUIT "
# define CMD_JOIN	"JOIN "
# define CMD_PART	"PART "
# define CMD_USERS	"USERS "
# define CMD_NAMES	"NAMES "
# define CMD_MSG	"PRIVMSG "
# define ERR_USAGE	"./server: Usage: ./server port\n"
# define ERR_PORT	"./server: port require number between 1 and 65535.\n"
# define ERR_PROTO	"./server: getprotobyname: can't get protocol.\n"
# define ERR_PROTOCLI	"./client: getprotobyname: can't get protocol.\n"
# define ERR_FD		"./server: no more file descriptors space.\n"
# define ERR_BUFF	"./server: write_buff: circular buffer overflow.\n"
# define ERR_MALLOC	"./server: malloc: malloc failed.\n"
# define ERR_PRINTF	"./server: snprintf: nickname copy failed.\n"
# define ERR_ELEM	"./server: add_chain: elem from list is NULL.\n"
# define ERR_NOUSER	"./server: getuser: no user found.\n"
# define ERR_WRONG	"./server: something went wrong.\n"
# define MSG_CONNECTED	"You are already connected to a server. Leave first.\n"
# define MSG_ERRCONNECT	"Connection failed with ip "
# define MSG_CMD	" is not a valid command.\n"
# define MSG_ARG	"Missing argument for command.\n"
# define MSG_WRONGIP	"Invalid IP address.\n"
# define MSG_DROPPED	"Server closed the connection.\n"
# define MSG_NONICK	"Missing argument nickname.\n"
# define MSG_NICKSIZE	"Nickname must be 9 characters max.\n"
# define MSG_MSGLEN	"Max length for message is 509 characters.\n"
# define MSG_NOCONNECT	"Connect to a server first.\n"
# define MSG_DEFQUIT	"Bye."
# define ANS_WELCOME	"001 "
# define ANS_YOURHOST	"002 "
# define ANS_CREATED	"003 "
# define ANS_MYINFO	"004 "
# define ANS_LIST	"322 "
# define ANS_ENDLIST	"323 "
# define ANS_NOTOPIC	"331 "
# define ANS_NAMRPLY	"353 "
# define ANS_ENDNAME	"366 "
# define ANS_NONICK	"401 "
# define ANS_NOCHAN	"403 "
# define ANS_CANTSEND	"404 "
# define ANS_NOTEXT	"412 "
# define ANS_NOCMD	"421 "
# define ANS_NICK_NULL	"431 "
# define ANS_NICK_ERR	"432 "
# define ANS_NICK_INUSE	"433 "
# define ANS_NOTONCHAN	"442 "
# define ANS_NOPARAMS	"461 "
# define ANS_BADCHAN	"479 "

typedef			int(*fct)();

typedef struct		s_cmd
{
  char			*cmd;
  fct			func;
}			t_cmd;

typedef struct		s_list
{
  void			*elem;
  struct s_list		*next;
  struct s_list		*back;
}			t_list;

typedef struct		s_buff
{
  int			end;
  int			start;
  char			buff[BUFF_SIZE];
}			t_buff;

typedef struct		s_chan
{
  char			*name;
  t_list		*users;
}			t_chan;

typedef struct		s_user
{
  t_list		*chans;
  t_buff		*input;
  t_buff		*output;
  char			**cmd;
  int			fd;
  char			nick[NICK_SIZE];
}			t_user;

typedef struct		s_client
{
  t_buff		*input;
  t_buff		*output;
  char			**cmd;
  int			fd;
}			t_client;

typedef struct		s_env
{
  t_list		*users;
  t_list		*chans;
  char			fd_type[MAX_FD];
  fct			fct_read[MAX_FD];
  fct			fct_write[MAX_FD];
}			t_env;

int			parse_ip(char *str);
int			find_crlf(char *str);
int			server_loop(t_env *env);
int			server_close(t_env *env);
int			msg_cli(t_client *client);
int			nick_cli(t_client *client);
int			send_to_user(t_user *user);
int			list_cli(t_client *client);
int			quit_cli(t_client *client);
int			join_cli(t_client *client);
int			part_cli(t_client *client);
int			users_cli(t_client *client);
int			names_cli(t_client *client);
int			nocmd_cli(t_client *client);
int			close_fd(t_env *env, int fd);
int			my_putstr(int fd, char *str);
int			server_cli(t_client *client);
int			read_input(t_client *client);
int			read_server(t_client *client);
int			check_nick_syntax(char *nick);
int			write_server(t_client *client);
int			close_connect(t_client *client);
int			read_client(t_env *env, int fd);
int			write_client(t_env *env, int fd);
int			no_cmd(t_env *env, t_user *user);
int			server_accept(t_env *env, int fd);
int			msg_cmd(t_env *env, t_user *user);
int			list_cmd(t_env *env, t_user *user);
int			join_cmd(t_env *env, t_user *user);
int			part_cmd(t_env *env, t_user *user);
int			user_cmd(t_env *env, t_user *user);
int			quit_cmd(t_env *env, t_user *user);
int			server_setup(t_env *env, int port);
int			nick_cmd(t_env *env, t_user *user);
int			read_buff(t_buff *buff, char *str);
int			names_cmd(t_env *env, t_user *user);
int			users_cmd(t_env *env, t_user *user);
int			add_chain(t_list *list, void *elem);
int			parse_cmd(t_env *env, t_user *user);
int			write_buff(t_buff *buff, char *str);
int			send_to_all(t_user *user, char *msg);
int			send_to_chan(t_chan *chan, char *msg);
int			set_nick(t_user *user, int fd, char *nick);
int			quit_chan(t_env *env, t_chan *chan, int fd);
int			prepare_connection(t_client *client, int port);
int			names_chan(t_chan *chan, t_user *user, int *first);
int			send_to_other(t_user *user, t_chan *chan, char *msg);
int			send_reply(t_user *user, char *src,
				   char *code, char *msg);
char			**my_strtowordtab(char *str, char sep);
void			free_tab(char **tab);
void			rm_list(t_list *list);
void			rm_buff(t_buff *buff);
void			rm_user(t_user *user);
void			rm_chan(t_chan *chan);
void			rm_chain(t_list *chain);
t_buff			*create_buff(void);
t_list			*create_list(void);
t_user			*create_user(int fd);
t_chan			*create_chan(char *name);
t_list			*getuserbyfd(t_list *list, int fd);
t_list			*getuserbynick(t_list *list, char *nick);
t_list			*getchanbyname(t_list *list, char *name);

#endif /* !MYIRC_H_ */
