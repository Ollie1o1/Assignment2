CC = gcc
CFLAGS = -std=c99 -Wall -Iinclude
BIN_DIR = bin
SRC_DIR = src
INC_DIR = include

EXEC = myContacts1341067

all: $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC): contactsA2.o createContactsA2.o readContactsA2.o searchContactsA2.o
	$(CC) $(CFLAGS) $^ -o $@

contactsA2.o: $(SRC_DIR)/contactsA2.c $(INC_DIR)/givenA2.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/contactsA2.c

createContactsA2.o: $(SRC_DIR)/createContactsA2.c $(INC_DIR)/givenA2.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/createContactsA2.c

readContactsA2.o: $(SRC_DIR)/readContactsA2.c $(INC_DIR)/givenA2.h $(INC_DIR)/helper.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/readContactsA2.c

searchContactsA2.o: $(SRC_DIR)/searchContactsA2.c $(INC_DIR)/givenA2.h $(INC_DIR)/helper.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/searchContactsA2.c

clean:
	del /Q *.o
	del /Q "$(BIN_DIR)\$(EXEC)"

.PHONY: all clean