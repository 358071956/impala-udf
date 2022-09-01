修改CMakeLists.txt的输出路径
执行 sh ./build.sh

包含
  一个“组内按位‘或’”计算的g_bit_or uda方法

  两个slideleft(int,int)，slideright(int,int)方法，对应impala中的shift方法，
     用于解决shift方法在进行不确定值进行位移的时候，会强制变为byte进行位移的bug

  两个bitcount(int...)，range_bitcount(int from,int to,int ...)
     bitcount(1,3) = 3        ， 统计总共有多少个1
     range_bitcount(2,3,15) = 2  ， 15=0x00FF,统计从低位第2位到第3位包含多少个1

案例
create aggregate function g_bit_or(bigint) returns bigint
  location '/user/hive/udf/libudamy.so'
  INIT_FN='BitOrInit'
  UPDATE_FN='BitOrUpdate'
  MERGE_FN='BitOrMerge'
  FINALIZE_FN='BitOrFinalize';

CREATE TABLE IF NOT EXISTS test(
    k STRING NOT NULL,
    v BIGINT NOT NULL,
    PRIMARY KEY (k)
) STORED AS KUDU;

Insert into test values("a",1),("a",2),("a",2048),("b",10),("b",128);

select k,g_bit_or(v) from test group by k
