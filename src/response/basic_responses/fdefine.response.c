//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end


LuaCEmbedResponse *send_raw(LuaCEmbed *args) {

  int firsr_type = LuaCEmbed_get_arg_type(args, 0);
  char *content_type = LuaCEmbed_get_str_arg(args, 1);

  unsigned short status_code = 200;
  if (LuaCEmbed_get_arg_type(args, 2) == LUA_CEMBED_NUMBER) {
    status_code = LuaCEmbed_get_long_arg(args, 2);
  }

  if (LuaCEmbed_has_errors(args)) {
    char *erro_msg = LuaCEmbed_get_error_message(args);
    return LuaCEmbed_send_error(erro_msg);
  }
  CwebHttpResponse *response = NULL;

  if (firsr_type == LUA_CEMBED_STRING) {
    lua_Integer  size;
    char *value = LuaCEmbed_get_raw_str_arg(args, &size, 0);
    response = cweb_send_any(content_type, size, (unsigned char *)value,
                                    status_code);
  } else if (firsr_type == LUA_TUSERDATA) {
    LuaCEmbed_generate_arg_clojure_evalation(
        args, 0, "function(value) raw_response = value:read('a') end ");
    if (LuaCEmbed_has_errors(args)) {
      char *erro_msg = LuaCEmbed_get_error_message(args);
      return LuaCEmbed_send_error(erro_msg);
    }
    long size;
    char *value = LuaCEmbed_get_global_raw_string(args, "raw_response", &size);
    if (LuaCEmbed_has_errors(args)) {
      char *erro_msg = LuaCEmbed_get_error_message(args);
      return LuaCEmbed_send_error(erro_msg);
    }
    response = cweb_send_any(content_type, size, (unsigned char *)value,
                                    status_code);
  }

  else {
    return LuaCEmbed_send_error("undefined type\n");
  }

  LuaCEmbedTable *table = LuaCembed_new_anonymous_table(args);
  LuaCEmbedTable_set_long_prop(table, "response_pointer", (serjao_ptr_cast)response);
  LuaCEmbedTable_set_bool_prop(table, "its_a_reference", false);
  LuaCEmbedTable_set_method(table, "__gc", clear_memory_response);
  return LuaCEmbed_send_table(table);
}
//------
LuaCEmbedResponse *send_file(LuaCEmbed *args) {
  char *rout_file = LuaCEmbed_get_str_arg(args, 0);

  unsigned short status_code = 200;
  if (LuaCEmbed_get_arg_type(args, 1) == LUA_CEMBED_NUMBER) {
    status_code = LuaCEmbed_get_long_arg(args, 1);
  }

  if (LuaCEmbed_has_errors(args)) {
    char *erro_msg = LuaCEmbed_get_error_message(args);
    return LuaCEmbed_send_error(erro_msg);
  }
  const char *content_type = NULL;
  if (LuaCEmbed_get_arg_type(args, 2) != LUA_CEMBED_NIL) {
    content_type = LuaCEmbed_get_str_arg(args, 2);
  }

  if (LuaCEmbed_has_errors(args)) {
    char *erro_msg = LuaCEmbed_get_error_message(args);
    return LuaCEmbed_send_error(erro_msg);
  }

  CwebHttpResponse *response =
      cweb_send_file(rout_file, content_type, status_code);

  LuaCEmbedTable *table = LuaCembed_new_anonymous_table(args);
  LuaCEmbedTable_set_long_prop(table, "response_pointer", (serjao_ptr_cast)response);
  LuaCEmbedTable_set_bool_prop(table, "its_a_reference", false);
  LuaCEmbedTable_set_method(table, "__gc", clear_memory_response);
  return LuaCEmbed_send_table(table);
}
//--------
LuaCEmbedResponse *send_text(LuaCEmbed *args) {

  const char *text = LuaCEmbed_get_str_arg(args, 0);

  unsigned short status_code = 200;
  if (LuaCEmbed_get_arg_type(args, 1) == LUA_CEMBED_NUMBER) {
    status_code = LuaCEmbed_get_long_arg(args, 1);
  }

  if (LuaCEmbed_has_errors(args)) {
    char *erro_msg = LuaCEmbed_get_error_message(args);
    return LuaCEmbed_send_error(erro_msg);
  }

  CwebHttpResponse *response = cweb_send_text(text, status_code);

  LuaCEmbedTable *table = LuaCembed_new_anonymous_table(args);
  LuaCEmbedTable_set_long_prop(table, "response_pointer", (serjao_ptr_cast)response);
  LuaCEmbedTable_set_bool_prop(table, "its_a_reference", false);
  LuaCEmbedTable_set_method(table, "__gc", clear_memory_response);
  return LuaCEmbed_send_table(table);
}

LuaCEmbedResponse *clear_memory_response(LuaCEmbedTable *self,
                                         LuaCEmbed *args) {

  bool its_a_refe = LuaCembedTable_get_bool_prop(self, "its_a_reference");

  if (!its_a_refe) {
    CwebHttpResponse *response =
        (CwebHttpResponse *)(serjao_ptr_cast)LuaCembedTable_get_long_prop(self, "response_pointer");
    CwebHttpResponse_free(response);
  }

  return NULL;
}
