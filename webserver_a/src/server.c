#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
  int sock_srv, socket_cli;
  struct sockaddr_in srv;
  struct sockaddr_in cli;
  socklen_t socklen;
  int i = 1;

  if ( (sock_srv = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // Initialize
  memset(&srv, 0, sizeof(srv));
  srv.sin_family = AF_INET;
  srv.sin_port = htons(4000);
  srv.sin_addr.s_addr = INADDR_ANY;

  socklen = sizeof(srv);
  if ( (bind(sock_srv, (struct sockaddr *)&srv, socklen)) < 0 ) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if( listen(sock_srv, 1) < 0 ) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  puts("TCP/IP socket available");
  printf("\taddr %s\n", inet_ntoa(srv.sin_addr));
  printf("\tport %d\n", ntohs(srv.sin_port));

  if ( (socket_cli = accept(sock_srv, (struct sockaddr *)&srv, &socklen)) < 0 ) {
    close(sock_srv);
  }
  puts("New connection granted\n");

  while(1) {
    int req, res;
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    req = read(socket_cli, buff, sizeof(buff));
    printf("req: %s\n", buff);

    if(strncmp(buff, "quit", sizeof(buff)) == 0) {
      break;
    }

    printf("res: ");
    scanf("%s", buff);

    res = write(socket_cli, buff, sizeof(buff));

    if(strncmp(buff, "quit", sizeof(buff)) == 0) {
      break;
    }
  }

  close(sock_srv);

  exit(EXIT_SUCCESS);
}
