#include<stdio.s> // fprintf(),
#include<sys/socket.h> // getaddrinfo(), AF_UNSPEC, SOCK_STREM, gai_strerror()
#include<sys/types.h> // getaddrinfo(), AF_UNSPEC, SOCK_STREM, gai_strerror()
#include<netdb.h> // getaddrinfo(), gai_strerror(),
#include<stdlib.h> // exit(), EXIT_FAILURE, EXIT_SUCCESS,
#include<string.h> // memset(),

#define SERVPORT "6789"

int main(int argc, char *argv){
	struct addrinfo hints, *servinfo;
	int status;

	if(argc != 2){
		fprintf(stderr, "tcpclient: Usage: ./tcpclient <host address>");
		exit(EXIT_FAILURE);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREM;

	if((status = getaddrinfo(argv[1], SERVPORT, &hints, &servinfo)) != 0){
		fprintf(stderr, "tcpclient: getaddrinfo\n%s\n", gai_strerror(status));
		exit(EXIT_SUCCESS);
	}
}
