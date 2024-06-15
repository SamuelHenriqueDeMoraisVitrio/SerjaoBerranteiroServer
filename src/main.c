#include "main.h"

CwebHttpResponse *main_sever(CwebHttpRequest *request) {
  cbrq = request;
  create_request(l);
  lw.evaluate(l, "serverresponse,server_status_code  = server_callback(request_main_server)");

  if (lw.has_errors(l)) {
    char *error = lw.get_error_message(l);
    printf("%s\n", error);
    lw.clear_errors(l);
    return cb.response.send_text("Interno server error", 500);
  }

  int status_code = 200;
  int status_type=lw.globals.get_type(l,"server_status_code");

  if(status_type != lw.types.NILL) {
      status_code = lw.globals.get_long(l,"server_status_code");
  }

  if (lw.has_errors(l)) {
      char *error = lw.get_error_message(l);
      printf("%s\n", error);
     return cb.response.send_text("Interno server error", 500);
  }

  int response_type = lw.globals.get_type(l, "serverresponse");


  if (response_type == lw.types.STRING) {
     char *value = lw.globals.get_string(l, "serverresponse");
     return cb.response.send_text(value, status_code);
  }

  if (response_type == lw.types.TABLE) {
     LuaCEmbedTable *table = lw.globals.get_table(l, "serverresponse");


     if(its_a_component(table)) {
         TextOrError content = render_component_raw(table);
         if(content.error){
             printf("error:%s\n",content.error->string_val);
             private_LuaCEmbedResponse_free(content.error);
             return cb.response.send_text("Interno server error", 500);
         }
         return cb.response.send_var_html_cleaning_memory(content.text,status_code);
     }


     if(lw.tables.get_type_prop(table,"response_pointer") == lw.types.NUMBER) {
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
     cJSON *parsed = lua_fluid_json_dump_table_to_cJSON(table);
     CwebHttpResponse * response =  cb.response.send_cJSON_cleaning_memory(parsed,status_code);
      return response;
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
