//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.globals.h"
//silver_chain_scope_end

bool lua_json_fluid_table_is_object(LuaCEmbedTable *table);
cJSON *serjao_json_dump_table_to_cJSON(LuaCEmbedTable *table);
cJSON *serjao_json_dump_to_cJSON_array(LuaCEmbedTable *table);
cJSON *serjao_json_dump_to_cJSON_object(LuaCEmbedTable *table);
LuaCEmbedResponse *send_json(LuaCEmbed *args);
LuaCEmbedResponse *send_json_string(LuaCEmbed *args);
