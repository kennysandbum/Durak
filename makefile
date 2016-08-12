all: debug

release:
	cd Release && make -f makefile && cp Durak ../Durak

debug: 
	cd Debug && make -f makefile clean && make -f makefile && cp Durak ../Durak
	
clean: 
	rm -f Durak && cd Debug && make clean && cd ../Release && make clean