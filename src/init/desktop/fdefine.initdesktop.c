//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../../src/imports/imports.fdeclare.h"
//silver_chain_scope_end


LuaCEmbedResponse *initdesktop(LuaCEmbed *arg) {
  #ifdef __linux__
  char *starter = LuaCEmbed_get_str_arg(arg, 0);

  if (LuaCEmbed_has_errors(arg)) {
    return LuaCEmbed_send_error("Uninformed arguments");
  }

  const char *functionvalue = "function(value) server_callback = value end";
  LuaCEmbed_generate_arg_clojure_evalation(arg, 1, functionvalue);

  if (LuaCEmbed_has_errors(arg)) {
    return LuaCEmbed_send_error("Uninformed arguments");
  }

  unsigned int windows_x = 800;
  unsigned int windows_y = 400;
  bool no_parans_window = (LuaCEmbed_get_arg_type(arg, 2) != LUA_CEMBED_NUMBER &&
                           LuaCEmbed_get_arg_type(arg, 3) != LUA_CEMBED_NUMBER);

  if (LuaCEmbed_get_arg_type(arg, 2) == LUA_CEMBED_NUMBER) {
    windows_x = LuaCEmbed_get_long_arg(arg, 2);
  }
  if (LuaCEmbed_get_arg_type(arg, 3) == LUA_CEMBED_NUMBER) {
    windows_y = LuaCEmbed_get_long_arg(arg, 3);
  }
  if (LuaCEmbed_get_arg_type(arg, 2) == LUA_CEMBED_NUMBER &&
      LuaCEmbed_get_arg_type(arg, 3) != LUA_CEMBED_NUMBER) {
    windows_y = windows_x;
  }

  if (LuaCEmbed_has_errors(arg)) {
    char *erro_msg = LuaCEmbed_get_error_message(arg);
    return LuaCEmbed_send_error(erro_msg);
  }

  char config_window[70] = {'\0'};
  sprintf(config_window, "--window-size=%d,%d", windows_x, windows_y);

  pid_t pid_server = 0;
  int port = 0;
  for (int i = 3000; i <= 5000; i++) {
    pid_server = fork();
    if (pid_server == 0) {
      struct CwebServer serverTEMP = newCwebSever(i, main_sever_handle);
      serverTEMP.function_timeout = 100;
      get_params_for_server_config(&serverTEMP);
      serverTEMP.single_process = true;
      serverTEMP.allow_cors = false;
      CwebServer_start(&serverTEMP);
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
  printf("server rodando em %d\n", port);
  pid_t pid_browser = fork();
  if (pid_browser == 0) {
    char comand[200] = {'\0'};
    sprintf(comand, "%s %s --app=http://localhost:%d/", starter,
            no_parans_window ? "--start-maximized" : config_window, port);
    system(comand);
    exit(0);
  }

  while (true) {
    int status;
    pid_t brwoser_result = waitpid(pid_browser, &status, WNOHANG);
    pid_t server_seult = waitpid(pid_server, &status, WNOHANG);

    if (brwoser_result != 0) {
      kill(server_seult, SIGKILL);
      break;
    }

    if (server_seult != 0) {
      kill(brwoser_result, SIGKILL);
      break;
    }
  }
  printf("application terminated\n");
  return NULL;
  #endif
}
