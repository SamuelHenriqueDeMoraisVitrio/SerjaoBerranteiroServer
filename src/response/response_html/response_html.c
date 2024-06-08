

LuaCEmbedResponse *send_HTML(LuaCEmbed *args){

     char *html_code = NULL;

    bool html_alocated = false;
    int type = lw.args.get_type(args,0);
    if(type == lw.types.TABLE){
        LuaCEmbedTable *html_component = lw.args.get_table(args,0);
        if(!its_a_component(html_component)){
            return lw.response.send_error("invalid type");
        }
        TextOrError content = render_component_raw(html_component);
        if(content.error){
            return  content.error;
        }
        html_code = content.text;
        html_alocated = true;
    }

    else{
        html_code = lw.args.get_str(args, 0);
    }

    short status_code = lw.args.get_long(args, 1);

    if(lw.has_errors(args)){
        const char *msg_error = lw.get_error_message(args);
        return lw.response.send_error(msg_error);
    }

    CwebHttpResponse *response = cb.response.send_var_html((char *)html_code, status_code);
    LuaCEmbedTable *table = lw.tables.new_anonymous_table(args);
    lw.tables.set_long_prop(table, "response_pointer", (long long)response);
    lw.tables.set_bool_prop(table, "its_a_reference", false);
    lw.tables.set_method(table, "__gc", clear_memory_response);
    if(html_alocated){
        free(html_code);
    }
    return lw.response.send_table(table);

}