#ifndef CUR_ARITHMATIC_H_
#define CUR_ARITHMATIC_H_

extern VALUE cCurrency;

typedef enum
{
  cur_op_add,
  cur_op_sub,
  cur_op_mul,
  cur_op_div
} op_type;

VALUE cur_add(VALUE self, VALUE other);
VALUE cur_sub(VALUE self, VALUE other);
VALUE cur_mul(VALUE self, VALUE other);
VALUE cur_div(VALUE self, VALUE other);

VALUE cur_op_number(VALUE self, VALUE other, op_type type);
VALUE cur_op_cur(VALUE self, VALUE other, op_type type);

#endif