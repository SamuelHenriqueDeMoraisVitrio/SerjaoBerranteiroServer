//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


void get_params_for_server_config(struct CwebServer *struct_server_TEMP) {
  #ifdef __linux__
  struct_server_TEMP->single_process =
      LuaCembedTable_get_bool_prop(serjao_server_config, "single_process");
    #endif
  struct_server_TEMP->allow_cors =
      LuaCembedTable_get_bool_prop(serjao_server_config, "allow_cors");
  struct_server_TEMP->use_static =
      LuaCembedTable_get_bool_prop(serjao_server_config, "use_folder");
  struct_server_TEMP->static_folder =
      LuaCembedTable_get_string_prop(serjao_server_config, "static_folder");
  struct_server_TEMP->function_timeout =
      LuaCembedTable_get_long_prop(serjao_server_config, "function_timeout");
  struct_server_TEMP->client_timeout =
      LuaCembedTable_get_long_prop(serjao_server_config, "client_timeout");
  struct_server_TEMP->max_queue =
      LuaCembedTable_get_long_prop(serjao_server_config, "max_queue");
  struct_server_TEMP->max_requests =
      LuaCembedTable_get_long_prop(serjao_server_config, "max_request");

}

void creat_table_for_config_server() {
  serjao_server_config = LuaCembed_new_global_table(serjao_lua_state, "set_server");
  LuaCEmbedTable_set_bool_prop(serjao_server_config, "single_process", false);
  LuaCEmbedTable_set_bool_prop(serjao_server_config, "allow_cors", true);
  LuaCEmbedTable_set_bool_prop(serjao_server_config, "use_folder", true);
  LuaCEmbedTable_set_string_prop(serjao_server_config, "static_folder", "static");
  LuaCEmbedTable_set_long_prop(serjao_server_config, "function_timeout", 30);
  LuaCEmbedTable_set_long_prop(serjao_server_config, "client_timeout", 5);
  LuaCEmbedTable_set_long_prop(serjao_server_config, "max_queue", 100);
  LuaCEmbedTable_set_long_prop(serjao_server_config, "max_request", 1000);
  LuaCEmbedTable_set_string_prop(serjao_server_config, "nullterminator", "NULL");
}
