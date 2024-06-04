
LuaCEmbedTable *private_lua_fluid_parse_array(LuaCEmbed *args, cJSON *json_array);
LuaCEmbedTable *Creat_table_from_json_object(LuaCEmbed *args, cJSON *json_object);
LuaCEmbedResponse *read_raw_body(LuaCEmbedTable *self,LuaCEmbed *args);
LuaCEmbedResponse *read_json_from_table(LuaCEmbedTable *self, LuaCEmbed *args);
