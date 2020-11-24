#
#  Makefile
#

#  最終目的のファイル
TARGET = j17429.exe

#  ソースファイルの一覧
SRCS = j17429.c
ICO = myicon.ico

# リソーススクリプトファイルの一覧
RC = ${ICO:.ico=.rc}

#  オブジェクトファイル(*.o)の一覧
OBJS = ${SRCS:.c=.o}
ICOOBJ = ${RC:.rc=.o}

#  ヘッダファイルの一覧
HEADERS =

#  コンパイラ・リンカの指定
CC = gcc
CCFLAGS = -Wall -I/usr/include/opengl
LD = gcc
LDFLAGS = 
LIBS = -lm -lglut32 -lglu32 -lopengl32
ECHO = echo
WINDRES = windres

#  OBJSからTARGETを作る方法
$(TARGET) : $(OBJS) $(ICOOBJ)
		$(LD) $(OBJS) $(ICOOBJ) $(LDFLAGS) -o $(TARGET) $(LIBS)

$(ICOOBJ) : $(RC)
	$(WINDRES) -i $(RC) -o $(ICOOBJ)

$(RC) : $(ICO)
	$(ECHO) GLUT_ICON ICON $(ICO) > $(RC)

#  *.cから*.oを作る方法
.c.o :
		$(CC) $(CCFLAGS) -c $<

#  *.o は HEADERS と Makefile に依存(これらが書き換わったときにも*.oを再構築)
$(OBJS) : $(HEADERS) Makefile
$(ICOOBJ) : $(RC) Makefile
$(RC) : $(ICO) Makefile

#  make cleanとしたときに実行されるコマンド
clean :
		rm -f $(TARGET) $(OBJS) $(ICOOBJ) $(RC) core *~
