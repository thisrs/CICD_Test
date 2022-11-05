#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
TEST_FILE="./grep_tests/test.txt ./grep_tests/test2.txt"
TEST_WORD="-e tHe"

cat /dev/null > log.txt
echo "Тест"

for var in -i -v -c -l -n -o -h -s
do
          TEST1="$var -i  -e is $TEST_WORD $TEST_FILE"
          TEST2="$var -e io -e '[A-Z]' -o $TEST_FILE"
          TEST3="$var $TEST_FILE -e '.' -c"
          TEST4="$var $TEST_FILE -e 1 -e 2 -e 3 -n"
          echo "$TEST1"
          ./s21_grep $TEST1 > s21_grep.txt
          grep $TEST1 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
          fi
          # === TEST 2 ===
          ./s21_grep $TEST2 > s21_grep2.txt
          grep $TEST2 > grep2.txt
          DIFF_RES="$(diff -s s21_grep2.txt grep2.txt)"
          if [ "$DIFF_RES" == "Files s21_grep2.txt and grep2.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            
          fi

          # === TEST 3 ===
          ./s21_grep $TEST3 > s21_grep3.txt
          grep $TEST3 > grep3.txt
          DIFF_RES="$(diff -s s21_grep3.txt grep3.txt)"
          if [ "$DIFF_RES" == "Files s21_grep3.txt and grep3.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
          fi

          # === TEST 4 ===
          ./s21_grep $TEST4 > s21_grep4.txt
          grep $TEST4 > grep4.txt
          DIFF_RES="$(diff -s s21_grep4.txt grep4.txt)"
          if [ "$DIFF_RES" == "Files s21_grep4.txt and grep4.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
          fi

          rm s21_grep*.txt grep*.txt
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"
