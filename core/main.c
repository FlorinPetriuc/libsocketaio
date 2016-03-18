#include "main.h"

int main(void)
{
	int sock;
	
	struct sockaddr_in addr;
	
	sock=socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock < 0)
    {
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(0);

    if(bind(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_in))<0)
    {
        return 1;
    }
	
	register_socket(sock, SOCK_STREAM);
}