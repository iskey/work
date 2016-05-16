INC_PATH+= -I./libpcap/include
LIB_PATH+= -L./libpcap/lib
CFLAGS+=$(INC_PATH)
LIBS+= -Wl,-Bstatic -lpcap -Wl,-Bdynamic
LDFLAGS+= $(INC_PATH) $(LIBS) $(LIB_PATH)
CC=armeb-unknown-linux-gnueabi-gcc
TARGET	=	sniffer
SRCS	=	sniffer.c
OBJS	=	$(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $< $(LDFLAGS) -o $@

.c:.o
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -fvr $(OBJS)
	@rm -fvr $(TARGET)
