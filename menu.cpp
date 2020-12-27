//
//  Date: 2017/09/05
//  Author: So Chigusa
//  Description: メニュー管理クラス
//

#include "class.h"

Menu::Menu()
  : dflag(0), x(0.0), y(0.0), select(0) {
}

void Menu::regist(Goban myGo, int num) {
  Kifu newKifu;
  newKifu.go.copy(myGo);
  int current = myGo.displayProcess();
  if(current <= 2) newKifu.ishi = myGo.shirokuro(current);
  else if(current == 3) newKifu.ishi = 1;
  else newKifu.ishi = 2;
  newKifu.n = (int)((num-1.0f)/myGo.displayM())+1;
  newKifu.m = (num-1)%myGo.displayM()+1;
  history.push_back(newKifu);
}

void Menu::mouse(double mx, double my) {
  x = mx; y = g_windowHeight-my;
  if(x >= g_windowWidth - 256.0f && y >= 192.0f &&
     x < g_windowWidth - 170.0f && y < 240.0f) // （ルール設定）トーラス
    select = 2;
  else if(x >= g_windowWidth - 256.0f && y >= 96.0f &&
     x < g_windowWidth - 170.0f && y < 144.0f) // （モード設定）通常モード
    select = 4;
  else if(x >= g_windowWidth - 170.0f && y >= 96.0f &&
     x < g_windowWidth - 85.0f && y < 144.0f)  // （モード設定）黒石セット
    select = 5;
  else if(x >= g_windowWidth - 85.0f && y >= 96.0f &&
     x < g_windowWidth && y < 144.0f)          // （モード設定）白石セット
    select = 6;
  else if(x >= g_windowWidth - 256.0f && y >= 0.0f &&
     x < g_windowWidth - 170.0f && y < 48.0f &&
     !history.empty())  // （盤面操作）やり直し
    select = 7;
  else if(x >= g_windowWidth - 170.0f && y >= 0.0f &&
     x < g_windowWidth - 85.0f && y < 48.0f)   // （盤面操作）パス
    select = 8;
  else if(x >= g_windowWidth - 85.0f && y >= 0.0f &&
     x < g_windowWidth && y < 48.0f)           // （盤面操作）リセット
    select = 9;
  else if(x >= g_windowWidth - 214.0f && y >= 288.0f &&
     x < g_windowWidth - 171.0f && y < 336.0f) // （盤面サイズ）N 10の位
    select = 10;
  else if(x >= g_windowWidth - 171.0f && y >= 288.0f &&
     x < g_windowWidth - 128.0f && y < 336.0f) // （盤面サイズ）N 1の位
    select = 11;
  else if(x >= g_windowWidth - 86.0f && y >= 288.0f &&
     x < g_windowWidth - 43.0f && y < 336.0f)  // （盤面サイズ）M 10の位
    select = 12;
  else if(x >= g_windowWidth - 43.0f && y >= 288.0f &&
     x < g_windowWidth && y < 336.0f)          // （盤面サイズ）M 1の位
    select = 13;
  else
    select = 0;
}

void Menu::mouseClick(Goban & myGo) {
  switch(select) {
  case 2:  // （ルール設定）トーラス
    myGo.torus(!myGo.torus());
    myGo.reset();
    history.clear();
    break;
  case 4:  // （モード設定）通常モード
    if(myGo.displayProcess() >= 3) {
      myGo.setProcess(1);
    }
    break;
  case 5:  // （モード設定）黒石セット
    myGo.setProcess(3);
    break;
  case 6:  // （モード設定）白石セット
    myGo.setProcess(4);
    break;
  case 7:  // （盤面操作）やり直し
    history.pop_back();
    if(history.empty()) myGo.reset();
    else myGo.copy(history.back().go);
    break;
  case 8:  //　（盤面操作）パス
    myGo.setProcess(myGo.shirokuro(myGo.displayProcess()));
    break;
  case 9:  //　（盤面操作）リセット
    myGo.reset();
    history.clear();
    break;
  case 10: //　（盤面サイズ）N 10の位
    {
      int newN = (myGo.displayN()+10)%100;
      if(newN == 0) newN = 1;
      myGo.setRange(newN, myGo.displayM());
      history.clear();
    }
    break;
  case 11: //　（盤面サイズ）N 1の位
    {
      int newN = myGo.displayN()+1;
      if(newN % 10 == 0) newN -= 10;
      if(newN == 0) newN = 1;
      myGo.setRange(newN, myGo.displayM());
      history.clear();
    }
    break;
  case 12: //　（盤面サイズ）M 10の位
    {
      int newM = (myGo.displayM()+10)%100;
      if(newM == 0) newM = 1;
      myGo.setRange(myGo.displayN(), newM);
      history.clear();
    }
    break;
  case 13: //　（盤面サイズ）M 1の位
    {
      int newM = myGo.displayM()+1;
      if(newM % 10 == 0) newM -= 10;
      if(newM == 0) newM = 1;
      myGo.setRange(myGo.displayN(), newM);
      history.clear();
    }
    break;
  }
}

int Menu::draw(Goban myGo, GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2) {
  dflag++;
  int id = 0;
  switch(dflag) {
  case 1:  // 進捗状況
    vx1 = g_windowWidth - 256.0f;
    vy1 = 604.0f;
    vx2 = g_windowWidth;
    vy2 = 652.0f;
    id = myGo.displayProcess();
    break;
  case 2:  // 盤面サイズ（盤面がリセットされます）
    vx1 = g_windowWidth - 256.0f;
    vy1 = 336.0f;
    vx2 = g_windowWidth;
    vy2 = 384.0f;
    id = 5;
    break;
  case 3:  // ルール設定（盤面がリセットされます）
    vx1 = g_windowWidth - 256.0f;
    vy1 = 240.0f;
    vx2 = g_windowWidth;
    vy2 = 288.0f;
    id = 6;
    break;
  case 4:  // モード設定
    vx1 = g_windowWidth - 256.0f;
    vy1 = 144.0f;
    vx2 = g_windowWidth;
    vy2 = 192.0f;
    id = 7;
    break;
  case 5:  // 盤面操作
    vx1 = g_windowWidth - 256.0f;
    vy1 = 48.0f;
    vx2 = g_windowWidth;
    vy2 = 96.0f;
    id = 8;
    break;
  case 6:  // （ルール設定）トーラス
    vx1 = g_windowWidth - 256.0f;
    vy1 = 192.0f;
    vx2 = g_windowWidth - 170.0f;
    vy2 = 240.0f;
    if((select == 2 && !myGo.torus()) ||
       (select != 2 && myGo.torus())) id = 10;
    else id = 9;
    break;
  case 7:  // （モード設定）通常モード
    vx1 = g_windowWidth - 256.0f;
    vy1 = 96.0f;
    vx2 = g_windowWidth - 170.0f;
    vy2 = 144.0f;
    if(myGo.displayProcess() <= 2 || select == 4) id = 12;
    else id = 11;
    break;
  case 8:  // （モード設定）黒石セット
    vx1 = g_windowWidth - 170.0f;
    vy1 = 96.0f;
    vx2 = g_windowWidth - 85.0f;
    vy2 = 144.0f;
    if(myGo.displayProcess() == 3 || select == 5) id = 14;
    else id = 13;
    break;
  case 9:  // （モード設定）白石セット
    vx1 = g_windowWidth - 85.0f;
    vy1 = 96.0f;
    vx2 = g_windowWidth;
    vy2 = 144.0f;
    if(myGo.displayProcess() == 4 || select == 6) id = 16;
    else id = 15;
    break;
  case 10:  // （盤面操作）やり直し
    vx1 = g_windowWidth - 256.0f;
    vy1 = 0.0f;
    vx2 = g_windowWidth - 170.0f;
    vy2 = 48.0f;
    if(select == 7) id = 18;
    else id = 17;
    break;
  case 11:  // （盤面操作）パス
    vx1 = g_windowWidth - 170.0f;
    vy1 = 0.0f;
    vx2 = g_windowWidth - 85.0f;
    vy2 = 48.0f;
    if(select == 8) id = 20;
    else id = 19;
    break;
  case 12:  // （盤面操作）リセット
    vx1 = g_windowWidth - 85.0f;
    vy1 = 0.0f;
    vx2 = g_windowWidth;
    vy2 = 48.0f;
    if(select == 9) id = 22;
    else id = 21;
    break;
  case 13:  // N
    vx1 = g_windowWidth - 256.0f;
    vy1 = 288.0f;
    vx2 = g_windowWidth - 214.0f;
    vy2 = 336.0f;
    id = 23;
    break;
  case 14:  // M
    vx1 = g_windowWidth - 128.0f;
    vy1 = 288.0f;
    vx2 = g_windowWidth - 86.0f;
    vy2 = 336.0f;
    id = 24;
    break;
  case 15:  // N 10の位
    vx1 = g_windowWidth - 214.0f;
    vy1 = 288.0f;
    vx2 = g_windowWidth - 171.0f;
    vy2 = 336.0f;
    id = (int)(myGo.displayN()/10)*2+25;
    if(select == 10) id++;
    break;
  case 16:  // N 1の位
    vx1 = g_windowWidth - 171.0f;
    vy1 = 288.0f;
    vx2 = g_windowWidth - 128.0f;
    vy2 = 336.0f;
    id = myGo.displayN()%10*2+25;
    if(select == 11) id++;
    break;
  case 17:  // M 10の位
    vx1 = g_windowWidth - 86.0f;
    vy1 = 288.0f;
    vx2 = g_windowWidth - 43.0f;
    vy2 = 336.0f;
    id = (int)(myGo.displayM()/10)*2+25;
    if(select == 12) id++;
    break;
  case 18:  // M 1の位
    vx1 = g_windowWidth - 43.0f;
    vy1 = 288.0f;
    vx2 = g_windowWidth;
    vy2 = 336.0f;
    id = myGo.displayM()%10*2+25;
    if(select == 13) id++;
    break;
  case 19:  // 棋譜一つ目背景
    if(history.empty()) {
      dflag = 0;
      id = 0;
    } else {
      vx1 = g_windowWidth - 256.0f;
      vy1 = 560.0f;
      vx2 = g_windowWidth;
      vy2 = 604.0f;
      id = 66;
    }
    break;
  case 30:  // 棋譜二つ目背景
    if(history.size() <= 1) {
      dflag = 0;
      id = 0;
    } else {
      vx1 = g_windowWidth - 256.0f;
      vy1 = 516.0f;
      vx2 = g_windowWidth;
      vy2 = 560.0f;
      id = 66;
    }
    break;
  case 41:  // 棋譜三つ目背景
    if(history.size() <= 2) {
      dflag = 0;
      id = 0;
    } else {
      vx1 = g_windowWidth - 256.0f;
      vy1 = 472.0f;
      vx2 = g_windowWidth;
      vy2 = 516.0f;
      id = 66;
    }
    break;
  case 52:  // 棋譜四つ目背景
    if(history.size() <= 3) {
      dflag = 0;
      id = 0;
    } else {
      vx1 = g_windowWidth - 256.0f;
      vy1 = 428.0f;
      vx2 = g_windowWidth;
      vy2 = 472.0f;
      id = 66;
    }
    break;
  case 63:  // 棋譜五つ目背景
    if(history.size() <= 4) {
      dflag = 0;
      id = 0;
    } else {
      vx1 = g_windowWidth - 256.0f;
      vy1 = 384.0f;
      vx2 = g_windowWidth;
      vy2 = 428.0f;
      id = 66;
    }
    break;
  default:
    if(dflag >= 20 && dflag < 30) {         // 棋譜一つ目
      id = drawKifu(myGo, 1, dflag-19, vx1, vy1, vx2, vy2);
    } else if(dflag >= 31 && dflag < 41) {  // 棋譜二つ目
      id = drawKifu(myGo, 2, dflag-30, vx1, vy1, vx2, vy2);
    } else if(dflag >= 42 && dflag < 52) {  // 棋譜三つ目
      id = drawKifu(myGo, 3, dflag-41, vx1, vy1, vx2, vy2);
    } else if(dflag >= 53 && dflag < 63) {  // 棋譜四つ目
      id = drawKifu(myGo, 4, dflag-52, vx1, vy1, vx2, vy2);
    } else if(dflag >= 64 && dflag < 74) {  // 棋譜五つ目
      id = drawKifu(myGo, 5, dflag-63, vx1, vy1, vx2, vy2);
    } else {
      dflag = 0;
      id = 0;
    }
    break;
  }
  return id;
}

int Menu::drawKifu(Goban myGo, int kifu, int moji, GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2) {
  int id = 0;
  int size = history.size() - kifu + 1;
  int N = myGo.displayN();
  int ishi = history[history.size()-kifu].ishi;
  int n = history[history.size()-kifu].n;
  int m = history[history.size()-kifu].m;

  int first = 0;
  if(size >= 1000) first += 4;
  else if(size >= 100) first += 3;
  else if(size >= 10) first += 2;
  else first++;

  int third = myGo.suuji(m);
  int forth = myGo.kansuuji(n);

  int place = 0, num = 0;
  if(moji > first + 1 + third + forth) {
    id = 65;
  } else if(moji > first + 1 + third) {
    place = moji - first - 1 - third;
    num = myGo.kansuuji(n, place);
    id = 54 + num;
  } else if(moji > first + 1) {
    place = moji - first - 1;
    num = myGo.suuji(m, place);
    id = 45 + num;
  } else if(moji > first) {
    if(ishi == 1) id = 67;
    else id = 68;
  } else {
    place = moji;
    if(place == 1 && first == 4) { num = (int)(size/1000.0f); }
    else if((place == 1 && first == 3) ||
	    (place == 2 && first == 4)) { num = (int)(size/100.0f)%10; }
    else if((place == 1 && first == 2) ||
	    (place == 2 && first == 3) ||
	    (place == 3 && first == 4)) { num = (int)(size/10.0f)%10; }
    else if((place == 1 && first == 1) ||
	    (place == 2 && first == 2) ||
	    (place == 3 && first == 3) ||
	    (place == 4 && first == 4)) { num = size%10; }
    id = 45 + num;
  }
  
  vx1 = g_windowWidth - 248.0f + 24.0f * (moji-1);
  vy1 = 562.0f - 44.0f * (kifu-1);
  vx2 = g_windowWidth - 224.0f + 24.0f * (moji-1);
  vy2 = 602.0f - 44.0f * (kifu-1);
  return id;
}
