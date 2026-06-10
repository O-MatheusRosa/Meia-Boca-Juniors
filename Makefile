
SRC = src/main.c src/arquivos.c src/album.c src/utilitarios.c src/textura.c


INCLUDES = -I include

ifeq ($(OS),Windows_NT)
	# --- SE FOR WINDOWS ---
	EXEC = jogo.exe
	RUN = .\$(EXEC)
	CLEAN = del $(EXEC)
	# Motor do Windows
	LIBS = -L lib -lraylib -lgdi32 -lwinmm
else
	# --- SE FOR LINUX (PCs da Facul / WSL) ---
	EXEC = jogo
	RUN = ./$(EXEC)
	CLEAN = rm -f $(EXEC)
	# Motor do Linux
	LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# 4. REGRAS DE COMPILAÇÃO
all:
	gcc $(SRC) $(INCLUDES) $(LIBS) -o $(EXEC)

run: all
	$(RUN)
	
clean:
	$(CLEAN)