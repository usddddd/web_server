/*
 * =====================================================================================
 *
 *       Filename:  simple_server.c
 *
 *    Description:  simple server for experimenting with C and networking
 *
 *        Version:  1.0
 *        Created:  03/27/2013 20:44:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: Adam Fowler 
 *        Company: Columbia University 
 *
 * =====================================================================================
 */


/*A simple stream server from Beej's guide to network programming*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490" // port users will connect to

#define BACKLOG 10 // limit pending connections

void sigchld_handler(int s){
        while(waitpid(-1, NULL, WNOHANG) > 0);
}

// got sockaddr, ipv4 or ipv6

void *get_in_addr(struct sockaddr *sa){
    if(sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void){
        int sockfd, new_fd; // listen on sock_fd, new connections on new_fd
        struct addrinfo hints, *servinfo, *p;
        struct sockaddr_storage their_addr; // connectors adddress
        socklen_t sin_size;
        struct sigaction sa;
        int yes=1;
        char s[INET6_ADDRSTRLEN];
        int rv;

        memset(&hints, 0 , sizeof(hints));
        hints.ai_family=AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE; // use my ip

        if((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0){
            fprintf(stderr, "getaddrrinfo: %s\n",gai_strerror(rv));
            return 1;
        }

        // loop through all the results andbind to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next){
            if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
                perror("server:socket");
                continue;
            }

            if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
                perror("setsockopt");
                exit(1);
            }

            if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
                close(sockfd);
                perror("server: bind");
                continue;
            }

            break;

        }
        
        if(p == NULL){
            fprintf(stderr, "server: failed to bind\n");
            return 2;
        }


        freeaddrinfo(servinfo); // done with this structure

        if(listen(sockfd, BACKLOG) == -1){
            perror("listen");
            exit(1);
        }
        
        sa.sa_handler = sigchld_handler; // reap all dead processes
        sigemptyset(&sa.sa_mask);
        sa.sa_flags=SA_RESTART;
            
        if(sigaction(SIGCHLD, &sa, NULL) == -1){
            perror("sigaction");
            exit(1);
        }

        printf("server: waiting for connections...\n");

        while(1){// main accept loop
                sin_size =sizeof(their_addr);
                new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
                if(new_fd == -1){
                    perror("accept");
                    continue;
                }

                inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
                printf("server: got connection from %s\n", s);

                if(!fork()){// this is the child process
                    close(sockfd);//child doesnt need the listener
                    if(send(new_fd, "Hello, World!", 13, 0) == -1)
                        perror("send");
                    close(new_fd);
                    exit(0);
                }
                close(new_fd); // parent doesnt need this

        }

        return 0;

}


    
