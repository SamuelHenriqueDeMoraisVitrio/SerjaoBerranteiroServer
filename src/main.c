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

    CwebHttpResponse *response_cb =
        (CwebHttpResponse *)lw.tables.get_long_prop(table, "response_pointer");
    if (lw.has_errors(l)) {
      char *error = lw.get_error_message(l);
      printf("%s\n", error);
      return cb.response.send_text("Interno server error", 500);
    }

    return response_cb;
  }

  return NULL;
}

LuaCEmbedResponse *initserver(LuaCEmbed *arg) {
  unsigned short initport = (unsigned short)lw.args.get_long(arg, 0);
  const unsigned short lastport = (unsigned short)lw.args.get_long(arg, 1);
  unsigned short port = initport;

  if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
  }

  const char *functionvalue = "function(value) server_callback = value end";
  lw.args.generate_arg_clojure_evalation(arg, 2, functionvalue);

  if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
  }

  bool errorInit = true;
  for(unsigned short i = initport; i <= lastport; i++){
      struct CwebServer serverTEMP = newCwebSever(i, main_sever);
      serverTEMP.function_timeout = 100;
      get_params_for_server_config(&serverTEMP);

      errorInit = cb.server.start(&serverTEMP);
      if(errorInit){
          continue;
      }
      if(!errorInit){
          port = i;
          break;
      }
  }
  if(errorInit){
      return lw.response.send_error("Não foi possivel usar das portas %hd a %hd.", initport, lastport);
  }
  return NULL;
}


LuaCEmbedResponse *initdesktop(LuaCEmbed *arg) {

    unsigned short initport = (unsigned short)lw.args.get_long(arg, 0);
    unsigned short lastport = (unsigned short)lw.args.get_long(arg, 1);
    char * starter = lw.args.get_str(arg,2);


    if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
    }

    const char *functionvalue = "function(value) server_callback = value end";
    lw.args.generate_arg_clojure_evalation(arg, 3, functionvalue);


    if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
    }

    pid_t pid_server =0;
    int port = 0;
    for(int i = initport; i <= lastport; i++){
    pid_server = fork();
    if(pid_server==0){
      struct CwebServer serverTEMP = newCwebSever(i, main_sever);
      serverTEMP.function_timeout = 100;
      get_params_for_server_config(&serverTEMP);
      serverTEMP.single_process = true;
      serverTEMP.allow_cors = false;
      cb.server.start(&serverTEMP);
      exit(0);
    }

    sleep(1);
    int status;
    pid_t result = waitpid(pid_server, &status, WNOHANG);
    if (result == 0) {
      // Processo filho ainda está vivo
      port = i;
      break;
    }

    }
    printf("server rodando em %d\n",port);
    pid_t pid_browser = fork();
    if(pid_browser == 0){
    char comand[200] = {'\0'};
    sprintf(comand,"%s --app=http://localhost:%d/",starter,port);
    system(comand);
    exit(0);
    }

    while (true){
    int status;
    pid_t brwoser_result = waitpid(pid_browser, &status, WNOHANG);
    if(brwoser_result != 0){
      kill(pid_server, SIGKILL) ;
      kill(pid_browser,SIGKILL);
      break;
    }
    pid_t server_seult = waitpid(pid_server,&status,WNOHANG);
    if(server_seult != 0){
      kill(pid_server, SIGKILL) ;
      kill(pid_browser,SIGKILL);
      break;
    }
    }
    printf("application terminated\n");
    return NULL;
}

int serjao_berranteiro_start_point(lua_State *state) {
  cb = newCwebNamespace();
  lw = newLuaCEmbedNamespace();
  l = lw.newLuaLib(state, false);
  lw.add_callback(l, "initserver", initserver);
  lw.add_callback(l, "initdesktop", initdesktop);
  lw.add_callback(l, "send_file", send_file);
  lw.add_callback(l, "send_html", send_HTML);
  lw.add_callback(l, "send_json_string", send_json_string);
  lw.add_callback(l, "send_json", send_json);
  lw.add_callback(l , "send_raw", send_raw);
  lw.add_callback(l, "send_text", send_text);
  lw.add_callback(l,"component",create_component);
  lw.add_callback(l,"fragment",create_fragment);




  creat_table_for_config_server();

  return lw.perform(l);
}
