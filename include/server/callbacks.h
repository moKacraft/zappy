#ifndef			CALLBACKS_H_H
# define		CALLBACKS_H_H

char		new_client_callback(int client_fd);
char		delete_client_callback(int client_fd);
char		send_event_callback(int client_fd);
char		rcv_event_callback(int client_fd);
#endif
