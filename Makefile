all:
	g++ main.cpp -o linux-keylogger -l X11

clean:
	rm linux-keylogger
