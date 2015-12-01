COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	proj.c video_gr.c vbe.c sprite.c keyboard.c timer.c
SRCS+=	Bitmap.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>