#
#  Makefile
#

#  最終目的のファイル
TARGET = j17429.exe

#  ソースファイル(*.c)の一覧
SRCS = j17429.c
#  オブジェクトファイル(*.o)の一覧
OBJS = ${SRCS:.c=.o}

#  ヘッダファイルの一覧
HEADERS =

#  コンパイラ・リンカの指定
CC = gcc
CCFLAGS = -Wall -I/usr/include/opengl
LD = gcc
LDFLAGS = 
LIBS = -lglut32 -lglu32 -lopengl32 myicon.o

#  OBJSからTARGETを作る方法
$(TARGET) : $(OBJS)
		$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET) $(LIBS)

#  *.cから*.oを作る方法
.c.o :
		$(CC) $(CCFLAGS) -c $<

#  *.o は HEADERS と Makefile に依存(これらが書き換わったときにも*.oを再構築)
$(OBJS) : $(HEADERS) Makefile

#  make cleanとしたときに実行されるコマンド
clean :
		rm -f $(TARGET) $(OBJS) core *~
