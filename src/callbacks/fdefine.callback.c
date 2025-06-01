//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


void calbback_main() {
  LuaCEmbed_add_callback(serjao_lua_state, "server", initserver);
  LuaCEmbed_add_callback(serjao_lua_state, "desktop", initdesktop);
  LuaCEmbed_add_callback(serjao_lua_state, "send_file", send_file);
  LuaCEmbed_add_callback(serjao_lua_state, "send_html", send_HTML);
  LuaCEmbed_add_callback(serjao_lua_state, "send_json_string", send_json_string);
  LuaCEmbed_add_callback(serjao_lua_state, "send_json", send_json);
  LuaCEmbed_add_callback(serjao_lua_state, "send_raw", send_raw);
  LuaCEmbed_add_callback(serjao_lua_state, "send_text", send_text);
  LuaCEmbed_add_callback(serjao_lua_state, "component", create_component);
  LuaCEmbed_add_callback(serjao_lua_state, "fragment", create_fragment);
  LuaCEmbed_add_callback(serjao_lua_state, "kill", kill_server);
}
