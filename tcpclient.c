#include<stdio.h> // fprintf(), printf(),
#include<sys/socket.h> // getaddrinfo(), AF_UNSPEC, SOCK_STREM, gai_strerror(), socket(), connect(),
#include<sys/types.h> // getaddrinfo(), AF_UNSPEC, SOCK_STREM, gai_strerror(), socket(), connect(),
#include<netdb.h> // getaddrinfo(), gai_strerror(),
#include<stdlib.h> // exit(), EXIT_FAILURE, EXIT_SUCCESS,
#include<string.h> // memset(),
#include<arpa/inet.h> // inet_ntop(),
#include<unistd.h> // close(),
#include<errno.h>
#include<netinet/in.h>

#define SERVPORT "6789"
#define MAXDATASIZE 100

void *get_inet_addr(struct sockaddr *sa_i){
	if((sa_i->sa_family) == AF_INET){
		return &(((struct sockaddr_in *)sa_i)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)sa_i)->sin6_addr);
}

int main(int argc, char *argv[]){
	struct addrinfo hints, *servinfo ,*p;
	int status;
	int clientfd;
	char str_serv_addr[INET6_ADDRSTRLEN];
	char buf[MAXDATASIZE];
	int byte_recv;

	if(argc != 2){
		fprintf(stderr, "tcpclient: Usage: ./tcpclient <host address>");
		exit(EXIT_FAILURE);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((status = getaddrinfo(argv[1], SERVPORT, &hints, &servinfo)) != 0){
		fprintf(stderr, "tcpclient: getaddrinfo\n%s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	for(p = servinfo; p != NULL; p = p->ai_next){
		if((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("tcpclient : socket\n");
			continue;
		}

		if((connect(clientfd, p->ai_addr, p->ai_addrlen)) != 0){
			perror("tcpclient : connect\n");
			close(clientfd);
			continue;
		}
		break;
	}

	if(p == NULL){
		fprintf(stderr, "tcpclient : failed to connect\n");
		exit(EXIT_FAILURE);
	}

	inet_ntop(p->ai_family, get_inet_addr((struct sockaddr *)p->ai_addr), str_serv_addr, sizeof(str_serv_addr));

	printf("tcpclient: connecting to server at %s\n", str_serv_addr);

	freeaddrinfo(servinfo);

	if((byte_recv = recv(clientfd, buf, sizeof(buf), 0)) == -1){
		perror("tcpclient: recv\n");
		close(clientfd);
		exit(EXIT_FAILURE);
	}

	buf[byte_recv] = '\0';

	printf("server msg: %s\n", buf);

	
	if((send(clientfd, "Hello server!", 200, 0)) == -1){
		perror("tcpclient: send\n");
		close(clientfd);
		exit(EXIT_FAILURE);
	}

	close(clientfd);

	exit(EXIT_SUCCESS);
}
