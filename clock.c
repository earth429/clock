// #define GLUT_DISABLE_ATEXIT_HACK // エラーメッセージでない
#include <gl/glut.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>

int mode = 0;

void Display(void);
void Reshape(int, int);
void GetTime(int *, int *, int *, int *, int *, int *, int *);
void Timer(int);
void PrintText(int, int, char *);
void PrintColorText(int, int, char *, int, int, int);
void PrintGrayText(int, int, char *, int, int, int);
void GetWday(int, char*);
void DrawClock(int, int, int);
void Keyboard(unsigned char, int, int);
void BackGround(int, int);

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Analog Clock ");
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); // ダブルバッファリング
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutTimerFunc(500, Timer, 0);

    glutKeyboardFunc(Keyboard);

    glutMainLoop();

    return 0;

}

// ウィンドウの表示内容を更新する関数
void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT); // 初期設定

    if (mode == 0) {
        DrawClock(0, 0, 0);
    } else if (mode == 1){
        DrawClock(255, 255, 255);
    }

    //char week[64];
    //GetWday(wday, week);
    // 時間を表示
    /*char time[256];
    sprintf(time, "%d/%d/%d(%s) %d:%02d:%02d", year + 1900, mon + 1, mday, week, hour, min, sec);
    int i;
    double strX, strY;
    strX = w / 10;
    strY = h / 10 * 9;
    PrintGrayText(strX, strY, time, 255, 255, 255);*/

    //glRasterPos2i(strX, strY);
    /*glRasterPos2i(10, 10);
    for (i = 0; i <= strlen(time); i++) {
        printf("%c", time[i]);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, time[i]);
        puts("");
    }*/
    //printf("h:%f m:%f s:%f\n", hour, min, sec);

    
    glFlush();
    glutSwapBuffers(); // 作業用バッファと表示用バッファ入れ替え
}

// ウィンドウのサイズ変更が発生したときに座標系を再設定する関数
void Reshape(int w, int h) {
    // printf("ウィンドウの幅と高さ=%d x %d\n", w, h); // 確認用
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
}

void GetTime(int *hour, int *min, int *sec, int *mday, int *mon, int *year, int *wday) {
    time_t tt;
    struct tm *ts;

    time(&tt);
    ts = localtime(&tt);

    *hour = ts->tm_hour;
    *min = ts->tm_min;
    *sec = ts->tm_sec;

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

void PrintText(int x, int y, char *str) {
    int i;

    glRasterPos2i(x, y);
    //glPushMatrix();
    //glScalef(3.0f, 3.0f, 0.0f);
    for (i = 0; i <= strlen(str); i++) {
        //glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }
    //glPopMatrix();
}

void PrintColorText(int x, int y, char *str, int r, int g, int b) {
    glColor3ub(r, g, b);
    PrintText(x, y, str);
}

void PrintGrayText(int x, int y, char *str, int r, int g, int b) {
    PrintColorText(x, y, str, r, g, b);
    PrintColorText(x - 1, y - 1, str, 204, 204, 204);
}

void GetWday(int wday, char *week) {
    switch (wday)
    {
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

void DrawClock(int r, int g, int b) {
    int i;
    double w, h;
    w = glutGet(GLUT_WINDOW_WIDTH);
    h = glutGet(GLUT_WINDOW_HEIGHT);

    double theta_s, x_s, y_s, l_s; // sec
    double theta_m, x_m, y_m; // min
    double theta_h, x_h, y_h; // hour
    double l_m, l_h, x_c, y_c; // 棒の長さ
    x_c = w / 2;
    y_c = h / 2;
    l_s = y_c - y_c / 4;
    l_m = y_c - y_c / 4;
    l_h = y_c - y_c / 2;
    //x_c = w / 2 + w / 4;
    //y_c = h / 2 - h / 4;
    //l_s = y_c - y_c / 4;
    //l_m = y_c - y_c / 4;
    //l_h = (y_c - y_c / 4) / 2;

    int hour, min, sec, mday, mon, year, wday;
    GetTime(&hour, &min, &sec, &mday, &mon, &year, &wday);


    // sec
    theta_s = 2 * M_PI * sec / 60;
    x_s = x_c + l_s * sin(theta_s);
    y_s = y_c - l_s * cos(theta_s);

    // min
    theta_m = 2 * M_PI * (60 * min + sec) / 3600;
    x_m = x_c + l_m * sin(theta_m);
    y_m = y_c - l_m * cos(theta_m);

    // hour
    theta_h = 2 * M_PI * (3600 * hour + 60 * min + sec) / 43200;
    x_h = x_c + l_h * sin(theta_h);
    y_h = y_c - l_h * cos(theta_h);

    // 描画開始

    // 背景
    BackGround(w, h);

    // sec
    glLineWidth(1.0);
    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    glVertex2i(x_c, y_c);
    glVertex2i(x_s, y_s);
    glEnd();

    // min
    glLineWidth(5.0);
    glColor3ub(r, g, b);
    glBegin(GL_LINES);
    glVertex2i(x_c, y_c);
    glVertex2i(x_m, y_m);
    glEnd();

    // hour
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glVertex2i(x_c, y_c);
    glVertex2i(x_h, y_h);
    glEnd();

    // 目盛り60本
    // 中心座標から150離れたところから中心に向かって5の線を引く
    double steps = 60; // 目盛りの個数
    double angle, radian, startX, startY, endX, endY; 

    for (i = 0; i < steps; i++) {
        angle = i * (360 / steps) - 90; // 角度
        radian = angle * M_PI / 180; // ラジアンに変換
        startX = x_c + (l_s - 5) * cos(radian);
        startY = y_c + (l_s - 5) * sin(radian);
        endX = x_c + l_s * cos(radian);
        endY = y_c + l_s * sin(radian);
        glLineWidth(1.0);
        glBegin(GL_LINES);
        glVertex2i(startX, startY);
        glVertex2i(endX, endY);
        glEnd();
    }
    

    // 中心の止める軸みたいなやつ
    /*glPointSize(15.0);
    glBegin(GL_POINTS);
    glVertex2i(x_c, y_c);
    glEnd();

    glColor3ub(0, 0, 0);
    glPointSize(10.0);
    glBegin(GL_POINTS);
    glVertex2i(x_c, y_c);
    glEnd();*/

}

// キー入力の処理
void Keyboard(unsigned char key, int x, int y) {
    if ((key == 'q') || (key == 27)) {
        puts("終了");
        exit(0);
    }
    if (key == 'l') {
        mode = 0;
    }
    if (key == 'd') {
        mode = 1;
    }
}

void BackGround(int w, int h) {
    if (mode == 0) {
        glColor3ub(255, 255, 255);
    } else {
        glColor3ub(0, 0, 0);
    }
    
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(0, h);
    glVertex2i(w, h);
    glVertex2i(w, 0);
    glEnd();
}

//void ChangeMode(int mode) {
    
//}