//
//  Date: 2017/09/02
//  Author: So Chigusa
//  Description: クラス定義と設定変数
//

#include <OpenGL/OpenGL.h>
#include <GLFW/glfw3.h>
#include <array>
#include <iostream>
#include <vector>

#ifndef CLASS_H
#define CLASS_H

const int g_windowWidth  = 908;
const int g_windowHeight = 652;

using namespace std;

struct Ichimoku {
  int num;          // 目番号
  int n, m;         // 碁盤座標
  int occupation;   // 空白、黒、白
  int region;       // 区分け番号
};

class Goban {
 private:
  int N, M;         // 碁盤の大きさ縦横
  bool torusFlag;   // トーラスモード
  GLfloat interval; // マス目間隔
  int dflag;        // 描画時に使用
  double x, y;      // マウス座標
  int process;      // 手番（黒番、白番）
  vector<Ichimoku> me;  // 各目の情報

 public:
  Goban();
  void copy(const Goban g);
  void reset();
  bool torus() const { return torusFlag; }
  void torus(bool t) { torusFlag = t; }
  void setRange(int h, int w);
  void setInterval();
  void setIchimoku();
  void setIchimoku(vector<Ichimoku> oldMe);
  void setProcess(int p) { process = p; }
  int displayN() const { return N; }
  int displayM() const { return M; }
  int displayProcess() const { return process; }
  vector<Ichimoku> displayMe() const { return me; }
  int drawGoban(GLfloat & vx, GLfloat & vy);
  int suuji(int n);
  int kansuuji(int n);
  int suuji(int n, int pos);
  int kansuuji(int n, int pos);
  int drawPos(GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2);
  /* int drawDot(GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2); */
  int drawCursor(GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2);
  int drawGoishi(GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2);
  void mouse(double mx, double my) { x = mx; y = g_windowHeight-my; }
  int mouseClick();
  int occupy(int hn, int wm);
  int occupyNormal(int hn, int wm);
  int occupySet(int id, int hn, int wm);
  void join(int num, int region);
  void joint(int r1, int r2);
  bool surround(int rnum);
  void pickup(int rnum);
  void newRegion(int num);
  int ctonm(int & hn, int & wm) { return ctonm(x,y,hn,wm); };
  int ctonm(double mx, double my, int & hn, int & wm);
  void nmtoc(int n, int m, GLfloat & vx, GLfloat & vy);
  void numtoc(int num, GLfloat & vx, GLfloat & vy);
  array<int, 4> neighbors(int hn, int wm);
  int shirokuro(int p);
};

struct Kifu {
  Goban go;  // その時点での碁盤の様子
  int ishi;  // 新たに打たれた石
  int n, m;  // 新たに打たれた場所
};

class Menu {
 private:
  int dflag;        // 描画時に使用
  double x, y;      // マウス座標
  int select;       // 選択中ボタン
  vector<Kifu> history;  // 棋譜およびやり直し用

 public:
  Menu();
  void regist(Goban myGo, int num);
  void mouse(double mx, double my);
  void mouseClick(Goban & myGo);
  int draw(Goban myGo, GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2);
  int drawKifu(Goban myGo, int kifu, int moji, GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2);
};

#endif
