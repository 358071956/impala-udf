#ifndef IMPALA_UDF_MY_UDA_H
#define IMPALA_UDF_MY_UDA_H

#include "udf/udf.h"

using namespace impala_udf;

void BitOrInit(FunctionContext* context, BigIntVal* val);
void BitOrInit(FunctionContext* context, IntVal* val);

void BitOrUpdate(FunctionContext* context, const BigIntVal& input, BigIntVal* val);
void BitOrUpdate(FunctionContext* context, const IntVal& input, IntVal* val);

void BitOrMerge(FunctionContext* context, const BigIntVal& src, BigIntVal* dst);
void BitOrMerge(FunctionContext* context, const IntVal& src, IntVal* dst);

BigIntVal BitOrFinalize(FunctionContext* context, const BigIntVal& val);
IntVal BitOrFinalize(FunctionContext* context, const IntVal& val);

#endif
