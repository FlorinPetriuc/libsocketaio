#include "main.h"

void accept_cb(const int sockFD)
{
	printf("accept on %d", sockFD);
	fflush(stdout);
}

void recv_cb(const int sockFD)
{
	printf("recv on %d", sockFD);
	fflush(stdout);
}

void rst_cb(const int sockFD)
{
	printf("rst on %d", sockFD);
	fflush(stdout);
}

int main(void)
{
	int sock;
	int res;
	
	struct sockaddr_in addr;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock < 0)
    {
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(80);

    if(bind(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_in))<0)
    {
        return 1;
    }
	
	listen(sock, 10);
	
	res = engine_init(8);
	if(res)
	{
		printf("engine init failed");
		return 1;
	}
	
	register_socket(sock, SOCK_STREAM, accept_cb, NULL, NULL);
	
	while(1)
	{
		sleep(1);
	}
}