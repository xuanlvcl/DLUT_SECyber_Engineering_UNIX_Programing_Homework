#include "unp.h"

int
main(int argc, char** argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	int maxfdp1;
	fd_set rset;
	char sendline[MAXLINE], recvline[MAXLINE];

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9999);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	Connect(sockfd, (SA*)&servaddr, sizeof(servaddr));


	FD_ZERO(&rset);
	for (; ; ) {
		FD_SET(fileno(stdin), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(stdin), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);
		if (FD_ISSET(sockfd, &rset)) { /* socket is readable */
			if (Readline(sockfd, recvline, MAXLINE) == 0)
				err_quit("str_cli: server terminated prematurely");
			Fputs(recvline, stdout);
		}
		if (FD_ISSET(fileno(stdin), &rset)) { /* input is readable */
			if (Fgets(sendline, MAXLINE, stdin) == NULL)
				return 0; /* all done */
			Writen(sockfd, sendline, strlen(sendline));
		}
	}


	exit(0);
}
