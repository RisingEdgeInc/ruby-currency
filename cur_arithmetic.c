#include "ruby.h"

#include "currency.h"
#include "cur_helpers.h"
#include "cur_arithmetic.h"

// Perform arithmetic operation on two values
long long cur_llop(long long first, long long second, int precision, op_type op);

VALUE
cur_add(VALUE self, VALUE other)
{
  VALUE r_val;
  switch (TYPE(other))
  {
    case T_FIXNUM:
    case T_BIGNUM:
    case T_FLOAT:
      r_val = cur_op_number(self, other, cur_op_add);
      break;
    case T_OBJECT:
      r_val = cur_op_cur(self, other, cur_op_add);
      break;
    default:
      rb_raise(rb_eTypeError, "Can only add Currency with numbers or other Currency");
      break;
  }
  
  return r_val;
}

VALUE
cur_sub(VALUE self, VALUE other)
{
  VALUE r_val;
  switch(TYPE(other))
  {
    case T_FIXNUM:
    case T_BIGNUM:
    case T_FLOAT:
      r_val = cur_op_number(self, other, cur_op_sub);
      break;
    case T_OBJECT:
      r_val = cur_op_cur(self, other, cur_op_sub);
      break;
    default:
      rb_raise(rb_eTypeError, "Can only subtract Currency with numbers or another Currency");
      return;
      break;
  }
  
  return r_val;
}

VALUE
cur_mul(VALUE self, VALUE other)
{
  VALUE r_val;
  switch(TYPE(other))
  {
    case T_FIXNUM:
    case T_BIGNUM:
    case T_FLOAT:
      r_val = cur_op_number(self, other, cur_op_mul);
      break;
    case T_OBJECT:
      r_val = cur_op_cur(self, other, cur_op_mul);
      break;
    default:
      rb_raise(rb_eTypeError, "Can only subtract Currency with numbers or another Currency");
      break;
  }
  
  return r_val;
}

VALUE
cur_div(VALUE self, VALUE other)
{
  VALUE r_val;
  switch(TYPE(other))
  {
    case T_FIXNUM:
    case T_BIGNUM:
    case T_FLOAT:
      r_val = cur_op_number(self, other, cur_op_div);
      break;
    case T_OBJECT:
      r_val = cur_op_cur(self, other, cur_op_div);
      break;
    default:
      rb_raise(rb_eTypeError, "Can only subtract Currency with numbers or another Currency");
      break;
  }
  
  return r_val;
}

VALUE
cur_op_number(VALUE self, VALUE other, op_type op)
{
  long long other_value = NUM2LL(other);
  long long curr_value = NUM2LL(rb_iv_get(self, "@value"));
  long double result = 0;
  
  VALUE argv[1];
  switch(op)
  {
    case cur_op_add:
      argv[0] = LL2NUM(curr_value + other_value);
      break;
    case cur_op_sub:
      argv[0] = LL2NUM(curr_value - other_value);
      break;
    case cur_op_mul:
      argv[0] = LL2NUM(curr_value * other_value);
      break;
    case cur_op_div:
      result = (long double) curr_value / other_value;
      argv[0] = LL2NUM(llroundl(result));
      break;
    default:
      rb_raise(rb_eTypeError, "Unknown operator type");
      return self;
      break;
  }
  
  return rb_class_new_instance(1, argv, cCurrency);
}

VALUE
cur_op_cur(VALUE self, VALUE other, op_type op)
{  
  int precision = NUM2INT(rb_iv_get(self, "@precision"));
  int other_precision = NUM2INT(rb_iv_get(other, "@precision"));
  
  long long value = NUM2LL(rb_iv_get(self, "@value"));
  long long other_value = NUM2LL(rb_iv_get(other, "@value"));
  
  int cmp_result = cur_precision_cmp(self, other);

  if (cmp_result > 0) {
    other_value = other_value * pow(10, (precision - other_precision));
  } else if (cmp_result < 0) {
    value = value * pow(10, (other_precision - precision));
    precision = other_precision;
  }
  
  VALUE argv[1];
  long long result = cur_llop(value, other_value, precision, op);
  argv[0] = LL2NUM(result);
  
  VALUE r_val = rb_class_new_instance(1, argv, cCurrency);
  rb_iv_set(r_val, "@precision", INT2NUM(precision));
  
  return r_val;
  
}

long long
cur_llop(long long first, long long second, int precision, op_type op)
{
  long long result = 0;
  long double dbl_result = 0;
  
  switch(op) {
    case cur_op_add:
      result = first + second;
      break;
    case cur_op_sub:
      result = first - second;
      break;
    case cur_op_mul:
      result = (first * second) / pow(10, precision);
      break;
    case cur_op_div:
      dbl_result = (long double) first / second;
      dbl_result = dbl_result * pow(10, precision);
      result = llroundl(dbl_result);
      break;
  }
  
  return result;
}
