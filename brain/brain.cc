
#include <iostream>
#include <math.h>
#include <string>
#include <unistd.h>

#include "robot.hh"

using std::cout;
using std::endl;
using namespace std;

int maxsound = 0;
bool turning = false ;
string lastdir = "forward";
int lastsound = 0;
float pos_t;
int stateCount;

void
callback(Robot* robot)
{
    /*cout << robot->get_line_status() << endl;

    cout << robot->get_noise_sensor() << endl;

    cout << robot->get_robot_theta() << endl;
*/
    // robot->set_vel(-5.0, 5.0)

    /*stateCount++;
    std::cout << "state count: " << stateCount << '\n';
    if(stateCount < 3){
         robot->set_vel(1.5, 1.5);
    } else if(stateCount < 6){
      robot->set_vel(-1.5, -1.5);
    } else if(stateCount > 6 ){
      stateCount = 0;
      robot->set_vel(-1.5, 1.5);
    }*/
cout << robot->get_noise_sensor() <<" "  << robot->get_robot_theta() << " "<< lastdir <<  endl;
pos_t = robot->get_robot_theta();
  

int currentsound = 0;


if (currentsound >= maxsound) {
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
	robot->set_vel(-2, 2);}

	}
else if(currentsound <= maxsound && lastdir == "backward" &&  lastsound != currentsound ) {
         if(pos_t > 1.25 && pos_t < 1.75) {
          robot->set_vel(4, 4);
          lastdir = "left";           
          lastsound= currentsound;

	 }
                          else{
          robot->set_vel(-2, 2);}
          
  
	}
  else if(currentsound <= maxsound && lastdir == "left" && lastsound != currentsound) {
           if(pos_t < -1.25 && pos_t > -1.75) {
            robot->set_vel(4, 4);
            lastdir = "right";          
            lastsound= currentsound;
 
	   }
                            else{
            robot->set_vel(-2, 2);}                
          }
else if (currentsound < maxsound  && lastdir == "right" && lastsound != currentsound ) {
        turning =  true ;
        if(abs(pos_t) < 1){
        robot->set_vel(4, 4);
        lastdir = "backward";
        lastsound= currentsound;

	}
                        else{
        robot->set_vel(-2, 2);}

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
    delete robot;
    return 0;
}
