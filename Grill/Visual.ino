byte upper_bound_of_scale = 190;
byte lower_bound_of_scale = 240;
byte width_of_object_pointer = 3;
byte object_in_percent = 0;

void Scale ()
{   
    if (graph_power < power) {
        myGLCD.setColor (VGA_LIME);
        myGLCD.fillRect (graph_power - width_of_object_pointer , lower_bound_of_scale, power, upper_bound_of_scale);
        graph_power = power;
    }
    if (graph_power > power) {
        myGLCD.setColor (VGA_BLACK);
        myGLCD.fillRect (power - width_of_object_pointer, lower_bound_of_scale, graph_power, upper_bound_of_scale);
        graph_power = power;
    }
    if (graph_object != object) {
        if (graph_object > graph_power) {
            myGLCD.setColor (VGA_BLACK);
            myGLCD.fillRect (graph_object - width_of_object_pointer, lower_bound_of_scale, graph_object, upper_bound_of_scale);
        } else {
            myGLCD.setColor (VGA_LIME);
            myGLCD.fillRect (graph_object - width_of_object_pointer, lower_bound_of_scale, graph_object, upper_bound_of_scale);
        }
        graph_object = object;
        if (graph_object != graph_power){
            myGLCD.setColor (VGA_RED);
            myGLCD.fillRect (graph_object - width_of_object_pointer, lower_bound_of_scale, graph_object, upper_bound_of_scale);
        }
    }
}

void Monitor ()
{
    if (graph_object != object) {
        object_in_percent = object * 100 / 255;
        myGLCD.setColor (VGA_LIME);
        //myGLCD.print("OBJECT: ", 20, 40);
        myGLCD.printNumI(object_in_percent, 190, 10, 2);
    }
    /*myGLCD.print("MODE: ", 20, 20);
    myGLCD.printNumI(mode, 130, 20);*/
  
    /*myGLCD.print("POWER: ", 20, 60);
    myGLCD.printNumI(power, 130, 60, 3);*/
    /*
    switch (result_of_scankeys) {
        case 0:
            myGLCD.print("            ", 20, 80);
            myGLCD.print("            ", 20, 100);
            break;
        case 1:
            myGLCD.print("The button A", 20, 80);
            myGLCD.print("is pressed:", 20, 100);
            break;
        case 2:
            myGLCD.print("The button B", 20, 80);
            myGLCD.print("is pressed:", 20, 100);
            break;
        case 3:
            myGLCD.print("The button C", 20, 80);
            myGLCD.print("is pressed:", 20, 100);
            break;
        case 4:
            myGLCD.print("The button D", 20, 80);
            myGLCD.print("is pressed:", 20, 100);
            break;
    }
  */
}
