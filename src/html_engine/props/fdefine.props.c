//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end


TextOrError render_style_prop_if_its_a_table(LuaCEmbedTable *props) {
  TextOrError result = {0};
  int type = LuaCEmbedTable_get_type_prop(props, "style");

  if (type != LUA_CEMBED_TABLE) {
    return result;
  }
  LuaCEmbedTable *style_table = LuaCEmbedTable_get_sub_table_by_key(props, "style");

  CTextStack *stack = newCTextStack_string_empty();
  int size = LuaCEmbedTable_get_full_size(style_table);
  CTextStack_format(stack, "style=\"");
  for (int i = 0; i < size; i++) {

    char *key = LuaCembedTable_get_key_by_index(style_table, i);
    char *value = LuaCembedTable_get_string_prop(style_table, key);

    if (LuaCEmbed_has_errors(props->main_object)) {
      CTextStack_free(stack);
      char *error_msg = LuaCEmbed_get_error_message(props->main_object);
      result.error = LuaCEmbed_send_error(error_msg);
      return result;
    }

    CTextStack_format(stack, "%s:%s;", key, value);
  }
  CTextStack_format(stack, "\"");
  result.text = CTextStack_self_transform_in_string_and_self_clear(stack);
  return result;
}

TextOrError render_table_as_prop(LuaCEmbedTable *prop) {
  TextOrError result = {0};

  long size = LuaCEmbedTable_get_full_size(prop);
  CTextStack *stack = newCTextStack_string_empty();
  for (int i = 0; i < size; i++) {
    char *key = LuaCembedTable_get_key_by_index(prop, i);

    if (strcmp(key, "style") == 0) {
      TextOrError result_style = render_style_prop_if_its_a_table(prop);
      if (result_style.error) {
        return result_style;
      }

      if (result_style.text != NULL) {
        CTextStack_format(stack, "%s", result_style.text);
        free(result_style.text);
        continue;
      }
    }

    char *value = LuaCembedTable_get_string_prop(prop, key);

    if (LuaCEmbed_has_errors(prop->main_object)) {
      CTextStack_free(stack);
      char *error_msg = LuaCEmbed_get_error_message(prop->main_object);
      result.error = LuaCEmbed_send_error(error_msg);
      return result;
    }

    CTextStack_format(stack, "%s=\"%s\" ", key, value);
  }
  result.text = CTextStack_self_transform_in_string_and_self_clear(stack);
  return result;
}

TextOrError render_props(LuaCEmbedTable *internal_props) {

  TextOrError result = {0};

  CTextStack *stack = newCTextStack_string_empty();

  int size = LuaCEmbedTable_get_full_size(internal_props);
  for (int i = 0; i < size; i++) {
    int element_type = LuaCEmbedTable_get_type_by_index(internal_props, i);
    if (element_type != LUA_CEMBED_TABLE) {
      continue;
    }

    LuaCEmbedTable *prop = LuaCEmbedTable_get_sub_table_by_index(internal_props, i);

    if (its_a_component(prop)) {
      continue;
    }

    TextOrError render_result = render_table_as_prop(prop);
    if (render_result.error) {
      return render_result;
    }

    CTextStack_format(stack, "%s", render_result.text);
    free(render_result.text);
  }

  result.text = CTextStack_self_transform_in_string_and_self_clear(stack);
  return result;
}
