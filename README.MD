# Virtual Memory Simulator

Este projeto simula um sistema de memória virtual com:

- Tabela de páginas (Page Table)
- TLB (Translation Lookaside Buffer)
- Paginação com substituição (Aging / LRU aproximado)
- Simulação de memória física e BACKING STORE
- Estatísticas de desempenho (TLB hits e page faults)

---

# ⚙️ Compilação

No diretório raiz do projeto:
```bash
mingw32-make

```
Ou manualmente:

```bash
gcc -Wall -Wextra -O2 -std=c11 -Iinclude src/main.c src/memory.c src/page_table.c src/statistics.c src/tlb.c -o vm

```
# ▶️ Execução
1. Gerar dados (primeira vez)

Entre na pasta data e execute:

```bash
python generate_data.py

```
2. Rodar o simulador

Volte para a raiz do projeto:


```bash
cd ..

```
Execute com um dos arquivos de entrada:

```bash
vm.exe < data\addresses_random.txt

```
ou

```
vm.exe < data\addresses_location.txt

```
# 📊 Saída

O programa exibe:

Tradução de endereços lógicos → físicos
Valor na memória
Estatísticas de execução (TLB hits e page faults)