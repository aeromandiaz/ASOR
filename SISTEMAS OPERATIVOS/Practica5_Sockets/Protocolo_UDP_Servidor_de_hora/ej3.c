#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>

#define BUF_SIZE 500

int
main(int argc, char *argv[])
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, s;
  struct sockaddr_storage peer_addr;
  socklen_t peer_addr_len;
  ssize_t nread;
  char buf[BUF_SIZE];

  if (argc != 2) {
    fprintf(stderr, "Usage: %s port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
  hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  s = getaddrinfo("::", argv[1], &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  /* getaddrinfo() returns a list of address structures.
  Try each address until we successfully bind(2).
  If socket(2) (or bind(2)) fails, we (close the socket
  and) try the next address. */

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1)
    continue;

    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
    break;                  /* Success */

    close(sfd);
  }

  if (rp == NULL) {               /* No address succeeded */
    fprintf(stderr, "Could not bind\n");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(result);           /* No longer needed */

  /* Read datagrams and echo them back to sender */

  for (;;) {
    fd_set fset;
    FD_ZERO(&fset);
    FD_SET(0,&fset);
    FD_SET(sfd, &fset);
    if(select(sfd+1, &fset, NULL, NULL, NULL)<=0) exit(0);
    if(FD_ISSET(sfd, &fset)){
      peer_addr_len = sizeof(struct sockaddr_storage);
      nread = recvfrom(sfd, buf, BUF_SIZE, 0,
        (struct sockaddr *) &peer_addr, &peer_addr_len);
      if (nread == -1)
      continue;               /* Ignore failed request */

      char host[NI_MAXHOST], service[NI_MAXSERV];

      s = getnameinfo((struct sockaddr *) &peer_addr,
      peer_addr_len, host, NI_MAXHOST,
      service, NI_MAXSERV, NI_NUMERICSERV);
      if (s == 0)
      printf("Received %ld bytes from %s:%s\n",
      (long) nread, host, service);
      else
      fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
    }else if(FD_ISSET(0,&fset)){
      nread = read(0, buf, 2);
      if(nread < 0) perror("Fallo al leer comando de stdin");
    }

    time_t raw;
    struct tm *tm;
    time(&raw);
    tm = localtime(&raw);

    if(nread==2)
      if(buf[0]=='t') nread = strftime(buf,BUF_SIZE,"%I:%M:%S\n", tm);
      else if(buf[0]=='d')nread = strftime(buf,BUF_SIZE,"%x\n", tm);
      else if(buf[0]=='q')exit(0);
    buf[nread]='\0';
    
    if(FD_ISSET(sfd, &fset)){
      if (sendto(sfd, buf, nread, 0, (struct sockaddr *) &peer_addr, peer_addr_len) != nread) fprintf(stderr, "Error sending response\n");
    }else if(FD_ISSET(0,&fset)) printf("%s\n",buf);
  }
}
