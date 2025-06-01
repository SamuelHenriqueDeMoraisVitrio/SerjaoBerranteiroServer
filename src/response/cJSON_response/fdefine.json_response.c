//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end


bool lua_json_fluid_table_is_object(LuaCEmbedTable *table) {
  long size = LuaCEmbedTable_get_full_size(table);
  for (int i = 0; i < size; i++) {
    if (LuaCembedTable_has_key_at_index(table, i) == false) {
      return false;
    }
  }
  return true;
}
//-------------
cJSON *serjao_json_dump_to_cJSON_array(LuaCEmbedTable *table) {
  long size = LuaCEmbedTable_get_full_size(table);
  cJSON *created_array = cJSON_CreateArray();
  for (int i = 0; i < size; i++) {
    int type = LuaCEmbedTable_get_type_by_index(table, i);

    if (type == LUA_CEMBED_NUMBER) {
      double value = LuaCEmbedTable_get_double_by_index(table, i);
      cJSON_AddItemToArray(created_array, cJSON_CreateNumber(value));
    }

    if (type == LUA_CEMBED_STRING) {
      char *value = LuaCEmbedTable_get_string_by_index(table, i);
      char *nil_code = LuaCembedTable_get_string_prop(serjao_server_config, "nullterminator");

      if (strcmp(nil_code, value) == 0) {
        cJSON_AddItemToArray(created_array, cJSON_CreateNull());

      } else {
        cJSON_AddItemToArray(created_array, cJSON_CreateString(value));
      }
    }

    if (type == LUA_CEMBED_BOOL) {
      bool value = LuaCEmbedTable_get_bool_by_index(table, i);
      cJSON_AddItemToArray(created_array, cJSON_CreateBool(value));
    }

    if (type == LUA_CEMBED_TABLE) {
      LuaCEmbedTable *internal = LuaCEmbedTable_get_sub_table_by_index(table, i);
      cJSON *value = serjao_json_dump_table_to_cJSON(internal);
      cJSON_AddItemToArray(created_array, value);
    }
  }
  return created_array;
}
//-----------
cJSON *serjao_json_dump_to_cJSON_object(LuaCEmbedTable *table) {
  long size = LuaCEmbedTable_get_full_size(table);
  cJSON *created_object = cJSON_CreateObject();
  for (int i = 0; i < size; i++) {
    char *key = LuaCembedTable_get_key_by_index(table, i);
    int type = LuaCEmbedTable_get_type_by_index(table, i);

    if (type == LUA_CEMBED_NUMBER) {
      double value = LuaCEmbedTable_get_double_by_index(table, i);
      cJSON_AddNumberToObject(created_object, key, value);
    }
    if (type == LUA_CEMBED_STRING) {
      char *value = LuaCEmbedTable_get_string_by_index(table, i);
      char *nil_code = LuaCembedTable_get_string_prop(serjao_server_config, "nullterminator");
      if (strcmp(nil_code, value) == 0) {
        cJSON_AddNullToObject(created_object, key);
      } else {
        cJSON_AddStringToObject(created_object, key, value);
      }
    }
    if (type == LUA_CEMBED_BOOL) {
      bool value = LuaCEmbedTable_get_bool_by_index(table, i);
      ;
      cJSON_AddBoolToObject(created_object, key, value);
    }

    if (type == LUA_CEMBED_TABLE) {
      LuaCEmbedTable *internal = LuaCEmbedTable_get_sub_table_by_index(table, i);
      cJSON *value = serjao_json_dump_table_to_cJSON(internal);
      cJSON_AddItemToObject(created_object, key, value);
    }
  }
  return created_object;
}
//------
cJSON *serjao_json_dump_table_to_cJSON(LuaCEmbedTable *table) {

  if (lua_json_fluid_table_is_object(table)) {

    return serjao_json_dump_to_cJSON_object(table);
  }
  return serjao_json_dump_to_cJSON_array(table);
}
//-----
LuaCEmbedResponse *send_json(LuaCEmbed *args) {
  cJSON *result = NULL;

  const int element_type = LuaCEmbed_get_arg_type(args, 0);

  unsigned short status_code = 200;
  if (LuaCEmbed_get_arg_type(args, 1) == LUA_CEMBED_NUMBER) {
    status_code = LuaCEmbed_get_long_arg(args, 1);
  }

  if (element_type == LUA_CEMBED_STRING) {
    char *value = LuaCEmbed_get_str_arg(args, 0);
    result = cJSON_CreateString(value);
  }

  else if (element_type == LUA_CEMBED_NUMBER) {
    double value = LuaCEmbed_get_double_arg(args, 0);
    result = cJSON_CreateNumber(value);
  }

  else if (element_type == LUA_CEMBED_BOOL) {
    bool value = LuaCEmbed_get_bool_arg(args, 0);
    result = cJSON_CreateBool(value);
  }

  else if (element_type == LUA_CEMBED_TABLE) {

    LuaCEmbedTable *value = LuaCEmbed_get_arg_table(args, 0);

    result = serjao_json_dump_table_to_cJSON(value);

  } else {
    return LuaCEmbed_send_error("element of type %s cannot be dumped",
                                  LuaCembed_convert_arg_code(element_type));
  }
  if (LuaCEmbed_has_errors(args)) {
    const char *msg_error = LuaCEmbed_get_error_message(args);
    return LuaCEmbed_send_error(msg_error);
  }

  CwebHttpResponse *response = cweb_send_cJSON(result, status_code);

  cJSON_Delete(result);

  LuaCEmbedTable *table = LuaCembed_new_anonymous_table(args);
  LuaCEmbedTable_set_long_prop(table, "response_pointer", (serjao_ptr_cast)response);
  LuaCEmbedTable_set_bool_prop(table, "its_a_reference", false);
  LuaCEmbedTable_set_method(table, "__gc", clear_memory_response);
  return LuaCEmbed_send_table(table);
}
//------
LuaCEmbedResponse *send_json_string(LuaCEmbed *args) {
  const char *json_string = LuaCEmbed_get_str_arg(args, 0);

  unsigned short status_code = 200;
  if (LuaCEmbed_get_arg_type(args, 1) == LUA_CEMBED_NUMBER) {
    status_code = LuaCEmbed_get_long_arg(args, 1);
  }

  if (LuaCEmbed_has_errors(args)) {
    const char *msg_error = LuaCEmbed_get_error_message(args);
    return LuaCEmbed_send_error(msg_error);
  }

  CwebHttpResponse *response =
      cweb_send_json_string((char *)json_string, status_code);

  LuaCEmbedTable *table = LuaCembed_new_anonymous_table(args);
  LuaCEmbedTable_set_long_prop(table, "response_pointer", (serjao_ptr_cast)response);
  LuaCEmbedTable_set_bool_prop(table, "its_a_reference", false);
  LuaCEmbedTable_set_method(table, "__gc", clear_memory_response);
  return LuaCEmbed_send_table(table);
}
