//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../src/imports/imports.fdeclare.h"
//silver_chain_scope_end


LuaCEmbedResponse *initserver(LuaCEmbed *arg) {
  unsigned short initport = (unsigned short)LuaCEmbed_get_long_arg(arg, 0);
  bool its_a_number = (LUA_CEMBED_NUMBER == LuaCEmbed_get_arg_type(arg, 1));

  const unsigned short lastport =
      its_a_number ? (unsigned short)LuaCEmbed_get_long_arg(arg, 1) : initport;
  unsigned short port = initport;

  if (LuaCEmbed_has_errors(arg)) {
    return LuaCEmbed_send_error("Uninformed arguments");
  }

  const char *functionvalue = "function(value) server_callback = value end";
  LuaCEmbed_generate_arg_clojure_evalation(arg, its_a_number ? 2 : 1,
                                         functionvalue);

  if (LuaCEmbed_has_errors(arg)) {
    return LuaCEmbed_send_error("Uninformed arguments");
  }

  bool errorInit = true;
  for (unsigned short i = initport; i <= lastport; i++) {
    struct CwebServer serverTEMP = newCwebSever(i, main_sever_handle);
    serverTEMP.function_timeout = 100;
    get_params_for_server_config(&serverTEMP);

    errorInit = CwebServer_start(&serverTEMP);
    if (errorInit) {
      continue;
    }

    port = i;
    break;
  }
  if (errorInit) {
    return LuaCEmbed_send_error("Não foi possivel usar das portas %hd a %hd.",
                                  initport, lastport);
  }
  return NULL;
}

CwebHttpResponse *main_sever_handle(CwebHttpRequest *request) {
  serjao_cweb_global_requiest = request;
  create_request(serjao_lua_state);
  LuaCEmbed_evaluate(serjao_lua_state, "serverresponse,server_status_code  = "
                 "server_callback(request_main_server)");

  if (LuaCEmbed_has_errors(serjao_lua_state)) {
    char *error = LuaCEmbed_get_error_message(serjao_lua_state);
    printf("%s\n", error);
    LuaCEmbed_clear_errors(serjao_lua_state);
    return cweb_send_text("Interno server error", 500);
  }

  int status_code = 200;
  
  int status_type = LuaCEmbed_get_global_type(serjao_lua_state, "server_status_code");

  if (status_type != LUA_CEMBED_NIL) {
    status_code = LuaCEmbed_get_global_long(serjao_lua_state, "server_status_code");
  }

  if (LuaCEmbed_has_errors(serjao_lua_state)) {
    char *error = LuaCEmbed_get_error_message(serjao_lua_state);
    printf("%s\n", error);
    LuaCEmbed_clear_errors(serjao_lua_state);

    return cweb_send_text("Interno server error", 500);
  }

  int response_type = LuaCEmbed_get_global_type(serjao_lua_state, "serverresponse");

  if (response_type == LUA_CEMBED_STRING) {
    char *value = LuaCEmbed_get_global_string(serjao_lua_state, "serverresponse");
    return cweb_send_text(value, status_code);
  }

  if (response_type == LUA_CEMBED_TABLE) {
    LuaCEmbedTable *table = LuaCembed_get_global_table(serjao_lua_state, "serverresponse");

    if (its_a_component(table)) {
      TextOrError content = render_component_raw(table);
      if (content.error) {
        printf("error:%s\n", content.error->string_val);
        private_LuaCEmbedResponse_free(content.error);
        return cweb_send_text("Interno server error", 500);
      }
      return cweb_send_var_html_cleaning_memory(content.text,
                                                       status_code);
    }

    if (LuaCEmbedTable_get_type_prop(table, "response_pointer") == LUA_CEMBED_NUMBER) {
      LuaCEmbedTable_set_bool_prop(table, "its_a_reference", true);
      CwebHttpResponse *response_cb =  (CwebHttpResponse *)(serjao_ptr_cast)LuaCembedTable_get_long_prop(table,"response_pointer");
      if (LuaCEmbed_has_errors(serjao_lua_state)) {
        char *error = LuaCEmbed_get_error_message(serjao_lua_state);
        printf("%s\n", error);
        LuaCEmbed_clear_errors(serjao_lua_state);

        return cweb_send_text("Interno server error", 500);
      }

      return response_cb;
    }
    cJSON *parsed = serjao_json_dump_table_to_cJSON(table);
    CwebHttpResponse *response =
        cweb_send_cJSON_cleaning_memory(parsed, status_code);
    return response;
  }

  return NULL;
}
