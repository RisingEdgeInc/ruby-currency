#ifndef CUR_HELPERS_H_
#define CUR_HELPERS_H_

#define MIN(a,b) (((a < b)) ? a : b)
#define MAX(a,b) (((a > b)) ? a : b)


/* Test whether two currences are of the same type */
int cur_code_cmp(VALUE self, VALUE other);

/* Compares the precision values of two Currency objects
  
   Returns 0 if both precisions have the same value
   Returns 1 if self has a greater precision than other
   Returns -1 if other has a greater precision than self
*/
int cur_precision_cmp(VALUE self, VALUE other);

/* Adds thousands separators to a string representation of a value */
void cur_ts_str(char *unformatted, char separator, char *formatted);
#endif