LIB_DIR = ./lib
INC_DIR = ./include

install:
	mkdir /usr/local/lib64 -p
	mkdir /usr/local/lib64/pkgconfig -p
	cp $(LIB_DIR)/librabbitmq.a /usr/local/lib64 -f
	cp $(LIB_DIR)/librabbitmq.so.4.3.0 /usr/local/lib64 -f
	ln -s /usr/local/lib64/librabbitmq.so.4.3.0 /usr/local/lib64/librabbitmq.so.4 -f
	ln -s /usr/local/lib64/librabbitmq.so.4.3.0 /usr/local/lib64/librabbitmq.so -f
	cp $(LIB_DIR)/librabbitmq.pc /usr/local/lib64/pkgconfig -f
	ldconfig
	cp $(INC_DIR)/*.h /usr/local/include -f

