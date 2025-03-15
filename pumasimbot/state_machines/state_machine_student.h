/********************************************************
 *                                                      *
 *                                                      *
 *      state_machine_student.h          		*
 *                                                      *
 *		Student:				*
 *		FI-UNAM					*
 *		2-15-2024                               *
 *                                                      *
 ********************************************************/


// Student State Machine 
AdvanceAngle reactive_students(Raw observations, float Mag_Advance, float max_angle, int num_sensors){

 AdvanceAngle gen_vector;
 int obs;
 int j;
 float left_side=0;
 float right_side=0;
 int value = 0;
 static int step=0;
 float reference  = 0.05; //Metros 

 step++;
 printf("\n\n **************** Student Reactive Behavior %d *********************\n",step);

 for(j=0;j<num_sensors/2;j++){
        right_side = observations.sensors[j] + right_side;
        printf("right side sensor[%d] %f\n",j,observations.sensors[j]);
 }

 for(j=num_sensors/2;j<num_sensors;j++){
        left_side = observations.sensors[j] + left_side;
        printf("left side sensor[%d] %f\n",j,observations.sensors[j]);
 }

 right_side = right_side/(num_sensors/2);
 left_side = left_side/(num_sensors/2);
 printf("Average right side %f\n",right_side);
 printf("Average left side %f\n",left_side);

 if( left_side < THRS) value = (value << 1) + 1;
 else value = (value << 1) + 0;

 if( right_side < THRS) value = (value << 1) + 1;
 else value = (value << 1) + 0;

 obs = value;


if (obs == 3){ //Obstacle in the front
        gen_vector = generate_output(LEFTADVANCETWICE, Mag_Advance, max_angle);
}else if(obs == 1){ //Obstacle in the right
        if(right_side<reference){
                gen_vector = generate_output(LEFTADVANCE, Mag_Advance, max_angle);
        }else if(right_side>reference){
                gen_vector = generate_output(RIGHTADVANCE, Mag_Advance, max_angle);
        }else{
                gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
        }
}else if (obs == 2){ //Obstacle in the left
        if(left_side<reference){
                gen_vector = generate_output(RIGHTADVANCE, Mag_Advance, max_angle);
        }else if(left_side>reference){
                gen_vector = generate_output(LEFTADVANCE, Mag_Advance, max_angle);
        }else{
                gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
        }
}else{
        gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
}


 return gen_vector;

}



                 


// Student State Machine 
AdvanceAngle state_machine_students(Raw observations, int dest, int intensity, int state, int *next_state, float Mag_Advance, float max_angle, int num_sensors, float angle_light){

 AdvanceAngle gen_vector;
 int obs;
 int j;
 float left_side=0;
 float right_side=0;
 int value = 0;
 float reference = 0.04; //Metros

 printf("\n\n **************** Student State Machine *********************\n");

 for(j=0;j<num_sensors/2;j++){
        right_side = observations.sensors[j] + right_side;
        printf("right side sensor[%d] %f\n",j,observations.sensors[j]);
 }

 for(j=num_sensors/2;j<num_sensors;j++){
        left_side = observations.sensors[j] + left_side;
        printf("left side sensor[%d] %f\n",j,observations.sensors[j]);
 }

 right_side = right_side/(num_sensors/2);
 left_side = left_side/(num_sensors/2);
 printf("Average right side %f\n",right_side);
 printf("Average left side %f\n",left_side);

 if( left_side < THRS) value = (value << 1) + 1;
 else value = (value << 1) + 0;

 if( right_side < THRS) value = (value << 1) + 1;
 else value = (value << 1) + 0;

 obs = value;
 printf("intensity %d obstacles %d dest %d\n",intensity,obs,dest);
 printf("Angle light %f\n",angle_light);

 switch ( state ) {

        case 0:
                // there is not obstacle
                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                printf("Present State: %d FORWARD\n", state);
                *next_state = 0;

                if (obs == 3){
                        // obstacle in the front
                        *next_state = 1;
                }
                else if (obs == 2){
                        // obstacle in the left
                        if(left_side<reference){
                                *next_state = 2;
                        }else if(left_side>reference){
                                *next_state = 3;
                        }else{
                                next_state = 0; 
                        }
                }
                else if (obs == 1){
                        // obtacle in the right
                        if (right_side<reference){
                                *next_state = 4;
                        }else if (right_side>reference){
                                *next_state = 5; 
                        }else{
                                *next_state = 0; 
                        }
                }
        break; 

        case 1: //obstacle in front, doing 2 lefts
                gen_vector = generate_output(LEFTADVANCETWICE, Mag_Advance, max_angle);
                printf("Present State: %d OBSTACLE IN FRONT\n", state);
                *next_state = 0; 
        break;

        case 2: //rigth turn
                gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                printf("Present State: %d TURNING RIGHT, OBSTACLE LEFT\n", state);
                *next_state = 0;
        break;

        case 3: //left turn
                gen_vector = generate_output(LEFT, Mag_Advance, max_angle);
                printf("Present State: %d TURNING LEFT, OBSTACLE LEFT\n", state);
                *next_state = 0;
        break;
        
        case 4: //left turn
                gen_vector = generate_output(LEFT, Mag_Advance, max_angle);
                printf("Present State: %d TURNING LEFT, OBSTACLE RIGHT\n", state);
                *next_state = 0;
        break;

        case 5: //right turn
                gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
                printf("Present State: %d TURNING RIGHT, OBSTACLE RIGHT\n", state);
                *next_state = 0;
        break;


	default:
		printf("State %d not defined used ", state);
                gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                next_state = 0;
                break;

                
 }

 return gen_vector;

}



                 

