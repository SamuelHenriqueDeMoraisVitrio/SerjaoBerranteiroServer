

LuaCEmbedResponse * create_html(LuaCEmbed *args){
    LuaCEmbedTable *fragment = raw_create_fragment(args);
    lw.tables.set_string_prop(fragment,"tag","html");
    return lw.response.send_table(fragment);
}

LuaCEmbedResponse * create_body(LuaCEmbed *args){
    LuaCEmbedTable *fragment = raw_create_fragment(args);
    lw.tables.set_string_prop(fragment,"tag","body");
    return lw.response.send_table(fragment);
}

LuaCEmbedResponse * create_h1(LuaCEmbed *args){
    LuaCEmbedTable *fragment = raw_create_fragment(args);
    lw.tables.set_string_prop(fragment,"tag","h1");
    return lw.response.send_table(fragment);
}

