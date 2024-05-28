#include "dependencies/CWebStudio.h"
#include "dependencies/LuaCEmbed.h"

CwebNamespace cb;
LuaCEmbedNamespace lw;
LuaCEmbed *l;
CwebHttpRequest *cbrq;
LuaCEmbedTable *set_server;
bool singleprocesses = false;
#include "configuring_the_server/set_server.h"
#include "request/request.h"
#include "response/response.h"

#include "configuring_the_server/set_server.c"
#include "request/request.c"
#include "response/response.c"

int serjao_berranteiro_start_point(lua_State *state);
CwebHttpResponse *main_sever(CwebHttpRequest *request);
LuaCEmbedResponse *initserver(LuaCEmbed *arg);

LuaCEmbedResponse *send_text(LuaCEmbedTable *self, LuaCEmbed *args) {

  char *texto = lw.args.get_str(args, 1);
  if (lw.has_errors(args)) {
    return NULL;
  }

  return lw.response.send_str(texto);
}

CwebHttpResponse *main_sever(CwebHttpRequest *request) {
  cbrq = request;
  create_request(l);

  LuaCEmbedTable *sub_get_type_response = lw.tables.new_anonymous_table(l);
  LuaCEmbedTable *get_type_response = lw.tables.new_anonymous_table(l);
  lw.tables.set_sub_table_prop(get_type_response, "response",
                               sub_get_type_response);
  lw.tables.set_method(sub_get_type_response, "send_text", send_text);

  lw.evaluate(l, "serverresponse = server_callback(request_main_server)");

  if (lw.has_errors(l)) {
    char *error = lw.get_error_message(l);
    printf("%s\n",error);
    return cb.response.send_text("Interno server error", 500);
  }

  int response_type = lw.globals.get_type(l,"serverresponse");

  if(response_type == lw.types.STRING){
      char *value = lw.globals.get_string(l,"serverresponse");
      return cb.response.send_text(value, 200);
  }

  if(response_type == lw.types.TABLE){
    LuaCEmbedTable *table = lw.globals.get_table(l,"serverresponse");

    CwebHttpResponse* response_cb=  (CwebHttpResponse*)lw.tables.get_long_prop(table,"response_pointer");
    if (lw.has_errors(l)) {
      char *error = lw.get_error_message(l);
      printf("%s\n",error);
      return cb.response.send_text("Interno server error", 500);
    }

    return response_cb;
  }


  return NULL;

}

LuaCEmbedResponse *initserver(LuaCEmbed *arg) {
  unsigned short port = (unsigned short)lw.args.get_long(arg, 0);

  if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
  }
  //
  const char *functionvalue = "function(value) server_callback = value end";
  lw.args.generate_arg_clojure_evalation(arg, 1, functionvalue);

  if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
  }

  bool errorInit = true;
  short i = 3000;
  do {
    struct CwebServer serverTEMP = newCwebSever(port, main_sever);
    serverTEMP.function_timeout = 100;
    get_params_for_server_config(&serverTEMP);

    errorInit = cb.server.start(&serverTEMP);
    port = errorInit ? i : port;
    i++;
    if (i == 5000) {
      break;
    }
  } while (errorInit);

  return NULL;
}

int serjao_berranteiro_start_point(lua_State *state) {
  cb = newCwebNamespace();
  lw = newLuaCEmbedNamespace();
  l = lw.newLuaLib(state, false);
  lw.add_callback(l, "initserver", initserver);
  lw.add_callback(l,"send_file",send_file);
  creat_table_for_config_server();

  return lw.perform(l);
}

