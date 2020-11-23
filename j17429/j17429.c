#include <gl/glut.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>

// 背景色と時計の種類を管理するグローバル変数
int backColorMode = 100;
int clockMode = 0;

// 関数のプロトタイプ宣言
void Display(void);
void Reshape(int, int);
void Keyboard(unsigned char, int, int);
void GetTime(int *, int *, int *, int *, int *, int *, int *);
void Timer(int);
int DayOrNight();
void PrintText(int, int, char *);
void PrintColorText(int, int, char *, int, int, int);
void PrintShadowedText(int, int, char *, int, int, int);
void GetWday(int, char*);
void BackGround(int, int);
void AnalogClock(int, int, int, int, int);
void DigitalClock(int, int, int, int, int);
void DrawSevenSeg(int, int, int, int, int, int, int);
void DrawColon();
void SevenSegClock(int, int, int, int, int);


int main(int argc, char **argv) {
    glutInit(&argc, argv); // GLUTの初期化
    glutInitWindowSize(400, 400); // ウィンドウのデフォルトサイズ指定
    glutCreateWindow("Clock"); // タイトルバーの文字列指定

    // コールバック関数の登録
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutTimerFunc(500, Timer, 0);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); // ダブルバッファリング
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutMainLoop(); // GLUTのメインループ(処理待ち)

    return 0;

}

// ウィンドウの表示内容を更新する関数
void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT); // 初期設定

    // 現在のウィンドウサイズ取得
    int w, h;
    w = glutGet(GLUT_WINDOW_WIDTH);
    h = glutGet(GLUT_WINDOW_HEIGHT);

    // 初回のみ実行
    if (backColorMode == 100) {
        backColorMode = DayOrNight();
    }

    if (backColorMode == 0) { // 背景が白
        if (clockMode == 0) { // アナログ時計
            AnalogClock(w, h, 0, 0, 0);
        } else if(clockMode == 1) { // デジタル時計
            DigitalClock(w, h, 0, 0, 0);
        } else if (clockMode == 2) { // 7セグメントLED時計
            SevenSegClock(w, h, 0, 0, 0);
        }
    } else if (backColorMode == 1) { // 背景が黒
        if (clockMode == 0) { // アナログ時計
            AnalogClock(w, h, 255, 255, 255);
        } else if (clockMode == 1) { // デジタル時計
            DigitalClock(w, h, 255, 255, 255);
        } else if (clockMode == 2) { // 7セグメントLED時計
            SevenSegClock(w, h, 255, 255, 255);
        }
    }
    
    glFlush();
    glutSwapBuffers(); // 作業用バッファと表示用バッファ入れ替え
}

// ウィンドウのサイズ変更が発生したときに座標系を再設定する関数
void Reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
}

// キー入力の処理
void Keyboard(unsigned char key, int x, int y) {
    if ((key == 'q') || (key == 27)) {
        puts("Quit");
        exit(0);
    }
    if (key == 'w') {
        puts("White");
        backColorMode = 0;
    }
    if (key == 'b') {
        puts("Black");
        backColorMode = 1;
    }
    if (key == 'a') {
        puts("Analog");
        clockMode = 0;
    }
    if (key == 'd') {
        puts("Digital");
        clockMode = 1;
    }
    if (key == 's') {
        puts("SevenSegment");
        clockMode = 2;
    }
}

// 現在時刻を取得
void GetTime(int *hour, int *min, int *sec, int *mday, int *mon, int *year, int *wday) {
    time_t tt;
    struct tm *ts;

    time(&tt); // 現在時刻を取得
    ts = localtime(&tt); // time_t型

    *hour = ts->tm_hour; // 時間(0～23)
    *min = ts->tm_min; // 分(0～59)
    *sec = ts->tm_sec; // 秒(0～60)
    *mday = ts->tm_mday; // 日(1～31)
    *mon = ts->tm_mon; // 1月からの通算月数(0～11)
    *year = ts->tm_year; // 1900年からの通算年数
    *wday = ts->tm_wday; // 0が日曜6が土曜
}

// タイマー割り込み
void Timer(int value) {
    glutPostRedisplay(); // ディスプレイを更新
    glutTimerFunc(500, Timer, 0); // 500msごとに再度呼ぶ
}

// 昼か夜かを判別
int DayOrNight() {
    time_t tt;
    struct tm *ts;
    int hour;

    time(&tt);
    ts = localtime(&tt);

    hour = ts->tm_hour;

    if (hour < 7 || hour > 18) { // 7時から18時を昼とする
        return 1;
    } else {
        return 0;
    }
}

// 指定した座標に文字を描画
void PrintText(int x, int y, char *str) {
    int i;

    glRasterPos2i(x, y);
    for (i = 0; i <= strlen(str); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }
}

// 指定した色で文字を描画
void PrintColorText(int x, int y, char *str, int r, int g, int b) {
    glColor3ub(r, g, b);
    PrintText(x, y, str);
}

// 影付きで文字を描画
void PrintShadowedText(int x, int y, char *str, int r, int g, int b) {
    if (backColorMode == 0) {
        PrintColorText(x + 2, y + 2, str, 204, 204, 204);
        PrintColorText(x + 1, y + 1, str, 204, 204, 204);
        PrintColorText(x, y, str, r, g, b);
        PrintColorText(x - 1, y - 1, str, r, g, b);
    } else if (backColorMode == 1) {
        PrintColorText(x + 2, y + 2, str, 77, 77, 77);
        PrintColorText(x + 1, y + 1, str, 77, 77, 77);
        PrintColorText(x, y, str, r, g, b);
        PrintColorText(x - 1, y - 1, str, r, g, b);
    }
}

// 曜日の文字列を取得
void GetWday(int wday, char *week) {
    switch (wday) {
    case 0:
        strcpy(week, "Sun");
    case 1:
        strcpy(week, "Mon");
    case 2:
        strcpy(week, "Tue");
    case 3:
        strcpy(week, "Wed");
    case 4:
        strcpy(week, "Thu");
    case 5:
        strcpy(week, "Fri");
    case 6:
        strcpy(week, "Sat");
    }
}

// 色のモードの切替時背景を再描画
void BackGround(int w, int h) {
    if (backColorMode == 0) {
        glColor3ub(255, 255, 255);
    } else if (backColorMode == 1) {
        glColor3ub(0, 0, 0);
    }
    
    // ウィンドウ全体を塗りつぶす
    glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(0, h);
        glVertex2i(w, h);
        glVertex2i(w, 0);
    glEnd();
}

// アナログ時計を描画
void AnalogClock(int w, int h, int r, int g, int b) {
    int i;
    double theta_s, x_s, y_s, l_s; // 秒に関する変数
    double theta_m, x_m, y_m; // 分に関する変数
    double theta_h, x_h, y_h; // 時間に関する変数
    double l_m, l_h, x_c, y_c; // 針の長さ
    // 画面の中央座標取得
    x_c = w / 2;
    y_c = h / 2;
    // 針の長さ指定
    l_s = y_c - y_c / 4;
    l_m = y_c - y_c / 4;
    l_h = y_c - y_c / 2;

    // 時間取得
    int hour, min, sec, mday, mon, year, wday;
    GetTime(&hour, &min, &sec, &mday, &mon, &year, &wday);

    // sec
    theta_s = 2.0 * M_PI * sec / 60.0;
    x_s = x_c + l_s * sin(theta_s);
    y_s = y_c - l_s * cos(theta_s);

    // min
    theta_m = 2.0 * M_PI * (60.0 * min + sec) / 3600.0;
    x_m = x_c + l_m * sin(theta_m);
    y_m = y_c - l_m * cos(theta_m);

    // hour
    theta_h = 2.0 * M_PI * (3600.0 * hour + 60.0 * min + sec) / 43200.0;
    x_h = x_c + l_h * sin(theta_h);
    y_h = y_c - l_h * cos(theta_h);


    // 描画開始

    // 背景描画
    BackGround(w, h);

    // 秒針を描画
    glLineWidth(1.0);
    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
        glVertex2i(x_c, y_c);
        glVertex2i(x_s, y_s);
    glEnd();

    // 分針を描画
    glLineWidth(5.0);
    glColor3ub(r, g, b);
    glBegin(GL_LINES);
        glVertex2i(x_c, y_c);
        glVertex2i(x_m, y_m);
    glEnd();

    // 時針を描画
    glLineWidth(5.0);
    glBegin(GL_LINES);
        glVertex2i(x_c, y_c);
        glVertex2i(x_h, y_h);
    glEnd();

    // 目盛りを60本描画
    // 中心座標から150離れたところから中心に向かって長さ5の線を引く
    double steps = 60.0; // 目盛りの個数
    double angle, radian, startX, startY, endX, endY, m_len;

    for (i = 0; i < steps; i++) {
        if (i == 0 || i == 15 || i == 30 || i == 45) { // 3,6,9,12のとき目盛りを太く長く
            glLineWidth(5.0);
            m_len = 10.0;
        } else {
            glLineWidth(2.0);
            m_len = 5.0;
        }
        angle = i * (360.0 / steps) - 90; // 角度
        radian = angle * M_PI / 180.0; // ラジアンに変換
        startX = x_c + (l_s - m_len) * cos(radian);
        startY = y_c + (l_s - m_len) * sin(radian);
        endX = x_c + l_s * cos(radian);
        endY = y_c + l_s * sin(radian);
        
        glBegin(GL_LINES);
            glVertex2i(startX, startY);
            glVertex2i(endX, endY);
        glEnd();
    }
    

    // 針の中心の軸
    steps = 10000.0;
    double radius = 5.0;
    double rotate;
    double x, y;
    glBegin(GL_POLYGON);
    for (i = 0; i < steps; i++) {
        rotate = (double)i / steps;
        x = x_c + radius * cos(2.0 * M_PI * rotate);
        y = y_c + radius * sin(2.0 * M_PI * rotate);
        glVertex2i(x, y);
    }
    glEnd();

    // 内側の軸を描画
    radius = 3.0;
    if (backColorMode == 0) {
        glColor3ub(255, 255, 255);
    } else if (backColorMode == 1) {
        glColor3ub(0, 0, 0);
    }
    glBegin(GL_POLYGON);
    for (i = 0; i < steps; i++) {
        rotate = (double)i / steps;
        x = x_c + radius * cos(2.0 * M_PI * rotate);
        y = y_c + radius * sin(2.0 * M_PI * rotate);
        glVertex2i(x, y);
    }
    glEnd();
}

// デジタル時計を描画
void DigitalClock(int w, int h, int r, int g, int b) {
    // 時間取得
    int hour, min, sec, mday, mon, year, wday;
    GetTime(&hour, &min, &sec, &mday, &mon, &year, &wday);

    // 曜日を文字列に
    char week[64];
    GetWday(wday, week);

    // 背景を描画
    BackGround(w, h);

    // 時間を描画
    char time[256];
    sprintf(time, "%d/%d/%d(%s) %d:%02d:%02d", year + 1900, mon + 1, mday, week, hour, min, sec);
    double strX, strY;
    strX = w / 20;
    strY = 9 * h / 10;
    PrintShadowedText(strX, strY, time, r, g, b);
}

// 7セグメントLED風の数字を描画
void DrawSevenSeg(int w, int h, int r, int g, int b, int slide, int number) {
    int wDivByTen = w / 10;
    int hDivByTen = h / 10;

    int wDivByHun = wDivByTen / 10;
    int hDivByHun = hDivByTen / 10;

    int shiftX = wDivByTen;
    int shiftY = hDivByTen;

    glColor3ub(r, g, b);

    // 横棒

    if (number == 0 || number == 2 || number == 3 || number == 5 || number == 6 || number == 7 || number == 8 || number == 9) {
        // 上
        glBegin(GL_POLYGON);
            glVertex2i(wDivByTen + slide, 5 * hDivByTen - shiftY);
            glVertex2i(wDivByTen + wDivByHun + slide,  5 * hDivByTen - hDivByHun - shiftY);
            glVertex2i(2 * wDivByTen - wDivByHun + slide, 5 * hDivByTen - hDivByHun - shiftY);
            glVertex2i(2 * wDivByTen + slide, 5 * hDivByTen - shiftY);
            glVertex2i(2 * wDivByTen - wDivByHun + slide, 5 * hDivByTen + hDivByHun - shiftY);
            glVertex2i(wDivByTen + wDivByHun + slide, 5 * hDivByTen + hDivByHun - shiftY);
        glEnd();
    }

    if (number == 2 || number == 3 || number == 4 || number == 5 || number == 6 || number == 8 || number == 9) {
        // 中
        glBegin(GL_POLYGON);
            glVertex2i(wDivByTen + slide, 5 * hDivByTen);  //  すべての基準点
            glVertex2i(wDivByTen + wDivByHun + slide,  5 * hDivByTen - hDivByHun);
            glVertex2i(2 * wDivByTen - wDivByHun + slide, 5 * hDivByTen - hDivByHun);
            glVertex2i(2 * wDivByTen + slide, 5 * hDivByTen);
            glVertex2i(2 * wDivByTen - wDivByHun + slide, 5 * hDivByTen + hDivByHun);
            glVertex2i(wDivByTen + wDivByHun + slide, 5 * hDivByTen + hDivByHun);
        glEnd();
    }

    if (number == 0 || number == 2 || number == 3 || number == 5 || number == 6 || number == 8 || number == 9) {
        // 下
        glBegin(GL_POLYGON);
            glVertex2i(wDivByTen + slide, 5 * hDivByTen + shiftY);
            glVertex2i(wDivByTen + wDivByHun + slide,  5 * hDivByTen - hDivByHun + shiftY);
            glVertex2i(2 * wDivByTen - wDivByHun + slide, 5 * hDivByTen - hDivByHun + shiftY);
            glVertex2i(2 * wDivByTen + slide, 5 * hDivByTen + shiftY);
            glVertex2i(2 * wDivByTen - wDivByHun + slide, 5 * hDivByTen + hDivByHun + shiftY);
            glVertex2i(wDivByTen + wDivByHun + slide, 5 * hDivByTen + hDivByHun + shiftY);
        glEnd();
    }


    // 縦棒

    if (number == 0 || number == 4 || number == 5 || number == 6 || number == 8 || number == 9) {
        // 左上
        glBegin(GL_POLYGON);
            glVertex2i(wDivByTen + slide, 5 * hDivByTen - shiftY);
            glVertex2i(wDivByTen + wDivByHun + slide, 5 * hDivByTen + hDivByHun - shiftY);
            glVertex2i(wDivByTen + wDivByHun + slide,  5 * hDivByTen - hDivByHun);
            glVertex2i(wDivByTen + slide, 5 * hDivByTen);
            glVertex2i(wDivByTen - wDivByHun + slide,  5 * hDivByTen - hDivByHun);
            glVertex2i(wDivByTen - wDivByHun + slide, 5 * hDivByTen + hDivByHun - shiftY);
        glEnd();
    }

    if (number == 0 || number == 2 || number == 6 || number == 8) {
        // 左下
        glBegin(GL_POLYGON);
            glVertex2i(wDivByTen + slide, 5 * hDivByTen);
            glVertex2i(wDivByTen + wDivByHun + slide, 5 * hDivByTen + hDivByHun);
            glVertex2i(wDivByTen + wDivByHun + slide,  5 * hDivByTen - hDivByHun + shiftY);
            glVertex2i(wDivByTen + slide, 5 * hDivByTen + shiftY);
            glVertex2i(wDivByTen - wDivByHun + slide,  5 * hDivByTen - hDivByHun + shiftY);
            glVertex2i(wDivByTen - wDivByHun + slide, 5 * hDivByTen + hDivByHun);
        glEnd();
    }

    if (number == 0 || number == 1 || number == 2 || number == 3 || number == 4 || number == 7 || number == 8 || number == 9) {
        // 右上
        glBegin(GL_POLYGON);
            glVertex2i(wDivByTen + shiftX + slide, 5 * hDivByTen - shiftY);
            glVertex2i(wDivByTen + wDivByHun + shiftX + slide, 5 * hDivByTen + hDivByHun - shiftY);
            glVertex2i(wDivByTen + wDivByHun + shiftX + slide,  5 * hDivByTen - hDivByHun);
            glVertex2i(wDivByTen + shiftX + slide, 5 * hDivByTen);
            glVertex2i(wDivByTen - wDivByHun + shiftX + slide,  5 * hDivByTen - hDivByHun);
            glVertex2i(wDivByTen - wDivByHun + shiftX + slide, 5 * hDivByTen + hDivByHun - shiftY);
        glEnd();
    }

    if (number == 0 || number == 1 || number == 3 || number == 4 || number == 5 || number == 6 || number == 7 || number == 8 || number == 9) {
        // 右下
        glBegin(GL_POLYGON);
            glVertex2i(wDivByTen + shiftX + slide, 5 * hDivByTen);
            glVertex2i(wDivByTen + wDivByHun + shiftX + slide, 5 * hDivByTen + hDivByHun);
            glVertex2i(wDivByTen + wDivByHun + shiftX + slide,  5 * hDivByTen - hDivByHun + shiftY);
            glVertex2i(wDivByTen + shiftX + slide, 5 * hDivByTen + shiftY);
            glVertex2i(wDivByTen - wDivByHun + shiftX + slide, 5 * hDivByTen - hDivByHun + shiftY);
            glVertex2i(wDivByTen - wDivByHun + shiftX + slide, 5 * hDivByTen + hDivByHun);
        glEnd();
    }    

}

// コロンを描画
void DrawColon(int w, int h, int slide) {
    int wDivByTen = w / 10;
    int hDivByTen = h / 10;
    int wDivByHun = wDivByTen / 10;
    int hDivByHun = hDivByTen / 10;
    int x = 2 * wDivByTen + 4 * wDivByHun;
    int y = 5 * hDivByTen - 6 * hDivByHun;
    int nextY = h / 10;
    int quadSize = 2 * wDivByHun;
    glBegin(GL_QUADS);
        glVertex2i(x + slide, y);
        glVertex2i(x + slide, y + quadSize);
        glVertex2i(x + quadSize + slide, y+quadSize);
        glVertex2i(x + quadSize + slide, y);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2i(x + slide, y + nextY);
        glVertex2i(x + slide, y + quadSize + nextY);
        glVertex2i(x + quadSize + slide, y + quadSize + nextY);
        glVertex2i(x + quadSize + slide, y + nextY);
    glEnd();
}

// 7セグメントLED風の時計を描画
void SevenSegClock(int w, int h, int r, int g, int b) {
    // 時間取得
    int hour, min, sec, mday, mon, year, wday;
    GetTime(&hour, &min, &sec, &mday, &mon, &year, &wday);

    // 背景を描画
    BackGround(w, h);

    // 座標を数字ごとずらす
    int slide = - w / 10 + 4 * w / 100;

    // 時間 
    DrawSevenSeg(w, h, r, g, b, slide, hour / 10 % 10);
    slide += w / 10  + 4 * w / 100;
    DrawSevenSeg(w, h, r, g, b, slide, hour % 10);
    DrawColon(w, h, slide);

    // 分
    slide += 2 * w / 10;
    DrawSevenSeg(w, h, r, g, b, slide, min / 10 % 10);
    slide += w / 10  + 4 * w / 100;
    DrawSevenSeg(w, h, r, g, b, slide, min % 10);
    DrawColon(w, h, slide);

    // 秒
    slide += 2 * w / 10;
    DrawSevenSeg(w, h, r, g, b, slide, sec / 10 % 10);
    slide += w / 10  + 4 * w / 100;
    DrawSevenSeg(w, h, r, g, b, slide, sec % 10);
}