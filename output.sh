#!/bin/sh

gcc matrix.c -lpthread

./a.out pequena 1 > output.txt
./a.out pequena 2 >> output.txt
./a.out pequena 4 >> output.txt
./a.out pequena 8 >> output.txt

./a.out media 1 >> output.txt
./a.out media 2 >> output.txt
./a.out media 4 >> output.txt
./a.out media 8 >> output.txt

./a.out grande 1 >> output.txt
./a.out grande 2 >> output.txt
./a.out grande 4 >> output.txt
./a.out grande 8 >> output.txt
