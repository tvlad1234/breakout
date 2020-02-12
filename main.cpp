/*
Breakout for Arduboy
Vlad Tomoiaga
February 2020
 */

#include <Arduino.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

const int top = 4;
const int bot = 64;
const int left = 0;
const int right = 128;

bool blocks[4][8];

int xcoord[4][8];
int ycoord[4][8];

const int xblock = 4;
const int yblock = 8;
const int xbsize = 12;
const int ybsize = 4;
const int xbdist = 3;
const int ybdist = 3;

const int ballsize = 2;
int xball = 64;
int yball = bot-(ballsize+1);
int ymov = 0; //poz sus, neg jos
int xmov = 1; //pos st, neg dr
int xpaddle = 64;
const int paddlesize = 23;
const int paddlespeed = 2;

int score = 0;
int lifes = 3;

int i,j;

void soundEdge()
{
  sound.tone(5000, 5);
}

void soundBlock()
{
  sound.tone(3100, 9);
}

void gameOverSound()
{
  for(int freq=7000;freq>=200;freq-=50)
  {
    sound.tone(freq, 10);
    delay(10);
  }
}

void winSound()
{
  for(int freq=200;freq<=7000;freq+=50)
  {
    sound.tone(freq, 10);
    delay(10);
  }
}

void initBlocks()
{
  for(int i=0;i<xblock;i++)
    for(int j=0;j<yblock;j++)
      blocks[i][j]=1;
}

void breakBlock(){
  blocks[i][j]=0;
  score++;
}

void setup(){
  arduboy.begin();
  arduboy.setFrameRate(60);
  initBlocks();
  for(i=0;i<xblock;i++)
    for(j=0;j<yblock;j++)
    {
      xcoord[i][j] = xblock+j*(xbsize+xbdist);
      ycoord[i][j] = yblock+i*(ybsize+ybdist);
    }
  arduboy.clear();
  arduboy.setCursor(0,16);
  arduboy.setTextSize(2);
  arduboy.print("Breakout");
  arduboy.setTextSize(1);
  arduboy.setCursor(32,32);
  arduboy.println("by Vlad Tomoiaga");
  arduboy.setCursor(36,40);
  arduboy.println("press A");
  arduboy.display();
  while(!arduboy.pressed(A_BUTTON));
  delay(250);

        

}

void loop(){
  if (!arduboy.nextFrame()) {
    return;  //proper frame timing
  }
  arduboy.clear();
  //deseneaza bila
  if(ymov)
    arduboy.fillCircle(xball, yball, ballsize);
  else arduboy.fillCircle(xpaddle+10,59,ballsize);

  //xpaddle=xball; //asta face jocul sa se joace singur

  ///deseneaza blocuri si detecteaza coliziuni
  for(i=0;i<xblock;i++)
    for(j=0;j<yblock;j++)
      if(blocks[i][j])
      {
        if((xball>xcoord[i][j]-2 && xball<xcoord[i][j]+xbsize+ballsize+2) && ymov)
        {
          if(yball==ycoord[i][j])
          {
            ymov=1;
            breakBlock();
            soundBlock();
          }
          else if(yball==ycoord[i][j]+ybsize-ballsize)
          {
            ymov=-1;
            breakBlock();
            soundBlock();
          }
        }
          
        arduboy.drawRoundRect(xcoord[i][j],ycoord[i][j],xbsize,ybsize,1);
      }
        
  if(score==xblock*yblock)
  {
    arduboy.clear();
    arduboy.setCursor(16,12);
    arduboy.setTextSize(2);
    arduboy.print("You Won!");
    arduboy.display();
    winSound();
    arduboy.setTextSize(1);
    score=0;
    lifes=3;
    initBlocks();
    ymov=0;
    yball=bot-(ballsize+1);
    delay(2000);
  }
  
  //paleta
  arduboy.drawRect(xpaddle,62,paddlesize,2);

  //scor
  arduboy.setCursor(70,0);
  arduboy.print("Score:");
  arduboy.print(score);
  arduboy.setCursor(0,0);
  arduboy.print("Lives:");
  arduboy.print(lifes);

  ///controale
  if(arduboy.pressed(LEFT_BUTTON) && (xpaddle>0))
    xpaddle-=paddlespeed;
  if(arduboy.pressed(RIGHT_BUTTON) && (xpaddle<128-paddlesize))
    xpaddle+=paddlespeed;
  if(arduboy.pressed(A_BUTTON)&&!ymov)
  {
    ymov=1;
    yball--;
    xball=xpaddle+7;
  }

  //pauza
  if(arduboy.pressed(B_BUTTON))
  {
    delay(300);
    arduboy.setCursor(26,40);
    arduboy.setTextSize(2);
    arduboy.print("Paused");
    arduboy.display();
    arduboy.setTextSize(1);
    while(!arduboy.pressed(B_BUTTON));
    delay(300);

  }


  ///detectarea coliziunilor cu peretii/paleta
  if(xball==left+(ballsize+1)&&ymov)
  {
    xmov=1;
    soundEdge();
  }
    
  else if(xball==right-(ballsize+1)&& ymov)
  {
    xmov=-1;
    soundEdge();
  } 
  if(yball<=top+(ballsize+1)&&ymov)
  {
    ymov=-1;
    soundEdge();
  }
    
    else if(yball>=bot-(ballsize+1) && ymov)
      {
        ymov=1;
        
        soundEdge();
        if((xball<xpaddle-5 || xball>xpaddle+paddlesize+5) && ymov)
        {
          delay(10);
          sound.tone(1000, 50);
          lifes--;
          ymov=0;
          //fa mingea sa clipeasca atunci cand nu nimereste paleta
          arduboy.fillCircle(xball, yball, ballsize, BLACK);
          arduboy.drawCircle(xball, yball, ballsize);
          arduboy.display();
          delay(400);
          arduboy.fillCircle(xball, yball, ballsize, BLACK);
          arduboy.display();
          delay(400);
        
        }
      }
      
  if(lifes<0)
  {
    arduboy.clear();
    arduboy.setCursor(7,12);
    arduboy.setTextSize(2);
    arduboy.print("GAME OVER");
    arduboy.display();
    gameOverSound();
    arduboy.setTextSize(1);
    score=0;
    lifes=3;
    initBlocks();
    yball=bot-(ballsize+1);
    ymov=0;
    delay(2000);
  }
      
  ///miscarea bilei
  if(xmov>0)
    xball++;
    else if(xmov<0)
      xball--;

  if(ymov>0)
    yball--;
    else if(ymov<0)
      yball++;


  arduboy.display();

}