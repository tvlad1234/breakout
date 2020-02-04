#include <Arduino.h>
#include <Arduboy2.h>

Arduboy2 arduboy;

const int top = 0;
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
const int xbn = 8;
const int ybn= 4;

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

void initBlocks()
{
  for(int i=0;i<ybn;i++)
    for(int j=0;j<xbn;j++)
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
  for(i=0;i<ybn;i++)
    for(j=0;j<xbn;j++)
    {
      xcoord[i][j] = xblock+j*(xbsize+xbdist);
      ycoord[i][j] = yblock+i*(ybsize+ybdist);
    }
        

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

  ///desenaza blocuri si detecteaza coliziuni
  for(i=0;i<ybn;i++)
    for(j=0;j<xbn;j++)
      if(blocks[i][j])
      {
        if((xball>xcoord[i][j] && xball<xcoord[i][j]+xbsize+ballsize) && (yball>ycoord[i][j]&& yball<ycoord[i][j]+ybsize) &&ymov)
        {
          breakBlock();
          if(yball>ycoord[i][j])
            ymov=-1;
          else if(yball<ycoord[i][j]+ybsize-ballsize)
            ymov=1;
        }
          
        arduboy.fillRoundRect(xcoord[i][j],ycoord[i][j],xbsize,ybsize,1);
      }
        
  if(score==ybn*xbn)
  {
    arduboy.clear();
    arduboy.setCursor(16,12);
    arduboy.setTextSize(2);
    arduboy.print("You Won!");
    arduboy.display();
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
  if(xball==left+(ballsize+1))
    xmov=1;
    
    else if(xball==right-(ballsize+1))
      xmov=-1;

  if(yball==top+(ballsize+1))
    ymov=-1;
    else if(yball==bot-(ballsize+1) && ymov)
      {
        ymov=1;
        if((xball<xpaddle || xball>xpaddle+paddlesize) && ymov)
        {
          lifes--;
          ymov=0;
        }
          
      }
      
  if(lifes<0)
  {
    arduboy.clear();
    arduboy.setCursor(7,12);
    arduboy.setTextSize(2);
    arduboy.print("GAME OVER");
    arduboy.display();
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