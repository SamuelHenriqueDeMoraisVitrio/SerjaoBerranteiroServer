

#include "dependencies/all.h"
#include "conf.h"

DtwNamespace dtw;
CTextStackModule stack;

void parse_code(CTextStack *final,const char *content){
    int content_size = strlen(content);
    for(int i = 0; i < content_size; i++)
    {
        char buffer[10] = {0};
        sprintf(buffer,"\\x%x",content[i]);
        stack.format(final,"%s",buffer);
    }
}
int  create_lua_code(){
    if(dtw.entity_type(LUA_FOLDER) != DTW_FOLDER_TYPE){
        printf("lua code its not a folder\n");
        return 1;
    }

    UniversalGarbage *garbage = newUniversalGarbage();


    CTextStack * final = stack.newStack_string_format("const char *%s= \"",LUA_VAR_NAME);
    UniversalGarbage_add(garbage,stack.free,final);

    DtwTree * tree = dtw.tree.newTree();
    UniversalGarbage_add(garbage,dtw.tree.free,tree);

    dtw.tree.add_tree_from_hardware(tree,LUA_FOLDER,(DtwTreeProps){
        .hadware_data = DTW_INCLUDE,
        .path_atributes = DTW_INCLUDE
    });

    const char *main_code = NULL;
    for(int i = 0; i < tree->size;i++){

        DtwTreePart *current_file = tree->tree_parts[i];

        DtwPath *current_path = current_file->path;

        char *full_name = dtw.path.get_full_name(current_path);
        //means its a folder
        if(full_name == NULL){
            continue;
        }

        char *extension = dtw.path.get_extension(current_path);

        if(extension){
            if(strcmp(extension,"lua") != 0){
                continue;
            }
        }


        if(current_file->content == NULL || current_file->is_binary){
            char *full_path = dtw.path.get_path(current_path);
            printf("impossible to load content of %s\n",full_path);
            UniversalGarbage_free(garbage);
            return 1;
        }



        if(strcmp(full_name,"main.lua")==0){
            main_code = (char*)current_file->content;
        }
        parse_code(final,(char*)current_file->content);
        parse_code(final,"\n");
    }
    if(main_code == NULL){
        printf("main code not provided\n");
        UniversalGarbage_free(garbage);
        return 1;
    }
    stack.format(final,"\";");

    dtw.write_string_file_content(OUTPUT,final->rendered_text);


    UniversalGarbage_free(garbage);
    return 0;
}
int main(){
    dtw = newDtwNamespace();
    stack = newCTextStackModule();

    int error = create_lua_code();
    if(error){
        return error;
    }



    CTextStack *final_compilation = stack.newStack_string_format("gcc c/main.c -o %s",FINAL_OUPTUT);
    error = system(final_compilation->rendered_text);
    stack.free(final_compilation);
    if(error){
        return error;
    }
    #ifdef  RUN_AFTER

    CTextStack *run_command = stack.newStack_string_format("./%s",FINAL_OUPTUT);
        error = system(run_command->rendered_text);
        stack.free(run_command);

        if(error){
            return error;
        }
    #endif


    return 0;
}
