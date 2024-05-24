#include "dependencies/CWebStudio.h"
#include "dependencies/LuaCEmbed.h"

CwebNamespace cb;
LuaCEmbedNamespace lw;


LuaCEmbedResponse *teste(LuaCEmbed *args){
  printf("aaaaaaaa\n");
  return NULL;

}
int serjao_berranteiro_start_point(lua_State *state) {
  cb = newCwebNamespace();
  lw = newLuaCEmbedNamespace();
  LuaCEmbed * l  = lw.newLuaLib(state,false);
  lw.add_callback(l,"teste",teste);

  return lw.perform(l);
}

