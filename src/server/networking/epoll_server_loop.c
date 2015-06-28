#include		<stdbool.h>
#include		<dbg.h>
#include		<server/defs.h>
#include		<server/callbacks.h>

#define			MAXEVENTS 64

static int Slisten_fd;
static t_epoll *Sepoll;
#define			CLIENT_IS_READY_FOR_READ Sepoll->events[i].events & EPOLLIN
#define			FD_HAS_ERROR_OR_DISCONNECT Sepoll->events[i].events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)
#define			CLIENT_IS_READY_FOR_WRITE Sepoll->events[i].events & EPOLLOUT
#define			NEW_CLIENT_CONNECTION client_fd == Slisten_fd

void				clean_up_epoll()
{
  check(Slisten_fd && Sepoll, "You need to call init_epoll before starting the loop");
  if (Sepoll->events)
    {
      free(Sepoll->events);
      Sepoll->events = NULL;
    }
  if (Sepoll)
    {
      free(Sepoll);
      Sepoll = NULL;
    }
  return ;
 error:
  log_err("trying to free uninitialized epoll");
}

void				push_client_write(int client_fd)
{
  Sepoll->event.data.fd = client_fd;
  Sepoll->event.events = EPOLLIN | EPOLLRDHUP | EPOLLOUT;
  epoll_ctl(Sepoll->efd, EPOLL_CTL_MOD, client_fd, &Sepoll->event);
  return ;
}

void				pop_client_write(int client_fd)
{
  Sepoll->event.data.fd = client_fd;
  Sepoll->event.events = EPOLLIN | EPOLLRDHUP;
  epoll_ctl(Sepoll->efd, EPOLL_CTL_MOD, client_fd, &Sepoll->event);
  return ;
}

static void			add_new_client()
{
  int			infd;
  struct sockaddr_in	client_sin;
  socklen_t		client_sin_len;

  client_sin_len = sizeof(client_sin);
  infd = accept(Slisten_fd, (struct sockaddr *)&client_sin, &client_sin_len);
  Sepoll->event.data.fd = infd;
  Sepoll->event.events = EPOLLIN | EPOLLRDHUP;
  epoll_ctl(Sepoll->efd, EPOLL_CTL_ADD, infd, &Sepoll->event);
  printf("New client\n");
  new_client_callback(infd);
  debug("accepted client with fd %i", infd);  //**********DEBUG
  return ;
}

static void			delete_client(int client_fd)
{
  if (client_fd == -1)
    close(client_fd);
  epoll_ctl (Sepoll->efd, EPOLL_CTL_DEL, client_fd, &Sepoll->event);
  delete_client_callback(client_fd);

  return ;
}

char			init_epoll(int listen_fd)
{
  t_epoll	*epoll;

  epoll = calloc (MAXEVENTS, sizeof(t_epoll));
  check_mem(epoll);
  epoll->efd = epoll_create1(0);
  check(epoll->efd != -1, "init_epoll");
  epoll->event.data.fd = listen_fd;
  epoll->event.events = EPOLLIN;
  epoll->s = epoll_ctl (epoll->efd, EPOLL_CTL_ADD, listen_fd, &epoll->event);
  check(epoll->s != -1, "init_epoll");
  epoll->events = calloc(MAXEVENTS, sizeof(epoll->event));
  check_mem(epoll->events);
  Slisten_fd = listen_fd;
  Sepoll = epoll;
  return (EXIT_SUCCESS);
 error:
  if (epoll->s != -1)
    close(epoll->s);
  if (epoll->efd != -1)
    close(epoll->efd);
  if (epoll->events)
    free(epoll->events);
  return (EXIT_FAILURE);
}

char		poll_for_client_events(void)
{
  int		i;
  int		client_fd;
  int		events;

  check(Slisten_fd && Sepoll, "You need to call init_epoll before starting the loop");
  events = epoll_wait (Sepoll->efd, Sepoll->events, MAXEVENTS, -1);
  for(i = 0; i < events; ++i)
    {
      client_fd = Sepoll->events[i].data.fd;
      debug("%i", client_fd);
      if (FD_HAS_ERROR_OR_DISCONNECT)
	delete_client(client_fd);
      else if (NEW_CLIENT_CONNECTION)
	add_new_client();
      else if (CLIENT_IS_READY_FOR_READ)
	rcv_event_callback(client_fd);/* read_request(client_fd); */
      else if (CLIENT_IS_READY_FOR_WRITE)
	send_event_callback(client_fd);
      else
	log_info("Client fd ready for something but didnt got catched by ifs Event: %i FD: %i", events, client_fd);
    }
  clean_up_epoll();
  return (EXIT_SUCCESS);
 error:
  clean_up_epoll();
  return (EXIT_FAILURE);
}
