
LuaCEmbedResponse *send_raw(LuaCEmbed *args){

  int firsr_type = lw.args.get_type(args,0);
  char *content_type = lw.args.get_str(args,1);
  int status_code = lw.args.get_long(args,2);

  if (lw.has_errors(args)) {
    char *erro_msg = lw.get_error_message(args);
    return lw.response.send_error(erro_msg);
  }
  CwebHttpResponse *response = NULL;

  if(firsr_type == lw.types.STRING){
    long size;
    char *value = lw.args.get_raw_str(args,&size,0);
    response = cb.response.send_any(content_type,size,(unsigned char*)value,status_code);
  }
  else if(firsr_type == LUA_TUSERDATA){
    lw.args.generate_arg_clojure_evalation(args,0,"function(value) raw_response = value:read('a') end ");
    if (lw.has_errors(args)) {
      char *erro_msg = lw.get_error_message(args);
      return lw.response.send_error(erro_msg);
    }
    long size;
    char* value = lw.globals.get_raw_string(args,"raw_response",&size);
    if (lw.has_errors(args)) {
      char *erro_msg = lw.get_error_message(args);
      return lw.response.send_error(erro_msg);
    }
    response = cb.response.send_any(content_type,size,(unsigned char*)value,status_code);
  }

  else{
    return  lw.response.send_error("undefined type\n");
  }

  LuaCEmbedTable *table = lw.tables.new_anonymous_table(args);
  lw.tables.set_long_prop(table, "response_pointer", (long long)response);
  return lw.response.send_table(table);

}
LuaCEmbedResponse *send_file(LuaCEmbed *args) {
  char *rout_file = lw.args.get_str(args, 0);
  short status_code = lw.args.get_long(args, 1);
  if (lw.has_errors(args)) {
    char *erro_msg = lw.get_error_message(args);
    return lw.response.send_error(erro_msg);
  }
  const char *content_type = NULL;
  if (lw.args.get_type(args, 2) != lw.types.NILL) {
    content_type = lw.args.get_str(args, 2);
  }

  if (lw.has_errors(args)) {
    char *erro_msg = lw.get_error_message(args);
    return lw.response.send_error(erro_msg);
  }

  CwebHttpResponse *response = cb.response.send_file(rout_file, content_type, status_code);

  LuaCEmbedTable *table = lw.tables.new_anonymous_table(args);
  lw.tables.set_long_prop(table, "response_pointer", (long long)response);
  return lw.response.send_table(table);
}

LuaCEmbedResponse *send_text(LuaCEmbed *args){

    const char *text = lw.args.get_str(args, 0);
    const short status_code = lw.args.get_long(args, 1);

    if(lw.has_errors(args)){
        char *erro_msg = lw.get_error_message(args);
        return lw.response.send_error(erro_msg);
    }

    CwebHttpResponse *response = cb.response.send_text(text, status_code);

    LuaCEmbedTable *table = lw.tables.new_anonymous_table(args);
    lw.tables.set_long_prop(table, "response_pointer", (long long)response);
    return lw.response.send_table(table);
}