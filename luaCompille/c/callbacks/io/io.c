LuaCEmbedResponse *custom_print(LuaCEmbed *args){

    int size = lua.args.size(args);
    for(int i = 0; i < size; i++){
        int type = lua.args.get_type(args,i);
        if(type == lua.types.NUMBER){
            printf("%lf",lua.args.get_double(args,i));
        }
        if(type == lua.types.STRING){
            printf("%s",lua.args.get_str(args,i));
        }
        if(type == lua.types.BOOL){
            char *bool_str = lua.args.get_bool(args,i) ? "true":"false";
            printf("%s",bool_str);
        }

        if(type == lua.types.NILL){
            printf("nil");
        }
    }
    return NULL;
}

LuaCEmbedResponse * get_str(LuaCEmbed *args){

    if(lua.args.get_type(args,0) != lua.types.NILL){
        char *str_message = lua.args.get_str(args,0);

        if(lua.has_errors(args)){
            char *error_msg = lua.get_error_message(args);
            return lua.response.send_error(error_msg);
        }
        printf("%s",str_message);
    }
    char buffer[500] ={0};
    scanf ("%s",buffer);
    return  lua.response.send_str(buffer);
}