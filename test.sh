#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./hicc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual" 
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

# Basic arithmetic
echo
echo "*** Basic arithmetic ***"
assert 0 '0;'
assert 42 '42;'
assert 21 '5+20-4;'
assert 41 ' 12 + 34 - 5 ;'
assert 47 '5+6*7;'
assert 15 '5*(9-6);'
assert 4 '(3+5)/2;'
assert 10 '-10+20;'
assert 2 '0+1+3-2;'
echo

# Conditional
echo "*** Conditional ***"
assert 0 '0==1;'
assert 1 '0<1;'
assert 1 '0==0;'
assert 0 '0==1;'
echo

# Local variables: a character
echo "*** Local variables: a character ***"
assert 5 'a=2; b=3; a+b;'
assert 20 'f=4; z = 5; f*z;'
# FIXME: Get wrong value below
#assert 325 'f=13; z=25; f*z;'
echo

# Local variables: string
echo "*** Local variables: string ***"
assert 7 'hoge = 3; fuga = 2 * 2; return hoge + fuga;'
echo

# return statement
echo "*** return statement ***"
assert 8 'a=2; b=3*2; a+b;'
assert 4 'a=2; b=3*2; return b - a;'
echo

# if statement.
echo "*** if statement ***"
assert 3 'if(0) return 2; return 3;'
assert 3 'if(1-1) return 2; return 3;'
assert 2 'if(1) return 2; return 3;'
assert 2 'if(1==1) return 2; return 3;'
assert 3 'if(1==0) return 2; return 3;'
assert 3 'if (1==0) return 2; else return 3;'
echo

#assert 1 'main () {1==1;}'
#assert 0 'main () {1==0;}'

echo -e "\e[32mOK\e[m"
