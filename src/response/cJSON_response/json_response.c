

bool lua_json_fluid_table_is_object(LuaCEmbedTable *table) {
  long size = lw.tables.get_size(table);
  for (int i = 0; i < size; i++) {
    if (lw.tables.has_key(table, i) == false) {
      return false;
    }
  }
  return true;
}

cJSON *lua_fluid_json_dump_to_cJSON_array(LuaCEmbedTable *table) {
  long size = lw.tables.get_size(table);
  cJSON *created_array = cJSON_CreateArray();
  for (int i = 0; i < size; i++) {
    int type = lw.tables.get_type_by_index(table, i);

    if (type == lw.types.NUMBER) {
      double value = lw.tables.get_double_by_index(table, i);
      cJSON_AddItemToArray(created_array, cJSON_CreateNumber(value));
    }

    if (type == lw.types.STRING) {
      char *value = lw.tables.get_string_by_index(table, i);
      char *nil_code = lw.tables.get_string_prop(set_server, "nullterminator");

      if (strcmp(nil_code, value) == 0) {
        cJSON_AddItemToArray(created_array, cJSON_CreateNull());

      } else {
        cJSON_AddItemToArray(created_array, cJSON_CreateString(value));
      }
    }

    if (type == lw.types.BOOL) {
      bool value = lw.tables.get_bool_by_index(table, i);
      cJSON_AddItemToArray(created_array, cJSON_CreateBool(value));
    }

    if (type == lw.types.TABLE) {
      LuaCEmbedTable *internal = lw.tables.get_sub_table_by_index(table, i);
      cJSON *value = lua_fluid_json_dump_table_to_cJSON(internal);
      cJSON_AddItemToArray(created_array, value);
    }
  }
  return created_array;
}

cJSON *lua_fluid_json_dump_to_cJSON_object(LuaCEmbedTable *table) {
  long size = lw.tables.get_size(table);
  cJSON *created_object = cJSON_CreateObject();
  for (int i = 0; i < size; i++) {
    char *key = lw.tables.get_key_by_index(table, i);
    int type = lw.tables.get_type_by_index(table, i);

    if (type == lw.types.NUMBER) {
      double value = lw.tables.get_double_by_index(table, i);
      cJSON_AddNumberToObject(created_object, key, value);
    }
    if (type == lw.types.STRING) {
      char *value = lw.tables.get_string_by_index(table, i);
      char *nil_code = lw.tables.get_string_prop(set_server, "nullterminator");
      if (strcmp(nil_code, value) == 0) {
        cJSON_AddNullToObject(created_object, key);
      } else {
        cJSON_AddStringToObject(created_object, key, value);
      }
    }
    if (type == lw.types.BOOL) {
      bool value = lw.tables.get_bool_by_index(table, i);
      ;
      cJSON_AddBoolToObject(created_object, key, value);
    }

    if (type == lw.types.TABLE) {
      LuaCEmbedTable *internal = lw.tables.get_sub_table_by_index(table, i);
      cJSON *value = lua_fluid_json_dump_table_to_cJSON(internal);
      cJSON_AddItemToObject(created_object, key, value);
    }
  }
  return created_object;
}

cJSON *lua_fluid_json_dump_table_to_cJSON(LuaCEmbedTable *table) {

  if (lua_json_fluid_table_is_object(table)) {

    return lua_fluid_json_dump_to_cJSON_object(table);
  }
  return lua_fluid_json_dump_to_cJSON_array(table);
}

LuaCEmbedResponse *send_json(LuaCEmbed *args) {
  cJSON *result = NULL;


  const int element_type = lw.args.get_type(args, 0);
  const short status_code = lw.args.get_long(args, 1);

  if (element_type == lw.types.STRING) {
    char *value = lw.args.get_str(args, 0);
    result = cJSON_CreateString(value);
  }

  else if (element_type == lw.types.NUMBER) {
    double value = lw.args.get_double(args, 0);
    result = cJSON_CreateNumber(value);
  }

  else if (element_type == lw.types.BOOL) {
    bool value = lw.args.get_bool(args, 0);
    result = cJSON_CreateBool(value);
  }

  else if (element_type == lw.types.TABLE) {

    LuaCEmbedTable *value = lw.args.get_table(args, 0);

    result = lua_fluid_json_dump_table_to_cJSON(value);

  }
  else {
    return lw.response.send_error("element of type %s cannot be dumped",
                                  lw.convert_arg_code(element_type));
  }
  if (lw.has_errors(args)) {
    const char *msg_error = lw.get_error_message(args);
    return lw.response.send_error(msg_error);
  }

  CwebHttpResponse *response = cb.response.send_cJSON(result, status_code);

  cJSON_Delete(result);

  LuaCEmbedTable *table = lw.tables.new_anonymous_table(args);
  lw.tables.set_long_prop(table, "response_pointer", (long long)response);
  lw.tables.set_bool_prop(table, "its_a_reference", false);
  lw.tables.set_method(table, "__gc", clear_memory_response);
  return lw.response.send_table(table);
}

LuaCEmbedResponse *send_json_string(LuaCEmbed *args) {
  const char *json_string = lw.args.get_str(args, 0);
  const short status_code = lw.args.get_long(args, 1);

  if (lw.has_errors(args)) {
    const char *msg_error = lw.get_error_message(args);
    return lw.response.send_error(msg_error);
  }

  CwebHttpResponse *response = cb.response.send_json_string((char *)json_string, status_code);

  LuaCEmbedTable *table = lw.tables.new_anonymous_table(args);
  lw.tables.set_long_prop(table, "response_pointer", (long long)response);
  lw.tables.set_bool_prop(table, "its_a_reference", false);
  lw.tables.set_method(table, "__gc", clear_memory_response);
  return lw.response.send_table(table);
}
