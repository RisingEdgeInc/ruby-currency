#include <stdbool.h>
#include "ruby.h"

#include "cur_helpers.h"

bool
cur_code_cmp(VALUE self, VALUE other)
{
  VALUE v_other_code = rb_iv_get(other, "@code");
  char *other_code = StringValueCStr(v_other_code);
  
  VALUE v_code = rb_iv_get(self, "@code");
  char *code = StringValueCStr(v_code);
  
  return strcmp(code, other_code);
}

int
cur_precision_cmp(VALUE self, VALUE other)
{
  int precision = NUM2INT(rb_iv_get(self, "@precision"));
  int other_precision = NUM2INT(rb_iv_get(other, "@precision"));

  if (precision > other_precision) {
    return 1;
  } else if (precision < other_precision) {
    return -1;
  } else {
    return 0;
  }
}