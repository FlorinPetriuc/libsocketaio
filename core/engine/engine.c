#include "engine.h"

int engine_register_bind_struct(struct socket_evt_bind *bind)
{
	printf("adding to engine: %p / %d / %u / %hu / %hhu / %hhu / %p / %p / %p",
							bind, bind->sockFD, bind->sin_addr, bind->sin_port, bind->sin_family,
							bind->sin_type, bind->accept_callback, bind->recv_callback, bind->rst_callback);
							

	
	free(bind);
	
	return 0;
}