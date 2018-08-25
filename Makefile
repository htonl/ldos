#Luke Cesarz

all:
	gcc -o ldos ldos.c syn_flood.c udp_spam.c sock_config.c -I .

clean:
	echo "Clean"
