


LuaCEmbedResponse * send_file(LuaCEmbed *args){
    char *filename = lw.args.get_str(args,0);
    bool status_code  = lw.args.get_long(args,1);
    if(lw.has_errors(args)){
        char * erro_msg = lw.get_error_message(args);
        return lw.response.send_error(erro_msg);
    }
    const char *content_type = NULL;
    if(lw.args.get_type(args,2) != lw.types.NILL){
        content_type = lw.args.get_str(args,2);
    }

    if(lw.has_errors(args)){
        char * erro_msg = lw.get_error_message(args);
        return lw.response.send_error(erro_msg);
    }

    CwebHttpResponse *response = cb.response.send_file(filename,content_type,status_code);


    LuaCEmbedTable *table = lw.tables.new_anonymous_table(args);
    lw.tables.set_long_prop(table,"response_pointer",(long long )response);
    return  lw.response.send_table(table);
}



