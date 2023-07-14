CC := gcc

SRC := hack.c
OBJ := $(patsubst %.c, %.o, $(SRC))
LIB := $(patsubst %.c,lib%.so, $(SRC))

FILE := sample.cry
KEY  := easy


all: $(LIB)


$(OBJ): $(SRC)
	$(CC) -m32 -fpic -c $(SRC) -o $(OBJ)

$(LIB): $(OBJ)
	$(CC) -m32 -shared $(OBJ) -o $(LIB)


run: $(LIB)
	@LD_PRELOAD=$(shell pwd)/$(LIB) ./docrypt $(FILE) $(KEY)

check:
	@./check.sh

clean:
	$(RM) $(OBJ) $(LIB)

.PHONY: check clean
