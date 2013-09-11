SYSTYPE=`uname -s`

default: prebuild

prebuild:
	if [ $(SYSTYPE) = 'FreeBSD' ]; then \
		make -f makefile.bsd;	\
	else	\
		make -f makefile.linux;	\
	fi;

clean:
	if [ $(SYSTYPE) = 'FreeBSD' ]; then \
		make -f makefile.bsd clean;	\
	else	\
		make -f makefile.linux clean;	\
	fi;
	rm -f *.o test test_cli test_list test_pool test_module test_async_queue test_threads test_daemon test_signal test_process \
		test_ae test_string test_x_palloc

.PHONY: default clean prebuild
