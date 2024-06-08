


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

        port = i;
        break;
    }
    if(errorInit){
        return lw.response.send_error("Não foi possivel usar das portas %hd a %hd.", initport, lastport);
    }
    return NULL;
}
