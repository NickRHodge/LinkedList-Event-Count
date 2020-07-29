CFLAGS = -D_POSIX_SOURCE
LDFLAGS =
CC = cc
LD = cc

TARG = a1

OBJS = a1.o

$(TARG): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARG)

clean:
	rm -f $(OBJS) $(TARG) core a.out
