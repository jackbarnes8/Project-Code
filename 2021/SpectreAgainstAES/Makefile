make unrolled: aesrun.c aes_core_unrolled.c
	gcc -O3 aesrun.c aes_core_unrolled.c -o aes
	./aes

make rolled: aesrun.c aes_core_rolled.c
	gcc -O3 aesrun.c aes_core_rolled.c -o aes
	./aes

make ni: aesrun_ni.c aes_core_rolled.c
	gcc -O3 aesrun_ni.c aes_core_rolled.c aesni.o -o aes
	./aes