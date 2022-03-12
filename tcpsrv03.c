#include "unp.h"
int
main(int argc, char** argv)
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;
	int nready, client[FD_SETSIZE];
	int port = 9999;
	ssize_t n;
	fd_set rset, wset, allset;
	char readLine[MAXLINE], sendLine[MAXLINE], show[MAXLINE], buff[MAXLINE];
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);
	FILE* fp = stdin;
	maxfd = listenfd; /* initialize */
	maxi = -1; /* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1; /* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	FD_SET(fileno(fp), &allset);
	/* end fig01 */
	/* include fig02 */
	for (; ; ) {
		rset = allset; /* structure assignment */
		nready = Select(maxfd + 1, &rset, &wset, NULL, NULL);
		if (FD_ISSET(listenfd, &rset)) { /* new client connection */
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA*)&cliaddr, &clilen);
			printf("new client: %s, port %d\n",
				Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
				ntohs(cliaddr.sin_port));

#ifdef NOTDEF
			printf("new client: %s, port %d\n",
				Inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
				ntohs(cliaddr.sin_port));
#endif
			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd; /* save descriptor */
					break;
				}
			if (i == FD_SETSIZE)
				err_quit("too many clients");
			FD_SET(connfd, &allset); /* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd; /* for select */
			if (i > maxi)
				maxi = i; /* max index in client[] array */
			if (--nready <= 0)
				continue; /* no more readable descriptors */
		}
		for (i = 0; i <= maxi; i++) { /* check all clients for data */
			if ((sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				if ((n = Read(sockfd, readLine, MAXLINE)) == 0) {
					/*4connection closed by client */
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				}
				else {
					readLine[n]=0;
					strcpy(show, "client: ");
					strcat(show, readLine);
					Fputs(show, stdout);

				}
				if (--nready <= 0)
					break; /* no more readable descriptors */
			}

			if (FD_ISSET(fileno(fp), &rset)) { /* input is readable */
				if (Fgets(sendLine, MAXLINE, fp) != NULL) /* all done */
					Writen(sockfd, sendLine, strlen(sendLine));
			}
		}
	}
}
