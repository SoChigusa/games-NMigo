//
//  Date: 2017/09/02
//  Author: So Chigusa
//  Description: 碁盤管理クラス
//

#include "class.h"

Goban::Goban()
  : N(9), M(9), dflag(0), x(0.0), y(0.0), process(1) {
  setInterval();
  setIchimoku();
}

void Goban::copy(const Goban g) {
  N = g.displayN();  M = g.displayM();
  dflag = 0;
  x = 0.0;  y = 0.0;
  process = g.displayProcess();
  setInterval();
  setIchimoku(g.displayMe());
}

void Goban::reset() {
  process = 1;
  setIchimoku();
}

void Goban::setRange(int h, int w) {
  N = h; M = w;
  process = 1;
  setInterval();
  setIchimoku();
};

void Goban::setInterval() {
  if(N > M) interval = (g_windowHeight-0.0f) / (N+1.0f);
  else interval = (g_windowWidth-256.0f) / (M+1.0f);
}

void Goban::setIchimoku() {
  me.clear();
  me.reserve(N*M);
  for(int h = 1; h <= N; h++) {
    for(int w = 1; w <= M; w++) {
      Ichimoku c;
      c.num = w + (h-1)*M;
      c.n = h; c.m = w;
      c.occupation = 0;
      c.region = 0;
      me.push_back(c);
    }
  }
}

void Goban::setIchimoku(vector<Ichimoku> oldMe) {
  me.clear();
  me.reserve(N*M);
  std::copy(oldMe.begin(), oldMe.end(), back_inserter(me));
}

int Goban::drawGoban(GLfloat & vx, GLfloat & vy) {
  dflag++;
  if(dflag <= 2 * M) {
    if(dflag % 2 == 1) {
      vx = interval * (dflag+1.0f) / 2.0f;
      vy = interval;
    } else {
      vx = interval * dflag / 2.0f;
      vy = interval * N;
    }
  } else if(dflag <= 2 * (N+M)) {
    if(dflag % 2 == 1) {
      vx = interval;
      vy = interval * (dflag-2*M+1.0f) / 2.0f;
    } else {
      vx = interval * M;
      vy = interval * (dflag-2*M) / 2.0f;
    }
  } else if(dflag > 2 * (N+M)) {
    dflag = 0;
  }
  return dflag;
}

int Goban::suuji(int m) {
  int moji = 0;
  if(m >= 10) moji += 2;
  else moji++;
  return moji;
}

int Goban::suuji(int m, int pos) {
  int num = 0;
  int len = suuji(m);
  if(pos == 1 && len == 2) { num = (int)(m/10.0f); }
  else { num = m%10; }
  return num;
}

int Goban::kansuuji(int n) {
  int moji = 0;
  if((N-n+1) >= 20) {
    if((N-n+1)%10 != 0) moji += 3;
    else moji += 2;
  } else if((N-n+1) > 10) {
    moji = 2;
  } else moji++;
  return moji;
}

int Goban::kansuuji(int n, int pos) {
  int num = 0;
  int len = kansuuji(n);
  if(pos == 1 && (N-n+1) >= 20) { num = (int)((N-n+1)/10.0f); }
  else if(pos == 1 && len >= 2) { num = 10; }
  else if(pos == 2 && (N-n+1) < 20) { num = (N-n+1)%10; }
  else if(pos == 2) { num = 10; }
  else if(N-n+1 == 10) { num = 10; }
  else { num = (N-n+1)%10; }
  return num;
}

int Goban::drawPos(GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2) {
  dflag++;

  int pos = 0;
  int n = 0, m = 0, moji = 0;
  for(int i = 1; i <= N; i++) {
    pos += kansuuji(i);
    if(pos >= dflag) {
      n = i;
      moji = kansuuji(i) - (pos - dflag);
      break;
    }
  }
  if(n == 0) {
    for(int i = 1; i <= M; i++) {
      pos += suuji(i);
      if(pos >= dflag) {
	m = i;
	moji = suuji(i) - (pos - dflag);
	break;
      }
    }
  }

  int id = 0;
  if(moji != 0) {
    int num = 0;
    if(n != 0) {
      num = kansuuji(n, moji);
      vx1 = 0.286f * interval * (moji-1);
      vy1 = (n - 0.25f) * interval;
      vx2 = 0.286f * interval * moji;
      vy2 = (n + 0.25f) * interval;
      id = 54 + num;
    } else {
      num = suuji(m, moji);
      vx1 = (m - 0.143f * suuji(m) + 0.286f * (moji-1)) * interval;
      vy1 = g_windowHeight - 0.5f * interval;
      vx2 = (m - 0.143f * suuji(m) + 0.286f * moji) * interval;
      vy2 = g_windowHeight;
      id = 45 + num;
    }
  } else {
    dflag = 0;
  }

  return id;
}

// int Goban::drawDot(GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2) {
//   dflag++;
//   int n = 0, m = 0;
//   GLfloat vxc, vyc, r;
//   int ndots, mdots;
//   if(N < 11) ndots = 0;
//   else if(N >= 11 && N < 19) ndots = 1;
//   else ndots = (N-19)%6+2;
//   if(M < 11) mdots = 0;
//   else if(M >= 11 && M < 19) mdots = 1;
//   else mdots = (M-19)%6+2;
  
//   if(ndots * mdots > 0) {
    
//     switch(dflag) {
//     case 1: n = 4; m = 4; break;
//     case 2: n = N-3; m = 4; break;
//     case 3: n = N-3; m = M-3; break;
//     case 4: n = 4; m = M-3; break;
//     default: dflag = 0; break;
//     }

//     nmtoc(n, m, vxc, vyc);
//     r = 5.0f;
//     if(5.0f > interval * 0.2f) r = interval * 0.2f;
//     vx1 = vxc - r;
//     vy1 = vyc - r;
//     vx2 = vxc + r;
//     vy2 = vyc + r;
    
//   } else dflag = 0;
  
//   return dflag;
// }

int Goban::drawCursor(GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2) {
  dflag++;
  vx1 = 0.0f;  vy1 = 0.0f;  vx2 = 0.0f;  vy2 = 0.0f;
  if(dflag == 1) {
    if(x < g_windowHeight) {
      int hn, wm, num;
      num = ctonm(hn, wm);
      if(num >= 1 && num <= N*M) {
	if(me[num-1].occupation == 0) {
	  vx1 = (wm-0.3f) * interval;
	  vy1 = (hn-0.3f) * interval;
	  vx2 = (wm+0.27f) * interval;
	  vy2 = (hn+0.27f) * interval;
	}
      }
    }
  }
  if(vx1 == 0.0f) dflag = 0;
  return dflag;
}

int Goban::drawGoishi(GLfloat & vx1, GLfloat & vy1, GLfloat & vx2, GLfloat & vy2) {
  bool stop = false;
  int occ = 0;
  while(dflag < N*M && !stop) {
    dflag++;
    if(me[dflag-1].occupation != 0) stop = true;
  }
  if(stop) {
    GLfloat vx = me[dflag-1].m * interval;
    GLfloat vy = me[dflag-1].n * interval;
    vx1 = vx - interval / 2.0f;
    vy1 = vy - interval / 2.0f;
    vx2 = vx + interval / 2.0f;
    vy2 = vy + interval / 2.0f;
    occ = me[dflag-1].occupation;
  } else {
    dflag = 0;
    occ = 0;
  }
  return occ;
}

int Goban::mouseClick() {

  int ret = 0;
  if(x < g_windowHeight) {
    int hn, wm, num;
    num = ctonm(hn, wm);
    if(num >= 1 && num <= N*M) {
      ret = occupy(hn, wm);
    }
  }

  return ret;
}

int Goban::occupy(int hn, int wm) {
  int ret = 0;
  if(process <= 2)  // 通常モード
    ret = occupyNormal(hn, wm);
  else if(process == 3)  // 黒石セット
    ret = occupySet(1, hn, wm);
  else if(process == 4)  // 白石セット
    ret = occupySet(2, hn, wm);
  return ret;
}

int Goban::occupyNormal(int hn, int wm) {

  int num = wm + (hn-1) * M;
  if(me[num-1].occupation != 0) return 0;
  
  me[num-1].occupation = process;
  array<int, 4> nei = neighbors(hn, wm);

  bool check = false;
  newRegion(num);  // 置けるかチェックのために暫定領域割り当て
  check = check || !surround(me[num-1].region);
  for(int i = 0; i < 4; i++) {
    if(nei[i] != 0) {
      if(me[nei[i]-1].occupation == process) {
        check = check || !surround(me[nei[i]-1].region);
      } else if(me[nei[i]-1].occupation == shirokuro(process)) {
	check = check || surround(me[nei[i]-1].region);
      }
    }
  }
  me[num-1].region = 0;
  if(!check) {
    me[num-1].occupation = 0;
    num = 0;
  }
    
  else {
    for(int i = 0; i < 4; i++) {
      if(nei[i] != 0) {
	if(me[nei[i]-1].occupation == process) {
	  join(num, me[nei[i]-1].region);
	} else if(me[nei[i]-1].occupation == shirokuro(process)) {
	  if(surround(me[nei[i]-1].region)) {
	    pickup(me[nei[i]-1].region);
	  }
	}
      }
    }
    if(me[num-1].region == 0) newRegion(num);
    process = shirokuro(process);

    if(torus()) {
      int acc = 0;
      if((hn == 1 && wm == 1) ||
	 (hn == N && wm == 1) ||
	 (hn == 1 && wm == M) ||
	 (hn == N && wm == M)) {
	acc = 1 + (1-1) * M;
	me[acc-1].occupation = me[num-1].occupation;
	me[acc-1].region = me[num-1].region;
	acc = 1 + (N-1) * M;
	me[acc-1].occupation = me[num-1].occupation;
	me[acc-1].region = me[num-1].region;
	acc = M + (1-1) * M;
	me[acc-1].occupation = me[num-1].occupation;
	me[acc-1].region = me[num-1].region;
	acc = M + (N-1) * M;
	me[acc-1].occupation = me[num-1].occupation;
	me[acc-1].region = me[num-1].region;
      } else if(hn == 1) {
	acc = wm + (N-1) * M;
	me[acc-1].occupation = me[num-1].occupation;
	me[acc-1].region = me[num-1].region;
      } else if(hn == N) {
	acc = wm + (1-1) * M;
	me[acc-1].occupation = me[num-1].occupation;
	me[acc-1].region = me[num-1].region;
      } else if(wm == 1) {
	acc = M + (hn-1) * M;
	me[acc-1].occupation = me[num-1].occupation;
	me[acc-1].region = me[num-1].region;
      } else if(wm == M) {
	acc = 1 + (hn-1) * M;
	me[acc-1].occupation = me[num-1].occupation;
	me[acc-1].region = me[num-1].region;
      }
    }
  }

  return num;
}

int Goban::occupySet(int id, int hn, int wm) {

  int num = wm + (hn-1) * M;
  if(me[num-1].occupation != 0) return 0;
  
  me[num-1].occupation = id;
  array<int, 4> nei = neighbors(hn, wm);

  for(int i = 0; i < 4; i++) {
    if(nei[i] != 0) {
      if(me[nei[i]-1].occupation == id) {
	join(num, me[nei[i]-1].region);
      }
    }
  }
  if(me[num-1].region == 0) newRegion(num);

  return num;
}

void Goban::join(int num, int region) {
  if(me[num-1].region == 0) {
    me[num-1].region = region;
  } else {
    joint(region, me[num-1].region);
  }
}

void Goban::joint(int r1, int r2) {
  int rmin, rmax;
  if(r1 < r2) { rmin = r1; rmax = r2; }
  else if(r1 > r2) { rmin = r2; rmax = r1; }
  else { return; }
  for(int i = 0; i < N*M; i++) {
    if(me[i].region == rmax) me[i].region = rmin;
  }
}

bool Goban::surround(int rnum) {
  bool sur = true;
  cout << rnum << " ";
  for(int i = 0; i < N*M; i++) {
    if(me[i].region == rnum) {
      array<int, 4> nei = neighbors(me[i].n, me[i].m);
      for(int j = 0; j < 4; j++) {
	if(nei[j] != 0) {
	  if(me[nei[j]-1].occupation == 0) {
	    sur = false;
	    cout << nei[j] << " ";
	  }
	}
      }
    }
  }
  cout << endl;
  return sur;
}

void Goban::pickup(int rnum) {
  for(int i = 0; i < N*M; i++) {
    if(me[i].region == rnum) {
      me[i].occupation = 0;
      me[i].region = 0;
    }
  }
}

void Goban::newRegion(int num) {
  bool regionOccupation[N*M];
  for(int i = 0; i < N*M; i++) {
    regionOccupation[i] = false;
  }
  for(int i = 0; i < N*M; i++) {
    if(me[i].region != 0) {
      regionOccupation[me[i].region-1] = true;
    }
  }
  int newr = 0;
  while(me[num-1].region == 0) {
    newr++;
    if(!regionOccupation[newr-1]) {
      me[num-1].region = newr;
    }
  }
}

int Goban::ctonm(double mx, double my, int & hn, int & wm) {
  wm = (int) (mx / interval + 0.5f);
  hn = (int) (my / interval + 0.5f);
  int num = 0;
  if(wm >= 1 && wm <= M && hn >= 1 && hn <= N)
    num = wm + (hn-1) * M;
  return num;
}

void Goban::nmtoc(int n, int m, GLfloat & vx, GLfloat & vy) {
  vx = m * interval;
  vy = n * interval;
}

void Goban::numtoc(int num, GLfloat & vx, GLfloat & vy) {
  int n = (int) ((num-1) / M) + 1;
  int m = num - (n-1)*M;
  nmtoc(n, m, vx, vy);
}

array<int, 4> Goban::neighbors(int hn, int wm) {
  int hns[4] = {hn+1, hn, hn-1, hn};  // 上右下左
  int wms[4] = {wm, wm+1, wm, wm-1};
  array<int, 4> res;
  for(int i = 0; i < 4; i++) {
    if(!torus()) {
      if(hns[i] >= 1 && hns[i] <= N && wms[i] >= 1 && wms[i] <= M)
	res[i] = wms[i] + (hns[i]-1) * M;
      else
	res[i] = 0;
    } else {
      if(hns[i] == 0) {
	if(N >= 3) hns[i] = N-1;
	else { wms[i] = 0; hns[i] = 1; }
      }
      if(hns[i] == N+1) {
	if(N >= 3) hns[i] = 2;
	else { wms[i] = 0; hns[i] = 1; }
      }
      if(wms[i] == 0) {
	if(M >= 3) wms[i] = M-1;
	else { wms[i] = 0; hns[i] = 1; }
      }
      if(wms[i] == M+1) {
	if(M >= 3) wms[i] = 2;
	else { wms[i] = 0; hns[i] = 1; }
      }
      res[i] = wms[i] + (hns[i]-1) * M;
    }
  }
  return res;
}

int Goban::shirokuro(int p) {
  if(p == 1) return 2;
  else if(p == 2) return 1;
  else return p;
}
