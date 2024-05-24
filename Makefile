COMP = clang
# OPC = -std=c17
NOME = EXE
START = echo "" && echo "Compilação concluida. Iniciando..." && echo "" && echo "" && ./$(NOME) && echo ""
FILE ?= main.c

all: clear
	$(COMP) $(OPC) -o $(NOME) $(FILE)
	@$(START)
	@echo ""

compile: clear
	$(COMP) $(OPC) -o $(NOME) $(FILE)

clear:
	@rm -rf EXE .*.c.swp .*.h.swp .cache
	clear
	@echo ""
	@ls -alh --color=auto
	@echo ""

run:
	@$(START)
	@echo ""
