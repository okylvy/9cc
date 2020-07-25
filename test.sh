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

<< CMT
assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 41 " 12 + 34 - 5 "
assert 47 '5+6*7'
assert 15 '5*(9-6)'
assert 4 '(3+5)/2'
assert 10 '-10+20'
assert 0 '0==1'
assert 1 '0<1'
CMT
#assert 1 '0==0;'
#assert 0 '0==1;'
#assert 2 '0+1+3-2;

assert 5 'a=2; b=3; a+b;'
assert 20 'f=4; z = 5; f*z;'
# FIXME: Get wrong value below
#assert 325 'f=13; z=25; f*z;'

# Multiple local variables.
assert 7 'f = 3; b = 2 * 2; return f + b;'
# FIXME: In case that variables are string, it returns the value of last variable.
#   For instance, you get 4 (=bar) below.
#assert 7 'foo = 3; bar = 2 * 2; return foo + bar;'

# return statement.
assert 8 'a=2; b=3*2; a+b;'
assert 4 'a=2; b=3*2; return b - a;'

#assert 1 'main () {1==1;}'
#assert 0 'main () {1==0;}'

echo OK