/* #define				NDEBUG */
#include		"../include/dbg.h"
#include		"../include/defs.h"


int		main(int argc, char **argv)
{
  int		port;
  int		listen_fd = -1;

  check_args(argc == 2 && (port = atoi(argv[1])) , "Usage %s host port", argv[0]);
  listen_fd = start_listen(port);
  check(listen_fd != -1, "start_listen failed");
  init_epoll(listen_fd);
  start_server_loop();
  check(close(listen_fd) != -1, "Closing server fd");
  return (EXIT_SUCCESS);
 error:
  if (listen_fd != -1)
    close(listen_fd);
  return (EXIT_FAILURE);
}
