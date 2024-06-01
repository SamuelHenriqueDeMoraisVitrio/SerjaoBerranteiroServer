
LuaCEmbedResponse *leitura_de_tipo(LuaCEmbed *args){
    const short get_number_type = lw.args.get_type(args, 0);
    const char *convert_to_string = lw.convert_arg_code(get_number_type);

    return lw.response.send_str(convert_to_string);
}