GCC=gcc
CFLAGS =-Wall -g -DUSE_SLEEP -lpcap
SRC=$(wildcard *.c)
PROGS=$(patsubst %.c,%,$(SRC))
#OBJS = $(SRC:.c=.o)

%: %.c
	$(GCC) $(CFLAGS+) $< $(LIBS) -o $@ 

all: $(PROGS)

clean:	
	rm -f *.core $(PROGS) *~

build:
	$(GCC) $(CFLAGS) sniff-filter.c -o server


