//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end


LuaCEmbedTable *private_serjao_parse_array(LuaCEmbed *args,
                                              cJSON *json_array) {
  LuaCEmbedTable *created = LuaCembed_new_anonymous_table(args);
  int size = cJSON_GetArraySize(json_array);
  for (int i = 0; i < size; i++) {
    cJSON *current = cJSON_GetArrayItem(json_array, i);
    if (cJSON_IsNumber(current)) {
      LuaCEmbedTable_append_double(created, cJSON_GetNumberValue(current));
    } else if (cJSON_IsString(current)) {
      LuaCEmbedTable_append_string(created, cJSON_GetStringValue(current));
    } else if (cJSON_IsBool(current)) {
      LuaCEmbedTable_append_bool(created, (bool)current->valueint);
    } else if (cJSON_IsNull(current)) {
      // char *nil_code = LuaCEmbed_get_global_string(args,
      // "private_lua_json_fluid_null_code");
      char *nil_code = LuaCembedTable_get_string_prop(serjao_server_config, "nullterminator");
      LuaCEmbedTable_append_string(created, nil_code);
    } else if (cJSON_IsObject(current)) {
      LuaCEmbedTable *internal_created =
          Creat_table_from_json_object(args, current);
      LuaCEmbedTable_append_table(created, internal_created);
    } else if (cJSON_IsArray(current)) {
      LuaCEmbedTable *internal_created =
          private_serjao_parse_array(args, current);
      LuaCEmbedTable_append_table(created, internal_created);
    }
  }
  return created;
}

LuaCEmbedTable *Creat_table_from_json_object(LuaCEmbed *args,
                                             cJSON *json_object) {
  LuaCEmbedTable *created = LuaCembed_new_anonymous_table(args);
  int size = cJSON_GetArraySize(json_object);
  for (int i = 0; i < size; i++) {
    cJSON *current = cJSON_GetArrayItem(json_object, i);
    char *key = current->string;
    if (cJSON_IsNumber(current)) {
      LuaCEmbedTable_set_double_prop(created, key, cJSON_GetNumberValue(current));
    } else if (cJSON_IsString(current)) {
      LuaCEmbedTable_set_string_prop(created, key, cJSON_GetStringValue(current));
    } else if (cJSON_IsBool(current)) {
      LuaCEmbedTable_set_bool_prop(created, key, (bool)current->valueint);
    } else if (cJSON_IsNull(current)) {
      // char *nil_code = LuaCEmbed_get_global_string(args,
      // "private_lua_json_fluid_null_code");
      char *nil_code = LuaCembedTable_get_string_prop(serjao_server_config, "nullterminator");
      LuaCEmbedTable_set_string_prop(created, key, nil_code);
    } else if (cJSON_IsObject(current)) {
      LuaCEmbedTable *internal_created =
          Creat_table_from_json_object(args, current);
      LuaCEmbedTable_set_sub_table_prop(created, key, internal_created);
    } else if (cJSON_IsArray(current)) {
      LuaCEmbedTable *internal_created =
          private_serjao_parse_array(args, current);
      LuaCEmbedTable_set_sub_table_prop(created, key, internal_created);
    }
  }
  return created;
}

LuaCEmbedResponse *read_json_from_table(LuaCEmbedTable *self, LuaCEmbed *args) {

  const long max_size = LuaCEmbed_get_long_arg(args, 0);

  if (LuaCEmbed_has_errors(args)) {
    char *error_message = LuaCEmbed_get_error_message(args);
    return LuaCEmbed_send_error(error_message);
  }

  cJSON *json = CWebHttpRequest_read_cJSON(serjao_cweb_global_requiest, max_size);

  if (!json) {
    return NULL;
  }

  if (cJSON_IsBool(json)) {
    LuaCEmbedResponse *response = LuaCEmbed_send_bool((bool)json->valueint);
    return response;
  }

  else if (cJSON_IsNumber(json)) {
    LuaCEmbedResponse *response =
        LuaCEmbed_send_double(cJSON_GetNumberValue(json));
    return response;
  }

  else if (cJSON_IsString(json)) {
    LuaCEmbedResponse *response =
        LuaCEmbed_send_str(cJSON_GetStringValue(json));
    return response;
  }

  else if (cJSON_IsNull(json)) {
    // char *nil_code = LuaCEmbed_get_global_string(args,
    // "private_lua_json_fluid_null_code");
    char *nil_code = LuaCembedTable_get_string_prop(serjao_server_config, "nullterminator");
    return LuaCEmbed_send_str(nil_code);
  } else if (cJSON_IsObject(json)) {
    LuaCEmbedTable *created = Creat_table_from_json_object(args, json);
    return LuaCEmbed_send_table(created);
  } else if (cJSON_IsArray(json)) {
    LuaCEmbedTable *created = private_serjao_parse_array(args, json);
    return LuaCEmbed_send_table(created);
  }
  return NULL;
}

LuaCEmbedResponse *read_raw_body(LuaCEmbedTable *self, LuaCEmbed *args) {
  long max_size = LuaCEmbed_get_long_arg(args, 0);

  if (LuaCEmbed_has_errors(args)) {
    char *error_message = LuaCEmbed_get_error_message(args);
    return LuaCEmbed_send_error(error_message);
  }

  unsigned char *content = CwebHttpRequest_read_content(serjao_cweb_global_requiest, max_size);

  if (content == NULL) {
    return NULL;
  }

  return LuaCEmbed_send_raw_string((char *)content, serjao_cweb_global_requiest->content_length);
}
