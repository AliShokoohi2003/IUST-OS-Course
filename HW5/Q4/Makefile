CC = gcc
CFLAGS = -Wall -Wextra

all: producer consumer controller

producer: producer.c
	$(CC) $(CFLAGS) -o producer producer.c -lrt -lpthread

consumer: consumer.c
	$(CC) $(CFLAGS) -o consumer consumer.c -lrt -lpthread

controller: controller.c
	$(CC) $(CFLAGS) -o controller controller.c -lrt -lpthread

clean:
	rm -f producer consumer controller
