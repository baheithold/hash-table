OBJS = integer.o real.o string.o hashmap.o da.o sll.o test-hashmap.o
EXECS = test-hashmap
OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -g

all: 	$(OBJS) test-hashmap

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
hashmap.o: 	hashmap.c hashmap.h da.h sll.h
		gcc $(OOPTS) hashmap.c

###############################################################################
# 																		TEST
test-hashmap.o: 	test-hashmap.c hashmap.c hashmap.h sll.c sll.h integer.c \
					integer.h real.c real.h string.c string.h
		gcc $(OOPTS) ./test-hashmap.c

test-hashmap: 	$(OBJS)
		gcc $(LOPTS) $(OBJS) -o test-hashmap

test: 	test-hashmap
		clear
		@echo Testing...
		@./test-hashmap

###############################################################################
# 																		VALGRIND
valgrind: 	test-hashmap
		clear
		@echo Testing with Valgrind...
		@valgrind ./test-hashmap

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
