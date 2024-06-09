

LuaCEmbedResponse *initdesktop(LuaCEmbed *arg) {

    char * starter = lw.args.get_str(arg,0);


    if (lw.has_errors(arg)) {
        return lw.response.send_error("Uninformed arguments");
    }

    const char *functionvalue = "function(value) server_callback = value end";
    lw.args.generate_arg_clojure_evalation(arg, 1, functionvalue);


    if (lw.has_errors(arg)) {
        return lw.response.send_error("Uninformed arguments");
    }


    unsigned int windows_x = 800;
    unsigned int windows_y = 400;
    bool no_parans_window = (lw.args.get_type(arg, 2) != lw.types.NUMBER && lw.args.get_type(arg, 3) != lw.types.NUMBER);

    if(lw.args.get_type(arg, 2) == lw.types.NUMBER){
        windows_x = lw.args.get_long(arg, 2);
    }
    if(lw.args.get_type(arg, 3) == lw.types.NUMBER){
        windows_y = lw.args.get_long(arg, 3);
    }
    if(lw.args.get_type(arg, 2) == lw.types.NUMBER && lw.args.get_type(arg, 3) != lw.types.NUMBER){
        windows_y = windows_x;
    }

    if(lw.has_errors(arg)){
        char *erro_msg = lw.get_error_message(arg);
        return lw.response.send_error(erro_msg);
    }

    char config_window[70] = {'\0'};
    sprintf(config_window, "--window-size=%d,%d", windows_x, windows_y);

    pid_t pid_server =0;
    int port = 0;
    for(int i = 3000; i <= 5000; i++){
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
        sprintf(comand,"%s %s --app=http://localhost:%d/", starter, no_parans_window ? "--start-maximized" : config_window, port);
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
