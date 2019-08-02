CC=gcc
all: app

app:source.o efl_color_converter.o
	$(CC) -o app source.o efl_color_converter.o `pkg-config --libs elementary`
source.o:source.c
	$(CC) -c source.c `pkg-config --cflags elementary`
efl_color_converter.o:efl_color_converter.c
	$(CC) -c efl_color_converter.c `pkg-config --cflags`
clean:
	rm -f source.o efl_color_converter.o app
