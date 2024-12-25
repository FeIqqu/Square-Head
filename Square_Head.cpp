#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include "graphics.h"

using namespace std;

//===============Глобальные переменные===============
int Pole[20][32]; // Основное игровое поле(Двумерный массив цветов)
int PoleFlags[20][32]; // Массив флагов основного игрового поля(Принадлежность клетки какому-либо из игроков: '0' - никому, '1' - первому, '2' - второму)
int PoleColor[7]; // Массив с цветами для выбора игроков
int Score1, Score2; // Счёт первого и второго игрока
int Pointer1, Pointer2; // Указатели на массивы цветов первого и второго игрока
int dp_PoleFlags[20][32]; 
int dp_Pole[20][32];
int lvl998[20][32];
int lvl999[20][32];
int s, s1, s2, pScore1, pScore2;
int impact;
int index_impact;
int gamer; // Номер игрока 1 - л. в. угол; 2 - п. н. угол
int last1, last2; // Цвет предшествующий выбранному
int FlagGamer[2]; // Флаги, указывающие на то, какой режим игры выбран для каждого из игроков: 1 - человек, 2,3,4,5 - боты по нарастанию сложности
vector<int> colors {RGB(255, 236, 214), RGB(13, 43, 69), RGB(208, 54, 89), RGB(255, 255, 255), RGB(255, 212, 163), RGB(141, 104, 122), RGB(208, 73, 51)}; // Вектор цветов
IMAGE *images[26]; // Массив изображений
//================================================

//================Параметры генерации================
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 6);
//================================================

//=================Прототипы функций================
void DrawMenu(int *state); // Отрисовка главного меню
void NewGame(); // Новая игра
bool DrawField(); // Отрисовка поля
bool Settings(); // Настройки(При начале новой игры)
void dfs_draw(int i, int j, int PF[20][32], int P[20][32], int P1, int P2, int g,int f); // Ход
void dfs_s(int i, int j, int PF[20][32], int g);
void ravenstvo(int a1[20][32], int a2[20][32]);
void Triangle(int x, int y); // Рисуем треугольник
void Rules();
void About();
//================================================

int main(){
   initwindow(800, 600, "Square Head", 100, 100); // Инициализация окна
   setbkcolor(RGB(84, 78, 104));
   settextstyle(TRIPLEX_FONT, 0, 1);
   for(int i = 0; i < 18; i++){ // Ввод изображений в массив
      char filename[25];
      sprintf(filename, ".\\Pictures\\Picture%d.bmp", i + 1);
      images[i] = loadBMP(filename);
   }
   while(true){
      clearviewport();
      static int state[4] = {0, 0, 0, 0}; // Начальные состояния
      int buttonbounds[4][4] = {{330, 290, 470, 325}, {330, 350, 470, 385}, {330, 410, 470, 445}, {330, 470, 470, 505}}; // Границы кнопок
      int x = mousex(), y = mousey(); // Получаем координаты указателя мыши
      DrawMenu(state); // Отрисовка главного меню
      if(kbhit()) getch(); // Чтобы не происходило лишних нажатий клавиш при начале игры
      if(ismouseclick(WM_LBUTTONDOWN)){ // Проверяем, было ли произведено нажатие на ЛКМ
         getmouseclick(WM_LBUTTONDOWN, x, y); // Запоминаем координаты места нажатия ЛКМ
         for(int i = 0; i < 4; i++){ // Выбор кнопки в главном меню
            if(x >= buttonbounds[i][0] && x <= buttonbounds[i][2] && y >= buttonbounds[i][1] && y <= buttonbounds[i][3]){
               switch(i){
                  case 0:
                     NewGame(); // Кнопка "Новая игра"
                     break;
                  case 1:
                     Rules(); // Кнопка "Правила"
                     break;
                  case 2:
                     About();// Кнопка "О программе"
                     break;
                  case 3:
                     exit(0); // Кнопка "Выход"
                     break;
               }
            }
         }
      }
      for(int i = 0; i < 4; i++){ // Смена изображения кнопки при наведении мыши
         if(x >= buttonbounds[i][0] && x <= buttonbounds[i][2] && y >= buttonbounds[i][1] && y <= buttonbounds[i][3] && state[i] == 0){
            state[i] = 1; // Меняем состояние нажатия кнопки
            putimage(buttonbounds[0][0], buttonbounds[i][1], images[i*2+1], COPY_PUT); // Вставляем изображение кнопки
         }
         else if(!(x >= buttonbounds[i][0] && x <= buttonbounds[i][2] && y >= buttonbounds[i][1] && y <= buttonbounds[i][3]) && state[i] == 1){
            state[i] = 0; // Меняем состояние нажатия кнопки
            putimage(buttonbounds[0][0], buttonbounds[i][1], images[i*2], COPY_PUT); // Вставляем изображение кнопки
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
   int flag = 0; //флаг изменения цвета клеток игрока
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
   int di[4] = {0, -1, 0, 1},  dj[4] = {-1, 0, 1, 0}; //Координаты перехода на смежную клетку
   for(int k = 0; k < 4; k++){
      ni = i + di[k];
      nj = j + dj[k];
      if(ni < 0 || nj < 0 || ni >= 20 || nj >= 32) continue;
      if((P[ni][nj] == last1 && g == 1 && flag) || (P[ni][nj] == last2 && g == 2 && flag) ||
         (((P[ni][nj] == PoleColor[P1] && g == 1) ||
         (P[ni][nj] == PoleColor[P2] && g == 2))
         && PF[ni][nj] == 0)) dfs_draw(ni, nj, PF, P, P1, P2, g, f); //Проверка возможности перейти на следующую клетку
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
   int di[4] = {0, -1, 0, 1},  dj[4] = {-1, 0, 1, 0}; //Координаты перехода на смежную клетку
   for(int k = 0; k < 4; k++){
      ni = i + di[k];
      nj = j + dj[k];
      if(ni < 0 || nj < 0 || ni >= 20 || nj >= 32) continue;
      if(flag) dfs_s(ni, nj, PF, g); //Проверка возможности перейти на следующую клетку
   }
}
void DrawMenu(int *state){
   putimage(0, 0, images[17]);
   for(int i = 0; i < 4; i++) // Вывод кнопок меню
      if(state[i]) putimage(330, (i + 1) * 60 + 230, images[i*2 + 1], COPY_PUT);
      else putimage(330, (i + 1) * 60 + 230, images[i*2], COPY_PUT);
} // Отрисовка главного меню

void Triangle(int x, int y){
   int i = 9;
   if(y == 92) i *= -1;
   int v[] = {x, y, x + 6, y + i, x + 12, y};
   fillpoly(3, v);
} // Рисуем треугольник

bool Settings(){
   clearviewport();
   static int state[4] = {0, 0}; // Начальные состояния
   int buttonbounds[4][4] = {{718, 10, 790, 32}, {225, 250, 295, 320}, {500, 250, 570, 320}, {375, 350, 425, 400}}; // Границы кнопок
   int x = mousex(), y = mousey(); // Получаем координаты указателя мыши
   if(kbhit()) getch(); // Чтобы не происходило лишних нажатий клавиш при начале игры
   settextstyle(GOTHIC_FONT, 0, 0.8);
   settextjustify(CENTER_TEXT, CENTER_TEXT);
   setusercharsize(1, 1, 1, 1);
   outtextxy(390, 50, "Настройки игры"), settextstyle(GOTHIC_FONT, 0, 1), outtextxy(250, 220, "Первый игрок"),  outtextxy(530, 220, "Второй игрок");
   if(state[0]) putimage(buttonbounds[0][0], buttonbounds[0][1], images[9], COPY_PUT);
   else putimage(buttonbounds[0][0], buttonbounds[0][1], images[8], COPY_PUT);
   if(state[1]) putimage(buttonbounds[3][0], buttonbounds[3][1], images[11], TRANSPARENT_PUT);
   else putimage(buttonbounds[3][0], buttonbounds[3][1], images[10], TRANSPARENT_PUT);
   
   putimage(buttonbounds[1][0], buttonbounds[1][1], images[11 + FlagGamer[0]], TRANSPARENT_PUT);
   putimage(buttonbounds[2][0], buttonbounds[2][1], images[11 + FlagGamer[1]], TRANSPARENT_PUT);
   
   if(ismouseclick(WM_LBUTTONDOWN)){ // Проверяем, было ли произведено нажатие на ЛКМ
      getmouseclick(WM_LBUTTONDOWN, x, y); // Запоминаем координаты места нажатия ЛКМ
      if(x >= buttonbounds[0][0] && x <= buttonbounds[0][2] && y >= buttonbounds[0][1] && y <= buttonbounds[0][3]) // Если нажата кнопка "Назад"
         return false;
      else if(x >= buttonbounds[1][0] && x <= buttonbounds[1][2] && y >= buttonbounds[1][1] && y <= buttonbounds[1][3]){ // Если нажата кнопка первого игрока
         if(FlagGamer[0] == 5) FlagGamer[0] = 1;
         else FlagGamer[0]++;
         Settings(); // Настройки(При начале новой игры)
         return true;
      }
      else if(x >= buttonbounds[2][0] && x <= buttonbounds[2][2] && y >= buttonbounds[2][1] && y <= buttonbounds[2][3]){ // Если нажата кнопка второго игрока
         if(FlagGamer[1] == 5) FlagGamer[1] = 1;
         else FlagGamer[1]++;
         Settings(); // Настройки(При начале новой игры)
         return true;
      }
      else if(x >= buttonbounds[3][0] && x <= buttonbounds[3][2] && y >= buttonbounds[3][1] && y <= buttonbounds[3][3]){ // Если нажата кнопка подтверждения выбора
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
            bool f = DrawField(); // Отрисовка поля игры
            if(!f){ // Если нажата кнопка кнопка выход во время игры
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
                     if(gamer == 1 && PoleColor[Pointer1] != Pole[0][0] && PoleColor[Pointer1] != Pole[19][31]){ // Если Enter нажат в подобающем месте для первого игрока
                        dfs_draw(0, 0, PoleFlags, Pole, Pointer1, Pointer2, 1,1); // Вызываем соответствующий обход
                        last1 = PoleColor[Pointer1];
                        gamer = 2; // Передаём ход второму игроку
                     }
                     else if(gamer == 2 && PoleColor[Pointer2] != Pole[19][31] && PoleColor[Pointer2] != Pole[0][0]){ // Если Enter нажат в подобающем месте для второго игрока
                        dfs_draw(19, 31, PoleFlags, Pole, Pointer1, Pointer2, 2,1); // Вызываем соответствующий обход
                        last2 = PoleColor[Pointer2];
                        gamer = 1; // Передаём ход первому игроку
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
            if(Score1 + Score2 == 640){ // Если игра закончилась
               DrawField(); // Отрисовка поля игры
               delay(1000);
               clearviewport();
               if(Score1 > Score2) outtextxy(300, 300, "Победа первого игрока");
               else if(Score1 < Score2) outtextxy(300, 300, "Победа второго игрока");
               else outtextxy(300, 300, "Ничья");
               swapbuffers();
               while(true) if(getch() == KEY_ENTER) break;
               return false;
               break;
            }
            delay(30);
         }
      }
   } // Отрисовка настроек
   if(x >= buttonbounds[3][0] && x <= buttonbounds[3][2] && y >= buttonbounds[3][1] && y <= buttonbounds[3][3] && state[3] == 0){
      state[1] = 1; // Меняем состояние нажатия кнопки
      putimage(buttonbounds[3][0], buttonbounds[3][1], images[10], COPY_PUT); // Вставляем изображение кнопки
   }
   else if(!(x >= buttonbounds[3][0] && x <= buttonbounds[3][2] && y >= buttonbounds[3][1] && y <= buttonbounds[3][3]) && state[3] == 1){
      state[1] = 0; // Меняем состояние нажатия кнопки
      putimage(buttonbounds[3][0], buttonbounds[3][1], images[11], COPY_PUT); // Вставляем изображение кнопки
   }
   if(x >= buttonbounds[0][0] && x <= buttonbounds[0][2] && y >= buttonbounds[0][1] && y <= buttonbounds[0][3] && state[0] == 0){
      state[0] = 1; // Меняем состояние нажатия кнопки
      putimage(buttonbounds[0][0], buttonbounds[0][1], images[9], COPY_PUT); // Вставляем изображение кнопки
   }
   else if(!(x >= buttonbounds[0][0] && x <= buttonbounds[0][2] && y >= buttonbounds[0][1] && y <= buttonbounds[0][3]) && state[0] == 1){
      state[0] = 0; // Меняем состояние нажатия кнопки
      putimage(buttonbounds[0][0], buttonbounds[0][1], images[8], COPY_PUT); // Вставляем изображение кнопки
   }
   swapbuffers();
   return true;
} // Настройки(При начале новой игры)

void NewGame(){
   for(int i = 0; i < 20; i++) // Заполняем массив Pole случайными цветами из вектора colors
      for(int j = 0; j < 32; j++)
         Pole[i][j] = colors[dis(gen)];
   shuffle(colors.begin(), colors.end(), gen);
   copy(colors.begin(), colors.begin() + 7, PoleColor); // Заполняем массив PoleFirst случайными цветами
   while(Pole[0][0] == Pole[19][31]) Pole[19][31] = colors[dis(gen)]; // Чтобы начальные цвета игроков не совпали
   for(int i = 0; i < 20; i++) // Заполняем массив флагов PoleFlags значением '0'
      for(int j = 0; j < 32; j++)
         PoleFlags[i][j] = 0;
   PoleFlags[0][0] = 1, PoleFlags[19][31] = 2; // Задаём принадлежность верхней левой и правой нижней клетки соответствующим игрокам
   Score1 = 1, Score2 = 1; // Задаём значения счёта игроков
   Pointer1 = 0, Pointer2 = 0; // Задаём значения указателей на массивы цветов первого и второго игрока
   last1 = Pole[0][0], last2 = Pole[19][31];
   gamer = 1; // Ход первого игрока
   FlagGamer[0] = 1, FlagGamer[1] = 1; // Задаём изначальный режим игры(игра двух людей)
   while(true){
      bool f = Settings(); // Настройки(При начале новой игры)
      if(!f){ // Если нажата кнопка кнопка выход в настройках
         clearviewport();
         DrawMenu([](){return new int[4]{0, 0, 0, 0};}()); // Отрисовка главного меню
         swapbuffers();
         break;
      }
      delay(30);
   }
} // Новая игра

bool DrawField(){
   clearviewport();
   static int state = 0; // Начальное состояние
   int buttonbounds[4] = {718, 10, 790, 32}; // Граница кнопки "Выход"
   int x = mousex(), y = mousey(); // Получаем координаты указателя мыши
   char str1[4], str2[4]; // Массивы символов для записи счёта первого и второго игрока
   sprintf(str1, "%03d", Score1), sprintf(str2, "%03d", Score2); // Заполняем строки счетами игроков
   if(state) putimage(buttonbounds[0], buttonbounds[1], images[9], COPY_PUT); // Кнопка "Выход"
   else putimage(buttonbounds[0], buttonbounds[1], images[8], COPY_PUT); // Кнопка "Выход"
   for(int i = 0; i < 7; i++){ // Отрисовываем поле первого игрока
      int color = PoleColor[i];
      int x = i * 20 + 80, y = 60;
      if(color == Pole[19][31] or color == Pole[0][0]){ // Если клетка не может быть выбрана игроком - рисуем её в крапинку
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
   for(int i = 0; i < 20; i++){ // Отрисовываем основное поле
      for(int j = 0; j < 32; j++){
         int color = Pole[i][j];
         int x = j * 20 + 80, y = i * 20 + 100;
         setfillstyle(SOLID_FILL, color);
         bar(x, y, x + 18, y + 18);
      }
   }
   for(int i = 0; i < 7; i++){ // Отрисовываем поле второго игрока
      int color = PoleColor[i];
      int x = i * 20 + 580, y = 520;
      if(color == Pole[19][31] or color == Pole[0][0]){ // Если клетка не может быть выбрана игроком - рисуем её в крапинку
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
   outtextxy(228, 75, str1), outtextxy(525, 535, str2); // Отрисовываем счета игроков
   setfillstyle(SOLID_FILL, WHITE);
   Triangle(83 + Pointer1 * 20, 92); // Отрисовываем треугольник(указатель), который показывает, какой цвет сейчас выбран у первого игрока
   Triangle(583 + Pointer2 * 20, 507); // Отрисовываем треугольник(указатель), который показывает, какой цвет сейчас выбран у второго игрока
   if(ismouseclick(WM_LBUTTONDOWN)){ // Проверяем, было ли произведено нажатие на ЛКМ
      getmouseclick(WM_LBUTTONDOWN, x, y); // Запоминаем координаты места нажатия ЛКМ
      if(x >= buttonbounds[0] && x <= buttonbounds[2] && y >= buttonbounds[1] && y <= buttonbounds[3]) // Если нажата кнопка "Выход"
         return false;
   }
   if(x >= buttonbounds[0] && x <= buttonbounds[2] && y >= buttonbounds[1] && y <= buttonbounds[3] && state == 0){
      state = 1; // Меняем состояние нажатия кнопки
      putimage(buttonbounds[0], buttonbounds[1], images[9], COPY_PUT); // Вставляем изображение кнопки
   }
   else if(!(x >= buttonbounds[0] && x <= buttonbounds[2] && y >= buttonbounds[1] && y <= buttonbounds[3]) && state == 1){
      state = 0; // Меняем состояние нажатия кнопки
      putimage(buttonbounds[0], buttonbounds[1], images[8], COPY_PUT); // Вставляем изображение кнопки
   }
   swapbuffers();
   return true;
} // Отрисовка поля

void Rules(){
   int xs=30, ys=190; //координаты вывода строк из файла
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
   outtextxy(400, 550, "Для выхода в меню нажмите (Esc)");
   swapbuffers();
   while(1){
      if(kbhit()){
         int p=getch();
         if(p==KEY_ESC) return;
      }
   }
}

void About(){
   int x0=510, y0=200; //координаты вывода первой строчки информации о программе
   int d=20; //расстояние между строками
   clearviewport();
   setcolor(WHITE);
   settextstyle(TRIPLEX_FONT, 0, USER_CHAR_SIZE);
   settextjustify(CENTER_TEXT, CENTER_TEXT);
   setusercharsize(2, 5, 16, 25);
   outtextxy(400, 550, "Для выхода в меню нажмите (Esc)");
   setusercharsize(3, 4, 6, 5);
   outtextxy(400, 50, "О программе");
   outtextxy(400, 100, "Игра ''Square Head''");
   settextjustify(LEFT_TEXT, CENTER_TEXT);
   setusercharsize(3, 8, 3, 5);
   outtextxy(x0, y0, "Кафедра прикладной математики");
   outtextxy(x0, y0+d, "и программирования");
   outtextxy(x0, y0+d*2, "Дисциплина: ''Учебная практика''");
   y0+=20;
   outtextxy(x0, y0+d*3, "Руководитель: Демидов А.К.");
   y0+=20;
   outtextxy(x0, y0+d*4, "Авторы:");
   outtextxy(x0, y0+d*5, "студент группы ЕТ-112");
   outtextxy(x0, y0+d*6, "Батыршин Р.Г.");
   outtextxy(x0, y0+d*7, "студент группы ЕТ-112");
   outtextxy(x0, y0+d*8, "Голиков К.В.");
   outtextxy(x0, y0+d*9, "студент группы ЕТ-112");
   outtextxy(x0, y0+d*10, "Дорофеев А.С.");
   outtextxy(x0, y0+d*11, "студент группы ЕТ-112");
   outtextxy(x0, y0+d*12, "Сурков В.А.");
   swapbuffers();
   while(1){
      if(kbhit()){
         int p=getch(1);
         if(p==KEY_ESC) return;
      }
   }
}