
HtmlorError  render_component_raw(LuaCEmbedTable *self){

    HtmlorError final_response = {0};


    if(lw.tables.get_type_prop(self,"type") != lw.types.STRING){
        final_response.error = lw.response.send_error("invalid type");
        return final_response;
    }
    char * type = lw.tables.get_string_prop(self,"type");
    if(strcmp(type,"component") != 0){
        final_response.error = lw.response.send_error("invalid type");
        return final_response;
    }

    LuaCEmbedTable *internal_elements = lw.tables.get_sub_table_prop(self,"internal_elements");
    int size = lw.tables.get_size(internal_elements);

    CTextStack *stack = newCTextStack(CTEXT_LINE_BREAKER,CTEXT_SEPARATOR);
    char *tag = NULL;
    if(lw.tables.get_type_prop(self,"tag") == lw.types.STRING){
        tag = lw.tables.get_string_prop(self,"tag");
    }
    if(tag != NULL){
        ctext_open(stack,tag);
    }

    for(int i = 0; i < size;i++){
        int element_type = lw.tables.get_type_by_index(internal_elements,i);

        if(element_type == lw.types.STRING){
            char *value  = lw.tables.get_string_by_index(internal_elements,i);
            CTextStack_segment_format(stack,"%s",value);
        }
        if(element_type == lw.types.TABLE){
            LuaCEmbedTable *internal = lw.tables.get_sub_table_by_index(internal_elements,i);
            HtmlorError internal_response = render_component_raw(internal);
            if(internal_response.error){
                CTextStack_free(stack);
                return  internal_response;
            }
            CTextStack_segment_format(stack,"%s",internal_response.html);
            free(internal_response.html);
        }

        if(element_type == lw.types.FUNCTION){
            char *value = lw.get_string_evaluation(
                self->main_object,
                "%s[%d]()",
                    internal_elements->global_name,
                    i+1
                );

            if(lw.has_errors(self->main_object)){
                CTextStack_free(stack);
                char *error_msg = lw.get_error_message(self->main_object);
                final_response.error = lw.response.send_error(error_msg);
                return  final_response;
            }
            CTextStack_segment_format(stack,"%s",value);
        }
    }
    if(tag != NULL){
        ctext_close(stack,tag);

    }
    final_response.html = CTextStack_self_transform_in_string_and_self_clear(stack);
    return final_response;
}


LuaCEmbedResponse * render_component(LuaCEmbedTable *self,LuaCEmbed *args){

    HtmlorError value  = render_component_raw(self);
    if(value.error){
        return  value.error;
    }

    LuaCEmbedResponse *response = lw.response.send_str(value.html);
    free(value.html);
    return  response;
}


LuaCEmbedResponse * create_fragment(LuaCEmbed *args){

    LuaCEmbedTable * self = lw.tables.new_anonymous_table(args);
    lw.tables.set_method(self,"render",render_component);
    lw.tables.set_string_prop(self,"type","component");

    LuaCEmbedTable *iternal_elements = lw.tables.new_anonymous_table(args);
    lw.tables.set_sub_table_prop(self,"internal_elements",iternal_elements);
    int total_args = lw.args.size(args);

    for(int i =0; i  <total_args;i++){
        lw.args.generate_arg_clojure_evalation(args,i,"function (value) current_arg = value end ");
        lw.tables.append_evaluation(iternal_elements,"current_arg");
    }
    return lw.response.send_table(self);

}

LuaCEmbedResponse * create_component(LuaCEmbed *args){
    char *tag = lw.args.get_str(args,0);
    if(lw.has_errors(args)){
        char *error_msg = lw.get_error_message(args);
        return lw.response.send_error(error_msg);
    }
    LuaCEmbedTable * self = lw.tables.new_anonymous_table(args);
    lw.tables.set_method(self,"render",render_component);
    lw.tables.set_string_prop(self,"type","component");

    lw.tables.set_string_prop(self,"tag",tag);

    LuaCEmbedTable *iternal_elements = lw.tables.new_anonymous_table(args);
    lw.tables.set_sub_table_prop(self,"internal_elements",iternal_elements);
    int total_args = lw.args.size(args);

    for(int i =1; i  <total_args;i++){
        lw.args.generate_arg_clojure_evalation(args,i,"function (value) current_arg = value end ");
        lw.tables.append_evaluation(iternal_elements,"current_arg");
    }
    return lw.response.send_table(self);

}