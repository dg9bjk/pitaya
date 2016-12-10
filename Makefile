
all:	health scan

health:
	gcc -g -O0 -lrp -lm health.c -o health
	
scan:
	gcc -g -O0 -lrp -lm scan.c -o scan

clean:
	rm -f health
	rm -f scan
	rm -f *~
