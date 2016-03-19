#include "socket.h"

int libsocketaio_register_socket(const int socket, const unsigned char socket_type, 
								callback_t accept_callback, callback_t recv_callback, 
								callback_t rst_callback)
{
	struct sockaddr_in addr;
	struct socket_evt_bind *bind;
	
	int len;
	int res;

	if(socket_type != SOCK_STREAM)
	{
		return -1;
	}
	
	len = sizeof(struct sockaddr_in);
	
	res = getsockname(socket, (struct sockaddr *)&addr, &len);
	
	if(res < 0)
	{
		return -1;
	}
	
	if(addr.sin_family != AF_INET)
	{
		return -1;
	}
	
	bind = xmalloc(sizeof(struct socket_evt_bind));
	
	bind->sin_addr = addr.sin_addr.s_addr;
	bind->sin_port = addr.sin_port;
	bind->sin_family = addr.sin_family;
	bind->sin_type = socket_type;
	
	bind->sockFD = socket;
	
	bind->accept_callback = accept_callback;
	bind->recv_callback = recv_callback;
	bind->rst_callback = rst_callback;
	
	res = engine_register_bind_struct(bind);
	
	if(res)
	{
		free(bind);
		
		return 1;
	}
	
	return 0;
}