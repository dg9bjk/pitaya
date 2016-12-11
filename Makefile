
all:	health scan dcf77 cw rtty psk txoff

health:	health.c
	gcc -g -O0 -lrp health.c -o health
	
scan:	scan.c
	gcc -g -O0 -lrp -lm scan.c -o scan

dcf77:	dcf77.c
	gcc -g -O0 -lrp dcf77.c -o dcf77
	
cw:	cw.c
	gcc -g -O0 -lrp cw.c -o cw

rtty:	rtty.c
	gcc -g -O0 -lrp rtty.c -o rtty

psk:	psk.c
	gcc -g -O0 -lrp psk.c -o psk

txoff:	txoff.c
	gcc -g -O0 -lrp txoff.c -o txoff
	
clean:
	rm -f health
	rm -f scan
	rm -f dcf77
	rm -f cw
	rm -f rtty
	rm -f psk
	rm -f txoff
	rm -f *~
