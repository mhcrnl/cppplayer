player++:
	cd daemon && $(MAKE)
	cd client_console && $(MAKE)

install:
	cp daemon/dplayer++ /usr/local/bin/		
	cp client_console/player++ /usr/local/bin

uninstall:
	rm /usr/local/bin/dplayer++
	rm /usr/local/bin/player++

