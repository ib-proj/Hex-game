CFLAGS =  -std=c99 -c -ggdb 
COMP=cc
GSL_PATH?=/usr/local/gsl


build: precommit server test players

precommit: 
	chmod +x ./src/scripts/install-hooks.bash
	./src/scripts/install-hooks.bash

server: server.o graph.o pile.o graph_aux.o
	$(COMP) -std=c99 -I ${GSL_PATH}/include -rdynamic --coverage -g -O0 -L ${GSL_PATH}/lib -o install/server graph.o pile.o graph_aux.o server.o -lgsl -lgslcblas -lm -ldl



test: test_graph.o graph.o pile.o graph_aux.o
	$(COMP) -std=c99  -I ${GSL_PATH}/include -L ${GSL_PATH}/lib   -o install/alltests graph.o test_graph.o pile.o graph_aux.o -lgsl -lgslcblas -lm -ldl
	$(COMP)  -std=c99 -I ${GSL_PATH}/include  -L ${GSL_PATH}/lib  --coverage  src/graph.c src/test_graph.c pile.o graph_aux.o -lgsl -lgslcblas -lm -ldl



alltests: server.o graph.o pile.o graph_aux.o
	$(COMP) -std=c99  -I ${GSL_PATH}/include -L ${GSL_PATH}/lib   -o install/alltests graph.o server.o pile.o graph_aux.o -lgsl -lgslcblas -lm -ldl
	$(COMP)  -std=c99 -I ${GSL_PATH}/include  -L ${GSL_PATH}/lib  --coverage  src/graph.c src/server.c pile.o graph_aux.o -lgsl -lgslcblas -lm -ldl

server.o: src/server.c
	$(COMP) $(CFLAGS) -I ${GSL_PATH}/include   -o server.o src/server.c

graph.o: src/graph.c
	$(COMP) $(CFLAGS) -I ${GSL_PATH}/include  -o graph.o src/graph.c 

graph_aux.o: src/graph_aux.c
	$(COMP) $(CFLAGS) -I ${GSL_PATH}/include  -o graph_aux.o src/graph_aux.c 

pile.o: src/pile.c
	$(COMP) $(CFLAGS) -I ${GSL_PATH}/include  -o pile.o src/pile.c 


test_graph.o: src/test_graph.c
	$(COMP) $(CFLAGS) -I ${GSL_PATH}/include   -o test_graph.o src/test_graph.c

players: player1.so player2.so

player1.so: 
	cc -std=c99  -fPIC -g -I ${GSL_PATH}/include -shared src/graph_aux.c src/player1.c src/graph.c src/pile.c -lm -lgsl -lgslcblas -ldl -L${GSL_PATH}/lib -L${GSL_PATH}/lib64 -o install/player1.so

player2.so:
	cc -std=c99  -fPIC -g -I${GSL_PATH}/include -shared src/graph_aux.c src/player2.c src/graph.c src/pile.c -lm -lgsl -lgslcblas -ldl -L${GSL_PATH}/lib -L${GSL_PATH}/lib64 -o install/player2.so



mem_check:
	#valgrind  --leak-check=full  install/alltests 
	valgrind  --leak-check=full  install/server -m 8

install: server players

show:
	install/server -p 1

clean:	
	rm -rf *.o server 
	rm -f install/server
	rm -f install/alltests install/*.so 
	rm  -f *gcov
	rm -f *gcno
	rm -f *gcda
	rm -f a.out
	rm -rf install/*.dSYM
	
