#include "dependencies/CWebStudio.h"
#include "dependencies/LuaCEmbed.h"

CwebNamespace cb;
LuaCEmbedNamespace lw;
LuaCEmbed *l;
CwebHttpRequest *cbrq;
bool singleprocesses = false;

#include "request/request.h"
#include "request/request.c"


CwebHttpResponse *main_sever(CwebHttpRequest *request) {
  cbrq = request;
  create_request(l);
  lw.evaluate(l, "serverresponse = server_callback(request_main_server)");

  if (lw.has_errors(l)) {
    char *error = lw.get_error_message(l);
    printf("\n\t%s\n\n", error);
    return cb.response.send_text("Interno server error", 500);
  }

  char *repost = lw.globals.get_string(l, "serverresponse");

  return cb.response.send_text(repost, 200);
}

LuaCEmbedResponse *initserver(LuaCEmbed *arg) {
  unsigned short port = (unsigned short)lw.args.get_long(arg, 0);

  if (lw.args.get_type(arg, 2) != lw.types.NILL) { singleprocesses = lw.args.get_bool(arg, 2); }

  if (lw.has_errors(arg)) { return lw.response.send_error("Uninformed arguments"); }
  //
  const char *functionvalue = "function(value) server_callback = value end";
  lw.args.generate_arg_clojure_evalation(arg, 1, functionvalue);

  if (lw.has_errors(arg)) { return lw.response.send_error("Uninformed arguments"); }
  struct CwebServer server = newCwebSever(port, main_sever);

  //server.allow_cors = true ;
  //server.single_process = false;
  //server.client_timeout = 10;
  //server.max_queue =10000 maximo de fila
  //server.max_requests = 50


  server.single_process = singleprocesses;
  bool errorInit = cb.server.start(&server);

  return lw.response.send_bool(errorInit);
}

int serjao_berranteiro_start_point(lua_State *state) {
  cb = newCwebNamespace();
  lw = newLuaCEmbedNamespace();
  l = lw.newLuaLib(state, false);
  lw.add_callback(l, "initserver", initserver);
  return lw.perform(l);
}
