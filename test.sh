SOURCE=$(ls -N -b *.c | tr "\n" " ")
tcc ${SOURCE} -o test.exe -g -run