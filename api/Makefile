LIBRP=lib/librp.so

all: $(LIBRP)

$(LIBRP):
	$(MAKE) -C rpbase/src

clean:
	$(MAKE) -C rpbase/src clean
	$(MAKE) -C rpbase/doc clean

install:
	cp -f lib/librp.* /usr/lib
	cp -f include/redpitaya/rp.h /usr/include
