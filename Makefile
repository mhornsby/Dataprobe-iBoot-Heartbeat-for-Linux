CC=gcc
CFLAGS=-Wall

heartbeat:

install: heartbeat
	cp heartbeat /usr/local/bin
	chmod 755 /usr/local/bin/heartbeat
clean:
	rm -rf heartbeat

