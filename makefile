OBJS = integer.o real.o string.o htable.o da.o sll.o test-htable.o
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
# 																		SLL
sll.o: 	sll.c sll.h
		gcc $(OOPTS) sll.c

###############################################################################
# 																		HTABLE
htable.o: 	htable.c htable.h da.h sll.h
		gcc $(OOPTS) htable.c

###############################################################################
# 																		TEST
test-htable.o: 	test-htable.c htable.c htable.h sll.c sll.h integer.c \
				integer.h real.c real.h string.c string.h
		gcc $(OOPTS) ./test-htable.c

test-htable: 	$(OBJS)
		gcc $(LOPTS) $(OBJS) -o test-htable

test: 	test-htable
		clear
		@echo Testing...
		@./test-htable

###############################################################################
# 																		VALGRIND
valgrind: 	test-htable
		clear
		@echo Testing with Valgrind...
		@valgrind ./test-htable

###############################################################################
# 																		CLEAN
clean:
		@echo Cleaning...
		@rm -f $(EXECS) *.o *.vgcore

###############################################################################
# 																		REBUILD
rebuild:
		@make clean -s;
		@echo Rebuilding...
		@make -s
