#include		<server/defs.h>
#include		<dbg.h>
#define		SLEEP_TIME 1000

char		new_client_callback(int client_fd)
{
  push_client_write(client_fd);
  write(1, "hello from client callback\n", strlen("hello from client callback\n"));
  usleep(SLEEP_TIME);
  return (EXIT_SUCCESS);
}

char		delete_client_callback(int client_fd)
{
  write(1, "Deleting client ...\n", strlen("Deleting client ...\n"));
  usleep(SLEEP_TIME);
  return (EXIT_SUCCESS);
}

char		send_event_callback(int client_fd)
{
  write(client_fd, "BIENVENUE\n", strlen("BIENVENUE\n"));
  usleep(SLEEP_TIME);
  pop_client_write(client_fd);
  return (EXIT_SUCCESS);
}

char		rcv_event_callback(int client_fd)
{
  int		r;
  char		buff[1024];

  r = read(client_fd, buff, 1023);
  check(r != -1, "server_loop");
  debug("read of %i", r);  //**********DEBUG
  buff[r] = '\0';
  write(1, buff, strlen(buff));
  usleep(SLEEP_TIME);
  return (EXIT_SUCCESS);
 error:
  return (EXIT_FAILURE);;
}
