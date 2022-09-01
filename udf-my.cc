#include "udf-my.h"
#include <bitset>

using namespace std;

/**
 * DROP FUNCTION slideleft(int,int);
 * CREATE FUNCTION slideleft(int,int) RETURNS INT LOCATION '/udf-dir/libudfmy.so' symbol='SlideLeft';
 */ 
IMPALA_UDF_EXPORT
IntVal SlideLeft(FunctionContext* context, const IntVal& arg1, const IntVal& arg2) {
    if (arg1.is_null || arg2.is_null) return IntVal::null();

    return arg1.val << arg2.val;
}

/**
 * DROP FUNCTION slideright(int,int);
 * CREATE FUNCTION slideright(int,int) RETURNS INT LOCATION '/udf-dir/libudfmy.so' symbol='SlideRight';
 */ 
IMPALA_UDF_EXPORT
IntVal SlideRight(FunctionContext* context, const IntVal& arg1, const IntVal& arg2) {
    if (arg1.is_null || arg2.is_null) return IntVal::null();

    if(0 == arg2.val){
        return arg1.val;
    }

    uint32_t mask = 0x7FFFFFFF;

    if(-1 == arg1.val && 1 == arg2.val){
        return arg1.val & mask;
    }

    if(arg1.val < 0){
        return (arg1.val & mask) >> (arg2.val-1);
    }

    return arg1.val >> arg2.val;
}


/**
 * DROP FUNCTION bitcount(int ...);
 * CREATE FUNCTION bitcount(int ...) RETURNS INT LOCATION '/udf-dir/libudfmy.so' symbol='BitCount';
 */
IMPALA_UDF_EXPORT
IntVal BitCount(FunctionContext* context, int num_args, const IntVal* args){

    bitset<32> bin;

    int count = 0;

    for(int i=0;i<num_args;i++)
    {
        bin = args[i].val;

        count += bin.count();
    }

    return count;
}

/**
 * 传入多个int32值，统计包含多少个1值，，from/end的值从0开始算起，其余参数位于左边的值为高位
 * 
 * range_bitcount(27,67,15,1,67108864) = 5
 * 
 * 参数0 = 15                                 参数1 = 1                                 参数2 = 67108864
 *                                    到第67位                                               从第27位
 * 0000 0000 0000 0000 0000 0000 0000 1111 , 0000 0000 0000 0000 0000 0000 0000 0001 , 0000 0100 0000 0000 0000 0000 0000 0000
 *                                    1111 , 0000 0000 0000 0000 0000 0000 0000 0001 , 0000 0
 * 
 * DROP FUNCTION range_bitcount(int,int,int ...);
 * CREATE FUNCTION range_bitcount(int,int,int ...) RETURNS INT LOCATION '/udf-dir/libudfmy.so' symbol='RangeBitCount';
 */
IMPALA_UDF_EXPORT
IntVal RangeBitCount(FunctionContext* context, const IntVal& from, const IntVal& end,int num_args, const IntVal* args){

    if(from.val < 0 || end.val < 0){
        return -1;
    }

    bitset<32> bin;

    int arg_from = from.val/32;
    int arg_end  = end.val/32;
    int count = 0;

    for(int i=arg_from;i<=arg_end;i++)
    {
        bin = args[num_args - i - 1].val;

        if(i == arg_from){
            //第一个参数要进行右移，去除低位多余信息
            bin >>= (from.val % 32);
            //假如有一个参数，那么需要移回去，避免下一步左移截断高位bit的时候出错
            bin <<= (from.val % 32);
        }

        if(i == arg_end){
            //最后一个参数要进行左移，去除高位多余信息
            bin <<= (32 - (end.val % 32 + 1));
        }

        count += bin.count();
    }

    return count;
}
