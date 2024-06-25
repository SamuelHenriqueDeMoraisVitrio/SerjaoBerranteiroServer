COMPILATOR = gcc
OPTIONS = -shared -fpic -o
END = serjao_berranteiro/serjao_berranteiro.so
FILE = src/main.c
CONFIG_LUA := config.mk

-include $(CONFIG_LUA)

LUA ?= teste.lua
RUN = echo "" && echo "" && echo "Compilação concluida. Executando..." && echo "" && lua $(LUA) && echo ""
ROUTERUN = cd /home/samuel/Documentos/proc/SerjaoBerranteiroServer/ && $(RUN)
g ?= ""
gt ?= ""

teste: clear
	@$(ROUTERUN)

comp: clear
	$(COMPILATOR) $(OPTIONS) $(END) $(FILE)

all: clear
	@echo ""
	$(COMPILATOR) $(OPTIONS) $(END) $(FILE)
	@$(ROUTERUN)

zip: clear
	@rm serjao_server.zip
	@zip -r serjao_server.zip serjao_berranteiro
	@echo ""
	@zip -T serjao_server.zip -v
	@echo ""
	@unzip -l serjao_server.zip
	@echo ""

clear:
	clear
	@ls --color=always -alh
	@echo ""

#TRocar o nome de forma permanente do teste2.lua

set_lua:
	@echo LUA=$(LUA) > $(CONFIG_LUA)
	@echo "$(LUA)"

#Para mexer com git

git: add
	@git push
	@echo ""

add:
ifeq ($(gt),)
	@git add .
	@git commit -m "$(g)"
	@echo ""
else
	@git add .
	@git commit -m "$(g)" -m "$(gt)"
	@echo ""
endif