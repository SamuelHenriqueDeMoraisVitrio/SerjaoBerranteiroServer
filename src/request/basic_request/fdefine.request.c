//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end


LuaCEmbedResponse *setHP_index(LuaCEmbed *args, CwebDict *hp,
                               char *(*get_unified_param)(CwebHttpRequest *,
                                                          const char *)) {

  if (LuaCEmbed_get_arg_type(args, 1) == LUA_CEMBED_STRING) {
    char *keyvalue = LuaCEmbed_get_str_arg(args, 1);
    char *value = get_unified_param(serjao_cweb_global_requiest, keyvalue);
    if (value == NULL) {
      return NULL;
    } // value == NULL

    return LuaCEmbed_send_str(value);
  } // if main

  if (LuaCEmbed_get_arg_type(args, 1) == LUA_CEMBED_NUMBER) {
    int index = (int)(LuaCEmbed_get_long_arg(args, 1) - 1);

    if (index >= hp->size) {
      return NULL;
    } // if index

    const char *value = hp->keys_vals[index]->value;
    const char *key = hp->keys_vals[index]->key;

    LuaCEmbedTable *tableKeys_vals = LuaCembed_new_anonymous_table(serjao_lua_state);
    LuaCEmbedTable_set_string_prop(tableKeys_vals, "key", key);
    LuaCEmbedTable_set_string_prop(tableKeys_vals, "value", value);
    return LuaCEmbed_send_table(tableKeys_vals);
  } // if main

  return LuaCEmbed_send_error("The index type is not compatible");
} // function

LuaCEmbedResponse *setHeaders(LuaCEmbedTable *self, LuaCEmbed *args) {
  return setHP_index(args, serjao_cweb_global_requiest->headers, CwebHttpRequest_get_header);
}

LuaCEmbedResponse *setParams(LuaCEmbedTable *self, LuaCEmbed *args) {
  return setHP_index(args, serjao_cweb_global_requiest->params, CwebHttpRequest_get_param);
}

void create_request(LuaCEmbed *args) {
  LuaCEmbedTable *tableServer = LuaCembed_new_global_table(args, "request_main_server");
  LuaCEmbedTable_set_string_prop(tableServer, "url", serjao_cweb_global_requiest->url);
  LuaCEmbedTable_set_string_prop(tableServer, "route", serjao_cweb_global_requiest->route);
  LuaCEmbedTable_set_string_prop(tableServer, "method", serjao_cweb_global_requiest->method);
  LuaCEmbedTable_set_string_prop(tableServer, "ip", serjao_cweb_global_requiest->client_ip);
  LuaCEmbedTable_set_long_prop(tableServer, "content_length", serjao_cweb_global_requiest->content_length);
  LuaCEmbedTable_set_long_prop(tableServer, "content_error", serjao_cweb_global_requiest->content_error);
  LuaCEmbedTable_set_long_prop(tableServer, "socket", serjao_cweb_global_requiest->socket);

  LuaCEmbedTable_set_method(tableServer, "read_body", read_raw_body);
  LuaCEmbedTable_set_method(tableServer, "read_json_body", read_json_from_table);

  LuaCEmbedTable *tableHeaders = LuaCembed_new_anonymous_table(args);
  LuaCEmbedTable_set_long_prop(tableHeaders, "size", serjao_cweb_global_requiest->headers->size);

  LuaCEmbedTable *tableParams = LuaCembed_new_anonymous_table(args);
  LuaCEmbedTable_set_long_prop(tableParams, "size", serjao_cweb_global_requiest->params->size);

  LuaCEmbedTable_set_sub_table_prop(tableServer, "header", tableHeaders);
  LuaCEmbedTable_set_sub_table_prop(tableServer, "params", tableParams);

  LuaCEmbedTable_set_method(tableParams, "__index", setParams);
  LuaCEmbedTable_set_method(tableHeaders, "__index", setHeaders);
}
