#!/bin/sh

num_tests=0
num_failed=0
for f in test*[0-9]; do
  num_tests=$(($num_tests + 1))
  t=$f
  `./$f > $t.out`
  diff -qb "$t.out" "$t.ans"
  if [ $? -eq 1 ]
  then
    echo "$f failed"
    num_failed=$(($num_failed + 1))
  else
    true # can't have empty else
    #echo "$f passed"
  fi
done

echo "$num_tests tests conducted, $num_failed failed."
exit $num_failed
