#include<stdio.h> // perror(), fprintf(), 
#include<stdlib.h> // exit(),
#include<unistd.h> // close(),
#include<string.h> // strerror(), memset()
#include<errno.h> // errno,
#include<sys/socket.h> // AF_UNSPEC, AI_PASSIVE, SOCK_STREAM, SOL_SOCKET, SO_REUSEADDR, EXIT_FAILURE, EXIT_SUCCESS, getaddrinfo(), gai_strerror(), socket(), setsockopt(), bind(), freeaddrinfo(), listen(), accept(), send(), recv(),
#include<sys/types.h> // getaddrinfo(), gai_strerror(), socket(), setsockopt(), bind(), freeaddrinfo(), listen(), accept(), waitpid(), fork(), send(), recv(),
#include<netdb.h> // getaddrinfo(), gai_strerror(), freeaddrinfo(),
#include<arpa/inet.h> // INET6_ADDRSTRLEN, 
#include<netinet/in.h>
#include<sys/wait.h> // waitpid(), 
#include<signal.h> // 

#define BACKLOG 5
#define SERVPORT "6789"

void signal_handler_func(int signum){
	int saved_errno = errno;
	while((waitpid(-1, NULL, WNOHANG)) > 0);
	errno = saved_errno;
}

void *get_inet_addr(struct sockaddr *sa_i){
	if((sa_i->sa_family) == AF_INET){
		return &(((struct sockaddr_in *)sa_i)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)sa_i)->sin6_addr);
}

int main(){
	int sockfd;
	int conn_sockfd;
	int status;
	struct addrinfo hints, *servinfo, *p;
	int numbytes;
	int yes = 1;
	struct sigaction sa;
	struct sockaddr_storage client_addr;
	socklen_t client_addrlen;
	char *str_client_addr[INET6_ADDRSTRLEN];
	int is_chld_proc;
	char *buf;
	int byte_sent;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	if((status = getaddrinfo(NULL, SERVPORT, &hints, &servinfo)) != 0){
		fprintf(stderr, "getaddinfo: %s\n", gai_strerror(status));
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

	freeaddrinfo(servinfo);

	if(p == NULL){
		fprintf(stderr, "server: failed to bind\n");
		fprintf(stderr, "error msg: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if((listen(sockfd, BACKLOG)) != 0){
		perror("server: listen\n");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	sa.sa_handler = signal_handler_func;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if((sigaction(SIGCHLD, &sa, NULL)) != 0){
		perror("server: sigaction\n");
		exit(EXIT_FAILURE);
	}

	printf("server: waiting for connections...\n");

	client_addrlen = sizeof(client_addr);

	while(1){
		if((conn_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addrlen)) == -1){
			perror("server: accept\n");
			continue;
		}

		inet_ntop(client_addr.ss_family, get_inet_addr((struct sockaddr *)&client_addr), &str_client_addr, sizeof(str_client_addr));

		printf("server: Got connection from %s\n", &str_client_addr);

		is_chld_proc = !fork();
		if(is_chld_proc){
			close(sockfd);
			
			byte_sent = send(conn_sockfd, "Hello client, send a msg of less than 200 bytes!\n", 49, 0);
			if(byte_sent == -1){
				perror("server: send\n");
			}

			if((recv(conn_sockfd, &buf, 200, 0)) == -1){
				perror("server: recv\n");
			}

			close(conn_sockfd);
			printf("client msg: %s\n", buf);
			exit(EXIT_SUCCESS);
		}

		close(conn_sockfd);
	}
	close(sockfd);
	exit(EXIT_SUCCESS);
}
