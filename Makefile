SRC = src/main.c src/arquivos.c src/album.c src/utilitarios.c src/textura.c src/diltu.c src/utfpr.c src/padaria.c

INCLUDES = -I include

# O truque do professor para descobrir se � Windows ou Linux
ifdef OS
  OS_DETECTED := $(strip $(OS))
else
  OS_DETECTED := $(strip $(shell uname))
endif

ifeq ($(OS_DETECTED),Windows_NT)
    # --- SE FOR WINDOWS ---
    EXEC = jogo.exe
    RUN = .\$(EXEC)
    CLEAN = del $(EXEC)
    # Aponta para a pasta do Windows que o prof enviou
    LIBS = -L libwin -lraylib -lgdi32 -lwinmm
else
    # --- SE FOR LINUX (PCs da Facul / Ubuntu) ---
    EXEC = jogo
    RUN = ./$(EXEC)
    CLEAN = rm -f $(EXEC)
    # Aponta para a pasta do Linux que o prof enviou
    LIBS = -L lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# 4. REGRAS DE COMPILACAO
all:
	gcc $(SRC) $(INCLUDES) $(LIBS) -o $(EXEC)

run: all
	$(RUN)
    
clean:
	$(CLEAN)