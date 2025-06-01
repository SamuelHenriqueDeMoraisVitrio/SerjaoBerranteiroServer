#include "imports/imports.fdefine.h"


int serjao_berranteiro_start_point(lua_State *state) {
  serjao_lua_state = newLuaCEmbedLib(state);

  calbback_main();

  commandsHTML();

  creat_table_for_config_server();

  return LuaCembed_perform(serjao_lua_state);
}
