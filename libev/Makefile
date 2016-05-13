CC := armeb-unknown-linux-gnueabi-gcc

PRJ_PATH := $(shell pwd)

INC_PATH += -I$(PRJ_PATH)/include
LIB_PATH += -L$(PRJ_PATH)/lib
LIBS += -lm
LIBS += -lev
LDFLAGS = $(LIB_PATH)
CFLAGS = $(INC_PATH)

#CONSOLE = echo >&101
CONSOLE = echo >/dev/null 
DEBUG = echo >/dev/null 

all: hello

show_var:
	@$(DEBUG) PRJ_PATH=$(PRJ_PATH)
	@$(DEBUG) LDFLAGS=$(LDFLAGS)
	@$(DEBUG) LIBS=$(LIBS) 

hello: hello_libev.c show_var
	@$(CONSOLE) "Building target [$@]"
	$(CC) $< $(LDFLAGS) $(CFLAGS) $(LIBS) 
