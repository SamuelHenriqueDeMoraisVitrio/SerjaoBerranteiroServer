//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end


LuaCEmbedResponse *send_HTML(LuaCEmbed *args) {

  char *html_code = NULL;

  bool html_alocated = false;
  int type = LuaCEmbed_get_arg_type(args, 0);
  if (type == LUA_CEMBED_TABLE) {
    LuaCEmbedTable *html_component = LuaCEmbed_get_arg_table(args, 0);
    if (!its_a_component(html_component)) {
      return LuaCEmbed_send_error("invalid type");
    }
    TextOrError content = render_component_raw(html_component);
    if (content.error) {
      return content.error;
    }
    html_code = content.text;
    html_alocated = true;
  }

  else {
    html_code = LuaCEmbed_get_str_arg(args, 0);
  }

  short status_code = 200;
  if (LuaCEmbed_get_arg_type(args, 1) == LUA_CEMBED_NUMBER) {
    status_code = LuaCEmbed_get_long_arg(args, 1);
  }

  if (LuaCEmbed_has_errors(args)) {
    const char *msg_error = LuaCEmbed_get_error_message(args);
    return LuaCEmbed_send_error(msg_error);
  }

  CwebHttpResponse *response =
      cweb_send_var_html((char *)html_code, status_code);
  LuaCEmbedTable *table = LuaCembed_new_anonymous_table(args);
  
  LuaCEmbedTable_set_long_prop(table, "response_pointer", (serjao_ptr_cast)response);
  LuaCEmbedTable_set_bool_prop(table, "its_a_reference", false);
  LuaCEmbedTable_set_method(table, "__gc", clear_memory_response);
  if (html_alocated) {
    free(html_code);
  }
  return LuaCEmbed_send_table(table);
}
