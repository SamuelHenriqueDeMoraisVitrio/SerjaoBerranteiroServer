


LuaCEmbedResponse * read_raw_body(LuaCEmbedTable *self,LuaCEmbed *args){
    long max_size = lw.args.get_long(args,0);
    if(lw.has_errors(args)){
        char *error_message = lw.get_error_message(args);
        return lw.response.send_error(error_message);
    }
    unsigned char *content= cb.request.read_content(cbrq,max_size);
    if(content == NULL){
        return  NULL;
    }
    return lw.response.send_raw_string((char*)content,cbrq->content_length);

}