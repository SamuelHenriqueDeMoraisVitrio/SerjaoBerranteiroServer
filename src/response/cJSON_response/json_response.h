

bool lua_json_fluid_table_is_object(LuaCEmbedTable *table);
cJSON *lua_fluid_json_dump_table_to_cJSON(LuaCEmbedTable *table);
cJSON *lua_fluid_json_dump_to_cJSON_array(LuaCEmbedTable *table);
cJSON *lua_fluid_json_dump_to_cJSON_object(LuaCEmbedTable *table);
LuaCEmbedResponse *send_json(LuaCEmbed *args);
LuaCEmbedResponse *send_json_string(LuaCEmbed *args);
