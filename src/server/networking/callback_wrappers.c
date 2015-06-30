#include		<dbg.h>
#include		<server/defs.h>

static int			error_event(client_data_t *data)
{
  if (client_fd == -1)
    close(client_fd);
  epoll_ctl (Sepoll->efd, EPOLL_CTL_DEL, client_fd, &Sepoll->event);
  delete_client_callback(client_fd);
  return ;
}


static char			read_event(client_data_t *data)
{
  if (NEW_CLIENT_CONNECTION)
    add_new_client();
}

static int			write_event(client_data_t *data)
{
}

int			send_response(const int client_fd, static *response)
{
}

static void		push_client_write(const int client_fd)
{
  Sepoll->event.data.fd = client_fd;
  Sepoll->event.events = EPOLLIN | EPOLLRDHUP | EPOLLOUT;
  epoll_ctl(Sepoll->efd, EPOLL_CTL_MOD, client_fd, &Sepoll->event);
  return ;
}

static void		pop_client_write(const int client_fd)
{
  Sepoll->event.data.fd = client_fd;
  Sepoll->event.events = EPOLLIN | EPOLLRDHUP;
  epoll_ctl(Sepoll->efd, EPOLL_CTL_MOD, client_fd, &Sepoll->event);
  return ;
}
