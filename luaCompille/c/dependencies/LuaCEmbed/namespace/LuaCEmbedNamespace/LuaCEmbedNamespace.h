

typedef struct{
    LuaCEmbedTypeModule  types;
    LuaCEmbedResponseModule  response;
    LuaCembedArgsModule args;
    LuaCEmbedGlobalModule  globals;
    LuaCembedTableModule tables;
    void (*clear_errors)(LuaCEmbed *self);
    LuaCEmbed * (*newLuaLib)(lua_State *state, bool public_functions);
    void (*set_delete_function)(LuaCEmbed *self,void (*delelte_function)(struct  LuaCEmbed *self));
    LuaCEmbed * (*newLuaEvaluation)();
    int (*perform)(LuaCEmbed *self);
    const char * (*convert_arg_code)(int arg_code);
    void (*set_memory_limit)(LuaCEmbed  *self, double limit);

    char * (*get_error_message)(LuaCEmbed *self);
    bool (*has_errors)(LuaCEmbed *self);
    void (*set_timeout)(LuaCEmbed *self,int seconds);
    int (*evaluate)(LuaCEmbed *self, const char *code, ...);
    char * (*get_string_evaluation)(LuaCEmbed *self,const char *code, ...);
    int  (*get_evaluation_type)(LuaCEmbed *self,const char *code,...);
    long (*get_evaluation_size)(LuaCEmbed *self,const char *code,...);
    long long  (*get_evaluation_long)(LuaCEmbed *self,const char *code,...);
    double (*get_evaluation_double)(LuaCEmbed *self,const char *code,...);
    bool (*get_evaluation_bool)(LuaCEmbed *self, const char *code,...);

    void (*set_long_lib_prop)(LuaCEmbed *self,const char *name,long long  value);

    void (*set_double_lib_prop)(LuaCEmbed *self,const char *name,double value);
    void (*set_bool_lib_prop)(LuaCEmbed *self,const char *name,bool value);
    void (*set_string_lib_prop)(LuaCEmbed *self,const char *name,const char * value);
    void (*set_table_lib_prop)(LuaCEmbed *self,const char *name,LuaCEmbedTable *value);

    long long  (*get_long_lib_prop)(LuaCEmbed *self,const char *name);
    double (*get_double_lib_prop)(LuaCEmbed *self,const char *name);
    bool (*get_bool_lib_prop)(LuaCEmbed *self,const char *name);
    char * (*get_string_lib_prop)(LuaCEmbed *self,const char *name);
    LuaCEmbedTable *(*get_table_lib_prop)(LuaCEmbed *self,const char *name);

    int (*evaluete_file)(LuaCEmbed *self, const char *file);
    void (*add_callback)(LuaCEmbed *self, const char *callback_name, LuaCEmbedResponse* (*callback)(LuaCEmbed *args) );
    void (*free)(LuaCEmbed *self);

} LuaCEmbedNamespace;

LuaCEmbedNamespace newLuaCEmbedNamespace();

