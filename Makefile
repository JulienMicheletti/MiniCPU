CC = gcc

LIBS = -lm

SRCS = miniCPU.c

OBJS = $(SRCS:.c=.o)

MAIN = miniCPU

.PHONY: depend clean

all:  $(MAIN)
	@echo  "Everything has been compiled"

$(MAIN): $(OBJS) 
	$(CC) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
