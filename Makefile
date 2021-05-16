.PHONY: any clean install uninstall

any: output

output: sds_ver.o elipt_cur.o hash_lib.o
	gcc sds_ver.o elipt_cur.o hash_lib.o -o sds_ver -lgmp

sds_ver.o: sds_ver.c elipt_cur.h hash_lib.h
	gcc -c sds_ver.c -lgmp

elipt_cur.o: elipt_cur.c
	gcc -c elipt_cur.c -lgmp

hash_lib.o: hash_lib.c
	gcc -c hash_lib.c -lgmp

clean: 
	rm *.o sds_ver

install:
	install ./sds_ver /usr/local/bin
	install -d /usr/local/etc/sds
	install ./ds_params /usr/local/etc/sds
	install ./public_accounts /usr/local/etc/sds

uninstall:
	rm -rf /usr/local/bin/sds_ver
	rm -rf /usr/local/etc/sds