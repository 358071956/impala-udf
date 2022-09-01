/**
 * BitOr对int/bigint类型的分组字段进行or运算
 */

#include "uda-my.h"
#include <assert.h>

using namespace impala_udf;

IMPALA_UDF_EXPORT
void BitOrInit(FunctionContext* context, IntVal* val) {
  val->is_null = false;
  val->val = 0;
}
IMPALA_UDF_EXPORT
void BitOrInit(FunctionContext* context, BigIntVal* val) {
  val->is_null = false;
  val->val = 0;
}

IMPALA_UDF_EXPORT
void BitOrUpdate(FunctionContext* context, const BigIntVal& input, BigIntVal* val) {
  if (input.is_null) return;
  val->val = val->val | input.val;
}
IMPALA_UDF_EXPORT
void BitOrUpdate(FunctionContext* context, const IntVal& input, IntVal* val) {
  if (input.is_null) return;
  val->val = val->val | input.val;
}

IMPALA_UDF_EXPORT
void BitOrMerge(FunctionContext* context, const BigIntVal& src, BigIntVal* dst) {
  dst->val = dst->val | src.val;
}
IMPALA_UDF_EXPORT
void BitOrMerge(FunctionContext* context, const IntVal& src, IntVal* dst) {
  dst->val = dst->val | src.val;
}

IMPALA_UDF_EXPORT
BigIntVal BitOrFinalize(FunctionContext* context, const BigIntVal& val) {
  return val;
}
IMPALA_UDF_EXPORT
IntVal BitOrFinalize(FunctionContext* context, const IntVal& val) {
  return val;
}
