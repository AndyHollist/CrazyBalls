#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "instinct2/instinct/include/instinct/instinct.h"
         
using namespace std;

//Crazy Balls!
/*Description:
		A game where the player controls a paddle at the bottom of the screen. 
		You must try and catch the blue balls for 50 points but avoid the 
		red balls which will give you a -25 point penalty.

		You must have the instinct graphics package in the same directory
		in order to compile this main.cpp.
*/

const int WINDOW_SIZE = 1000;
const int LEFTBOUND = 200;
const int RIGHTBOUND = 800;
const int LOWBOUND = 200;
const int TOPBOUND = 800;
const double GRAVITY = .5;
const int PADDLE_SPEED = 20;
const int TIMELIMIT = 50000;//ms
const int WINDOW_PAUSE = 25;
const int BALL_SIZE = 15;
const int BALL_SPEED = 20;


struct Ball
{
  bool is_alive;
  bool is_blue;
  double x_pos;
  double y_pos;
  double x_vel;
  double y_vel;
  
  Ball()
  :is_alive(true)
  {
  }
};

struct Paddle
{
  int x_pos;
  int y_pos;
  int length;
  
  Paddle()
  :x_pos(0),y_pos(0),length(1)
  {
  }
};

//create a new ball
void spawn_ball( vector<Ball>& v, int init_y, int left, int right)
{
  Ball* b_ptr = new Ball();
  srand(time(NULL));
  b_ptr->is_blue = rand()%2;
  b_ptr->y_pos = init_y;
  b_ptr->x_pos = rand()%(right - left) + left;
  b_ptr->y_vel = -(rand()%(BALL_SPEED / 2)+1);
  b_ptr->x_vel = rand()%(BALL_SPEED * 3)- BALL_SPEED * (3/2.0);
  
  //search for first available slot
  for( unsigned i = 0; i < v.size(); ++i)
  {
    if( !(v[i].is_alive) )
    {
			Ball ball = *b_ptr;
      v[i] = ball;
      delete b_ptr;
      return;
    }
  }
  
  v.push_back(*b_ptr);
  delete b_ptr;
}

//move all balls to their next position using their velocities
void next_step( vector<Ball>& v, Paddle& p)
{
  for( int i = 0; i < v.size(); ++i)
  {
    if( v[i].is_alive)
    {
      v[i].x_pos += v[i].x_vel;
      v[i].y_pos += v[i].y_vel;
      v[i].y_vel -= GRAVITY;
    }
  }
  if( win.is_key_down("left") && !win.is_key_down("right"))
  {
    p.x_pos -= PADDLE_SPEED;
  }
  if( win.is_key_down("right") && !win.is_key_down("left"))
  {
    p.x_pos += PADDLE_SPEED;
  }
}

//check if any balls collide with walls, ground, or paddle
void check_collision( vector<Ball>& v, const Paddle& p, int left, 
  int right, int top, int bottom, int& score)
{
  for( int i = 0; i < v.size(); ++i)
  {
    if( v[i].is_alive)
    {
      //check sides
      if( v[i].x_pos >= right || v[i].x_pos <= left)
        v[i].x_vel = -(v[i].x_vel);
      //check paddle
      if( v[i].y_pos <= bottom)
      {
        
        if( v[i].x_pos >= (p.x_pos -.5) && v[i].x_pos <= (p.x_pos + p.length + .5) )
        {
          if( v[i].is_blue) score += 50;
          else score -= 25;
					v[i].is_alive = false;
        }
      }
			//check if past the paddle
		  if( v[i].y_pos < bottom - 5)
			{
				v[i].is_alive = false;
			}
    }
  }
}

void draw_frame( const vector<Ball>&v, const Paddle& p,const Timer& t,
  const int& score)
{
  win.clear_buffer();
  //draw balls
  for( int i = 0; i < v.size(); ++i)
  {
    if( v[i].is_alive)
    {
      Circle c( Point(v[i].x_pos, v[i].y_pos),BALL_SIZE);
      if( v[i].is_blue) c.set_color(BLUE);
      else c.set_color(RED);
      c.set_filled_in("is filled");
      win << c;
    }
  }
  //draw lines
  win << Line(Point(LEFTBOUND,LOWBOUND), Point(LEFTBOUND,TOPBOUND))
      << Line( Point(RIGHTBOUND,LOWBOUND), Point(RIGHTBOUND,TOPBOUND));
  //draw paddle
  win << Rect(Point(p.x_pos,p.y_pos), p.length, 1);
  //draw timer
  win << Message( Point( RIGHTBOUND+LEFTBOUND / 2 ,TOPBOUND + 1), 
          t.get_time());
  //draw score
  win << Message( Point( (RIGHTBOUND+LEFTBOUND) / 2, LOWBOUND - 100),
          score);
  win.output_buffer();
}

void run_game()
{
  win.set_title("Crazy Balls!");
  win.coord( 0,0,WINDOW_SIZE,WINDOW_SIZE);
  Timer t;
  t.start();
  int score = 0;
  vector<Ball> v;
  Paddle p; 
  p.x_pos = (RIGHTBOUND + LEFTBOUND) / 2;
  p.y_pos = (LOWBOUND);
  p.length = 50;
  int count = 0;
  
  spawn_ball( v, TOPBOUND, LEFTBOUND, RIGHTBOUND);
  while(!t.has_passed(TIMELIMIT))
  {
    draw_frame( v, p, t, score);
    next_step( v, p);
    if( count%30 == 0)  spawn_ball(v, TOPBOUND, LEFTBOUND, RIGHTBOUND);
    check_collision(v, p, LEFTBOUND, RIGHTBOUND, TOPBOUND, LOWBOUND, score);
    count++;
    wait_for(WINDOW_PAUSE);
  }
}


int instinct_main()
{
  cout << "begin" << endl;
  run_game();
  
  
  return 0;
}
