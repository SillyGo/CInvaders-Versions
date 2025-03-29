#include <SPI.h>
#include <TFT.h>         

#define cs   10
#define dc   9
#define rst  8

//this is some of the ugliest code i think i have ever written, i i've written a LOT of bad code
typedef struct { float x; float y; } bullet;
typedef struct { float x; float y; } alien;
typedef struct { float x; float y; float xl; float yl; } player;
int bullet_reps = 0;
int alien_count = 24;
alien aliens[24];
int bullet_counter = 0;
int botao_esquerda = 2;
int botao_direita = 3;
int gun_trigger = 7;
bullet balas[40];
float controls = 0;

TFT screen = TFT(cs, dc, rst);
int game_clock_ms = 1;
int bullet_clock = 0;
bool update_bullet_clock = false;
bool dont_c = false;
player player1;
bool update_y = false;
int x_dir = 1;

bullet En_bullets[100];
int En_bullet_count = 0;

//-------------------------------------------------------------------------------------------------------------------------------//
void draw_explosion(int x, int y)
{
  screen.stroke(0,0,255);
  screen.point(x-2,y-2); screen.point(x-1,y-2); screen.point(x,y-2); screen.point(x+1, y-2); screen.point(x+2, y-2);
  screen.point(x-2,y-1); screen.point(x-1,y-1); screen.point(x,y-1); screen.point(x+1, y-1); screen.point(x+2, y-1);
  screen.point(x-2,y-0); screen.point(x-1,y-0); screen.point(x,y-0); screen.point(x+1, y-0); screen.point(x+2, y-0);
  screen.point(x-2,y+1); screen.point(x-1,y+1); screen.point(x,y+1); screen.point(x+1, y+1); screen.point(x+2, y+1);
  screen.point(x-2,y+2); screen.point(x-1,y+2); screen.point(x,y+2); screen.point(x+1, y+2); screen.point(x+2, y+2);
}

//-------------------------------------------------------------------------------------------------------------------------------//

void delete_explosion(int x, int y)
{
  screen.stroke(0,0,0);
  screen.point(x-2,y-2); screen.point(x-1,y-2); screen.point(x,y-2); screen.point(x+1, y-2); screen.point(x+2, y-2);
  screen.point(x-2,y-1); screen.point(x-1,y-1); screen.point(x,y-1); screen.point(x+1, y-1); screen.point(x+2, y-1);
  screen.point(x-2,y-0); screen.point(x-1,y-0); screen.point(x,y-0); screen.point(x+1, y-0); screen.point(x+2, y-0);
  screen.point(x-2,y+1); screen.point(x-1,y+1); screen.point(x,y+1); screen.point(x+1, y+1); screen.point(x+2, y+1);
  screen.point(x-2,y+2); screen.point(x-1,y+2); screen.point(x,y+2); screen.point(x+1, y+2); screen.point(x+2, y+2);
}

//-------------------------------------------------------------------------------------------------------------------------------//

void draw_alien1(int x, int y)
{
  screen.stroke(0,255,0);
  screen.point(x-1,y); screen.point(x+1,y);
  screen.point(x+1,y-1); screen.point(x-1,y-1); screen.point(x, y-1);
  screen.point(x+1, y-2); screen.point(x, y-2); screen.point(x-1, y-2);
  screen.point(x+2, y-3); screen.point(x, y-3); screen.point(x-2, y-3);
  screen.point(x+2, y-4); screen.point(x+1, y-4); screen.point(x, y-4); 
  screen.point(x-1, y-4); screen.point(x-2, y-4);
}

//-------------------------------------------------------------------------------------------------------------------------------//

void delete_alien1(int x, int y)
{
  screen.stroke(0,0,0);
  screen.point(x-1,y); screen.point(x+1,y);
  screen.point(x+1,y-1); screen.point(x-1,y-1); screen.point(x, y-1);
  screen.point(x+1, y-2); screen.point(x, y-2); screen.point(x-1, y-2);
  screen.point(x+2, y-3); screen.point(x, y-3); screen.point(x-2, y-3);
  screen.point(x+2, y-4); screen.point(x+1, y-4); screen.point(x, y-4); 
  screen.point(x-1, y-4); screen.point(x-2, y-4);
}

//-------------------------------------------------------------------------------------------------------------------------------//

void draw_bullet(int x, int y)
{
  screen.stroke(255,255,0);
  screen.point(x,y); screen.point(x,y+1);
  screen.point(x+1, y); screen.point(x+1, y+1);
}

//-------------------------------------------------------------------------------------------------------------------------------//

void delete_bullet(int x, int y)
{
  screen.stroke(0,0,0);
  screen.point(x,y); screen.point(x,y+1);
  screen.point(x+1, y); screen.point(x+1, y+1);
}

//-------------------------------------------------------------------------------------------------------------------------------//

void draw_player(int x, int y)
{
  screen.stroke(255,255,255);
  screen.point(x,y);
  screen.point(x-1,y+1); screen.point(x, y+1); screen.point(x+1, y+1);
  screen.point(x-2, y+2); screen.point(x-1, y+2); screen.point(x, y+2); screen.point(x+1, y+2); screen.point(x+2, y+2);
  screen.point(x-2, y+3); screen.point(x-1, y+3); screen.point(x, y+3); screen.point(x+1, y+3); screen.point(x+2, y+3); screen.point(x-3, y+3); screen.point(x+3, y+3);
  screen.point(x-2, y+4); screen.point(x-1, y+4); screen.point(x, y+4); screen.point(x+1, y+4); screen.point(x+2, y+4); screen.point(x-3, y+4); screen.point(x+3, y+4);
  screen.point(x-2, y+4); screen.point(x-1, y+4); screen.point(x, y+4); screen.point(x+1, y+4); screen.point(x+2, y+4); screen.point(x-3, y+4); screen.point(x+3, y+4);
}

//-------------------------------------------------------------------------------------------------------------------------------//

void delete_player(int x, int y)
{
  screen.stroke(0,0,0);
  screen.point(x,y);
  screen.point(x-1,y+1); screen.point(x, y+1); screen.point(x+1, y+1);
  screen.point(x-2, y+2); screen.point(x-1, y+2); screen.point(x, y+2); screen.point(x+1, y+2); screen.point(x+2, y+2);
  screen.point(x-2, y+3); screen.point(x-1, y+3); screen.point(x, y+3); screen.point(x+1, y+3); screen.point(x+2, y+3); screen.point(x-3, y+3); screen.point(x+3, y+3);
  screen.point(x-2, y+4); screen.point(x-1, y+4); screen.point(x, y+4); screen.point(x+1, y+4); screen.point(x+2, y+4); screen.point(x-3, y+4); screen.point(x+3, y+4);
  screen.point(x-2, y+4); screen.point(x-1, y+4); screen.point(x, y+4); screen.point(x+1, y+4); screen.point(x+2, y+4); screen.point(x-3, y+4); screen.point(x+3, y+4);
}

//-------------------------------------------------------------------------------------------------------------------------------//
//SETUP//

void setup() 
{
  player1.x = screen.width()/2; 
  player1.y = screen.height() - 15;
  player1.xl = player1.x; 
  player1.yl = player1.y;
  for (int i = 0; i < alien_count; i++)
  {
    aliens[i].x = ((screen.width()/8)*(i%8)) + 12;
    aliens[i].y = (int(i/8))*12 + 12;
  }
  Serial.begin(9600);
  screen.begin();
  screen.background(0,0,0);
}

//-------------------------------------------------------------------------------------------------------------------------------//
//LOOP / GAME LOOP//

void loop() 
{
  int random_index = random(0, alien_count);
  if (game_clock_ms%50==0)
  {
    float random_x = aliens[random_index].x;
    float random_y = aliens[random_index].y;
    En_bullets[En_bullet_count] = {random_x, random_y};
    En_bullet_count = En_bullet_count + 1;
  }
  for (int i = 0; i < En_bullet_count; i++)
  {
    delete_bullet(En_bullets[i].x, En_bullets[i].y);
    En_bullets[i].y = En_bullets[i].y + 2;
    draw_bullet(En_bullets[i].x, En_bullets[i].y);
    if ((En_bullets[i].x <= player1.x + 3 && En_bullets[i].x >= player1.x - 3) && (En_bullets[i].y <= player1.y + 3 && En_bullets[i].y >= player1.y - 5))
    {
      Serial.println(player1.x);
      Serial.println(En_bullets[i].x);
      while (1);
    }
  }
  if (En_bullet_count >= 100)
  {
    En_bullet_count = 0;
  }
  if (alien_count == 0)
  {
    for (int i = 0; i < screen.width(); i++)
    {
      for (int j = 0; j < screen.height(); j++)
      {
        screen.stroke(255,255,255);
        screen.point(i, j);
      }
    }
  }
  if (controls > screen.width()/2 || controls < -screen.width()/2)
  {
    controls  = 0;
  }
  if (dont_c)
  {
    bullet_reps = 40;
  }
  else
  {
    bullet_reps = bullet_counter;
  }
  float condensed_aCount = alien_count/24;
  if (bullet_clock >= 20 + 40*condensed_aCount)
  {
    update_bullet_clock = false;
    bullet_clock = 0;
  }
  if (bullet_counter >= 40)
  {
    dont_c = true;
    bullet_counter = 0;
  }
  if (digitalRead(gun_trigger)== 1 && bullet_clock == 0)
  {
    balas[bullet_counter] = {screen.width()/2 + controls ,screen.height() - 15};
    bullet_counter = bullet_counter + 1;
    update_bullet_clock = true;
  }
  int counter = 0;
  if (game_clock_ms%1==0)
  {
    while (counter < alien_count)
    {
      if (aliens[counter].x < 0)
      {
        aliens[counter].x = 0;
        update_y = true;
        x_dir = 1;
        break;
      }
      if (aliens[counter].x > screen.width())
      {
        aliens[counter].x = screen.width();
        update_y = true;
        x_dir = -1;
        break;
      }
      counter = counter + 1;
    }
  }
  counter = 0;
  if (game_clock_ms%2==0)
  {
    for (int i = 0; i < alien_count; i++)
    {
      if (update_y)
      {
        delete_alien1(aliens[i].x, aliens[i].y);
        aliens[i].y = aliens[i].y + 5;
        draw_alien1(aliens[i].x, aliens[i].y);
      }
      else
      {
        delete_alien1(aliens[i].x, aliens[i].y - 5);
        delete_alien1(aliens[i].x, aliens[i].y);
        aliens[i].x = aliens[i].x + 1.0*(x_dir);
        draw_alien1(aliens[i].x, aliens[i].y);
      }
      for (int j = 0; j < bullet_reps; j++)
      {
        delete_bullet(balas[j].x, balas[j].y);
        balas[j].y = balas[j].y - 1;
        draw_bullet(balas[j].x, balas[j].y);
        if ((aliens[i].x <= balas[j].x + 6 && aliens[i].x  >= balas[j].x - 6) && (aliens[i].y <= balas[j].y + 10 && aliens[i].y >= balas[j].y - 8))
        {
          draw_explosion(aliens[i].x, aliens[i].y);
          delete_alien1(aliens[i].x, aliens[i].y);
          delay(100);
          delete_explosion(aliens[i].x, aliens[i].y);
          delete_bullet(balas[j].x, balas[j].y);
          balas[j].x = screen.width() + 10;
          alien save1 = aliens[i];
          for (int k = i; k < alien_count; k++)
          {
            aliens[k] = aliens[k+1];
          }
          aliens[alien_count - 1] = save1;
          delete_alien1(aliens[alien_count - 1].x, aliens[alien_count - 1].y);
          alien_count = alien_count - 1;
        }
      }
    }
    update_y = false;
  }
  if (digitalRead(botao_direita) == 1 && game_clock_ms%1==0)
  {
    controls = controls + 1.8;
  }
  if (digitalRead(botao_esquerda) == 1 && game_clock_ms%1==0)
  {
    controls = controls - 1.8;
  }

  delete_player(player1.x, player1.y);
  player1.x = screen.width()/2 + controls;
  draw_player(player1.x, player1.y);

  if (update_bullet_clock)
  {
    bullet_clock = bullet_clock + 1;
  }
  game_clock_ms = game_clock_ms + 1;
}
