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

        float Tolerance = 0.05;
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
        float eta = 2; 
        float delta0 = 0.04;
        float d1 = 10; 

        //Constantes de triangulacion 
        coord Antena1; 
        coord Antena2; 
        coord Antena3; 
        float VarianzaRuido = 0.0;
       
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
              printf("Las coordenadas del destino originales son x = %f, y = %f\n", Dest.xc, Dest.yc);
              printf("Las condiciones iniciales del carrito son x = %f, y = %f, orientacion = %f\n", Position.xc, Position.yc, Position.anglec);
              printf("Las coordenadas del destino proyectadas al sistema del carrito son: x = %f, y = %f\n", DestinoTransformado.xc, DestinoTransformado.yc);

       //Calcular el campo atractivo
              coord CampoAtractivo; 
              CampoAtractivo = CalcularCampoAtractivo(PosicionActual, DestinoTransformado, d1, epsilon1, epsilon2);
              printf("Las coordenadas del campo atractivo son: Xatractivo = %f; Yatractivo = %f, tomando en cuenta el destino Xdestino = %f; Ydestino = %f y de Posición Inicial XInicial = %f; Yinicial = %f\n", CampoAtractivo.xc, CampoAtractivo.yc, DestinoTransformado.xc, DestinoTransformado.yc, PosicionActual.xc, PosicionActual.yc);

       // Calcular Centroide de Obstáculo
              coord ObstaculosDerecha[num_sensors/2];
              coord ObstaculosIzquierda[num_sensors/2];

              int cuenta1 = 0, cuenta2 = 0;

              for (int i = 0; i < num_sensors; i++) {
                  float sensor_angle = origen_angle + i * delta_angle;
              
                  if (observations.sensors[i] < THRS) {
                      coord Obstaculo = ObtenerCoordenadasDeObstaculo(observations.sensors[i], sensor_angle);
              
                      if (i < num_sensors / 2) {
                          if (cuenta1 < num_sensors / 2) {
                              ObstaculosDerecha[cuenta1++] = Obstaculo;
                          }
                      } else {
                          if (cuenta2 < num_sensors / 2) {
                              ObstaculosIzquierda[cuenta2++] = Obstaculo;
                          }
                      }
                  }
              }

              // Calcular centroides de cada lado
              coord CentroideDerecho = {0, 0, 0};
              coord CentroideIzquierdo = {0, 0, 0};

              if (cuenta1 > 0) {
                  CentroideDerecho = ObtenerCoordenadasDeCentroideObstaculo(ObstaculosDerecha, cuenta1);
              }
              if (cuenta2 > 0) {
                  CentroideIzquierdo = ObtenerCoordenadasDeCentroideObstaculo(ObstaculosIzquierda, cuenta2);
              }

              // Mostrar centroides
              printf("Centroide del obstáculo a la derecha: X = %f, Y = %f\n", CentroideDerecho.xc, CentroideDerecho.yc);
              printf("Centroide del obstáculo a la izquierda: X = %f, Y = %f\n", CentroideIzquierdo.xc, CentroideIzquierdo.yc);

       // Calcular el campo repulsivo
              coord CamposRepulsivos[2] = {{0, 0, 0}, {0, 0, 0}};

              if (cuenta1 > 0) {
                  CamposRepulsivos[0] = CalcularCampoRepulsivo(PosicionActual, CentroideDerecho, eta, d0);
              }
              if (cuenta2 > 0) {
                  CamposRepulsivos[1] = CalcularCampoRepulsivo(PosicionActual, CentroideIzquierdo, eta, d0);
              }

              // Mostrar fuerzas repulsivas
              for (int i = 0; i < 2; i++) {
                  printf("Campo repulsivo[%d]: X = %f, Y = %f\n", i, CamposRepulsivos[i].xc, CamposRepulsivos[i].yc);
              }



       //Calcular campo potencial 
             coord CampoPotencial = CalcularCamporPotencial(CampoAtractivo, CamposRepulsivos, 2); 
              printf("Las coordenadas del campo potencial son XPotencial=%f, YPotencial=%f\n", CampoPotencial.xc, CampoPotencial.yc);

       //Calcular y avanzar a siguiente coordenada
              coord PosicionNueva = CalcularNuevaPosicion(CampoPotencial, delta0); 
              printf("La nueva coordenada respecto al carrito es: Xcarrito=%f, Ycarrito = %f\n", PosicionNueva.xc, PosicionNueva.yc);

              PosicionNueva.anglec = atan2(PosicionNueva.yc, PosicionNueva.xc);
              SiguientePaso = magnitude(PosicionNueva);
              
              gen_vector = generate_output(LEFTADVANCE, SiguientePaso, PosicionNueva.anglec); 

              printf("Girando %f grados y avanzando %f metros\n", PosicionNueva.anglec, SiguientePaso); 
       
       //Triangulación de la posición del carrito
              //Posiciones de antenas       
              Antena1.xc = 0.1; 
              Antena1.yc = 0.1; 

              printf("La posición de la primera antena es: X = %f, Y = %f\n", Antena1.xc, Antena1.yc); 

              Antena2.xc = 1; 
              Antena2.yc = 1.8; 

              printf("La posición de la segunda antena es: X = %f, Y = %f\n", Antena2.xc, Antena2.yc); 

              Antena3.xc = 1.9; 
              Antena3.yc = 0.1;

              printf("La posición de la tercera antena es: X = %f, Y = %f\n", Antena3.xc, Antena3.yc); 

              //Radios
              float Radio1 = CalcularMagnitudAntena_Carrito(Antena1, Position,VarianzaRuido); 
              float Radio2 = CalcularMagnitudAntena_Carrito(Antena2, Position, VarianzaRuido);
              float Radio3 = CalcularMagnitudAntena_Carrito(Antena3, Position, VarianzaRuido);

              printf("La magnitud de la antena1 es %f\n", Radio1); 
              printf("La magnitud de la antena2 es %f\n", Radio2);
              printf("La magnitud de la antena3 es %f\n", Radio3);

              //Triangulacion
              coord PosicionTriangulada = TriangularPosicion(Antena1, Radio1, Antena2, Radio2, Antena3, Radio3); 

              printf("La posición del carrito es: X = %f, Y = %f\n", PosicionTriangulada.xc, PosicionTriangulada.yc);

              //Creacion de archivos para tomar muestras 
              // Abrir archivo en modo append para no sobreescribir
              /*FILE *archivo = fopen("SensoresConRuido.csv", "a");
              if (archivo != NULL) {
                  // Si el archivo está vacío, escribe el encabezado (opcional, solo la primera vez)
                  fseek(archivo, 0, SEEK_END);
                  if (ftell(archivo) == 0) {
                      fprintf(archivo, "x,y,theta,sensor_id,sensor_value\n");
                  }
           
                  // Guardar una línea por cada sensor
                  for (int i = 0; i < num_sensors; i++) {
                      fprintf(archivo, "%f,%f,%f,%d,%f\n", 
                              Position.xc, Position.yc, Position.anglec, 
                              i + 1, observations.sensors[i]);
                  }
           
                  fclose(archivo);
              } else {
                  printf("Error al abrir archivo para guardar datos.\n");
              }*/


       }       

       
        return gen_vector;
       
}


                 

