#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define SERVPORT "6789"

int main(){
	int sockfd;
	int rv;
	struct addrinfo hints, *servinfo, *p;
	int numbytes;
	int yes = 1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	if((rv = getaddrinfo(NULL, SERVPORT, &hints, &servinfo)) != 0){
		fprintf(strerr, "getaddinfo: %s\n", gai_strerror(rv));
		exit(EXIT_FAILURE);
	}

	for(p = servinfo; p != NULL; p = p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("server: socket\n");
			continue;
		}
		if((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) != 0){
			perror("server: setsockopt\n");
			exit(EXIT_FAILURE);
		}
		if((bind(sockfd, p->ai_addr, p->ai_addrlen)) != 0){
			perror("server: bind\n");
			close(sockfd);
			continue;
		}
		break;
	}
}
