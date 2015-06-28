/* #define				NDEBUG */
#include		<dbg.h>
#include		<server/defs.h>


int		main(int argc, char **argv)
{
  int		port;
  int		listen_fd = -1;
  char		is_running = 1;

  check_args(argc == 2 && (port = atoi(argv[1])) , "Usage %s host port", argv[0]);
  listen_fd = start_listen(port);
  check(listen_fd != -1, "start_listen failed");
  init_epoll(listen_fd);
  while(is_running)
    {
      poll_for_client_events();
      debug("Tick");
    }
  check(close(listen_fd) != -1, "Closing server fd");
  clean_up_epoll();
  return (EXIT_SUCCESS);
 error:
  if (listen_fd != -1)
    close(listen_fd);
  clean_up_epoll();
  return (EXIT_FAILURE);
}
