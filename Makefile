include ../Make.defines

PROGS =  \
pfipcli01\
pfipsrv01\
tcpstrcli02\
tcpstrsrv02\
tcpcli03\
tcpsrv03\
udpcli04\
udpsrv04		

all:	${PROGS}

pfipcli01:	pfipcli01.o
		${CC} ${CFLAGS} -o $@ pfipcli01.o ${LIBS}

pfipsrv01:	pfipsrv01.o
		${CC} ${CFLAGS} -o $@ pfipsrv01.o ${LIBS}

tcpstrcli02:	tcpstrcli02.o
		${CC} ${CFLAGS} -o $@ tcpstrcli02.o ${LIBS}

tcpstrsrv02:	tcpstrsrv02.o sigchldwaitpid.o
		${CC} ${CFLAGS} -o $@ tcpstrsrv02.o sigchldwaitpid.o ${LIBS}

sigchldwaitpid.o:

tcpcli03:	tcpcli03.o
		${CC} ${CFLAGS} -o $@ tcpcli03.o ${LIBS}

tcpsrv03:	tcpsrv03.o
		${CC} ${CFLAGS} -o $@ tcpsrv03.o ${LIBS}

udpcli04:	udpcli04.o
		${CC} ${CFLAGS} -o $@ udpcli04.o ${LIBS}

udpsrv04:	udpsrv04.o
		${CC} ${CFLAGS} -o $@ udpsrv04.o ${LIBS}


clean:
		rm -f ${PROGS} ${CLEANFILES}
