#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0

testing(){
    ./s21_cat $var bytes.txt> test_s21_cat.log 
    cat $var bytes.txt> test_cat.log
    if cmp -s test_s21_cat.log test_cat.log
    then
    (( SUCCESS++ ))
    (( COUNTER++ ))

    echo "Test  №$COUNTER SUCCESS $var";
    else
    (( FAIL++ ))
    (( COUNTER++ ))

    echo "Test  №$COUNTER FAIL $var"
    fi
    rm test_s21_cat.log test_cat.log
}

testing

for var1 in e b n s t v
do  
    var="-$var1"
    testing $var
done

for var1 in e b n s t v
do  
    for var2 in e b n s t v
    do  
        if [ $var1 != $var2 ]
        then
            var="-$var1 -$var2"
            testing $var
        fi
    done
done

for var1 in e b n s t v
do  
    for var2 in e b n s t v
    do  
        for var3 in e b n s t v
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then  
                var="-$var1 -$var2 -$var3"
                testing $var
            fi
        done
    done
done

for var1 in -number-nonblank -number -squeeze-blank
do  
    for var2 in -number-nonblank -number -squeeze-blank
    do  
        for var3 in -number-nonblank -number -squeeze-blank
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then  
                var="-$var1 -$var2 -$var3"
                testing $var
            fi
        done
    done
done
 
for var1 in e b n s t v
do  
    for var2 in e b n s t v
    do  
        for var3 in e b n s t v
        do
            for var4 in e b n s t v
            do
                if [ $var1 != $var2 ] && [ $var1 != $var3 ] && [ $var1 != $var4 ] && [ $var2 != $var3 ] && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then  
                    var="-$var1 -$var2 -$var3 -$var4"
                    testing $var
                fi
            done
        done
    done
done

for var1 in e b n s t v
do  
    for var2 in e b n s t v
    do  
        for var3 in e b n s t v
        do
            for var4 in e b n s t v
            do  
                for var5 in e b n s t v
                do
                    if [ $var1 != $var2 ] && [ $var1 != $var3 ] && [ $var1 != $var4 ] && [ $var1 != $var5 ] && [ $var2 != $var3 ] && [ $var2 != $var4 ] && [ $var2 != $var5 ] && [ $var3 != $var4 ] && [ $var3 != $var5 ] && [ $var4 != $var5 ]
                    then  
                        var="-$var1 -$var2 -$var3 -$var4 -$var5"
                        testing $var
                    fi
                done
            done
        done
    done
done

for var1 in e b n s t v
do  
    for var2 in e b n s t v
    do  
        for var3 in e b n s t v
        do
            for var4 in e b n s t v
            do  
                for var5 in e b n s t v
                do
                    for var6 in e b n s t v
                    do
                        if [ $var1 != $var2 ] && [ $var1 != $var3 ] && [ $var1 != $var4 ] && [ $var1 != $var5 ] && [ $var1 != $var6 ] && [ $var2 != $var3 ] && [ $var2 != $var4 ] && [ $var2 != $var5 ] && [ $var2 != $var6 ] && [ $var3 != $var4 ] && [ $var3 != $var5 ] && [ $var3 != $var6 ] && [ $var4 != $var5 ] && [ $var4 != $var6 ] && [ $var5 != $var6 ]
                        then
                            var="-$var1 -$var2 -$var3 -$var4 -$var5 -$var6"
                            testing $var
                        fi
                    done
                done
            done
        done
    done
done

echo "SUCCESS  $SUCCESS from $COUNTER ";
echo "FAILED  $FAIL from $COUNTER ";