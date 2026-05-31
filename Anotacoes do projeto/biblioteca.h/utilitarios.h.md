# Atualizações Visuais (Perfumaria e UI)

**Arquivos de Origem:** album.c e utilitarios.h
**Responsável:** Melhorar a interface do usuário (TUI) com indicadores de status, cores e sons multiplataforma.

## O que foi feito
- Opção 1 (Mostrar Álbum): Colocamos um if/else no printf de listagem. Agora as cartas aparecem com [ X ] (se já estiverem coladas) ou [   ] (se estiverem soltas), além de mostrar a quantidade de repetidas na mesma linha.
- Cores e Beeps: Integramos o arquivo do nosso parceiro (utilitarios.c) ajustando o Makefile. Adicionamos constantes ANSI de cores (CIANO, VERDE, VERMELHO) direto nos textos e sons de sistema (Som_Sucesso, Som_Erro) blindados com macros #ifdef _WIN32 para rodar liso tanto no Windows quanto no Linux.