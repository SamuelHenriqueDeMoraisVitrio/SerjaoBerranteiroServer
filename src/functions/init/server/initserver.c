
#include "../uniq.definitions_requirements.h"

LuaCEmbedResponse *initserver(LuaCEmbed *arg) {
  unsigned short initport = (unsigned short)lw.args.get_long(arg, 0);
  bool its_a_number = (lw.types.NUMBER == lw.args.get_type(arg, 1));

  const unsigned short lastport =
      its_a_number ? (unsigned short)lw.args.get_long(arg, 1) : initport;
  unsigned short port = initport;

  if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
  }

  const char *functionvalue = "function(value) server_callback = value end";
  lw.args.generate_arg_clojure_evalation(arg, its_a_number ? 2 : 1,
                                         functionvalue);

  if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
  }

  bool errorInit = true;
  for (unsigned short i = initport; i <= lastport; i++) {
    struct CwebServer serverTEMP = newCwebSever(i, main_sever);
    serverTEMP.function_timeout = 100;
    get_params_for_server_config(&serverTEMP);

    errorInit = cb.server.start(&serverTEMP);
    if (errorInit) {
      continue;
    }

    port = i;
    break;
  }
  if (errorInit) {
    return lw.response.send_error("Não foi possivel usar das portas %hd a %hd.",
                                  initport, lastport);
  }
  return NULL;
}
