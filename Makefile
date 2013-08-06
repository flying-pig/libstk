SYSTYPE=`uname -s`

default: prebuild

prebuild:
	if [ $(SYSTYPE) = 'FreeBSD' ]; then \
		make -f makefile.bsd;	\
	else	\
		make -f makefile.linux;	\
	fi;

clean:
	rm -f *.o libstk.a
	cd test; make clean

.PHONY: default clean prebuild
