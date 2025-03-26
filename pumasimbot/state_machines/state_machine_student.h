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

AdvanceAngle rayando_el_sol(Raw observations, float Mag_Advance, float max_angle, int num_sensors)
{

        AdvanceAngle gen_vector;
        int obs;
        int j;
        float left_side = 0;
        float right_side = 0;
        static int step = 0;

        step++;
        printf("\n\n **************** Student Reactive Behavior %d *********************\n", step);

        for (j = 0; j < num_sensors / 2; j++)
        {
                right_side = observations.sensors[j] + right_side;
                printf("right side sensor[%d] %f\n", j, observations.sensors[j]);
        }

        for (j = num_sensors / 2; j < num_sensors; j++)
        {
                left_side = observations.sensors[j] + left_side;
                printf("left side sensor[%d] %f\n", j, observations.sensors[j]);
        }

        right_side = right_side / (num_sensors / 2);
        left_side = left_side / (num_sensors / 2);
        printf("Average right side %f\n", right_side);
        printf("Average left side %f\n", left_side);

        obs = proximidad(left_side, right_side);
        printf("obstacles %d\n", obs);

        if (obs == 0)
        {

                gen_vector = generate_output(FORWARD, Mag_Advance, max_angle);
                printf("FORWARD\n");
        }
        else if (obs == 1)
        {
                // Obtacle in the left
                gen_vector = generate_output(LEFTADVANCE, (Mag_Advance) / 3, PI / 16);
                printf("TURN LEFT\n");
        }
        else if (obs == 2)
        {
                // obtacle in the right
                gen_vector = generate_output(RIGHTADVANCE, (Mag_Advance) / 3, PI / 16);
                printf("TURN RIGHT\n");
        }
        else if (obs == 3 || obs > 5)
        {
                // obstacle in the front
                gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                gen_vector = generate_output(RIGHT, Mag_Advance, PI / 20);

                printf("TURN RIGHT\n");
        }
        else if (obs == 4)
        {
                // obtacle in the LEFT
                gen_vector = generate_output(RIGHTADVANCE, (Mag_Advance) / 3, PI / 8);
                printf("TURN RIGHT\n");
        }
        else if (obs == 5)
        {
                // obtacle in the right
                gen_vector = generate_output(LEFTADVANCE, (Mag_Advance) / 3, PI / 8);
                printf("TURN LEFT\n");
        }
        return gen_vector;
}




AdvanceAngle segundo_codigo_jejeje(Raw observations, int num_sensors, int state, int *next_state, float Mag_Advance, float max_angle, float xr, float yr, float angle_r)
{

        AdvanceAngle gen_vector;
        int obs;
        int j;
        float left_side = 0;
        float right_side = 0;
        int value = 0;
        static int desatorar = 0;
        static int bandera = 0;
        static float desatorar2[5] = {0, 0, 0, 0, 0};

        for (j = 0; j < num_sensors / 2; j++)
        {
                right_side = observations.sensors[j] + right_side;
#ifdef DEBUG
                printf("right side sensor[%d] %f\n", j, observations.sensors[j]);
#endif
        }

        for (j = num_sensors / 2; j < num_sensors; j++)
        {
                left_side = observations.sensors[j] + left_side;
#ifdef DEBUG
                printf("left side sensor[%d] %f\n", j, observations.sensors[j]);
#endif
        }
        printf("Right: %f\n", right_side);
        printf("Angulo: %f\n", angle_r);
        right_side = right_side / (num_sensors / 2);
        left_side = left_side / (num_sensors / 2);
#ifdef DEBUG
        printf("Average right side %f\n", right_side);
        printf("Average left side %f\n", left_side);
#endif

        obs = proximidad(left_side, right_side);
#ifdef DEBUG
        printf("obs %d\n", obs);
#endif

        switch (state)
        {

        case 0:
                if (obs == 0)
                {
                        // there is not obstacle
                        gen_vector = generate_output(FORWARD, Mag_Advance * 0.2, max_angle);
                        printf("Present State: %d FORWARD\n", state);
                        state = 100;
                        *next_state = 0;
                }
                else if (obs == 1)
                {
                        // obstacle in the left
                        *next_state = 7;
                }
                else if (obs == 2)
                {
                        // obstacle in the right
                        *next_state = 9;
                }
                else if (obs == 3 || obs > 5)
                {
                        // obstacle in front
                        gen_vector = generate_output(STOP, Mag_Advance, max_angle);
                        printf("Present State: %d STOP\n", state);
                        *next_state = 1;
                }
                else if (obs == 4)
                {
                        // obstacle in the LEFT
                        *next_state = 6;
                }
                else if (obs == 5)
                {
                        // obstacle in the RIGHT
                        *next_state = 8;
                }
                break;

        case 1:
                desatorar = 0;
                gen_vector = generate_output(LEFT, Mag_Advance, PI / 6);
                printf("Present State: %d TURN LEFT\n", state);
                *next_state = 2;
                break;

        case 2:
                desatorar = 0;
                gen_vector = generate_output(LEFT, Mag_Advance, PI / 6);
                printf("Present State: %d TURN LEFT\n", state);
                *next_state = 3;
                break;

        case 3:
                desatorar = 0;
                gen_vector = generate_output(LEFT, Mag_Advance, PI / 6);
                printf("Present State: %d TURN LEFT\n", state);
                if (obs == 3 || obs > 5)
                {
                        *next_state = 0;
                }
                else
                {
                        *next_state = 4;
                }

                break;

        case 4:
                desatorar = 0;
                gen_vector = generate_output(FORWARD, (Mag_Advance) * 0.2, max_angle);
                printf("Present State: %d FORWARD\n", state);
                if (right_side > (THRS) * 0.8)
                {
                        *next_state = 5;
                }
                else if (obs == 3 || obs > 5)
                {
                        *next_state = 0;
                }
                else if (obs == 5)
                {
                        *next_state = 11;
                }
                else if (obs == 2)
                {
                        if (bandera == 1)
                                *next_state = 17;
                        else
                                *next_state = 4;
                }
                break;

        case 5:
                bandera = 1;
                gen_vector = generate_output(RIGHT, Mag_Advance, PI / 6);
                printf("Present State: %d RIGHT\n", state);
                *next_state = 12;
                break;

        case 6:
                desatorar = 0;
                gen_vector = generate_output(RIGHT, Mag_Advance, PI / 6);
                printf("Present State: %d RIGHT\n", state);
                *next_state = 10;
                break;

        case 7:

                desatorar += 1;
                gen_vector = generate_output(LEFT, Mag_Advance, PI / 6);
                printf("Present State: %d TURN LEFT\n", state);
                *next_state = 0;
                if (desatorar > 3)
                {
                        *next_state = 14;
                }
                break;

        case 8:
                desatorar = 0;
                gen_vector = generate_output(LEFT, Mag_Advance, PI / 6);
                printf("Present State: %d TURN LEFT\n", state);
                *next_state = 10;
                break;
        case 9:
                gen_vector = generate_output(RIGHT, Mag_Advance, PI / 6);
                printf("Present State: %d RIGHT\n", state);
                *next_state = 0;
                desatorar += 1;
                break;
        case 10:
                gen_vector = generate_output(FORWARD, (Mag_Advance) * 0.2, max_angle);
                printf("Present State: %d FORWARD\n", state);
                *next_state = 0;
                break;
        case 11:
                desatorar = 0;
                gen_vector = generate_output(LEFT, Mag_Advance, PI / 6);
                printf("Present State: %d TURN LEFT\n", state);
                *next_state = 4;
                break;
        case 12:
                desatorar = 0;
                gen_vector = generate_output(RIGHT, Mag_Advance, PI / 6);
                printf("Present State: %d RIGHT\n", state);
                *next_state = 13;
                break;
        case 13:
                desatorar = 0;
                gen_vector = generate_output(RIGHT, Mag_Advance, PI / 6);
                printf("Present State: %d RIGHT\n", state);
                *next_state = 16;
                break;
        case 14:
                desatorar = 0;
                gen_vector = generate_output(LEFT, Mag_Advance, PI / 6);
                printf("Present State: %d TURN LEFT\n", state);
                *next_state = 15;
                break;
        case 15:
                desatorar = 0;
                gen_vector = generate_output(FORWARD, (Mag_Advance) * 0.2, max_angle);
                printf("Present State: %d FORWARD\n", state);
                *next_state = 0;
                break;
        case 16:
                desatorar = 0;
                gen_vector = generate_output(FORWARD, (Mag_Advance) * 0.2, max_angle);
                printf("Present State: %d FORWARD\n", state);
                *next_state = 0;
                break;

        case 17:
                gen_vector = generate_output(RIGHT, Mag_Advance, PI / 6);
                printf("Present State: %d RIGHT\n", state);
                *next_state = 18;
                break;
        case 18:
                gen_vector = generate_output(FORWARD, (Mag_Advance) * 0.2, max_angle);
                printf("Present State: %d FORWARD\n", state);
                *next_state = 4;
                break;
        }

        if (state != 0)
        {
                if (desatorar2[0] == xr && desatorar2[1] == yr && desatorar2[2] == angle_r)
                {
                        desatorar2[3]++;
                        desatorar2[4] = 0;
                        if (desatorar2[3] >= 5)
                        {
                                gen_vector = generate_output(BACKWARD, Mag_Advance * 0.2, max_angle);
                                desatorar2[3] = 0;
                                *next_state = 14;
                                printf("\n\n\n******************** BACKWARD*\n\n\n");
                        }
                }
                else if (desatorar2[0] == xr && desatorar2[1] == yr)
                {
                        desatorar2[4]++;
                        desatorar2[3] = 0;
                        if (desatorar2[4] >= 20)
                        {
                                gen_vector = generate_output(BACKWARD, Mag_Advance * 0.2, max_angle);
                                desatorar2[4] = 0;
                                *next_state = 14;
                                printf("\n\n\n*Present State: %d BACKWARD\n\n\n", state);
                        }
                }
                else
                {
                        desatorar2[3] = 0;
                        desatorar2[4] = 0;
                }
        }
        printf("\n**si\n");
        desatorar2[0] = xr;
        desatorar2[1] = yr;
        desatorar2[2] = angle_r;
        // printf("Next State: %d\n", *next_state);
        return gen_vector;
}

AdvanceAngle ya_me_enoje(Raw observations, int num_sensors, int state, int *next_state, float Mag_Advance, float max_angle, float xo, float yo, float xr, float yr, float xf, float yf, int dest, int intensity, float angle_r)
{

        AdvanceAngle gen_vector;
        int obs;
        float left_side = 0;
        float right_side = 0;
        static float obs0[3] = {0, 0, 0};
        static float obs_min[2] = {0, 0};
        float vector[2];
        static float modulo[2] = {1, 1};
        static int steps = 0;
        static int bandera_de_salida = 0, bandera2 = 20;
        static int i = 0;
        /*






        */
        i++;
        printf("\n\n\n\n\n*%d\n\n", i);
        printf("\nPosición actual: (x: %f, y: %f)\n", xr, yr);
        printf("\nBandera 0 %f\n", obs0[2]);
        printf("\nBandera 1 %i\n", bandera_de_salida);
        printf("\nPosicion 0: (%f\t%f)\n", obs0[0], obs0[1]);
        printf("\nPosicion minima: (%f\t%f)\n", obs_min[0], obs_min[1]);

        printf("\nStep %i\n", steps);
        /*







        */

        if (obs0[2] == 1)
        {
                vector[0] = xf - xr;
                vector[1] = yf - yr;
                modulo[0] = sqrt(pow(vector[0], 2) + pow(vector[1], 2));
                if (modulo[0] < modulo[1])
                {
                        modulo[1] = modulo[0];
                        obs_min[0] = xr;
                        obs_min[1] = yr;
                }
        }
        if (steps > 0)
        {
                steps++;
        }

        if (bandera2 > 0)
        {
                bandera2++;
        }

        if (((xr >= (obs0[0]) - 0.05) && (yr >= (obs0[1]) - 0.05)) && ((xr <= (obs0[0]) + 0.05) && (yr <= (obs0[1]) + 0.05)) && (steps > 100))
        {
                bandera_de_salida++;
                *next_state = 0;
        }

        /*




        */

        for (int j = 0; j < num_sensors / 2; j++)
        {
                right_side = observations.sensors[j] + right_side;
#ifdef DEBUG
                printf("right side sensor[%d] %f\n", j, observations.sensors[j]);
#endif
        }

        for (int j = num_sensors / 2; j < num_sensors; j++)
        {
                left_side = observations.sensors[j] + left_side;
#ifdef DEBUG
                printf("left side sensor[%d] %f\n", j, observations.sensors[j]);
#endif
        }

        right_side = right_side / (num_sensors / 2);
        left_side = left_side / (num_sensors / 2);
#ifdef DEBUG
        printf("Average right side %f\n", right_side);
        printf("Average left side %f\n", left_side);
#endif

        obs = proximidad(left_side, right_side);
#ifdef DEBUG
        printf("obs %d\n", obs);
#endif
        if (((xr >= (obs_min[0]) - 0.05) && (yr >= (obs_min[1]) - 0.05)) && ((xr <= (obs_min[0]) + 0.05) && (yr <= (obs_min[1]) + 0.05)) && (bandera_de_salida > 0))
        {
                obs0[0] = 0;
                obs0[1] = 0;
                obs0[2] = 0;
                steps = 0;
                bandera_de_salida = 0;
                bandera2++;
                *next_state = 0;
                obs_min[0] = 0;
                obs_min[1] = 0;
                modulo[0] = 1;
                modulo[1] = 1;
        }
        else if (obs0[2] == 0)
        {

                printf("obs intensity %d dest %d\n", intensity, dest);
                if (obs != 0 && bandera2 > 10)
                {

                        obs0[0] = xr;
                        obs0[1] = yr;
                        obs0[2] = 1;
                        steps++;
                        bandera2 = 0;
                        *next_state = 1;
                }
                else
                {
                        printf("\n*");
                        printf("\n*SEGUIDOR DE LUZ*");
                        printf("\n*\n");
                        gen_vector = state_machine_destination(dest, intensity, state, next_state, Mag_Advance, max_angle);
                }
        }
        else
        {
                /*printf("\n*");
                printf("\n**MAQUINA DE ESTADOS");
                printf("\n*\n");*/
                gen_vector = segundo_codigo_jejeje(observations, num_sensors, state, next_state, Mag_Advance, max_angle, xr, yr, angle_r);
        }
        return gen_vector;
}

void f_a_M(const float epsilon_1, float q_0[], float q_dest[], float f_atr[]);
void f_a_m(const float epsilon_2, float q_0[], float q_dest[], float f_atr[]);
void f_r(const float eta, const float d_0, float q_0[], float q_obs[], float f_rep[]);
void f_t(float f_total[], float f_atr[], float f_rep_1[], float f_rep_2[]);
void avance(const float delta_0, float f_total[]);
float modulo(float f_total[]);
float angulo(float f_total[]);

AdvanceAngle campos_de_concentracion(Raw observations, int num_sensors, int state, int *next_state, float xr, float yr, float xf, float yf, int intensity, float angle_r, float Mag_Advance, float max_angle)
{
        AdvanceAngle gen_vector;
        float q_0[2];
        float q_dest[2];
        float q_obs[2][2] = {{0, 0}, {0, 0}};
        const float d_0 = 0.05, d_1 = 0.3, epsilon_1 = 1, epsilon_2 = 1, eta = 2, delta_0 = 0.02;
        float f_total[2];
        float f_atr[2];
        float f_rep[2][2] = {{0, 0}, {0, 0}};
        float left_side = 0;
        float right_side = 0;
        float angle_sensors;
        float angle_sensors_inicial;
        int num_coo = 0;
        static float xy_steps[4] = {0, 0, 0, 0};

        q_0[0] = xr;
        q_0[1] = yr;
        q_dest[0] = xf;
        q_dest[1] = yf;

        angle_sensors = (2 * PI) / (num_sensors);
        angle_sensors_inicial = ((angle_sensors / 2) - (angle_sensors * (num_sensors / 2))) + angle_r;

        for (int j = 0; j < num_sensors / 2; j++)
        {
                if (observations.sensors[j] < 0.1)
                {
                        num_coo++;
                        q_obs[0][0] += observations.sensors[j] * cos(angle_sensors_inicial + (angle_sensors * j));
                        q_obs[0][1] += observations.sensors[j] * sin(angle_sensors_inicial + (angle_sensors * j));
                }
                // right_side = observations.sensors[j] + right_side;
                // printf("right side sensor[%d] %f\n", j, observations.sensors[j]);
        }
        if (q_obs[0][0] > 0 || q_obs[0][0] < 0)
        {
                q_obs[0][0] /= num_coo;
                q_obs[0][1] /= num_coo;
                q_obs[0][0] += xr;
                q_obs[0][1] += yr;
                printf("\nCampo potencial DERECHO en x: %f\ty: %f\n", q_obs[0][0], q_obs[0][1]);
                num_coo = 0;
        }

        for (int j = num_sensors / 2; j < num_sensors; j++)
        {
                if (observations.sensors[j] < 0.1)
                {
                        num_coo++;
                        q_obs[1][0] += observations.sensors[j] * cos(angle_sensors_inicial + (angle_sensors * j));
                        q_obs[1][1] += observations.sensors[j] * sin(angle_sensors_inicial + (angle_sensors * j));
                }
                // left_side = observations.sensors[j] + left_side;
                // printf("left side sensor[%d] %f\n", j, observations.sensors[j]);
        }
        if (q_obs[1][0] > 0 || q_obs[1][0] < 0)
        {
                q_obs[1][0] /= num_coo;
                q_obs[1][1] /= num_coo;
                q_obs[1][0] += xr;
                q_obs[1][1] += yr;
                printf("\nCampo potencial IZQUIERDO en x: %f\ty: %f\n", q_obs[1][0], q_obs[1][1]);
        }

        if (d_1 >= (sqrt(pow((xf - xr), 2) + pow((yf - yr), 2))))
        {
                f_a_M(epsilon_1, q_0, q_dest, f_atr);
        }
        else
        {
                f_a_m(epsilon_2, q_0, q_dest, f_atr);
        }
        if (d_0 >= (sqrt(pow((q_obs[0][0] - q_0[0]), 2) + pow((q_obs[0][1] - q_0[1]), 2))))
        {
                f_r(eta, d_0, q_0, q_obs[0], f_rep[0]);
        }
        if (d_0 >= (sqrt(pow((q_obs[1][0] - q_0[0]), 2) + pow((q_obs[1][1] - q_0[1]), 2))))
        {
                f_r(eta, d_0, q_0, q_obs[1], f_rep[1]);
        }
        printf("\nFuerza [0][0]: %f", f_rep[0][0]);
        printf("\nFuerza [0][1]: %f", f_rep[0][1]);
        printf("\nFuerza [1][0]: %f", f_rep[1][0]);
        printf("\nFuerza [1][1]: %f\n", f_rep[1][1]);
        f_t(f_total, f_atr, f_rep[0], f_rep[1]);
        avance(delta_0, f_total);

        printf("\nVertor de fuerxa final:    %f %f", f_total[0], f_total[1]);
        printf("\nModulo:  %f", modulo(f_total));
        printf("\nAngulo:  %f\n", angulo(f_total));

        gen_vector = generate_output(LEFTADVANCE, modulo(f_total), angulo(f_total) - angle_r);
        printf("posicion x: %f\ty: %f\n", xr, yr);
        printf("angulo: %f\n\n\n", angle_r);

        printf("\n\n%f\n\n", xy_steps[2]);
        if (xy_steps[3] != 0)
        {
                gen_vector = rayando_el_sol(observations, Mag_Advance, max_angle, num_sensors);
                xy_steps[3]++;
                if (xy_steps[3] > 20)
                        xy_steps[3] = 0;
        }
        else if (fmod(xy_steps[2], 2) == 0)
        {
                if ((xy_steps[0] - 0.01 <= xr && xy_steps[1] - 0.01 <= yr) && (xy_steps[0] + 0.01 >= xr && xy_steps[1] + 0.01 >= yr))
                {
                        gen_vector = rayando_el_sol(observations, Mag_Advance, max_angle, num_sensors);
                        printf("\n\nSE CAMBIO EL COMPORTAMIENTO A REACTIVO\n\n");
                        xy_steps[3]++;
                }

                xy_steps[0] = xr;
                xy_steps[1] = yr;
        }
        xy_steps[2]++;

        return gen_vector;
}

void f_a_M(const float epsilon_1, float q_0[], float q_dest[], float f_atr[])
{
        f_atr[0] = epsilon_1 * (q_0[0] - q_dest[0]);
        f_atr[1] = epsilon_1 * (q_0[1] - q_dest[1]);
        printf("Fuerza MAYOR");
        printf("\nVector M %f %f", f_atr[0], f_atr[1]);
}
void f_a_m(const float epsilon_2, float q_0[], float q_dest[], float f_atr[])
{
        f_atr[0] = (epsilon_2 * (q_0[0] - q_dest[0])) / (sqrt(pow((q_0[0] - q_dest[0]), 2) + pow((q_0[1] - q_dest[1]), 2)));
        f_atr[1] = (epsilon_2 * (q_0[1] - q_dest[1])) / (sqrt(pow((q_0[0] - q_dest[0]), 2) + pow((q_0[1] - q_dest[1]), 2)));
        printf("Fuerza menor");
        printf("\nVector m %f %f", f_atr[0], f_atr[1]);
}
void f_r(const float eta, const float d_0, float q_0[], float q_obs[], float f_rep[])
{
        float diferencia[2] = {q_0[0] - q_obs[0], q_0[1] - q_obs[1]};
        float modulo_d = sqrt(pow(diferencia[0], 2) + pow(diferencia[0], 2));
        f_rep[0] = -eta * ((1 / modulo_d) - (1 / d_0)) * (1 / pow(modulo_d, 2)) * (diferencia[0] / modulo_d);
        f_rep[1] = -eta * ((1 / modulo_d) - (1 / d_0)) * (1 / pow(modulo_d, 2)) * (diferencia[1] / modulo_d);
}
void f_t(float f_total[], float f_atr[], float f_rep_1[], float f_rep_2[])
{
        float magnitud;
        f_total[0] = f_atr[0] + f_rep_1[0] + f_rep_2[0];
        f_total[1] = f_atr[1] + f_rep_1[1] + f_rep_2[1];
        magnitud = sqrt(pow(f_total[0], 2) + pow(f_total[1], 2));
        printf("\n%f\n", magnitud);
        f_total[0] = f_total[0] / magnitud;
        f_total[1] = f_total[1] / magnitud;
}
void avance(const float delta_0, float f_total[])
{
        f_total[0] = -1 * delta_0 * f_total[0];
        f_total[1] = -1 * delta_0 * f_total[1];
}
float modulo(float f_total[])
{
        return (sqrt(pow(f_total[0], 2) + pow(f_total[1], 2)));
}
float angulo(float f_total[])
{
        float angulo;
        if (f_total[0] < 0)
        {
                if (f_total[1] < 0)
                {
                        angulo = PI + atan(abs(f_total[1]) / abs(f_total[0]));
                }
                else
                {
                        angulo = PI - atan(abs(f_total[1]) / abs(f_total[0]));
                }
        }
        else
        {
                angulo = atan(f_total[1] / f_total[0]);
        }
        return angulo;
}