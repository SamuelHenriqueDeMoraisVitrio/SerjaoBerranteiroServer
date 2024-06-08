#include "main.h"

CwebHttpResponse *main_sever(CwebHttpRequest *request) {
  cbrq = request;
  create_request(l);
  lw.evaluate(l, "serverresponse = server_callback(request_main_server)");

  if (lw.has_errors(l)) {
    char *error = lw.get_error_message(l);
    printf("%s\n", error);
    return cb.response.send_text("Interno server error", 500);
  }

  int response_type = lw.globals.get_type(l, "serverresponse");


  if (response_type == lw.types.STRING) {
     char *value = lw.globals.get_string(l, "serverresponse");
     return cb.response.send_text(value, 200);
  }

  if (response_type == lw.types.TABLE) {
     LuaCEmbedTable *table = lw.globals.get_table(l, "serverresponse");
     lw.tables.set_bool_prop(table, "its_a_reference", true);
     CwebHttpResponse *response_cb = (CwebHttpResponse *)lw.tables.get_long_prop(table, "response_pointer");

     if (lw.has_errors(l)) {
      char *error = lw.get_error_message(l);
      printf("%s\n", error);
      lw.clear_errors(l);

      return cb.response.send_text("Interno server error", 500);
    }


    return response_cb;
  }

  return NULL;
}

int serjao_berranteiro_start_point(lua_State *state) {
    cb = newCwebNamespace();
    lw = newLuaCEmbedNamespace();
    l = lw.newLuaLib(state);

    calbback_main();

    commandsHTML();

    creat_table_for_config_server();

  return lw.perform(l);
}
