#include "../dependencies/CWebStudio.h"
#include "../dependencies/LuaCEmbed.h"

CwebNamespace cb;
LuaCEmbedNamespace lw;

CwebHttpResponse *main_server(CwebHttpRequest request) {
  return cb.response.send_text("Hello World", 200);
}

void startserver(unsigned short porta) {
  struct CwebServer server = newCwebSever(porta, main_sever);
  cb.server.start(&server);
}

int luaopen_main(lua_State *state) {
  cb = newCwebNamespace();
  lw = newLuaCEmbedNamespace();

  bool set_functions_as_public = false;
  LuaCEmbed *l = lua_n.newLuaLib(state, set_functions_as_public);

  return 0;
}
