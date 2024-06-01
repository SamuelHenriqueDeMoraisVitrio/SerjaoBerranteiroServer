

LuaCEmbedResponse *setHP_index(LuaCEmbed *args, CwebDict *hp, char *(*get_unified_param)(CwebHttpRequest *, const char *)) {

  if (lw.args.get_type(args, 1) == lw.types.STRING) {
    char *keyvalue = lw.args.get_str(args, 1);
    char *value = get_unified_param(cbrq, keyvalue);
    if (value == NULL) {
      return NULL;
    } // value == NULL

    return lw.response.send_str(value);
  } // if main

  if (lw.args.get_type(args, 1) == lw.types.NUMBER) {
    int index = (int)(lw.args.get_long(args, 1) - 1);

    if (index >= hp->size) {
      return NULL;
    } // if index

    const char *value = hp->keys_vals[index]->value;
    const char *key = hp->keys_vals[index]->key;

    LuaCEmbedTable *tableKeys_vals = lw.tables.new_anonymous_table(l);
    lw.tables.set_string_prop(tableKeys_vals, "key", key);
    lw.tables.set_string_prop(tableKeys_vals, "value", value);
    return lw.response.send_table(tableKeys_vals);
  } // if main

  return lw.response.send_error("The index type is not compatible");
} // function

LuaCEmbedResponse *setHeaders(LuaCEmbedTable *self, LuaCEmbed *args) {
  return setHP_index(args, cbrq->headers, cb.request.get_header);
}

LuaCEmbedResponse *setParams(LuaCEmbedTable *self, LuaCEmbed *args) {
  return setHP_index(args, cbrq->params, cb.request.get_param);
}

void create_request(LuaCEmbed *args) {
  LuaCEmbedTable *tableServer =
      lw.globals.new_table(args, "request_main_server");
  lw.tables.set_string_prop(tableServer, "url", cbrq->url);
  lw.tables.set_string_prop(tableServer, "route", cbrq->route);
  lw.tables.set_string_prop(tableServer, "method", cbrq->method);
  lw.tables.set_method(tableServer,"read_body",read_raw_body);
  LuaCEmbedTable *tableHeaders = lw.tables.new_anonymous_table(args);
  LuaCEmbedTable *tableParams = lw.tables.new_anonymous_table(args);

  lw.tables.set_sub_table_prop(tableServer, "header", tableHeaders);
  lw.tables.set_sub_table_prop(tableServer, "params", tableParams);

  lw.tables.set_method(tableParams, "__index", setParams);
  lw.tables.set_method(tableHeaders, "__index", setHeaders);
}
