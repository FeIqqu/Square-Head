#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include "graphics.h"

using namespace std;

//===============���������� ����������===============
int Pole[20][32]; // �������� ������� ����(��������� ������ ������)
int PoleFlags[20][32]; // ������ ������ ��������� �������� ����(�������������� ������ ������-���� �� �������: '0' - ������, '1' - �������, '2' - �������)
int PoleColor[7]; // ������ � ������� ��� ������ �������
int Score1, Score2; // ���� ������� � ������� ������
int Pointer1, Pointer2; // ��������� �� ������� ������ ������� � ������� ������
int dp_PoleFlags[20][32]; 
int dp_Pole[20][32];
int lvl998[20][32];
int lvl999[20][32];
int s, s1, s2, pScore1, pScore2;
int impact;
int index_impact;
int gamer; // ����� ������ 1 - �. �. ����; 2 - �. �. ����
int last1, last2; // ���� �������������� ����������
int FlagGamer[2]; // �����, ����������� �� ��, ����� ����� ���� ������ ��� ������� �� �������: 1 - �������, 2,3,4,5 - ���� �� ���������� ���������
vector<int> colors {RGB(255, 236, 214), RGB(13, 43, 69), RGB(208, 54, 89), RGB(255, 255, 255), RGB(255, 212, 163), RGB(141, 104, 122), RGB(208, 73, 51)}; // ������ ������
IMAGE *images[26]; // ������ �����������
//================================================

//================��������� ���������================
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 6);
//================================================

//=================��������� �������================
void DrawMenu(int *state); // ��������� �������� ����
void NewGame(); // ����� ����
bool DrawField(); // ��������� ����
bool Settings(); // ���������(��� ������ ����� ����)
void dfs_draw(int i, int j, int PF[20][32], int P[20][32], int P1, int P2, int g,int f); // ���
void dfs_s(int i, int j, int PF[20][32], int g);
void ravenstvo(int a1[20][32], int a2[20][32]);
void Triangle(int x, int y); // ������ �����������
void Rules();
void About();
//================================================

int main(){
   initwindow(800, 600, "Square Head", 100, 100); // ������������� ����
   setbkcolor(RGB(84, 78, 104));
   settextstyle(TRIPLEX_FONT, 0, 1);
   for(int i = 0; i < 18; i++){ // ���� ����������� � ������
      char filename[25];
      sprintf(filename, ".\\Pictures\\Picture%d.bmp", i + 1);
      images[i] = loadBMP(filename);
   }
   while(true){
      clearviewport();
      static int state[4] = {0, 0, 0, 0}; // ��������� ���������
      int buttonbounds[4][4] = {{330, 290, 470, 325}, {330, 350, 470, 385}, {330, 410, 470, 445}, {330, 470, 470, 505}}; // ������� ������
      int x = mousex(), y = mousey(); // �������� ���������� ��������� ����
      DrawMenu(state); // ��������� �������� ����
      if(kbhit()) getch(); // ����� �� ����������� ������ ������� ������ ��� ������ ����
      if(ismouseclick(WM_LBUTTONDOWN)){ // ���������, ���� �� ����������� ������� �� ���
         getmouseclick(WM_LBUTTONDOWN, x, y); // ���������� ���������� ����� ������� ���
         for(int i = 0; i < 4; i++){ // ����� ������ � ������� ����
            if(x >= buttonbounds[i][0] && x <= buttonbounds[i][2] && y >= buttonbounds[i][1] && y <= buttonbounds[i][3]){
               switch(i){
                  case 0:
                     NewGame(); // ������ "����� ����"
                     break;
                  case 1:
                     Rules(); // ������ "�������"
                     break;
                  case 2:
                     About();// ������ "� ���������"
                     break;
                  case 3:
                     exit(0); // ������ "�����"
                     break;
               }
            }
         }
      }
      for(int i = 0; i < 4; i++){ // ����� ����������� ������ ��� ��������� ����
         if(x >= buttonbounds[i][0] && x <= buttonbounds[i][2] && y >= buttonbounds[i][1] && y <= buttonbounds[i][3] && state[i] == 0){
            state[i] = 1; // ������ ��������� ������� ������
            putimage(buttonbounds[0][0], buttonbounds[i][1], images[i*2+1], COPY_PUT); // ��������� ����������� ������
         }
         else if(!(x >= buttonbounds[i][0] && x <= buttonbounds[i][2] && y >= buttonbounds[i][1] && y <= buttonbounds[i][3]) && state[i] == 1){
            state[i] = 0; // ������ ��������� ������� ������
            putimage(buttonbounds[0][0], buttonbounds[i][1], images[i*2], COPY_PUT); // ��������� ����������� ������
         }
      }
      swapbuffers();
      delay(30);
   }
}
void ravenstvo(int a1[20][32], int a2[20][32]){
   for(int i = 0; i < 20; i++){
      for(int j = 0; j < 32; j++){
         a1[i][j] = a2[i][j];
      }
   }
}
void dfs_draw(int i, int j, int PF[20][32], int P[20][32], int P1, int P2, int g, int f){
   int flag = 0; //���� ��������� ����� ������ ������
   if(g == 1){
      if(PF[i][j] == 0){
         PF[i][j] = 1;
         if(f == 1){ 
            Score1++;
            pScore1 += lvl998[i][j];
         }
         else if(f == 0) s1++;
         else s1 += lvl998[i][j];
      }
      if(P[i][j] != PoleColor[P1]){
         P[i][j] = PoleColor[P1];
         flag = 1;
      }
   }
   else{
      if(PF[i][j] == 0){
         PF[i][j] = 2;
         if(f == 1){
            Score2++;
            pScore2 += lvl999[i][j];
         }
         else if(f == 0) s2++;
         else s2 += lvl999[i][j];
      }
      if(P[i][j] != PoleColor[P2]){
         P[i][j] = PoleColor[P2];
         flag = 1;
      }
   }
   int ni, nj;
   int di[4] = {0, -1, 0, 1},  dj[4] = {-1, 0, 1, 0}; //���������� �������� �� ������� ������
   for(int k = 0; k < 4; k++){
      ni = i + di[k];
      nj = j + dj[k];
      if(ni < 0 || nj < 0 || ni >= 20 || nj >= 32) continue;
      if((P[ni][nj] == last1 && g == 1 && flag) || (P[ni][nj] == last2 && g == 2 && flag) ||
         (((P[ni][nj] == PoleColor[P1] && g == 1) ||
         (P[ni][nj] == PoleColor[P2] && g == 2))
         && PF[ni][nj] == 0)) dfs_draw(ni, nj, PF, P, P1, P2, g, f); //�������� ����������� ������� �� ��������� ������
   }
}
void dfs_s(int i, int j, int PF[20][32], int g){
   int flag = 0;
   if(PF[i][j] != g){
      PF[i][j] = g;
      flag = 1;
      s++;
   }
   int ni, nj;
   int di[4] = {0, -1, 0, 1},  dj[4] = {-1, 0, 1, 0}; //���������� �������� �� ������� ������
   for(int k = 0; k < 4; k++){
      ni = i + di[k];
      nj = j + dj[k];
      if(ni < 0 || nj < 0 || ni >= 20 || nj >= 32) continue;
      if(flag) dfs_s(ni, nj, PF, g); //�������� ����������� ������� �� ��������� ������
   }
}
void DrawMenu(int *state){
   putimage(0, 0, images[17]);
   for(int i = 0; i < 4; i++) // ����� ������ ����
      if(state[i]) putimage(330, (i + 1) * 60 + 230, images[i*2 + 1], COPY_PUT);
      else putimage(330, (i + 1) * 60 + 230, images[i*2], COPY_PUT);
} // ��������� �������� ����

void Triangle(int x, int y){
   int i = 9;
   if(y == 92) i *= -1;
   int v[] = {x, y, x + 6, y + i, x + 12, y};
   fillpoly(3, v);
} // ������ �����������

bool Settings(){
   clearviewport();
   static int state[4] = {0, 0}; // ��������� ���������
   int buttonbounds[4][4] = {{718, 10, 790, 32}, {225, 250, 295, 320}, {500, 250, 570, 320}, {375, 350, 425, 400}}; // ������� ������
   int x = mousex(), y = mousey(); // �������� ���������� ��������� ����
   if(kbhit()) getch(); // ����� �� ����������� ������ ������� ������ ��� ������ ����
   settextstyle(GOTHIC_FONT, 0, 0.8);
   settextjustify(CENTER_TEXT, CENTER_TEXT);
   setusercharsize(1, 1, 1, 1);
   outtextxy(390, 50, "��������� ����"), settextstyle(GOTHIC_FONT, 0, 1), outtextxy(250, 220, "������ �����"),  outtextxy(530, 220, "������ �����");
   if(state[0]) putimage(buttonbounds[0][0], buttonbounds[0][1], images[9], COPY_PUT);
   else putimage(buttonbounds[0][0], buttonbounds[0][1], images[8], COPY_PUT);
   if(state[1]) putimage(buttonbounds[3][0], buttonbounds[3][1], images[11], TRANSPARENT_PUT);
   else putimage(buttonbounds[3][0], buttonbounds[3][1], images[10], TRANSPARENT_PUT);
   
   putimage(buttonbounds[1][0], buttonbounds[1][1], images[11 + FlagGamer[0]], TRANSPARENT_PUT);
   putimage(buttonbounds[2][0], buttonbounds[2][1], images[11 + FlagGamer[1]], TRANSPARENT_PUT);
   
   if(ismouseclick(WM_LBUTTONDOWN)){ // ���������, ���� �� ����������� ������� �� ���
      getmouseclick(WM_LBUTTONDOWN, x, y); // ���������� ���������� ����� ������� ���
      if(x >= buttonbounds[0][0] && x <= buttonbounds[0][2] && y >= buttonbounds[0][1] && y <= buttonbounds[0][3]) // ���� ������ ������ "�����"
         return false;
      else if(x >= buttonbounds[1][0] && x <= buttonbounds[1][2] && y >= buttonbounds[1][1] && y <= buttonbounds[1][3]){ // ���� ������ ������ ������� ������
         if(FlagGamer[0] == 5) FlagGamer[0] = 1;
         else FlagGamer[0]++;
         Settings(); // ���������(��� ������ ����� ����)
         return true;
      }
      else if(x >= buttonbounds[2][0] && x <= buttonbounds[2][2] && y >= buttonbounds[2][1] && y <= buttonbounds[2][3]){ // ���� ������ ������ ������� ������
         if(FlagGamer[1] == 5) FlagGamer[1] = 1;
         else FlagGamer[1]++;
         Settings(); // ���������(��� ������ ����� ����)
         return true;
      }
      else if(x >= buttonbounds[3][0] && x <= buttonbounds[3][2] && y >= buttonbounds[3][1] && y <= buttonbounds[3][3]){ // ���� ������ ������ ������������� ������
         int s = 30;
         for(int i = 0; i < 20; i++){
            s = s + i;
            for(int j = 0; j < 32; j++){
               lvl998[i][j] = s;
               lvl999[19 - i][31 - j] = s;
               s++;
            }
         }
         while(true){
            bool f = DrawField(); // ��������� ���� ����
            if(!f){ // ���� ������ ������ ������ ����� �� ����� ����
               return false;
               break;
            }
            if(FlagGamer[gamer - 1] == 1){
               if(kbhit()){
                  int ch = getch();
                  if(ch == KEY_LEFT){
                     if(gamer == 1 && Pointer1 > 0) Pointer1--;
                     else if(gamer == 2 && Pointer2 > 0) Pointer2--;
                  }
                  else if(ch == KEY_RIGHT){
                     if(gamer == 1 && Pointer1 < 6) Pointer1++;
                     else if(gamer == 2 && Pointer2 < 6) Pointer2++;
                  }
                  else if(ch == KEY_ENTER){
                     if(gamer == 1 && PoleColor[Pointer1] != Pole[0][0] && PoleColor[Pointer1] != Pole[19][31]){ // ���� Enter ����� � ���������� ����� ��� ������� ������
                        dfs_draw(0, 0, PoleFlags, Pole, Pointer1, Pointer2, 1,1); // �������� ��������������� �����
                        last1 = PoleColor[Pointer1];
                        gamer = 2; // ������� ��� ������� ������
                     }
                     else if(gamer == 2 && PoleColor[Pointer2] != Pole[19][31] && PoleColor[Pointer2] != Pole[0][0]){ // ���� Enter ����� � ���������� ����� ��� ������� ������
                        dfs_draw(19, 31, PoleFlags, Pole, Pointer1, Pointer2, 2,1); // �������� ��������������� �����
                        last2 = PoleColor[Pointer2];
                        gamer = 1; // ������� ��� ������� ������
                     }
                  }
               }
            }
            else{
               impact = 0;
               index_impact = 0;
               if(FlagGamer[gamer - 1] == 2){
                  for(int i = 0; i < 7; i++){
                     ravenstvo(dp_PoleFlags, PoleFlags);
                     ravenstvo(dp_Pole, Pole);
                     s1 = Score1;
                     s2 = Score2;
                     if(gamer == 1 && PoleColor[i] != Pole[0][0] && PoleColor[i] != Pole[19][31]){ 
                        dfs_draw(0, 0, dp_PoleFlags, dp_Pole, i, i, 1, 0);
                        if (s1 - Score1 >= impact){
                           impact = s1 - Score1;
                           index_impact = i;
                        }
                     }  
                     else if(gamer == 2 && PoleColor[i] != Pole[19][31] && PoleColor[i] != Pole[0][0]){ 
                        dfs_draw(19, 31, dp_PoleFlags, dp_Pole, i, i, 2, 0);
                        if (s2 - Score2 >= impact){
                           impact = s2 - Score2;
                           index_impact = i;
                        }
                     }
                  }
               }
               else if(FlagGamer[gamer - 1] == 3){
                  for(int i = 0; i < 7; i++){
                     ravenstvo(dp_PoleFlags, PoleFlags);
                     ravenstvo(dp_Pole, Pole);
                     s1 = Score1;
                     s2 = Score2;
                     if(i != Pointer1 && i != Pointer2){
                        dfs_draw(0, 0, dp_PoleFlags, dp_Pole, i, i, 1, 0); 
                        ravenstvo(dp_PoleFlags, PoleFlags);
                        ravenstvo(dp_Pole, Pole);
                        dfs_draw(19, 31, dp_PoleFlags, dp_Pole, i, i, 2, 0);
                        if(gamer == 1 && s1 - Score1 > 0){
                           if (s1 - Score1 + (s2 - Score2) / 3 >= impact){
                              impact = s1 - Score1 + ((s2 - Score2) / 3);
                              index_impact = i;
                           }
                        }
                        if(gamer == 2 && s2 - Score2 > 0){
                           if (s2 - Score2 + (s1 - Score1) / 4 >= impact){
                              impact = s2 - Score2 + ((s1 - Score1) / 4);
                              index_impact = i;
                           }
                        }
                     }  
                  }
               }
               else if(FlagGamer[gamer - 1] == 4){
                  impact = 641;
                  int c = 0;
                  for(int i = 0; i < 7; i++){
                     ravenstvo(dp_PoleFlags, PoleFlags);
                     ravenstvo(dp_Pole, Pole);
                     s = 0;
                     if(gamer == 1 && PoleColor[i] != Pole[0][0] && PoleColor[i] != Pole[19][31]){
                        dfs_draw(0, 0, dp_PoleFlags, dp_Pole, i, i, 1, 0);
                        dfs_s(19, 31, dp_PoleFlags, 1);
                        if (s <= impact){
                           if(s == impact) c++;
                           impact = s;
                           index_impact = i;
                        }
                     }  
                     else if(gamer == 2 && PoleColor[i] != Pole[19][31] && PoleColor[i] != Pole[0][0]){
                        dfs_draw(19, 31, dp_PoleFlags, dp_Pole, i, i, 2, 0);
                        dfs_s(0, 0, dp_PoleFlags, 2);
                        if (s <= impact){
                           if(s == impact) c++;
                           impact = s;
                           index_impact = i;
                        }
                     }
                  }
                  if(c == 4){
                     int x;
                     while(true){
                        x = dis(gen);
                        index_impact = x;
                        if(x != Pointer1 && x != Pointer2) break;
                     }
                  }
               }
               else if(FlagGamer[gamer - 1] == 5){
                  for(int i = 0; i < 7; i++){
                     ravenstvo(dp_PoleFlags, PoleFlags);
                     ravenstvo(dp_Pole, Pole);
                     s1 = pScore1;
                     s2 = pScore2;
                     if(gamer == 1 && PoleColor[i] != Pole[0][0] && PoleColor[i] != Pole[19][31]){ 
                        dfs_draw(0, 0, dp_PoleFlags, dp_Pole, i, i, 1, 2);
                        if (s1 - pScore1 >= impact){
                           impact = s1 - pScore1;
                           index_impact = i;
                        }
                     }  
                     else if(gamer == 2 && PoleColor[i] != Pole[19][31] && PoleColor[i] != Pole[0][0]){ 
                        dfs_draw(19, 31, dp_PoleFlags, dp_Pole, i, i, 2, 2);
                        if (s2 - pScore2 >= impact){
                           impact = s2 - pScore2;
                           index_impact = i;
                        }
                     }
                  }
               }
               if(impact == 0 && ((Pointer1 == 0) || (Pointer2 == 0))){
                  index_impact = 1;
                  if((Pointer1 == 1) || (Pointer2 == 1)){
                     index_impact = 2;
                  }
               }
               if(gamer == 1){
                  dfs_draw(0, 0, PoleFlags, Pole, index_impact, index_impact, 1, 1);
                  last1 = PoleColor[index_impact];
                  gamer = 2; 
                  Pointer1 = index_impact;
               }
               else{
                  dfs_draw(19, 31, PoleFlags, Pole, index_impact, index_impact, 2, 1);
                  last2 = PoleColor[index_impact];
                  gamer = 1;
                  Pointer2 = index_impact;
               }
            }
            if(Score1 + Score2 == 640){ // ���� ���� �����������
               DrawField(); // ��������� ���� ����
               delay(1000);
               clearviewport();
               if(Score1 > Score2) outtextxy(300, 300, "������ ������� ������");
               else if(Score1 < Score2) outtextxy(300, 300, "������ ������� ������");
               else outtextxy(300, 300, "�����");
               swapbuffers();
               while(true) if(getch() == KEY_ENTER) break;
               return false;
               break;
            }
            delay(30);
         }
      }
   } // ��������� ��������
   if(x >= buttonbounds[3][0] && x <= buttonbounds[3][2] && y >= buttonbounds[3][1] && y <= buttonbounds[3][3] && state[3] == 0){
      state[1] = 1; // ������ ��������� ������� ������
      putimage(buttonbounds[3][0], buttonbounds[3][1], images[10], COPY_PUT); // ��������� ����������� ������
   }
   else if(!(x >= buttonbounds[3][0] && x <= buttonbounds[3][2] && y >= buttonbounds[3][1] && y <= buttonbounds[3][3]) && state[3] == 1){
      state[1] = 0; // ������ ��������� ������� ������
      putimage(buttonbounds[3][0], buttonbounds[3][1], images[11], COPY_PUT); // ��������� ����������� ������
   }
   if(x >= buttonbounds[0][0] && x <= buttonbounds[0][2] && y >= buttonbounds[0][1] && y <= buttonbounds[0][3] && state[0] == 0){
      state[0] = 1; // ������ ��������� ������� ������
      putimage(buttonbounds[0][0], buttonbounds[0][1], images[9], COPY_PUT); // ��������� ����������� ������
   }
   else if(!(x >= buttonbounds[0][0] && x <= buttonbounds[0][2] && y >= buttonbounds[0][1] && y <= buttonbounds[0][3]) && state[0] == 1){
      state[0] = 0; // ������ ��������� ������� ������
      putimage(buttonbounds[0][0], buttonbounds[0][1], images[8], COPY_PUT); // ��������� ����������� ������
   }
   swapbuffers();
   return true;
} // ���������(��� ������ ����� ����)

void NewGame(){
   for(int i = 0; i < 20; i++) // ��������� ������ Pole ���������� ������� �� ������� colors
      for(int j = 0; j < 32; j++)
         Pole[i][j] = colors[dis(gen)];
   shuffle(colors.begin(), colors.end(), gen);
   copy(colors.begin(), colors.begin() + 7, PoleColor); // ��������� ������ PoleFirst ���������� �������
   while(Pole[0][0] == Pole[19][31]) Pole[19][31] = colors[dis(gen)]; // ����� ��������� ����� ������� �� �������
   for(int i = 0; i < 20; i++) // ��������� ������ ������ PoleFlags ��������� '0'
      for(int j = 0; j < 32; j++)
         PoleFlags[i][j] = 0;
   PoleFlags[0][0] = 1, PoleFlags[19][31] = 2; // ����� �������������� ������� ����� � ������ ������ ������ ��������������� �������
   Score1 = 1, Score2 = 1; // ����� �������� ����� �������
   Pointer1 = 0, Pointer2 = 0; // ����� �������� ���������� �� ������� ������ ������� � ������� ������
   last1 = Pole[0][0], last2 = Pole[19][31];
   gamer = 1; // ��� ������� ������
   FlagGamer[0] = 1, FlagGamer[1] = 1; // ����� ����������� ����� ����(���� ���� �����)
   while(true){
      bool f = Settings(); // ���������(��� ������ ����� ����)
      if(!f){ // ���� ������ ������ ������ ����� � ����������
         clearviewport();
         DrawMenu([](){return new int[4]{0, 0, 0, 0};}()); // ��������� �������� ����
         swapbuffers();
         break;
      }
      delay(30);
   }
} // ����� ����

bool DrawField(){
   clearviewport();
   static int state = 0; // ��������� ���������
   int buttonbounds[4] = {718, 10, 790, 32}; // ������� ������ "�����"
   int x = mousex(), y = mousey(); // �������� ���������� ��������� ����
   char str1[4], str2[4]; // ������� �������� ��� ������ ����� ������� � ������� ������
   sprintf(str1, "%03d", Score1), sprintf(str2, "%03d", Score2); // ��������� ������ ������� �������
   if(state) putimage(buttonbounds[0], buttonbounds[1], images[9], COPY_PUT); // ������ "�����"
   else putimage(buttonbounds[0], buttonbounds[1], images[8], COPY_PUT); // ������ "�����"
   for(int i = 0; i < 7; i++){ // ������������ ���� ������� ������
      int color = PoleColor[i];
      int x = i * 20 + 80, y = 60;
      if(color == Pole[19][31] or color == Pole[0][0]){ // ���� ������ �� ����� ���� ������� ������� - ������ � � ��������
         for(int i = x; i <= x + 18; i += 2){
            for(int j = y; j <= y + 18; j += 2){
               putpixel(i, j, color);
            }
         }
         continue;
      }
      setfillstyle(SOLID_FILL, color);
      bar(x, y, x + 18, y + 18);
   }
   for(int i = 0; i < 20; i++){ // ������������ �������� ����
      for(int j = 0; j < 32; j++){
         int color = Pole[i][j];
         int x = j * 20 + 80, y = i * 20 + 100;
         setfillstyle(SOLID_FILL, color);
         bar(x, y, x + 18, y + 18);
      }
   }
   for(int i = 0; i < 7; i++){ // ������������ ���� ������� ������
      int color = PoleColor[i];
      int x = i * 20 + 580, y = 520;
      if(color == Pole[19][31] or color == Pole[0][0]){ // ���� ������ �� ����� ���� ������� ������� - ������ � � ��������
         for(int i = x; i <= x + 18; i += 2){
            for(int j = y; j <= y + 18; j += 2){
               putpixel(i, j, color);
            }
         }
         continue;
      }
      setfillstyle(SOLID_FILL, color);
      bar(x, y, x + 18, y + 18);
   }
   settextjustify(LEFT_TEXT, CENTER_TEXT);
   outtextxy(228, 75, str1), outtextxy(525, 535, str2); // ������������ ����� �������
   setfillstyle(SOLID_FILL, WHITE);
   Triangle(83 + Pointer1 * 20, 92); // ������������ �����������(���������), ������� ����������, ����� ���� ������ ������ � ������� ������
   Triangle(583 + Pointer2 * 20, 507); // ������������ �����������(���������), ������� ����������, ����� ���� ������ ������ � ������� ������
   if(ismouseclick(WM_LBUTTONDOWN)){ // ���������, ���� �� ����������� ������� �� ���
      getmouseclick(WM_LBUTTONDOWN, x, y); // ���������� ���������� ����� ������� ���
      if(x >= buttonbounds[0] && x <= buttonbounds[2] && y >= buttonbounds[1] && y <= buttonbounds[3]) // ���� ������ ������ "�����"
         return false;
   }
   if(x >= buttonbounds[0] && x <= buttonbounds[2] && y >= buttonbounds[1] && y <= buttonbounds[3] && state == 0){
      state = 1; // ������ ��������� ������� ������
      putimage(buttonbounds[0], buttonbounds[1], images[9], COPY_PUT); // ��������� ����������� ������
   }
   else if(!(x >= buttonbounds[0] && x <= buttonbounds[2] && y >= buttonbounds[1] && y <= buttonbounds[3]) && state == 1){
      state = 0; // ������ ��������� ������� ������
      putimage(buttonbounds[0], buttonbounds[1], images[8], COPY_PUT); // ��������� ����������� ������
   }
   swapbuffers();
   return true;
} // ��������� ����

void Rules(){
   int xs=30, ys=190; //���������� ������ ����� �� �����
   char str[1000];
   clearviewport();
   setcolor(WHITE);
   settextstyle(TRIPLEX_FONT, 0, USER_CHAR_SIZE);
   FILE * f=fopen("rules.txt", "r");
   setusercharsize(2, 5, 16, 25);
   settextjustify(LEFT_TEXT, CENTER_TEXT);
   while(fgets(str, sizeof(str), f)){
      outtextxy(xs, ys, str);
      ys+=textheight("E") + 2;
   }
   fclose(f);
   settextjustify(CENTER_TEXT, CENTER_TEXT);
   setusercharsize(2, 5, 16, 25);
   outtextxy(400, 550, "��� ������ � ���� ������� (Esc)");
   swapbuffers();
   while(1){
      if(kbhit()){
         int p=getch();
         if(p==KEY_ESC) return;
      }
   }
}

void About(){
   int x0=510, y0=200; //���������� ������ ������ ������� ���������� � ���������
   int d=20; //���������� ����� ��������
   clearviewport();
   setcolor(WHITE);
   settextstyle(TRIPLEX_FONT, 0, USER_CHAR_SIZE);
   settextjustify(CENTER_TEXT, CENTER_TEXT);
   setusercharsize(2, 5, 16, 25);
   outtextxy(400, 550, "��� ������ � ���� ������� (Esc)");
   setusercharsize(3, 4, 6, 5);
   outtextxy(400, 50, "� ���������");
   outtextxy(400, 100, "���� ''Square Head''");
   settextjustify(LEFT_TEXT, CENTER_TEXT);
   setusercharsize(3, 8, 3, 5);
   outtextxy(x0, y0, "������� ���������� ����������");
   outtextxy(x0, y0+d, "� ����������������");
   outtextxy(x0, y0+d*2, "����������: ''������� ��������''");
   y0+=20;
   outtextxy(x0, y0+d*3, "������������: ������� �.�.");
   y0+=20;
   outtextxy(x0, y0+d*4, "������:");
   outtextxy(x0, y0+d*5, "������� ������ ��-112");
   outtextxy(x0, y0+d*6, "�������� �.�.");
   outtextxy(x0, y0+d*7, "������� ������ ��-112");
   outtextxy(x0, y0+d*8, "������� �.�.");
   outtextxy(x0, y0+d*9, "������� ������ ��-112");
   outtextxy(x0, y0+d*10, "�������� �.�.");
   outtextxy(x0, y0+d*11, "������� ������ ��-112");
   outtextxy(x0, y0+d*12, "������ �.�.");
   swapbuffers();
   while(1){
      if(kbhit()){
         int p=getch(1);
         if(p==KEY_ESC) return;
      }
   }
}