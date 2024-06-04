


LuaCEmbedTable *private_lua_fluid_parse_array(LuaCEmbed *args, cJSON *json_array){
    LuaCEmbedTable *created = lw.tables.new_anonymous_table(args);
    int size = cJSON_GetArraySize(json_array);
    for(int i = 0; i < size; i++){
        cJSON *current = cJSON_GetArrayItem(json_array,i);
        if(cJSON_IsNumber(current)){
            lw.tables.append_double(created,cJSON_GetNumberValue(current));
        }
        else if(cJSON_IsString(current)){
            lw.tables.append_string(created,cJSON_GetStringValue(current));
        }
        else if(cJSON_IsBool(current)){
            lw.tables.append_bool(created,(bool)current->valueint);
        }
        else if(cJSON_IsNull(current)){
            //char *nil_code = lw.globals.get_string(args, "private_lua_json_fluid_null_code");
            char *nil_code = lw.tables.get_string_prop(set_server, "nullterminator");
            lw.tables.append_string(created, nil_code);
        }
        else if(cJSON_IsObject(current)){
            LuaCEmbedTable *internal_created = Creat_table_from_json_object(args, current);
            lw.tables.append_table(created, internal_created);
        }
        else if(cJSON_IsArray(current)){
            LuaCEmbedTable *internal_created = private_lua_fluid_parse_array(args,current);
            lw.tables.append_table(created,internal_created);
        }
    }
    return created;
}

LuaCEmbedTable *Creat_table_from_json_object(LuaCEmbed *args, cJSON *json_object){
    LuaCEmbedTable *created = lw.tables.new_anonymous_table(args);
    int size = cJSON_GetArraySize(json_object);
    for(int i = 0; i < size; i++){
        cJSON *current = cJSON_GetArrayItem(json_object,i);
        char *key =current->string;
        if(cJSON_IsNumber(current)){
            lw.tables.set_double_prop(created,key, cJSON_GetNumberValue(current));
        }
        else if(cJSON_IsString(current)){
            lw.tables.set_string_prop(created,key, cJSON_GetStringValue(current));
        }
        else if(cJSON_IsBool(current)){
            lw.tables.set_bool_prop(created,key, (bool)current->valueint);
        }
        else if(cJSON_IsNull(current)){
            //char *nil_code = lw.globals.get_string(args, "private_lua_json_fluid_null_code");
            char *nil_code = lw.tables.get_string_prop(set_server, "nullterminator");
            lw.tables.set_string_prop(created, key, nil_code);
        }
        else if(cJSON_IsObject(current)){
            LuaCEmbedTable *internal_created = Creat_table_from_json_object(args, current);
            lw.tables.set_sub_table_prop(created,key,internal_created);
        }
        else if(cJSON_IsArray(current)){
            LuaCEmbedTable *internal_created = private_lua_fluid_parse_array(args,current);
            lw.tables.set_sub_table_prop(created,key,internal_created);
        }
    }
    return created;
}

LuaCEmbedResponse *read_json_from_table(LuaCEmbedTable *self, LuaCEmbed *args){

    const long max_size = lw.args.get_long(args, 0);

    if(lw.has_errors(args)){
        char *error_message = lw.get_error_message(args);
        return lw.response.send_error(error_message);
    }

    cJSON *json = cb.request.read_cJSON(cbrq, max_size);

    if(!json){
        lw.response.send_error("content its not a valid json");
    }

    if(cJSON_IsBool(json)){
        LuaCEmbedResponse *response =lw.response.send_bool((bool)json->valueint);
        cJSON_Delete(json);
        return response;
    }

    else if(cJSON_IsNumber(json)){
        LuaCEmbedResponse *response = lw.response.send_double(cJSON_GetNumberValue(json));
        cJSON_Delete(json);
        return response;
    }

    else if(cJSON_IsString(json)){
        LuaCEmbedResponse *response = lw.response.send_str(cJSON_GetStringValue(json));
        cJSON_Delete(json);
        return response;
    }

    else if(cJSON_IsNull(json)){
        cJSON_Delete(json);
        //char *nil_code = lw.globals.get_string(args, "private_lua_json_fluid_null_code");
        char *nil_code = lw.tables.get_string_prop(set_server, "nullterminator");
        return  lw.response.send_str(nil_code);
    }
    else if(cJSON_IsObject(json)){
        LuaCEmbedTable *created = Creat_table_from_json_object(args, json);
        cJSON_Delete(json);
        return lw.response.send_table(created);
    }
    else if(cJSON_IsArray(json)){
        LuaCEmbedTable *created = private_lua_fluid_parse_array(args,json);
        cJSON_Delete(json);
        return lw.response.send_table(created);
    }
    return NULL;
}

LuaCEmbedResponse *read_raw_body(LuaCEmbedTable *self,LuaCEmbed *args){
    long max_size = lw.args.get_long(args,0);

    if(lw.has_errors(args)){
        char *error_message = lw.get_error_message(args);
        return lw.response.send_error(error_message);
    }

    unsigned char *content= cb.request.read_content(cbrq,max_size);

    if(content == NULL){
        return  NULL;
    }

    return lw.response.send_raw_string((char*)content,cbrq->content_length);
}