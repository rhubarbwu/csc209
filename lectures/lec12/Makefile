count: counting.c
	gcc -pthread -o $@ $<

mutex: counting.c
	gcc -pthread -DMUTEX -o $@ $<

logs: count mutex
	TIME=1 sh counting_time.sh ./count 0
	TIME=1 sh counting_time.sh ./count 1
	TIME=1 sh counting_time.sh ./mutex 0
	TIME=1 sh counting_time.sh ./mutex 1

clean:
	rm count mutex
