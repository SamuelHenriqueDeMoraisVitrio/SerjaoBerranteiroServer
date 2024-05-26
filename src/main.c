#include "dependencies/CWebStudio.h"
#include "dependencies/LuaCEmbed.h"

CwebNamespace cb;
LuaCEmbedNamespace lw;
LuaCEmbed *l;
CwebHttpRequest *cbrq;
bool singleprocesses = false;

LuaCEmbedResponse *returnHeadearsPro(LuaCEmbedTable *self, LuaCEmbed *args) {
  if (lw.args.get_type(args, 1) == lw.types.STRING) {
    char *keyvalue = lw.args.get_str(args, 1);

    char *value = cb.request.get_header(cbrq, keyvalue);

    if (value == NULL) { return NULL; }

    return lw.response.send_str(value);
  }

  if (lw.args.get_type(args, 1) == lw.types.NUMBER) {
    int index = (int)(lw.args.get_long(args, 1) - 1);

    if (index >= cbrq->headers->size) { return NULL; }

    char *value = cbrq->headers->keys_vals[index]->value;

    return lw.response.send_str(value);
  }

  return lw.response.send_error("Index incompatible");
}

CwebHttpResponse *main_sever(CwebHttpRequest *request) {
  cbrq = request;
  LuaCEmbedTable *tableServer = lw.globals.new_table(l, "request_main_server");
  lw.tables.set_string_prop(tableServer, "url", request->url);
  lw.tables.set_string_prop(tableServer, "route", request->route);
  lw.tables.set_string_prop(tableServer, "method", request->method);
  LuaCEmbedTable *tableHeaders = lw.tables.new_anonymous_table(l);
  lw.tables.set_sub_table_prop(tableServer, "header", tableHeaders);
  lw.tables.set_method(tableHeaders, "__index", returnHeadearsPro);
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
