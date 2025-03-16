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
#define MAX_POSITIONS 1000  // Definir un límite de almacenamiento

#define MAX_POSITIONS 1000  // Límite de almacenamiento

typedef struct {
    float x;
    float y;
    float distance_to_goal;  // Nueva variable para almacenar la distancia al objetivo
} Position;

 float min = 100;

// Inicializa el arreglo con valores vacíos (0.0)
static Position visited_positions[MAX_POSITIONS];
static int position_count = 0;

//Función para registrar la posición
void save_position(float x, float y, float distance) {
    if (position_count < MAX_POSITIONS) {
        visited_positions[position_count].x = x;
        visited_positions[position_count].y = y;
        visited_positions[position_count].distance_to_goal = distance;  // Guardar distancia al objetivo
        if (min > distance){
        	min = distance;
        }
        position_count++;
    }
}

//Calcular la distancia al objetivo
float calculate_distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

//Funciòn para  calcular el ángulo hacia el objetivo
// Calcular el ángulo hacia el objetivo
float calculate_angle_to_target(float x1, float y1, float x2, float y2) {
    return atan2(y2 - y1, x2 - x1);
}


//Función para verificar si la posición ya fue visitada
int check_visited(float x, float y) {
    for (int i = 0; i < position_count; i++) {
        float dx = visited_positions[i].x - x;
        float dy = visited_positions[i].y - y;
        if (sqrt(dx * dx + dy * dy) < 0.01) {  // Umbral de 5 cm
            return 1;  // Posición ya visitada
        }
    }
    return 0;  // Posición nueva
}

//función para recorrer el arreglo y encontrar la menor distancia
float get_min_distance() {
    if (position_count == 0) {
        return -1.0;  // Retorna un valor negativo si no hay posiciones guardadas
    }

    float min_distance = visited_positions[0].distance_to_goal;  // Iniciar con la primera distancia

    for (int i = 1; i < position_count; i++) {
        if (visited_positions[i].distance_to_goal < min_distance) {
        	if (visited_positions[i].distance_to_goal > 0.0){
            		min_distance = visited_positions[i].distance_to_goal;
        	}
        }
    }
	printf("\n\nel minimo es %f\n\n", min_distance); 
    return min_distance;
}


// Student State Machine 
AdvanceAngle reactive_students(Raw observations, int dest, int intensity, float Mag_Advance, float max_angle, int num_sensors){

 AdvanceAngle gen_vector;
 int obs;
 int j;
 float left_side=0;
 float right_side=0;
 int value = 0;
 static int step=0;

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
 printf("intensity %d obstacles %d dest %d\n",intensity,obs,dest);

 if (intensity == 1){
	// Constants STOP, TURN RIGHT, ETC, are defined in ../utilities/constants.h
	// generate_output function in ../utilities/utilities.h
	gen_vector=generate_output(STOP,Mag_Advance,max_angle);
        printf("STOP\n");
	printf("\n **************** Reached light source ******************************\n");
 }
 else if (obs == 0){
	// There is not obstacle
        //gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
	//printf("FORWARD\n");

	if (dest == 0){
                // go right twice
		gen_vector=generate_output(RIGHTADVANCETWICE,Mag_Advance,max_angle);
                printf("TURN RIGHT TWICE\n");
        }
        else if (dest == 1){
                // go left twice
                gen_vector=generate_output(LEFTADVANCETWICE,Mag_Advance,max_angle);
                printf("TURN LEFT TWICE\n");
        }
        else if (dest == 2){
                 // go right
                 gen_vector=generate_output(RIGHTADVANCE,Mag_Advance,max_angle);
                 printf("TURN RIGHT\n");
        }
        else if (dest == 3){
                // go left
                gen_vector=generate_output(LEFTADVANCE,Mag_Advance,max_angle);
                printf("TURN LEFT\n");
        }
 }
 else if (obs == 1){
        // Obtacle in the right
        gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
	printf("TURN LEFT\n");
 }
 else if (obs == 2){
        // obtacle in the left
	gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
	printf("TURN RIGHT\n");
 }
 else if (obs == 3){
	// obstacle in the front
        gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
	printf("TURN LEFT\n");
 }


 return gen_vector;

}



                 


// Student State Machine 
AdvanceAngle state_machine_students(Raw observations, int dest, int intensity, int state, int *next_state, 
                                    float Mag_Advance, float max_angle, int num_sensors, float angle_light, 
                                    coord coord_robot, coord coord_dest) {



 AdvanceAngle gen_vector;
 int obs;
 int j;
 float left_side=0;
 float right_side=0;
 int value = 0;
 int turns = 0;
 float distAc;
 float target_angle ;
 float angle_difference;

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
        	// Calcular el ángulo hacia el destino
		target_angle = calculate_angle_to_target(coord_robot.xc, coord_robot.yc, coord_dest.xc, coord_dest.yc);
		angle_difference = target_angle - coord_robot.anglec;
		
		if (angle_difference > M_PI) {
		    angle_difference -= 2 * M_PI;
		} else if (angle_difference < -M_PI) {
		    angle_difference += 2 * M_PI;
		}
		
		// Si la diferencia es mayor que 45°, girar en la dirección más corta
		if (fabs(angle_difference) > 0.7854) {  // 0.7854 radianes = 45°
		    if (angle_difference > 0) {
			gen_vector = generate_output(LEFT, Mag_Advance, max_angle);
			printf("Girando a la IZQUIERDA hacia el objetivo - Ángulo objetivo: %f, Ángulo actual: %f\n",
			       target_angle, coord_robot.anglec);
		    } else {
			gen_vector = generate_output(RIGHT, Mag_Advance, max_angle);
			printf("Girando a la DERECHA hacia el objetivo - Ángulo objetivo: %f, Ángulo actual: %f\n",
			       target_angle, coord_robot.anglec);
		    }
		    *next_state = 0;
		}   
		   
        	else if (check_visited(coord_robot.xc, coord_robot.yc)) {
        		gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                        *next_state = 17;

                        printf("Present State: %d STOP\n", state);
        	}
                else if (obs == 0){
                        // There is not obstacle
                        gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                        *next_state = 0;
                }
                else{
			// obstacle front
			if (obs == 3){
				gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                        	*next_state = 2;

                        	printf("Present State: %d STOP\n", state);
			
			}
			else if (obs == 2){
				
                			gen_vector=generate_output(BACKWARD,Mag_Advance,max_angle);
                			*next_state = 12;

					printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
          
			}
                        else{
                        	if(turns < 8){
                        		gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                			*next_state = 0;
					turns ++;
					printf("Present State: %d TURN RIGHT\n", state);
                        	}
                		else{
                			gen_vector=generate_output(BACKWARD,Mag_Advance,max_angle);
                			*next_state = 0;
                			turns = 0;

					printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                		}
                        }
                }

                break;

        case 2: //
                if (obs == 0){
			// There is not obstacle
                        gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                        printf("Present State: %d STOP\n", state);
                        *next_state = 7;
                }
                else{
                        gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                        printf("Present State: %d STOP\n", state);

                        if (obs == 3){
                                // obtacle in the  front
                                *next_state = 11;
                        }
                        else if (obs == 2){
                                // obtacle in the left
                                *next_state = 9;
                        }
                        else if (obs == 1) {
			    // Antes de girar nuevamente, intentamos retroceder para salir del bucle
			    gen_vector = generate_output(BACKWARD, Mag_Advance, max_angle);
			    printf("🔙 Retrocediendo para evitar oscilación\n");
			    *next_state = 3;  // Cambiar a otro estado en lugar de volver a 0
}
                }
                break;

        case 3: // right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 11;

		printf("Present State: %d TURN RIGHT\n", state);
                break;

        case 4: // Backward, obstacle in the left
                gen_vector=generate_output(BACKWARD,Mag_Advance,max_angle);
                *next_state = 11;

		printf("Present State: %d BACKWARD, obstacle left\n", state);
                break;

        case 5: // left turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 2;

		printf("Present State: %d TURN RIGTH\n", state);
                break;

        case 6: // Backward, obstacle in front
                gen_vector=generate_output(BACKWARD,Mag_Advance,max_angle);
                *next_state = 11;

		printf("Present State: %d BACKWARD, obstacle FRONT\n", state);
                break;

	case 7: /// Left turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 8;

		printf("Present State: %d TURN 1 LEFT\n", state);
                break;

        case 8:// Left turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 9;

		printf("Present State: %d TURN 2 LEFT\n", state);
                break;

        case 9: // Forward
                if (check_visited(coord_robot.xc, coord_robot.yc)) {
        		gen_vector=generate_output(BACKWARD,Mag_Advance,max_angle);
                        printf("Present State: %d BACKWARD\n", state);
                        *next_state = 14;
			min = get_min_distance();
        	}
        	else {
                	gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
			save_position(coord_robot.xc, coord_robot.yc, calculate_distance(coord_robot.xc, coord_robot.yc, coord_dest.xc, coord_dest.yc));
        		*next_state = 2;
        	}
                printf("Present State: %d 1 FORWARD\n", state);
                break;

        case 10: // Forward
                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                *next_state = 9;
		save_position(coord_robot.xc, coord_robot.yc, calculate_distance(coord_robot.xc, coord_robot.yc, coord_dest.xc, coord_dest.yc));
                printf("Present State: %d 2 FOWARD\n", state);
                break;

	case 11: // Right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 12;

                printf("Present State: %d turn 1 RIGHT\n", state);
                break;

        case 12: // Right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 2;

                printf("Present State: %d turn 2 RIGHT\n", state);
                break;


        case 13: // // check destination
		 if (dest == 0){
                                // go right
                                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                                *next_state = 5;

                                printf("Present State: %d RIGHT\n", state);
                 }
                 else if (dest == 1){
                                // go left
                                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                                *next_state = 3;

                                printf("Present State: %d LEFT\n", state);
                 }
                 else if (dest == 2){
                                // go right single
                                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                                *next_state = 5;

                                printf("Present State: %d FORWARD\n", state);
                 }
                 else if (dest == 3){
                                // go left single
                                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                                *next_state = 3;

                                printf("Present State: %d FORWARD\n", state);
                 }
                break;
	case 14: // Right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 23;

                printf("Present State: %d turn 1 RIGHT\n", state);
                break;

	case 15: //
		distAc = calculate_distance(coord_robot.xc, coord_robot.yc, coord_dest.xc, coord_dest.yc);
		if (distAc <= min+0.0001){
			gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                        printf("Present State: %d STOP\n", state);
                        *next_state = 26;
		}
		else if (obs == 0){
			// There is not obstacle
                        gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                        printf("Present State: %d STOP\n", state);
                        *next_state = 16;
                }
                else{
                        gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                        printf("Present State: %d STOP\n", state);

                        if (obs == 3){
                                // obtacle in the  front
                                *next_state = 19;
                        }
                        else if (obs == 1){
                                // obtacle in the right
                                *next_state = 18;
                        }
                        else if (obs == 2){
				// obstacle in the left
                                *next_state = 0;
                        }
                }
                break;
                
	case 16: /// Right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 17;

		printf("Present State: %d TURN 1 RIGHT\n", state);
                break;

        case 17:// Right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 18;

		printf("Present State: %d TURN 2 RIGHT\n", state);
                break;

	case 18: // Forward
                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                *next_state = 15;

                printf("Present State: %d 1 FORWARD\n", state);
                break;

	case 19: // LEFT turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 20;

                printf("Present State: %d turn 1 LEFT\n", state);
                break;

        case 20: // LEFT turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 15;

                printf("Present State: %d turn 2 LEFT\n", state);
                break;
	case 21: // LEFT turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 20;

                printf("Present State: %d turn 1 LEFT\n", state);
                break;

        case 22: // LEFT turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 18;

                printf("Present State: %d turn 2 LEFT\n", state);
                break;
        case 23: // Right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 24;

                printf("Present State: %d turn 1 RIGHT\n", state);
                break;
        case 24: // Right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 25;

                printf("Present State: %d turn 1 RIGHT\n", state);
                break;
        case 25: // Right turn
                gen_vector=generate_output(RIGHT,Mag_Advance,max_angle);
                *next_state = 15;

                printf("Present State: %d turn 1 RIGHT\n", state);
                break;
	case 26: // LEFT turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 27;

                printf("Present State: %d turn 1 LEFT\n", state);
                break;

        case 27: // LEFT turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 28;

                printf("Present State: %d turn 2 LEFT\n", state);
                break;
	case 28: // Forward
                gen_vector=generate_output(FORWARD,Mag_Advance,max_angle);
                *next_state = 0;

                printf("Present State: %d 1 FORWARD\n", state);
                break;
	case 29: /// Left turn
                gen_vector=generate_output(LEFT,Mag_Advance,max_angle);
                *next_state = 2;

		printf("Present State: %d TURN 1 LEFT\n", state);
                break;
	default:
		printf("State %d not defined used ", state);
                gen_vector=generate_output(STOP,Mag_Advance,max_angle);
                next_state = 0;
                break;
                
 }

 return gen_vector;

}




