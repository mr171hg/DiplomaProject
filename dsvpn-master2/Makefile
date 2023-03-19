CFLAGS_FILE=.cflags
COMPILE_TEST_FILE=.test.c
PREFIX?=C:/Program Files/dsvpn
CC=gcc -std=c99 -Wall -lws2_32

all: dsvpn.exe 

dsvpn.exe:  Makefile src/vpn.c src/charm.c src/os.c include/charm.h include/vpn.h include/os.h
	$(CC) $(OPTFLAGS) -Iinclude -o $@ src/vpn.c src/charm.c src/os.c
	strip $@

install: dsvpn.exe
	if not exist $(PREFIX) mkdir $(PREFIX)
	if not exist $(PREFIX)\sbin mkdir $(PREFIX)\sbin
	copy dsvpn.exe $(PREFIX)\sbin

uninstall:
	del $(PREFIX)\sbin\dsvpn.exe

clean:
	del dsvpn.exe *~ $(CFLAGS_FILE) $(COMPILE_TEST_FILE)

$(CFLAGS_FILE):
	if not exist "$(CFLAGS_FILE)" (
		set CFLAGS=""		
		if "%CFLAGS%"=="" (
			if not exist "$(COMPILE_TEST_FILE)" echo "int main(void) { return 0; }" > "$(COMPILE_TEST_FILE)"
			for %%F in (-march=native -mtune=native -Ofast -Wno-unused-command-line-argument) do (
				$(CC) %CFLAGS% %%F "$(COMPILE_TEST_FILE)" >nul 2>&1 && set CFLAGS="%CFLAGS% %%F"
			)
			if exist a.out del a.out
			set CFLAGS="%CFLAGS% -Wall -W -Wshadow -Wmissing-prototypes"
		)
		echo %CFLAGS% > "$(CFLAGS_FILE)"
	) else (
		type "$(CFLAGS_FILE)"
	)