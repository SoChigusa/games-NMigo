//
//  Date: 2017/09/01
//  Author: So Chigusa
//  Description: N * M の碁盤で囲碁をやる
//

#include <OpenGL/OpenGL.h>
#include <GLFW/glfw3.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "class.h"

#define  textureNum 3
#define  menuNum    68

GLuint g_texID[textureNum];
GLuint g_menuID[menuNum];

// 座標、ID指定のrender method
void render(GLuint texID, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLuint angle)
{
  const GLfloat vtx[] = {
    x1, y1,
    x2, y1,
    x2, y2,
    x1, y2,
  };
  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // テクスチャの領域指定
  static const GLfloat texuv[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
  };
  static const GLfloat texuvLeft[] = {
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
  };
  static const GLfloat texuvRight[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
  };
  static const GLfloat texuvFlipped[] = {
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
  };
  if(angle == 180) glTexCoordPointer(2, GL_FLOAT, 0, texuvFlipped);
  else if(angle == 90) glTexCoordPointer(2, GL_FLOAT, 0, texuvLeft);
  else if(angle == -90) glTexCoordPointer(2, GL_FLOAT, 0, texuvRight);
  else glTexCoordPointer(2, GL_FLOAT, 0, texuv);
  
  // テクスチャの画像指定
  glBindTexture(GL_TEXTURE_2D, texID);

  // テクスチャの描画
  glEnable(GL_TEXTURE_2D);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glDrawArrays(GL_QUADS, 0, 4);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisable(GL_TEXTURE_2D);
}

// 座標指定の四角形描画
void renderRectangular(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
  glBegin(GL_QUADS);
  glVertex2i(x1,y1);
  glVertex2i(x2,y1);
  glVertex2i(x2,y2);
  glVertex2i(x1,y2);
  glEnd();
}

void render(Goban & myGo, Menu & myMenu)
{
  // 情報取り出し
  int N = myGo.displayN();
  int M = myGo.displayM();
  int id;
  GLfloat vx1, vy1, vx2, vy2;

  // 碁盤
  glLineWidth(1.0f);
  glBegin(GL_LINES);
  glColor4f(0.0f,0.0f,0.0f,0.0f);
  GLfloat vx, vy;
  while(myGo.drawGoban(vx, vy) != 0) {
    glVertex2i(vx, vy);
  }
  glEnd();

  // 碁盤の座標表示
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  while((id = myGo.drawPos(vx1, vy1, vx2, vy2)) != 0) {
    render(g_menuID[id-1], vx1, vy1, vx2, vy2, 0);
  }
  
  // 碁盤のドット
  // while(myGo.drawDot(vx1, vy1, vx2, vy2) != 0) {
  //   render(g_texID[2], vx1, vy1, vx2, vy2, 0);
  // }
  
  // カーソル
  glBegin(GL_LINE_STRIP);
  while(myGo.drawCursor(vx1, vy1, vx2, vy2) != 0) {
    glVertex2i(vx1, vy1);
    glVertex2i(vx2, vy1);
    glVertex2i(vx2, vy2);
    glVertex2i(vx1, vy2);
    glVertex2i(vx1, vy1);
  }
  glEnd();
  
  // 碁石
  while((id = myGo.drawGoishi(vx1, vy1, vx2, vy2)) != 0) {
    render(g_texID[id-1], vx1, vy1, vx2, vy2, 0);
  }

  // メニューバー
  glColor4f(0.0f,0.0f,0.0f,0.0f);
  renderRectangular(g_windowWidth-256.0f,0.0f,g_windowWidth,g_windowHeight);
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  
  // メニュー
  while((id = myMenu.draw(myGo, vx1, vy1, vx2, vy2)) != 0) {
    render(g_menuID[id-1], vx1, vy1, vx2, vy2, 0);
  }
}

void setupTexture( GLuint texID, const char *file)
{
  // IplImageとして画像をロード
  IplImage* image = cvLoadImage(file);
  int w = image->width;
  int h = image->height;

  // 色の反転
  cvCvtColor(image, image, CV_BGR2RGB);
  
  // テキスチャと変数をバインド（関連付け）
  glBindTexture(GL_TEXTURE_2D, texID);
  // 各種フィルタの設定
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  
  // 実際のテキスチャを設定
  glTexImage2D(GL_TEXTURE_2D, // テキスチャの種類。
	       0,             // level: 解像度が単一の場合は0
	       GL_RGB,       // どの成分が画像のテクセルに使用されているか
	       w, h,          // サイズ
	       0,             // border: 境界の幅
	       GL_RGB,       // 描画フォーマット
	       GL_UNSIGNED_BYTE, // データの型
	       // imageData          // 実際のデータ
	       image->imageData
	       );

  // Step4. テクスチャの各種設定
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  cvReleaseImage(&image);
  
}

int main(int argc, char ** argv) {
  
  if( !glfwInit() ){
    return -1;
  }

  GLFWwindow *const window(glfwCreateWindow(g_windowWidth, g_windowHeight, "NMigo", NULL, NULL));
  if(!window) {
    glfwTerminate();
    return -1;
  }

  // 作成したウィンドウをOpenGL の処理対象にする
  glfwMakeContextCurrent(window);
    
  // 描画範囲の指定
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, g_windowWidth, 0.0f, g_windowHeight, -1.0f, 1.0f);

  // テクスチャのロード
  glGenTextures(textureNum, g_texID);
  glGenTextures(menuNum, g_menuID);
  setupTexture(g_texID[0], "image/kuro.png");
  setupTexture(g_texID[1], "image/shiro.png");
  setupTexture(g_texID[2], "image/dot.png");
  for(int i = 1; i <= menuNum; i++) {
    ostringstream os;
    os << "image/text" << i << ".png";
    setupTexture(g_menuID[i-1], os.str().c_str());
  }
  
  // 初期化の色
  glClearColor(220.0f/256.0f, 173.0f/256.0f, 89.0f/256.0f, 0.0f);

  // クラスの準備
  Goban myGo;
  // myGo.setRange(1,20);
  Menu myMenu;
  
  // ゲームループ
  double mousex, mousey;
  while (!glfwWindowShouldClose(window)) {

    // マウス処理
    glfwGetCursorPos(window, &mousex, &mousey);
    myGo.mouse(mousex, mousey);
    myMenu.mouse(mousex, mousey);

    // マウスクリック処理
    static int oldState = GLFW_RELEASE;
    int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if(oldState == GLFW_RELEASE && newState == GLFW_PRESS) {
      int num = 0;
      if((num = myGo.mouseClick()) != 0) myMenu.regist(myGo, num);
      myMenu.mouseClick(myGo);
    }
    oldState = newState;
    
    // 画面の初期化
    glClear(GL_COLOR_BUFFER_BIT);

    // 描画
    render(myGo, myMenu);
    
    // バッファの入れ替え
    glfwSwapBuffers(window);

    // イベントを取り出す
    glfwWaitEvents();
  }
  
  glfwTerminate();
  
  return 0;
}
