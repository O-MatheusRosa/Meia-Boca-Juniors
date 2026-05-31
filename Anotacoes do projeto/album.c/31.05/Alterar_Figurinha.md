# Função: Alterar_Figurinha (O Painel de Colagem)

**Arquivo de Origem:** album.c
**Responsável:** Permitir que o usuário pesquise uma carta no monte e cole ela no álbum manualmente.

## O que foi feito
Implementada a Opção 4 do menu. O sistema agora permite buscar cartas específicas e possui travas de segurança para evitar corrupção lógica (como colar uma carta que já está colada).

## Como funciona a engrenagem
1. Limpeza de Input: Usa fgets e strcspn para ler o nome do teclado sem o lixo do \n.
2. Trava Preventiva: Após achar a carta, verifica se colada == 1. Se for, o sistema bloqueia a ação avisando que a carta já está no fichário. Se for 0, libera o menu de 1-Sim / 0-Não e altera a variável na struct.