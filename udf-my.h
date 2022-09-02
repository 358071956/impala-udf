#ifndef IMPALA_UDF_MY_UDF_H
#define IMPALA_UDF_MY_UDF_H

#include "udf/udf.h"

using namespace impala_udf;


IntVal SlideLeft(FunctionContext* context, const IntVal& arg1, const IntVal& arg2);
IntVal SlideRight(FunctionContext* context, const IntVal& arg1, const IntVal& arg2);
IntVal BitCount(FunctionContext* context, int num_var_args, const IntVal* args);
IntVal RangeBitCount(FunctionContext* context, const IntVal& from, const IntVal& to, int num_var_args, const IntVal* args);

#endif
