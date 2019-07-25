CFLAGS= -Wall -fpic -coverage -lm -std=c99

test: testme.c
	gcc -o quiz testme.c -g $(CFLAGS)
runTest: test
	-./quiz
	gcov -b testme.c

clean:
	rm -f *.o quiz quiz.exe *.gcov *.gcda *.gcno
