CC = gcc
CFLAGS = -Wall -O3 -g

OBJS = main.o metodo1.o metodo2.o metodo3.o leitura_e_armazenamento.o auxialiares.o utils.o limpeza.o 

all: ortografia

ortografia: $(OBJS)
	$(CC) $(CFLAGS) -o ortografia $(OBJS)

main.o: main.c projeto.h
	$(CC) $(CFLAGS) -c main.c

metodo1.o: metodo1.c projeto.h
	$(CC) $(CFLAGS) -c metodo1.c

metodo2.o: metodo2.c projeto.h 
	$(CC) $(CFLAGS) -c metodo2.c

metodo3.o: metodo3.c projeto.h 
	$(CC) $(CFLAGS) -c metodo3.c

leitura_e_armazenamento.o: leitura_e_armazenamento.c projeto.h
	$(CC) $(CFLAGS) -c leitura_e_armazenamento.c

auxialiares.o: auxialiares.c projeto.h
	$(CC) $(CFLAGS) -c auxialiares.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

limpeza.o: limpeza.c projeto.h
	$(CC) $(CFLAGS) -c limpeza.c

clean:
	rm -f *.o ortografia
