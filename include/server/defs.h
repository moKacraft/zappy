#ifndef		DEFS_H_
# define	DEFS_H_
# define	UNUSED(x) (void)(x)
# include			<unistd.h>
# include			<stdio.h>
# include			<stdlib.h>
# include			<strings.h>
# include			<string.h>
# include			<sys/types.h>
# include			<sys/socket.h>
# include			<netinet/in.h>
# include			<arpa/inet.h>
# include			<time.h>
# include			<errno.h>
# include			<sys/socket.h>
# include			<netdb.h>
#include			<sys/epoll.h>

typedef struct		s_listen
{
  struct sockaddr_in	s_in;
  socklen_t		size;
  int			port;
  int			socket_fd;
}			t_listen;

typedef struct		s_epoll
{
  struct epoll_event	event;
  struct epoll_event	*events;
  int			s;
  int			efd;
}			t_epoll;

/* void		start_socket(void); */
int		start_listen(int p);
char		 poll_for_client_events(void);
char		init_epoll(int listen_fd);
void		pop_client_write(int client_fd);
void		push_client_write(int client_fd);
void		clean_up_epoll();
#endif
