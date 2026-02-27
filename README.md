# CompProjectBOB - Description


This project is a robot that will navigate through mazes made out of tape and other similar materials.


---


## Project Creators
- Alex Hansler
- Celeste Wilke-Van Moorsel


---


## Function Categories
- Gyro.h - contains all functions relating to the robot's gyro. Functions in this category are used for navigation and orienting the robot.


- Movement.h - contains all functions relating to the robot's wheels. Functions in in this category include moving, turning, and stopping.


- Weewoo.h - contains all functions that control the robot's led. Currently the only function in this category is weewoo which acts like an emergency vehicle's lights.


- Light_sensor.h - contains all functions for the robot's sensors below its body. Functions in this category detect when the robot is on tape or not.


- Ultrasonic_sensor.h - contains all functions for the robot's ultrasonic sensor on the front of its 'face.' Functions in this category detect when the robot is in front of a wall and how far from a wall the robot is.


---


## Known Problems
- While the robot is running, it tends to drift slightly to the right. This has been mitigated through adjusting the direction the robot is driving while the robot drives.


- The robot's gyro tends to become inaccurate over time and requires many resets to prevent extreme inaccuracy.


- Light sensors require manual readjusting to be able to function properly including the left sensor which tends to have light readings triple that of the other sensors.


## Future changes
- Increase the speed that the robot drives at to navigate mazes faster.


- Let the robot have happiness.


---


## How To Run (Unfinished)