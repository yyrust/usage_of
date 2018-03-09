DURATION=2

output.svg:perf.data
	sudo perf timechart --highlight race

perf.data:race
	@sudo echo Begin # (step 1) just to ensure you don't need to input password at step 3
	nohup ./race $(DURATION) >/dev/null &
	sudo perf timechart record sleep $(DURATION)

race:main.cpp
	g++ -o $@ $< -g -O2 -lpthread -std=c++11

open:output.svg
	firefox $<

clean:
	rm -f perf.data
	rm -f perf.data.old
	rm -f race
	rm -f *.svg
