#include "socket.h"

int register_socket(const int socket, const unsigned char socket_type)
{
	struct sockaddr_in addr;
	
	int len;
	int res;
	
	in_addr_t ip;
	
	unsigned short int port;
	
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
	
	ip = addr.sin_addr.s_addr;
	port = addr.sin_port;
		
	return 0;
}