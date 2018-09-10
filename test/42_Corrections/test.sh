gcc test0.c -o bin/test0
gcc test1.c -o bin/test1
gcc test2.c -o bin/test2
gcc test3.c -o bin/test3
gcc test3bis.c -o bin/test3bis
gcc test4.c -o bin/test4

echo -- test0:
./run.sh /usr/bin/time -l bin/test0
read
echo -- test1:
./run.sh /usr/bin/time -l bin/test1
read
echo -- test2:
./run.sh /usr/bin/time -l bin/test2
read
echo -- test3:
./run.sh bin/test3
read
echo -- test3bis:
./run.sh bin/test3bis
read
echo -- test4:
./run.sh bin/test4
