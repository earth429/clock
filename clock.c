// #define GLUT_DISABLE_ATEXIT_HACK // エラーメッセージでない
#include <gl/glut.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

void Display(void);
void Reshape(int, int);
void GetTime(double *, double *, double *);
void Timer(int);

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Analog Clock ");
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); // ダブルバッファリング
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutTimerFunc(500, Timer, 0);

    glutMainLoop();

    return 0;

}

// ウィンドウの表示内容を更新する関数
void Display(void)
{
    double w, h;
    w = glutGet(GLUT_WINDOW_WIDTH);
    h = glutGet(GLUT_WINDOW_HEIGHT);

    double hour, min, sec;
    GetTime(&hour, &min, &sec);
    printf("h:%f m:%f s:%f\n", hour, min, sec);

    double theta_s, x_s, y_s, l_s; // sec
    double theta_m, x_m, y_m; // min
    double theta_h, x_h, y_h; // hour
    double l_m, l_h, x_c, y_c; // 棒の長さ
    x_c = w / 2;
    y_c = h / 2;
    l_s = y_c - y_c / 4;
    l_m = y_c - y_c / 4;
    l_h = (y_c - y_c / 4) / 2;
    //l_s = 150;
    //l_m = 150;
    //l_h = 75;

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

    glClear(GL_COLOR_BUFFER_BIT); // 初期設定

    // sec
    glLineWidth(1.0);
    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    glVertex2i(x_c, y_c);
    glVertex2i(x_s, y_s);
    glEnd();

    // min
    glLineWidth(5.0);
    glColor3ub(255, 255, 255);
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
    // double mem_l = 5;
    int i;
    double steps = 60; // 目盛りの個数
    double angle, radian, startX, startY, endX, endY;
    //double theta = M_PI - M_PI / 30;
    //double first_x = x_c + l_s * sin(2 * M_PI);
    //double first_y = y_c - l_s * cos(2 * M_PI);    

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

        //first_x = first_x * cos(theta) - first_y * sin(theta);
        //first_y = first_x * sin(theta) + first_y * cos(theta);
    }
        



    glFlush();
    glutSwapBuffers(); // 作業用バッファと表示用バッファ入れ替え
}

// ウィンドウのサイズ変更が発生したときに座標系を再設定する関数
void Reshape(int w, int h)
{
    // printf("ウィンドウの幅と高さ=%d x %d\n", w, h); // 確認用
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
}

void GetTime(double *hour, double *min, double *sec)
{
    time_t tt;
    struct tm *ts;

    time(&tt);
    ts = localtime(&tt);

    *hour = ts->tm_hour;
    *min = ts->tm_min;
    *sec = ts->tm_sec;
}

// タイマー割り込み
void Timer(int value)
{
    glutPostRedisplay(); // ディスプレイを更新
    glutTimerFunc(500, Timer, 0); // 500msごとに再度呼ぶ
}