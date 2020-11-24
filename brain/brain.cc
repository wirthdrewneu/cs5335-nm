
#include <iostream>
#include <math.h>
#include <string>
#include <unistd.h>
#include <iterator> 
#include <map> 
#include "robot.hh"
extern "C"{
#include "gfx.c"
}

using std::cout;
using std::endl;
using namespace std;

map<pair<int, int>,int>occgrid;

int maxsound = 0;
bool turning = false ;
string lastdir = "forward";
int lastsound = 0;
double pos_t;
int stateCount;
double pos_x;
double pos_y;
void  mapviz(int x, int y, int sound ){
cout << "Map Viz "<<y<< "," <<x << endl;
gfx_color(0 , 255  /*sound*4.25*/ , 0);
for(int t = x*10-5; t <= x*10+5; t++ ){ 
for(int z = y*10-5; z<= y*10+5; z++ ){
gfx_point(z+200,t+200);
cout << "Map Viz "<<y<< "," <<x << endl;
/*sleep(.25);*/
}}
}
int tcounter = 0;

void
callback(Robot* robot)
{
//	robot->set_vel(4.0, 4.0);

    /*cout << robot->get_line_status() << endl;

    cout << robot->get_noise_sensor() << endl;

    cout << robot->get_robot_theta() << endl;
*/
    // robot->set_vel(-5.0, 5.0)

/*    stateCount++;
    std::cout << "state count: " << stateCount << '\n';
    if(stateCount < 3){
         robot->set_vel(1.5, 1.5);
    } else if(stateCount < 6){
      robot->set_vel(-1.5, -1.5);
    } else if(stateCount > 6 ){
      stateCount = 0;
      robot->set_vel(-1.5, 1.5);
    }*/
//cout << robot->get_noise_sensor() <<" "  << robot->get_robot_theta() << " "<< lastdir <<  endl;
pos_t = robot->get_robot_theta();
pos_x = robot->get_robot_x();
pos_y = robot->get_robot_y();
cout << pos_x <<" "  << pos_y << " xy working?  "<< lastdir <<  endl;
//mapviz (round(pos_x),round(pos_y));  

int intx = round(pos_x);
int inty = round(pos_y); 	
int currentsound = robot->get_noise_sensor();
mapviz(round(pos_x),round(pos_y),currentsound);
occgrid.insert(make_pair(make_pair(intx,inty),currentsound));

/*if (true){

cout << " Theta  "  << robot->get_robot_theta()  <<  endl;

if ( tcounter % 2 == 0 )
{
	// robot->set_vel(0, 0);
	
		robot->set_vel(-4, 4);
tcounter++;
}
else {
 robot->set_vel(4, 4);
 tcounter++;
}
}

else*/ if (currentsound >= maxsound) {
        robot->set_vel(4, 4);
	maxsound= currentsound;
}

else if (currentsound < maxsound  && lastdir == "forward" && lastsound != currentsound  ) {
	turning =  true ;
	if(abs(pos_t) > 2.5){
	robot->set_vel(4, 4);
	lastdir = "backward";
	lastsound= currentsound;	
	}
			else{
	robot->set_vel(-4, 4);}

	}
else if(currentsound <= maxsound && lastdir == "backward" &&  lastsound != currentsound ) {
         if(pos_t > 1.25 && pos_t < 1.75) {
          robot->set_vel(4, 4);
          lastdir = "left";           
          lastsound= currentsound;

	 }
                          else{
          robot->set_vel(-4, 4);}
          
  
	}
  else if(currentsound <= maxsound && lastdir == "left" && lastsound != currentsound) {
           if(pos_t < -1.25 && pos_t > -1.75) {
            robot->set_vel(4, 4);
            lastdir = "right";          
            lastsound= currentsound;
 
	   }
                            else{
            robot->set_vel(-4, 4);}                
          }
else if (currentsound < maxsound  && lastdir == "right" && lastsound != currentsound ) {
        turning =  true ;
        if(abs(pos_t) < 1){
        robot->set_vel(4, 4);
        lastdir = "backward";
        lastsound= currentsound;

	}
                        else{
        robot->set_vel(-4, 4);}

        }
else {
        robot->set_vel(4, 4);
        
}
    return;
}

int
main(int argc, char* argv[])
{
    stateCount = 0;
    Robot* robot = 0;

    std::string bname(basename(argv[0]));
    std::cout << "bin: [" << bname << "]" << endl;
    int ysize = 1000;
    int xsize = 1000;
    char c;
    
    gfx_open(xsize,ysize,"Example Graphics Program");
    
    if(bname == "gz_brain") {
      std::cout << "making robot: Gazebo mode" << '\n';
      robot = new GzRobot(argc, argv, callback);
    }

    if(bname == "rg_brain") {
      std::cout << "making robot: Ranger mode" << '\n';
      robot = new RgRobot(argc, argv, callback);
    }

	


    sleep(2);

    robot->do_stuff();
    while(1) {
                // Wait for the user to press a character.
        c = gfx_wait();

                // Quit if it is the letter q.
       if(c=='q') break;}
    delete robot;
    return 0;
}
