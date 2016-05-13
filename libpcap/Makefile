CFLAGS+=
LIBS+=-lpcap
LDFLAGS+= $(LIBS)
CC=gcc
TARGET	=	sniffer
SRCS	=	sniffer.c
OBJS	=	$(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $<

.c:.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<
