
#include "dependencies/all.h"
#include "lua_code.h"
LuaCEmbedNamespace lua;

int lua_exit = 0;

#include "callbacks/declaration.h"

#include "callbacks/definition.h"

void add_callbacks(LuaCEmbed *main_obj){
    lua.add_callback(main_obj,"print",custom_print);
    lua.add_callback(main_obj,"input",get_str);
    lua.add_callback(main_obj,"tonumber",convert_to_number);
    lua.add_callback(main_obj,"exit",generate_exit);

}


int main(int argc,char *argv[]){
    lua  = newLuaCEmbedNamespace();
    LuaCEmbed * main_obj = lua.newLuaEvaluation();
    add_callbacks(main_obj);
    lua.evaluate(main_obj,lua_code);
    if(lua.has_errors(main_obj)){
        char *error = lua.get_error_message(main_obj);
        if(strcmp(error,"") != 0){
            printf("%s\n",error);
        }
        lua.free(main_obj);
        return lua_exit;
    }
    lua.free(main_obj);
    return 0;
}