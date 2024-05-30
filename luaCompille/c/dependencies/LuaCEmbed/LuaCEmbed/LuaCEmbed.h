
typedef struct LuaCEmbed{
    lua_State *state;
    const char *current_function;
    bool is_lib;
    bool public_functions;
    int total_args;
    char *error_msg;
    double memory_limit;
    int used_memory;
    int timeout;
    void (*delete_function)(struct  LuaCEmbed *self);
    void *global_tables;
    void *func_tables;
    int lib_identifier;
    int stack_leve;
    char *main_lib_table;
    bool field_protection;
}LuaCEmbed;


LuaCEmbed  *global_current_lua_embed_object;

