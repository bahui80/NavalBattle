CFLAGS= -Wall -ansi -pedantic -Wuninitialized -g


TARGET1=batallaNavalBack
TARGET2=batallaNavalFront
TARGET3=getnum
TARGET4=rands
TARGET5=batallaNaval


$(TARGET5):	src/$(TARGET1).c src/$(TARGET2).c src/$(TARGET3).c src/$(TARGET4).c
	gcc $(CFLAGS) -o $(TARGET5) src/$(TARGET1).c src/$(TARGET2).c src/$(TARGET3).c src/$(TARGET4).c
