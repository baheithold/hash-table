OBJS = integer.o real.o string.o htable.o
EXECS = test-htable
OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -g

all: 	$(OBJS) test-htable

###############################################################################
# 													Modules for Primitive Types
# 	INTEGER
integer.o: 	integer.h integer.c
		gcc $(OOPTS) integer.c

# 	REAL
real.o: 	real.h real.c
		gcc $(OOPTS) real.c

# 	STRING
string.o: 	string.h string.c
		gcc $(OOPTS) string.c

###############################################################################
# 																		DA
da.o: 	da.c da.h
		gcc $(OOPTS) da.c

###############################################################################
# 																		TEST

###############################################################################
# 																		VALGRIND

###############################################################################
# 																		CLEAN
clean:
		rm -f $(EXECS) *.o *.vgcore
