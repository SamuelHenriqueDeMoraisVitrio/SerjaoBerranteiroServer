COMP = gcc
OPC = -shared -fpic -o
NOME = serjao_berranteiro/serjao_berranteiro.so
STRLUA ?= lua teste.lua
START = echo "" && echo "Compilação concluida. Iniciando..." && echo "" && echo "" && $(STRLUA) && echo ""
FILE ?= src/main.c

compile: clear
	$(COMP) $(OPC) $(NOME) $(FILE)
	@echo ""

all: clear
	$(COMP) $(OPC) $(NOME) $(FILE)
	@$(START)
	@echo ""

teste: clear
	@$(START)
	@echo ""

zip:
	@rm serjao_server.zip
	@zip -r serjao_server.zip serjao_berranteiro || echo "Error when compressing \"serjao_berranteiro\" directory"
	@echo ""
	@sleep 1
	@unzip -l serjao_server.zip || echo "Zip does not exist"
	@echo ""

clear:
	rm -rf .*.c.swp .*.h.swp .cache
	clear
	@echo ""
	@ls -alh --color=auto
	@echo ""
