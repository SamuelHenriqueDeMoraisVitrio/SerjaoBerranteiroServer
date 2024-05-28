#include "dependencies/CWebStudio.h"
#include "dependencies/LuaCEmbed.h"
#include <stdio.h>
#include <time.h>

CwebNamespace cb;
LuaCEmbedNamespace lw;
LuaCEmbed *l;
CwebHttpRequest *cbrq;
LuaCEmbedTable *set_server;
bool singleprocesses = false;

#include "configuring_the_server/set_server.c"
#include "configuring_the_server/set_server.h"
#include "request/request.c"
#include "request/request.h"

int serjao_berranteiro_start_point(lua_State *state);
CwebHttpResponse *main_sever(CwebHttpRequest *request);
LuaCEmbedResponse *initserver(LuaCEmbed *arg);

CwebHttpResponse *main_sever(CwebHttpRequest *request) {
  cbrq = request;
  create_request(l);

  LuaCEmbedTable *create_response_type = lw.tables.new_anonymous_table(l);
  LuaCEmbedTable *create_sub_response = lw.tables.new_anonymous_table(l);
  lw.tables.set_sub_table_prop(create_response_type, "response",
                               create_sub_response);
  lw.tables.set_string_prop(create_sub_response, "send_string", "nill");

  char *valor_de_response =
      lw.tables.get_string_prop(create_sub_response, "send_string");

  lw.evaluate(l, "serverresponse = server_callback(request_main_server)");

  if (lw.has_errors(l)) {
    char *error = lw.get_error_message(l);
    printf("\n\t%s\n\n", error);
    return cb.response.send_text("Interno server error", 500);
  }

  char *repost = lw.globals.get_string(l, "serverresponse");

  return cb.response.send_text(valor_de_response, 200);
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
  creat_table_for_config_server();

  return lw.perform(l);
}
