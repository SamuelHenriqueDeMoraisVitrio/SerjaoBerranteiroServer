

LuaCEmbedResponse * convert_to_number(LuaCEmbed *args){
    int type =lua.args.get_type(args,0);

    if(type == lua.types.NILL){
        return  lua.response.send_long(0);
    }

    if(type == lua.types.BOOL){
        bool result = lua.args.get_bool(args,0);
        return lua.response.send_long(result);
    }

    if(type == lua.types.NUMBER){
        double value = lua.args.get_double(args,0);
        return lua.response.send_double(value);
    }

    if(type == lua.types.STRING)
    {
        char *str = lua.args.get_str(args,0);
        double value;
        int result = sscanf(str,"%lf",&value);
        if(result){
            return lua.response.send_double(value);
        }

    }
    return NULL;
}

LuaCEmbedResponse * generate_exit(LuaCEmbed *args){
    int status = lua.args.get_long(args,0);
    if(lua.has_errors(args)){
        char *menssage = lua.get_error_message(args);
        return  lua.response.send_error(menssage);
    }
    lua_exit = status;
    return lua.response.send_error("");
}
