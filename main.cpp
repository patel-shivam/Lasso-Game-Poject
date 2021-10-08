#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace simplecpp;

//movingObject.h

#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

int coincnt=0;

class MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:

 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta)
    : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }

  double getXPos();

  double getYPos();

  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t)
{
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos()
{
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos()
{
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta)
{
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()



void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//coin.h
#ifndef __COIN_H__

#define __COIN_H__



class Coin : public MovingObject
{
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;

  // Moving parts
  Circle coin_circle;

 public:

 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta)
  {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }

  void initCoin();

  void resetCoin();

};
 // End class Coin

#endif

//lasso.h

#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960

#define WINDOW_X 1400
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 300
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 140
#define COIN_ANGLE_DEG 75

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5

class Lasso : public MovingObject
 {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  int num_coins;

  void initLasso();

  public:

 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta)
  {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }


  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  int getNumCoins() { return num_coins; }

}; // End class Lasso

#endif

//coin.h


void Coin::initCoin() {
  coin_start_x = (PLAY_X_START+WINDOW_X)/2;
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("blue"));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}


void Coin::resetCoin() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

//lasso.cpp


void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()


void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);

  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);

  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);

  lasso_circle.setColor(COLOR("red"));

  lasso_circle.setFill(true);

  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);

  lasso_loop.setColor(COLOR("green"));

  lasso_loop.setFill(true);

  addPart(&lasso_circle);

  addPart(&lasso_loop);

  lasso_looped = false;

  the_coin = NULL;

  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("brown"));

  lasso_band.setColor(COLOR("blue"));
  draw_lasso_band();

} // End Lasso::initLasso()



void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {

    the_coin->resetCoin();
  }
} // End Lasso::yank()

void Lasso::loopit()
 {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()


void Lasso::addAngle(double angle_deg)
 {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()


void Lasso::addSpeed(double speed)
{
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()


void Lasso::nextStep(double stepTime)
{
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()


void Lasso::check_for_coin(Coin *coinPtr)
 {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
     num_coins++;
     coincnt++;
    the_coin->getAttachedTo(this);
  }
} // End Lasso::check_for_coin()


    void displayinfo()
{

    cout<<"The general rules for the Lasso game are as follows:";
    cout<<"\n";
    cout<<"1. The coins in the game need to be caught using the lasso.";
    cout<<"\n";
    cout<<"2. Commands-";
    cout<<"\n";
    cout<<"\t"<<"2.1 t - Throw the lasso.";
    cout<<"\n";
    cout<<"\t";
    cout<<"2.2 y - Yank the Lasso( reset it to initial position ) ";
    cout<<"\n";
    cout<<"\t";
    cout<<"2.3 l - Loop the Lasso( good luck catching coins:) !)";
    cout<<"\n";
    cout<<"\t";
    cout<<"2.4 [ - Deacrease the Lasso angle of projection";
    cout<<"\n";
    cout<<"\t";
    cout<<"2.5 ] - Increase the Lasso angle of projection";
    cout<<"\n";
    cout<<"\t";
    cout<<"2.6 - (minus key) - Decrease the Lasso speed";
    cout<<"\n";
    cout<<"\t";
    cout<<"2.7 = (equal to sign) - Increase the Lasso speed";
    cout<<"\n";
    cout<<"\t";
    cout<<"2.8 q - End the Lasso game";
    cout<<"\n";
    cout<<"The level till which player has advanced in the game will be displayed after the game is quit.";
    cout<<"\n";
    return;

}
// function to display the rules of the game on the console input window


main_program {

  initCanvas("Lasso", WINDOW_X, WINDOW_Y);

  //displaying the word lasso as a game description
  Turtle t1,t2;
  t1.penUp();
  t2.penUp();
  Line ll1(150,50,150,100);
  ll1.imprint();
  Line ll2(150,100,168,100);
  ll2.imprint();
  //letter l displayed
  Line aa1(170,100,200,50);
  aa1.imprint();
  Line aa2(200,50,220,100);
  aa2.imprint();
  Line aa3(190,75,210,75);
  aa3.imprint();
  //letter a displayed
  t1.moveTo(240,50);
  t1.penDown();
  t1.left(180);
  //displaying the two letters s

  repeat(9)
  {
        t1.forward(4);
        t1.left(20);
  }
  repeat(9)
  {
    t1.forward(4);
    t1.right(20);
  }
  //one s letter printed

  t2.moveTo(270,50);
  t2.penDown();
  t2.left(180);

  repeat(9)
  {
        t2.forward(4);
        t2.left(20);
  }
  repeat(9)
  {
    t2.forward(4);
    t2.right(20);
  }
  //second s printed

  Circle xo(320,75,25);
  xo.imprint();
  //letter o printed

  Line sg1(150,110,150,160);
  sg1.imprint();
  Line sg2(150,110,170,110);
  sg2.imprint();
  Line sg3(150,160,170,160);
  sg3.imprint();
  Line sg4(170,160,170,135);
  sg4.imprint();
  Line sg5(160,135,170,135);
  sg5.imprint();
  //prints g

  Line sa1(175,160,185,110);
  sa1.imprint();
  Line sa2(185,110,195,160);
  sa2.imprint();
  Line sa3(180,135,190,135);
  sa3.imprint();
  //prints a

  Line sm1(200,160,200,110);
  sm1.imprint();
  Line sm2(220,160,220,110);
  sm2.imprint();
  Line sm3(210,110,210,135);
  sm3.imprint();
  Line sm4(200,110,220,110);
  sm4.imprint();
  //prints letter m

  Line se1(230,110,250,110);
  se1.imprint();
  Line se2(230,135,240,135);
  se2.imprint();
  Line se3(230,160,250,160);
  se3.imprint();
  Line se4(230,110,230,160);
  se4.imprint();
  //prints the letter e

  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;



  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());

  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);



  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit
 displayinfo();

  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;

    bool pendingEv = checkEvent(e);

    if(pendingEv)
     {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c)
      {
      case 't':
	lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
    {

	//command to exit the lasso game
	//exit also quits the canvas
	//pressing any key will further close the codeblocks console input window

	//move the turtle to starting position of the point where Game over is written
	/*purpose of using turtle - bigger fonts unlike limited font size we learnt in Text class of
	initCanvas(). more control over graphics*/

	//now printing the word game
	Line l1g(510,105,590,105);
	l1g.imprint();
	Line l2g(550,150,590,150);
	l2g.imprint();
	Line l3g(510,190,590,190);
	l3g.imprint();
	Line l4g(510,105,510,190);
	l4g.imprint();
	Line l5g(590,150,590,190);
	l5g.imprint();
	//print the letter g

	Line l1a(610,190,650,105);
	l1a.imprint();
	Line l2a(650,105,690,190);
	l2a.imprint();
	Line l3a(630,150,670,150);
	l3a.imprint();
	//prints the letter a

	Line l1m(710,190,710,105);
	l1m.imprint();
	Line l2m(750,105,750,150);
	l2m.imprint();
	Line l3m(790,105,790,190);
	l3m.imprint();
	Line l4m(710,105,790,105);
	l4m.imprint();
	//prints the letter m

	Line le1(810,105,880,105);
	le1.imprint();
	Line le2(810,150,850,150);
	le2.imprint();
	Line le3(810,190,880,190);
	le3.imprint();
	Line le4(810,190,810,105);
	le4.imprint();
	//prints the letter e

    //now printing the word over
	Circle co(550,250,50);
	co.imprint();
	//prints the letter o

	Line l1v(610,200,650,300);
	l1v.imprint();
	Line l2v(650,300,690,200);
	l2v.imprint();
	//prints the letter v

	Line l1e(710,200,780,200);
	l1e.imprint();
	Line l2e(710,250,750,250);
	l2e.imprint();
	Line l3e(710,300,780,300);
	l3e.imprint();
	Line l4e(710,200,710,300);
	l4e.imprint();
	//prints the letter e

	Line l1r(810,200,810,300);
	l1r.imprint();
	Line l2r(810,250,880,300);
	l2r.imprint();
	//now printing the circle for the letter r
	Circle cr(845,230,38);
	cr.imprint();
	//letter r printed

	//deciding levels for the game
	//to be displayed only after the player quits the game
	int level;
	if(coincnt>=0&&coincnt<3)
	level=1;
	else if(coincnt>=3&&coincnt<=5)
	level=2;
	else if(coincnt>5&&coincnt<8)
	level=3;
	else if(coincnt>=8)
	level=4;

	Text c(700,350,"Congratulations!!");

	Text l(700,370,"Your game progress level is mentioned in the codeblocks console input window.");


    cout<<endl;
    cout<<endl;
	//switch case body here
	switch(level)
	{
	case 1: cout<<"You played till level one of the game.";
            cout<<"\n";
            cout<<"Better luck next time.";
            cout<<"\n";
            break;

    case 2: cout<<"You have played till the second level of the game.";
            cout<<"\n";
            cout<<"You can do better next time :)";
            cout<<"\n";
            break;

    case 3: cout<<"You reached the third level of the Lasso game";
            cout<<"\n";
            cout<<"Just close to the best score.....";
            cout<<"\n";
            break;

    case 4: cout<<"Congratulations..";
            cout<<"\n";
            cout<<"You just won the Lasso Game!!!";
            cout<<"\n";
               break;

    default: break;

	}

	wait(5);
	//wait for the letters on the screen
	exit(0);

	}
      default:
	break;
      }
    }

    lasso.nextStep(stepTime);

    coin.nextStep(stepTime);
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin();
      last_coin_jump_end = currTime;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    stepCount++;
    currTime += stepTime;
    wait(stepTime);
  } // End for(;;)

  wait(5);
} // End main_program
