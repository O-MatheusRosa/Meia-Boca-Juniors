all:
	gcc src/main.c src/arquivos.c src/album.c -I include -o jogo

run: all
	.\jogo.exe