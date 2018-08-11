CFLAGS = gcc -c -g -Wall
OFLAGS = gcc -o
VFLAGS = --tool=memcheck --leak-check=yes --show-reachable=yes
BIN = ./../bin/
OBJ = ./../obj/
SRC = ./../src/
INC = ./../include/

$(BIN)server : $(OBJ)server.o $(OBJ)server_functions.o $(OBJ)functions_other.o $(OBJ)register_function.o $(OBJ)sign_in_authentication.o $(OBJ)syncserverfunction.o
	$(OFLAGS) $(BIN)server $(OBJ)server.o $(OBJ)server_functions.o $(OBJ)functions_other.o $(OBJ)register_function.o $(OBJ)sign_in_authentication.o $(OBJ)syncserverfunction.o
	mv server $(BIN)

$(OBJ)server.o : $(SRC)server.c $(INC)header.h
	$(CFLAGS) $(SRC)server.c -I $(INC)
	mv server.o $(OBJ)

$(OBJ)server_functions.o : $(SRC)server_functions.c $(INC)header.h
	$(CFLAGS) $(SRC)server_functions.c -I $(INC)
	mv server_functions.o $(OBJ) 

$(OBJ)functions_other.o : $(SRC)functions_other.c $(INC)header.h
	$(CFLAGS) $(SRC)functions_other.c -I $(INC)
	mv functions_other.o $(OBJ) 

$(OBJ)register_function.o : $(SRC)register_function.c $(INC)header.h
	$(CFLAGS) $(SRC)register_function.c -I $(INC)
	mv register_function.o $(OBJ)

$(OBJ)sign_in_authentication.o : $(SRC)sign_in_authentication.c $(INC)header.h
	$(CFLAGS) $(SRC)sign_in_authentication.c -I $(INC)
	mv sign_in_authentication.o $(OBJ)

$(OBJ)syncserverfunction.o : $(SRC)syncserverfunction.c $(INC)header.h
	$(CFLAGS) $(SRC)syncserverfunction.c -I $(INC)
	mv syncserverfunction.o $(OBJ)

valgrind : $(BIN)server
	valgrind $(VFLAGS) $(BIN)server

clean : 
	rm $(BIN)server
	rm $(OBJ)*.o

run:
	$(BIN)server

