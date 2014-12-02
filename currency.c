#include <stdlib.h>
#include <string.h>
#include "ruby.h"

#include "currency.h"
#include "cur_helpers.h"
#include "cur_arithmetic.h"

// Converters
VALUE c_to_s(VALUE self);
VALUE c_to_f(VALUE self);
VALUE c_to_i(VALUE self);
VALUE c_to_json(VALUE self);

// Accessors
VALUE c_value(VALUE self);

// Initialization
static VALUE c_init(VALUE klass, VALUE amount);

VALUE 
c_to_i(VALUE self)
{
  return rb_iv_get(self, "@value");
}

VALUE 
c_to_f(VALUE self)
{
  long long value = NUM2LL(rb_iv_get(self, "@value"));
  int precision = NUM2INT(rb_iv_get(self, "@precision"));
  double f_value = value / pow(10, precision);
  return rb_float_new(f_value);
}

VALUE 
c_to_s(VALUE self)
{
  char *u_dec_left;
  char *formatted = ALLOC_N(char, 256);
  
  long long value = NUM2LL(rb_iv_get(self, "@value"));
  int frac_digits = NUM2INT(rb_iv_get(self, "@fraction_digits"));
  VALUE v_decimal_pt = rb_iv_get(self, "@decimal_point");
  VALUE v_symbol = rb_iv_get(self, "@symbol");
    
  char *symbol = StringValueCStr(v_symbol);
  char *decimal_pt = StringValueCStr(v_decimal_pt);
  
  long long i_value = (long long) (value / pow(10, frac_digits));
  long long no_dec_value = i_value * pow(10, frac_digits);
  long long fraction = value - no_dec_value;
  
  VALUE v_integral = rb_sprintf("%lld", llabs(i_value));
  u_dec_left = StringValueCStr(v_integral);
  
  VALUE v_thousands_sep = rb_iv_get(self, "@thousands_separator");
  char *thousands_sep = StringValueCStr(v_thousands_sep);
  
  cur_ts_str(u_dec_left, thousands_sep[0], formatted); 

  char *neg = (value > 0 ? "" : "-");
  VALUE r_val = rb_sprintf("%s%s%s%s%.2lld", neg, symbol, formatted, decimal_pt, fraction);
  return r_val;
//    xfree(formatted);
//    return r_val;
}

VALUE
c_value(VALUE self)
{
  return rb_iv_get(self, "@value");
}

VALUE
c_set_precision(VALUE self, VALUE new_precision)
{
  rb_iv_set(self, "@precision", new_precision);
  return self;
}

VALUE 
c_set_code(VALUE self, VALUE new_code)
{
  rb_iv_set(self, "@code", new_code);
  return self;
}

// Intialization
static VALUE 
c_init(VALUE self, VALUE amount)
{
  rb_iv_set(self, "@symbol", rb_str_new2("$"));
  rb_iv_set(self, "@code", rb_str_new2("USD"));
  rb_iv_set(self, "@value", amount);
  rb_iv_set(self, "@precision", INT2NUM(2));
  rb_iv_set(self, "@fraction_digits", INT2NUM(2));
  rb_iv_set(self, "@decimal_point", rb_str_new2("."));
  rb_iv_set(self, "@thousands_separator", rb_str_new2(","));
  return self;
}

void 
Init_Currency()
{
  cCurrency = rb_define_class("Currency", rb_cObject);
  rb_define_method(cCurrency, "initialize", c_init, 1);
  
  // Setters
  rb_define_method(cCurrency, "precision=", c_set_precision, 1);
  rb_define_method(cCurrency, "code=", c_set_code, 1);
  
  // Conversions
	rb_define_method(cCurrency, "to_s", c_to_s, 0);
  rb_define_method(cCurrency, "to_i", c_to_i, 0);
  rb_define_method(cCurrency, "to_f", c_to_f, 0);
  
  // Arithmatic
  rb_define_method(cCurrency, "-", cur_sub, 1);  
  rb_define_method(cCurrency, "+", cur_add, 1);
  rb_define_method(cCurrency, "*", cur_mul, 1);
  rb_define_method(cCurrency, "/", cur_div, 1);
}
