#### Build
Change CMakeLists.txt outout path for youself and execute `sh ./build.sh`

#### Contain Functions
    g_bit_or  : group bit 'or' operator 

    slideleft(int,int)  : like shiftleft , only int support
    slideright(int,int) : like shiftright, only int support
        there is a bug for shiftleft see https://issues.apache.org/jira/browse/IMPALA-11462?jql=text%20~%20%22shiftleft%22
    
    bitcount(int...)    : counts how many true values in args, for example bitcount(15) = 4 , bitcount(3,15) = 6 
    
    range_bitcount(int from,int to,int ...) : counts how many true values in args with bit range , start from right to left
        for example 
        
        range_bitcount(27,67,15,1,67108864) = 5
        
        arg0 = 15                                 arg1 = 1                                  arg2 = 67108864
                                           to 67                                                 from 27
        0000 0000 0000 0000 0000 0000 0000 1111 , 0000 0000 0000 0000 0000 0000 0000 0001 , 0000 0100 0000 0000 0000 0000 0000 0000
                                           1111 , 0000 0000 0000 0000 0000 0000 0000 0001 , 0000 0

#### g_bit_or CASE
```
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
```
