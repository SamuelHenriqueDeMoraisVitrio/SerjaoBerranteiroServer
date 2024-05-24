#include "../dependencies/CWebStudio.h"
#include "../dependencies/LuaCEmbed.h"

CwebNamespace cb;
LuaCEmbedNamespace lw;


int luaopen_serjao_berranteiro(lua_State *state) {
  cb = newCwebNamespace();
  lw = newLuaCEmbedNamespace();
  LuaCEmbed * l  = lua_n.newLuaLib(state,false);
  return 0;
}

