#include			<server/defs.h>
#include			<dbg.h>
#define				SOCKET_ERROR -1

static char			init_socket(t_listen *v)
{
  struct protoent		*s_p;

  s_p = getprotobyname("TCP");
  check(s_p, "init_socket");
  v->socket_fd = socket(AF_INET, SOCK_STREAM, s_p->p_proto);
  check(v->socket_fd != -1, "init_socket");
  return (EXIT_SUCCESS);
 error:
  return(EXIT_FAILURE);
}

static char			do_bind(t_listen *v)
{
  int				result;

  v->size = sizeof(v->s_in);
  memset(&v->s_in, 0, v->size);
  v->s_in.sin_family = AF_INET;
  v->s_in.sin_port = htons(v->port);
  v->s_in.sin_addr.s_addr = INADDR_ANY;
  result = bind(v->socket_fd, (struct sockaddr *)&v->s_in, v->size);
  check(result != -1, "do_bind");
  return (EXIT_SUCCESS);
 error:
  close(v->socket_fd);
  return(EXIT_FAILURE);
}

static char			do_listen(t_listen *v)
{
  int				result;

  result = listen(v->socket_fd, SOMAXCONN);
  check(result != -1, "do_listen");
  return (EXIT_SUCCESS);
 error:
  close(v->socket_fd);
  return(EXIT_FAILURE);
}

int				start_listen(int p)
{
  t_listen			v;

  v.port = p;
  if ((init_socket(&v))
      || do_bind(&v)
      || do_listen(&v))
    return (SOCKET_ERROR);
  return (v.socket_fd);
}
