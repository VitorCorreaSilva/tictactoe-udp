all::
	make -C client
	make -C server
clean::
	make -C client clean
	make -C server clean
distclean::
	make -C client distclean
	make -C server distclean