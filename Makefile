
FLAGS = -Wall -Wextra -Wno-unused-result -Wno-unused-parameter -g

DEPS = horario.h
OBJ = horario.o main.o

LIBED_DEPS = $(patsubst %.c,%.o,$(wildcard ./src/ed/*.c)) 
LIBSEARCH_DEPS = $(patsubst %.c,%.o,$(wildcard ./src/search/*.c)) 

all: main

%.o: %.c %.h
	gcc $(FLAGS) -c -o $@ $< 

libed.a: $(LIBED_DEPS)
	ar -crs libed.a $(LIBED_DEPS)

libsearch.a: $(LIBSEARCH_DEPS)
	ar -crs libsearch.a $(LIBSEARCH_DEPS)

main: main.c libed.a libsearch.a 
	gcc $(FLAGS) -o main main.c -I src/search -I src/ed -L . -led -lsearch -lm

clean:
	rm -f main libed.a libsearch.a $(LIBSEARCH_DEPS) $(LIBED_DEPS)
	
run:
	./main

