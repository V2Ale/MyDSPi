MyDSPi	:	main.o libi2c.o libDSPMath.o libDSPI2C.o libDSPBiquad.o libini.o liberror.o
	gcc main.o libi2c.o libDSPMath.o libDSPI2C.o libDSPBiquad.o libini.o liberror.o -o MyDSPi -lm
 
main.o : *.c
	gcc -c *.c -o main.o 

libDSPMath.o : *.c
	gcc -c includes/DSP/DSPMath.c  -o libDSPMath.o -L includes/DSP 
libDSPI2C.o : *.c
	gcc -c includes/DSP/DSPI2C.c -o libDSPI2C.o -L includes/DSP

liberror.o : *.c
	gcc -c includes/error/error.c -o liberror.o -L includes/error

libDSPBiquad.o : *.c
	gcc -c includes/DSP/DSPBiquad.c -o libDSPBiquad.o -L includes/DSP 

libini.o : *.c
	gcc -c includes/libini/libini.c -o libini.o -L includes/libini
libi2c.o : *.c
	gcc -c includes/libi2c/libi2c.c -o libi2c.o -L includes/libii2c
	
# clean
clean:
	rm -rf *.bak rm -rf *.o
	rm *.o
	
	# mrproper
mrproper: clean
	rm -rf test
	