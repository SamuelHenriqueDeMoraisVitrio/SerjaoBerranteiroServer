

LuaCEmbedResponse *send_HTML(LuaCEmbed *args){
    const char *html_code = lw.args.get_str(args, 0);
    short status_code = lw.args.get_long(args, 1);

    if(lw.has_errors(args)){
        const char *msg_error = lw.get_error_message(args);
        return lw.response.send_error(msg_error);
    }

    CwebHttpResponse *response = cb.response.send_var_html((char *)html_code, status_code);

    LuaCEmbedTable *table = lw.tables.new_anonymous_table(args);
    lw.tables.set_long_prop(table, "response_pointer", (long long)response);
    return lw.response.send_table(table);
}