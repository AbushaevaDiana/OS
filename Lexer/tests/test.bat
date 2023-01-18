@echo off

set PROGRAM="Lexer.exe"

::1
set IN1="test1.txt"
set OUT1="output.txt"
set CORRECT1="output1.txt"

echo "checking numbers, identifiers, logical operators, data type, string, char, keywords, seporators, input and output operators, endl"
%PROGRAM% %IN1% %OUT1% || goto err
fc %OUT1% %CORRECT1% || goto err

::2
set IN2="test2.txt"
set OUT2="output.txt"
set CORRECT2="output2.txt"

echo "checking for, if, true, false, data type, numbers with point, return, const, while, vector, separators, arifmetic operators, break, continue"
%PROGRAM% %IN2% %OUT2% || goto err
fc %OUT2% %CORRECT2% || goto err

::3
set IN3="test3.txt"
set OUT3="output.txt"
set CORRECT3="output3.txt"

echo "checking using namespace std, void, logical operators"
%PROGRAM% %IN3% %OUT3% || goto err
fc %OUT3% %CORRECT3% || goto err

::4
set IN4="test4.txt"
set OUT4="output.txt"
set CORRECT4="output4.txt"

echo "checking comments, struct"
%PROGRAM% %IN4% %OUT4% || goto err
fc %OUT4% %CORRECT4% || goto err

::5
set IN5="test5.txt"
set OUT5="output.txt"
set CORRECT5="output5.txt"

echo "checking negativ"
%PROGRAM% %IN5% %OUT5% || goto err
fc %OUT5% %CORRECT5% || goto err

echo ALL TESTS PASSED
exit /B 0

:err
echo PROGRAM TESTING FAILE
exit /B 1