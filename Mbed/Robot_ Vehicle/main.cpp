/*
 *  Files: executable->main.cpp
 *
 *  Authors: Alfonso de la Morena and Thomas Diorio
 *  EE 3420
 *  Program: #8
 *
 */

 #include "mbed.h"
 #include "stdint.h"
// #include <iostream>
 using namespace std;
  
  Serial pc(USBTX, USBRX);
  Serial bluetooth(PTE16,PTE17);

  // Left Motor
  PwmOut motor_1_enable(PTB19);
  DigitalOut motor_1_backward(PTE3);
  DigitalOut motor_1_forward(PTE2);
  
  // Right Motor
  PwmOut motor_2_enable(PTB18);
  DigitalOut motor_2_backward(PTC7);
  DigitalOut motor_2_forward(PTC6);
  
  //Sonar
  DigitalOut distance_trigger(PTC13);
  InterruptIn distance_echo(PTC16);
  Ticker distance_start;
  Timer distance_timer;
  float distance_time, distance_inches = 10000.0f, distance_cm = 10000.0f;
  bool sonar = false;
  
  // Vehicle starts stopped
  float motor_1_duty_cycle = 0.0f;
  float motor_2_duty_cycle = 0.0f;
     
  void pulse_trigger()
  {
    distance_trigger.write(1);
    wait(10E-6);
    distance_trigger.write(0);
  }

  void echo_start()
  {
    distance_timer.reset();
  }

  void echo_end()
  {
    distance_time=distance_timer.read();
    if(distance_time < 30E-3)   // delay longer than 30 ms indicates no echo
    {
        distance_inches=distance_time*13503.9*0.5;     // speed of sound = 13503.9 inches/second and distance to object is 1/2 path length;
        distance_cm=distance_time*34300.0*0.5;     // speed of sound = 34300 cm/second and distance to object is 1/2 path length;
    }
    else
    {
        distance_inches= 10000;
        distance_cm= 10000;
    }
    pc.printf("\nDistance cms: %f\n", distance_cm);
    if(sonar && (distance_cm < 3.0f))
    {
////////////////////////////////////////////////////////////////////////////////
/// Motor 1 and 2
                 // Stop both motors
                 motor_1_enable.write(0.0f);
                 motor_2_enable.write(0.0f);
    
                 // Reset direction to starting positions
                 motor_1_forward  = 1;
                 motor_1_backward = 0;
                 motor_2_forward  = 1;
                 motor_2_backward = 0;
                 
                 // Reset speed of both motors to 0
                 motor_1_duty_cycle = 0.0f;
                 motor_2_duty_cycle = 0.0f;
    }
  }
  
 void manualCommands();
 void patternedMovement();
 void manualMovement(Serial& pc, 
                     PwmOut& motor_1_enable, 
                     DigitalOut& motor_1_backward,
                     DigitalOut& motor_1_forward,
                     PwmOut& motor_2_enable, 
                     DigitalOut& motor_2_backward,
                     DigitalOut& motor_2_forward);

 int main()
 {
     pc.baud(115200);
     bluetooth.baud(115200);
     
     distance_echo.rise(&echo_start);
     distance_echo.fall(&echo_end);
     distance_start.attach(&pulse_trigger, 100E-3);
     distance_timer.start();
     
     char motor_command;
     
     float frequency      = 1000.0f; // Adjust value to increase max speed
     float period         = 1.0f/frequency; // Don't adjust this
     
     motor_1_forward  = 1;
     motor_1_backward = 0;
 
     motor_2_forward  = 1;
     motor_2_backward = 0;
 
     motor_1_enable.period(period);
     motor_1_enable.write(0.0f);
 
     motor_2_enable.period(period);
     motor_2_enable.write(0.0f);
     
     while(true)
     {
         bluetooth.printf("\n\nWhat would you like to do?\n");
         bluetooth.printf("1) Manual Movement\n");
         bluetooth.printf("2) Sonar ON/OFF\n");
         bluetooth.printf("3) Follow Line\n");
         bluetooth.printf("4) Exit\n");
         
         // User inputs
         motor_command = bluetooth.getc();
         
         switch(motor_command)
         {

        // Check for correct input
        //if(motor_command > '4' || motor_command < '1')
        //{pc.printf("That was not a menu choice!\n");}
        
        // Exit the loop
        case'4':
        {bluetooth.printf("Thank you for playing!\n"); return 0;}

        // Follow line of electric tape on a white floor
        case '3':
        {bluetooth.printf("Not yet implemented!\n");break;}
        
        // Predetermined movements such as going in a circle, zigzagging, etc
        case '2':
        {
            if(!sonar){
            sonar = !sonar;
            bluetooth.printf("Sonar SET!\n");}
            
            else/*(!sonar)*/{
            sonar = !sonar;
            bluetooth.printf("Sonar DEACTIVATED!\n");}
            
            break;
            
        }
        
        // Manual Controls
        case '1':
        {
            bluetooth.printf("\n\n");
            manualMovement(bluetooth, motor_1_enable, motor_1_backward,motor_1_forward,
                           motor_2_enable, motor_2_backward, motor_2_forward);
            break;
        }
        default:
        {break;}
        } // end of switch
        
        wait(.5);

     }
     
    //return 0;
 }
 
 void manualCommands()
 {
     bluetooth.printf("W       - Speed forwards\n");
     bluetooth.printf("A       - Steer left\n");
     bluetooth.printf("S       - Speed backwards\n");
     bluetooth.printf("D       - Steer right\n");
     bluetooth.printf("E       - Stop\n");
     bluetooth.printf("R       - Reapeat these options\n");
     bluetooth.printf("Q       - Go back to main menu\n");
     
 }
 
 void patternedMovement()
 {
     bluetooth.printf("Not yet implemented.\n");
     
 }
 
//******************************************************************************
//******************************************************************************
// Manual Controls for the both the motors using manualCommands as
// instructions for user
//******************************************************************************
//******************************************************************************

 void manualMovement(Serial& pc, 
                     PwmOut& motor_1_enable, 
                     DigitalOut& motor_1_backward,
                     DigitalOut& motor_1_forward,
                     PwmOut& motor_2_enable, 
                     DigitalOut& motor_2_backward,
                     DigitalOut& motor_2_forward)
 {
     // For user input
     char vehicle_command = '1';
     
     // For exiting the menu.
     bool continues = true;
     
     float min_duty       = 0.00f; // Motor is stopped           
     float max_duty       = 1.0f; // Motor is at maximum capacity
     float duty_increment = 0.10f; // Adjust value to have more control of speed
     
     // Display menu options
     manualCommands();
     
     while(continues)
     {
           vehicle_command = bluetooth.getc(); // change to bluetooth.getc(); when
                                      // using bluetooth device
                                      
        // Add all data so user can see whats going on.
         
         switch(vehicle_command)
         {
//******************************************************************************
// Moving both motors forward - W
//******************************************************************************
             case 'w':
             {
////////////////////////////////////////////////////////////////////////////////
/// Motor 1

                 // We have to check is motor individually because we want
                 // the ability of one going back and one going forward
                 if(motor_1_forward == 1)
                 {
                     // Check for max speed
                     if(motor_1_duty_cycle == max_duty){break;}
                     
                     // If not at max speed, increase speed.
                     else{motor_1_duty_cycle += duty_increment;}
                 }
                 
                 // If it was going backwards and we increase forward speed
                 // we have to account for switching to moving forward.
                 else if(motor_1_backward == 1)
                 {
                     // Check for turning point from backwards to forward.
                     if(motor_1_duty_cycle == min_duty)
                     {
                         // Switch directions and increase speed.
                         motor_1_backward = 0;
                         motor_1_forward  = 1;
                         motor_1_duty_cycle += duty_increment;
                     }
                     
                     // If not at min speed, decrease speed.
                     else{motor_1_duty_cycle -= duty_increment;}
                 }
////////////////////////////////////////////////////////////////////////////////
/// Motor 2

                 if(motor_2_forward == 1)
                 {
                     // Check for max speed
                     if(motor_2_duty_cycle == max_duty){break;}
                     
                     // If not at max speed, increase speed.
                     else{motor_2_duty_cycle += duty_increment;}
                 }
                 
                 else if(motor_2_backward == 1)
                 {
                     if(motor_2_duty_cycle == min_duty)
                     {
                         motor_2_backward = 0;
                         motor_2_forward  = 1;
                         motor_2_duty_cycle += duty_increment;
                     }
                     
                     else{motor_2_duty_cycle -= duty_increment;}
                 }
                 
                 // Set both duty cycles to the enables
                 motor_1_enable.write(motor_1_duty_cycle);
                 motor_2_enable.write(motor_2_duty_cycle);
                 break;
             } //end of case w
             
//******************************************************************************
// Steer left == Make right motor (motor 2) move faster and make left
//               motor (motor 1) move slower - A
//******************************************************************************
             case 'd':
             {
////////////////////////////////////////////////////////////////////////////////
/// Motor 2

                 // Motor 2 moves forward so we use same reasoning as
                 // move forward (W) command
                 if(motor_2_forward == 1)
                 {
                     if(motor_2_duty_cycle == max_duty){break;}
                     
                     else{motor_2_duty_cycle += duty_increment;}
                     
                 }
                 else if(motor_2_backward == 1)
                 {
                     if(motor_2_duty_cycle == min_duty)
                     {
                         motor_2_backward = 0;
                         motor_2_forward  = 1;
                         motor_2_duty_cycle += duty_increment;
                     }
                     
                     else{motor_2_duty_cycle -= duty_increment;}
                 }
////////////////////////////////////////////////////////////////////////////////
/// Motor 1
                 // Motor 1 is going backwards so we use same reasoning as
                 // move backwards (S) command
                 if(motor_1_backward == 1)
                 {
                     if(motor_1_duty_cycle == max_duty){break;}
                     
                     else{motor_1_duty_cycle += duty_increment;}
                 }
                 else if(motor_1_forward == 1)
                 {
                     if(motor_1_duty_cycle == min_duty)
                     {
                         motor_1_forward  = 0;
                         motor_1_backward = 1;
                         motor_1_duty_cycle += duty_increment;
                     }
                     
                     else{motor_1_duty_cycle -= duty_increment;}
                 }
                 
                 // Set both duty cycles to the enables
                 motor_1_enable.write(motor_1_duty_cycle);
                 motor_2_enable.write(motor_2_duty_cycle);
                 break;
             } // end of case a
 
//******************************************************************************            
// Moving both motors backwards - S
//******************************************************************************
             case 's':
             {
////////////////////////////////////////////////////////////////////////////////
/// Motor 1

                 // If motor is moving backwards and we are not at the max
                 // duty cycle then increase the duty cycle
                 if(motor_1_backward == 1)
                 {
                     // Check for max speed
                     if(motor_1_duty_cycle == max_duty){break;}
                     
                     // Increase speed if not at max
                     else{motor_1_duty_cycle += duty_increment;}
                 }
                 // If motor is going forward we need to reduce it's speed
                 // if it is at the minimum duty then we switch its direction
                 // and increase the speed instead
                 else if(motor_1_forward == 1)
                 {
                     if(motor_1_duty_cycle == min_duty)
                     {
                         motor_1_forward  = 0;
                         motor_1_backward = 1;
                         motor_1_duty_cycle += duty_increment;
                     }
                     
                     else{motor_1_duty_cycle -= duty_increment;}
                 }
////////////////////////////////////////////////////////////////////////////////
/// Motor 2
                 // Same logic as motor 1
                 if(motor_2_backward == 1)
                 {
                     if(motor_2_duty_cycle == max_duty){break;}
                     
                     else{motor_2_duty_cycle += duty_increment;}
                 }

                 else if(motor_2_forward == 1)
                 {
                     if(motor_2_duty_cycle == min_duty)
                     {
                         motor_2_forward  = 0;
                         motor_2_backward = 1;
                         motor_2_duty_cycle += duty_increment;
                     }
                     
                     else{motor_2_duty_cycle -= duty_increment;}
                 }
                 
                 // Set both duty cycles to the enables
                 motor_1_enable.write(motor_1_duty_cycle);
                 motor_2_enable.write(motor_2_duty_cycle);
                 break;
             } // end of case s
             
//******************************************************************************
// Steer right == Make left motor (motor 1) move faster and make right
//               motor (motor 2) move slower - D
//******************************************************************************
             case 'a':
             {
////////////////////////////////////////////////////////////////////////////////
/// Motor 1

                 // Same logic as moving motor 1 forward
                 if(motor_1_forward == 1)
                 {
                     if(motor_1_duty_cycle == max_duty){break;}
                     
                     else{motor_1_duty_cycle += duty_increment;}
                 }
                 
                 else if(motor_1_backward == 1)
                 {
                     if(motor_1_duty_cycle == min_duty)
                     {
                         motor_1_backward = 0;
                         motor_1_forward  = 1;
                         motor_1_duty_cycle += duty_increment;
                     }
                     
                     else{motor_1_duty_cycle -= duty_increment;}
                 }
////////////////////////////////////////////////////////////////////////////////
/// Motor 2
                 // Same logic reversing motor 2
                 if(motor_2_backward == 1)
                 {
                     if(motor_2_duty_cycle == max_duty){break;}
                     
                     else{motor_2_duty_cycle += duty_increment;}
                 }

                 else if(motor_2_forward == 1)
                 {
                     if(motor_2_duty_cycle == min_duty)
                     {
                         motor_2_forward  = 0;
                         motor_2_backward = 1;
                         motor_2_duty_cycle += duty_increment;
                     }
                     
                     else{motor_2_duty_cycle -= duty_increment;}
                 }
                 
                 // Set both duty cycles to the enables
                 motor_1_enable.write(motor_1_duty_cycle);
                 motor_2_enable.write(motor_2_duty_cycle);
                 break;
             } // end of case d
             
//******************************************************************************
// Stop moving both motors - E
//******************************************************************************
             case 'e':
             {
////////////////////////////////////////////////////////////////////////////////
/// Motor 1 and 2
                 // Stop both motors
                 motor_1_enable.write(min_duty);
                 motor_2_enable.write(min_duty);
    
                 // Reset direction to starting positions
                 motor_1_forward  = 1;
                 motor_1_backward = 0;
                 motor_2_forward  = 1;
                 motor_2_backward = 0;
                 
                 // Reset speed of both motors to 0
                 motor_1_duty_cycle = min_duty;
                 motor_2_duty_cycle = min_duty;
                 break;
                 
             } // end of case e
//******************************************************************************
// Print current menu options for user - R
//******************************************************************************
             case 'r':
             {
////////////////////////////////////////////////////////////////////////////////
/// On console
               // Calls menu function.
                manualCommands();
                break;
             } // end of case r
             
//******************************************************************************
// Go back to previous menu - Q
//******************************************************************************
             case 'q':
             {
////////////////////////////////////////////////////////////////////////////////
/// Motor 1 and 2
                 // If we are quitting we have to stop both motors
                 // and reset everything
                 motor_1_enable.write(min_duty);
                 motor_2_enable.write(min_duty);

                 motor_1_forward  = 1;
                 motor_1_backward = 0;
                 motor_2_forward  = 1;
                 motor_2_backward = 0;
                 
                 motor_1_duty_cycle = min_duty;
                 motor_2_duty_cycle = min_duty;
////////////////////////////////////////////////////////////////////////////////
/// On console
                // set our check to false so we exit function
                continues = false;
                break;
                
                
             } // end of case q
             
////////////////////////////////////////////////////////////////////////////////
/// Default
             default:
             {
                 break;
                 
             } // end of default
         } // end of switch statement
     } // end of while (continues)
 } // end of function
