

LuaCEmbedResponse *send_text_cleaning_memory(LuaCEmbed *args){
    const char *get_text_by_param = lw.args.get_str(args, 0);
    const short status_cod = lw.args.get_long(args, 1);

    if(lw.has_errors(args)){
        const char *texto_by_error = lw.get_error_message(args);
        return lw.response.send_error(texto_by_error);
    }

    CwebHttpResponse *response = cb.response.send_text_cleaning_memory(get_text_by_param, status_cod);

    LuaCEmbedTable *table = lw.tables.new_anonymous_table(args);
    lw.tables.set_long_prop(table, "response_pointer", (long long)response);
    return lw.response.send_table(table);
}

LuaCEmbedResponse *send_html_cleaning_memory(LuaCEmbed *args){
    //return response_table_html_pipe(cb.response.send_var_html_cleaning_memory);
    return NULL;
}