///////////////////
SPECTRE AGAINST AES
///////////////////

This program will launch the Spectre attack against three implementations of AES: rolled, unrolled and AES-NI.
The code was written in collaboration with Chitchanok Chuengsatiansup, Sioli O'Connell, Rui Qi Sim and Yuval Yarom.
aesrun.c and aesrun-ni.c are based on Mastik: A Micro-Architectural Side-Channel Toolkit by Yuval Yarom.
The AES Implementation is from OpenSSL.

The included makefile will take care of compilation and execution. The three implementations can be selected by entering in terminal:
	make rolled		(for rolled AES)
	make unrolled	(for unrolled AES)
	make ni			(for AES-NI)
	
Parameters for the attack may then be entered. The byte to leak should be an integer value between 0 and 15 (so you can leak any of the 16 bytes of the ciphertext). The number of rounds for training and to perform architecturally can be any reasonable integer value, but it makes the most sense to train on a fewer numbers of round than you run on architecturally. So, for example, training on 10 rounds and then running on 12.
The number of samples are how many times you would like to repeat the experiment. Higher number of samples may be more representative of how successful the attack is. I recommend at least 100 samples. Most of the experiments were performed with 1000 samples.

The output will display the value of two bytes, T and K, and then any cache hits for a given sample. K is the value of the ciphertext byte that results from running the encryption with the architectural key, and T is the byte that results from running with the training key. T is ultimately what we aim to leak, and so the attack is only successful if there are hits for T in the output.

Please note that as this is a microarchitectural attack, how successful the attack is will depend on the hardware it is run on. We have observed leakage for unrolled and rolled AES on an Intel Core i7-6700K CPU, but no leakage occurred when run on an Intel Core i7-9750H CPU.