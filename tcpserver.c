#include<stdio.h> // perror(), fprintf(), 
#include<stdlib.h> // exit(),
#include<unistd.h> // close(),
#include<string.h> // strerror(), memset()
#include<errno.h> // errno,
#include<sys/socket.h> // AF_UNSPEC, AI_PASSIVE, SOCK_STREAM, SOL_SOCKET, SO_REUSEADDR, EXIT_FAILURE, EXIT_SUCCESS, getaddrinfo(), gai_strerror(), socket(), setsockopt(), bind(), freeaddrinfo(), listen(),
#include<sys/types.h> // getaddrinfo(), gai_strerror(), socket(), setsockopt(), bind(), freeaddrinfo(), listen(),
#include<netdb.h> // getaddrinfo(), gai_strerror(), freeaddrinfo(),
#include<arpa/inet.h>
#include<netinet/in.h>

#define BACKLOG 5
#define SERVPORT "6789"

void signal_handler_func(int signum){
	int saved_errno = errno;
	while((waitpid(-1, NULL, WOHANG)) > 0);
	errno = saved_errno;
}

int main(){
	int sockfd;
	int status;
	struct addrinfo hints, *servinfo, *p;
	int numbytes;
	int yes = 1;
	struct sigaction sa;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	if((status = getaddrinfo(NULL, SERVPORT, &hints, &servinfo)) != 0){
		fprintf(strerr, "getaddinfo: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	for(p = servinfo; p != NULL; p = p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("server: socket\n");
			continue;
		}
		if((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) != 0){
			perror("server: setsockopt\n");
			fprintf(strerr, "error msg: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		if((bind(sockfd, p->ai_addr, p->ai_addrlen)) != 0){
			perror("server: bind\n");
			close(sockfd);
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo);

	if(p == NULL){
		fprintf(strerr, "server: failed to bind\n");
		fprintf(strerr, "error msg: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if((listen(sockfd, BACKLOG)) != 0){
		perror("server: listen\n");
		fprintf(strerr, "error msg: %s\n", strerror(errno));
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	sa.sa_handler = signal_handler_func;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if((sigaction(SIGCHLD, &sa, NULL)) != 0){
		perror("server: sigaction\n");
		fprintf(strerr, "error msg: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}
