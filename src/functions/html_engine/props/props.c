
#include "../uniq.definitions_requirements.h"

TextOrError render_style_prop_if_its_a_table(LuaCEmbedTable *props) {
  TextOrError result = {0};
  int type = lw.tables.get_type_prop(props, "style");

  if (type != lw.types.TABLE) {
    return result;
  }
  LuaCEmbedTable *style_table = lw.tables.get_sub_table_prop(props, "style");

  CTextStack *stack = newCTextStack_string_empty();
  int size = lw.tables.get_size(style_table);
  CTextStack_format(stack, "style=\"");
  for (int i = 0; i < size; i++) {

    char *key = lw.tables.get_key_by_index(style_table, i);
    char *value = lw.tables.get_string_prop(style_table, key);

    if (lw.has_errors(props->main_object)) {
      CTextStack_free(stack);
      char *error_msg = lw.get_error_message(props->main_object);
      result.error = lw.response.send_error(error_msg);
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

  long size = lw.tables.get_size(prop);
  CTextStack *stack = newCTextStack_string_empty();
  for (int i = 0; i < size; i++) {
    char *key = lw.tables.get_key_by_index(prop, i);

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

    char *value = lw.tables.get_string_prop(prop, key);

    if (lw.has_errors(prop->main_object)) {
      CTextStack_free(stack);
      char *error_msg = lw.get_error_message(prop->main_object);
      result.error = lw.response.send_error(error_msg);
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

  int size = lw.tables.get_size(internal_props);
  for (int i = 0; i < size; i++) {
    int element_type = lw.tables.get_type_by_index(internal_props, i);
    if (element_type != lw.types.TABLE) {
      continue;
    }

    LuaCEmbedTable *prop = lw.tables.get_sub_table_by_index(internal_props, i);

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
