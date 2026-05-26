
SRC = src/main.c src/arquivos.c src/album.c
INCLUDES = -I include

ifeq ($(OS),Windows_NT)
	# Configurações para Windows
	EXEC = jogo.exe
	RUN = .\$(EXEC)
	CLEAN = del $(EXEC)
else
	# Configurações para Linux / Mac
	EXEC = jogo
	RUN = ./$(EXEC)
	CLEAN = rm -f $(EXEC)
endif

all:
	gcc $(SRC) $(INCLUDES) -o $(EXEC)

run: all
	$(RUN)

clean:
	$(CLEAN)