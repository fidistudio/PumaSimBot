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
       AdvanceAngle state_machine_students(Raw observations, coord Dest, coord Position, int state, int *next_state, float Mag_Advance, float max_angle, int num_sensors){
       
        AdvanceAngle gen_vector;
        int obs;
        int j;
        float left_side=0;
        float right_side=0;
        int value = 0;

        float Tolerance = 0.5;
        coord PosicionActual = dif_vectors(Position, Position);
        float origen_angle = -2.3561; 
        float range_angle = 4.7122;
        float delta_angle = range_angle/num_sensors; 
        float SiguientePaso =0; 
        float SiguienteAngulo = 0;

        //Constantes de campos potenciales 
        float d0 = 5;
        float epsilon1 = 1; 
        float epsilon2 = 1; 
        float eta = 0.5; 
        float delta0 = 0.5;
        float d1 = 5; 
       
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

       if (magnitude(dif_vectors(Position, Dest)) <= Tolerance){
              gen_vector = generate_output(STOP, Mag_Advance, max_angle); 
              printf("STOP\n");
              printf("\n **************** Destination Close Enough ******************************\n");
              }else{

              
       

       //Transformar las coordenadas del destino al sistema móvil del carrito
              coord DestinoTransformado; 
              DestinoTransformado = ProyectarCoordDestinoEnCarrito(Position, Dest); 
              printf("Las coordenadas del destino proyectadas al sistema del carrito son: x = %f, y = %f", DestinoTransformado.xc, DestinoTransformado.yc);

       //Calcular el campo atractivo
              coord CampoAtractivo; 
              CampoAtractivo = CalcularCampoAtractivo(PosicionActual, DestinoTransformado, d1, epsilon1, epsilon2);
              printf("Las coordenadas del campo atractivo son: Xatractivo = %f; Yatractivo = %f, tomando en cuenta el destino Xdestino = %f; Ydestino = %f y de Posición Inicial XInicial = %f; Yinicial = %f", CampoAtractivo.xc, CampoAtractivo.yc, DestinoTransformado.xc, DestinoTransformado.yc, PosicionActual.xc, PosicionActual.yc);

              coord CamposRepulsivos[num_sensors];

              for ( int i = 0; i<num_sensors; i++){
              float sensor_angle = origen_angle + i * delta_angle; 

              if(observations.sensors[i]<THRS_SENSOR){

                     CamposRepulsivos[i] = CalcularCampoRepulsivo(PosicionActual, ObtenerCoordenadasDeObstaculo(observations.sensors[i], sensor_angle), eta, d0); 

              }else{

                     CamposRepulsivos[i] = coord {0, 0, 0};

              }

                     printf("Campo repulsivo[%d]: X = %f, Y = %f\n", i, CamposRepulsivos[i].xc, CamposRepulsivos[i].yc);                        
              }

       //Calcular campo potencial 
              coord CampoPotencial; 
              CampoPotencial = CalcularCamporPotencial(CampoAtractivo, CamposRepulsivos, num_sensors); 
              printf("Las coordenadas del campo potencial son XPotencial=%f, YPotencial=%f", CampoPotencial.xc, CampoPotencial.yc);

       //Calcular y avanzar a siguiente coordenada
              coord PosicionNueva; 
              PosicionNueva = CalcularNuevaPosicion(CampoPotencial, delta0); 
              printf("La nueva coordenada respecto al carrito es: Xcarrito=%f, Ycarrito = %f", PosicionNueva.xc, PosicionNueva.yc);

              PosicionNueva.anglec = atan2(PosicionNueva.yc, PosicionNueva.xc);
              SiguientePaso = magnitude(PosicionNueva);

              if(Position.anglec-PosicionNueva.anglec < 0){//Girar a la derecha
                     gen_vector = generate_output(RIGHTADVANCE, SiguientePaso, abs(Position.anglec-PosicionNueva.anglec)); 
                     printf("Girando: %f grados \n", abs(Position.anglec-PosicionNueva.anglec)); 
                                printf("Avanzando: %f metros", SiguientePaso); 
                       }else{
                                gen_vector = generate_output(LEFTADVANCE, SiguientePaso, abs(Position.anglec-PosicionNueva.anglec)); 
                                printf("Girando: %f grados \n", abs(Position.anglec-PosicionNueva.anglec)); 
                                printf("Avanzando: %f metros", SiguientePaso);
                       }


       }
       

       
        return gen_vector;
       
       }


                 

