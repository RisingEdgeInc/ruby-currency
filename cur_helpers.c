#include <stdbool.h>
#include "ruby.h"

#include "cur_helpers.h"

int
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

void
cur_ts_str(char *unformatted, char separator, char *formatted)
{
    int u_pos = strlen(unformatted);
    int seps = u_pos / 3;
    if(u_pos % 3 == 0) { seps--; }
    int f_pos = u_pos + seps;
    int d_count = 0;
    
    while(f_pos >= 0)
    {
        formatted[f_pos--] = unformatted[u_pos--];
        if( (d_count > 0) && (d_count % 3 == 0) )
        {
            formatted[f_pos--] = separator;
        }
        d_count++;
    }
}
