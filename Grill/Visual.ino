byte upper_bound_of_scale = 193;
byte lower_bound_of_scale = 234;
byte width_of_object_pointer = 3;
byte object_in_percent = 0;
byte graph_power = 0;//мощность, которая в данный момент выводится на экран
byte graph_object = 0;//цель, которая в данный момент выводится на экран
byte scale_flag = 1;

void Scale ()
{   
    if (scale_flag == 1) {
        myGLCD.setColor(VGA_YELLOW);
        myGLCD.drawRect (0,188,268,239);
        myGLCD.drawRect (1,189,267,238);
        scale_flag = 0;
    }
    
    if (graph_power < power) {
        myGLCD.setColor (VGA_LIME);
        myGLCD.fillRect (graph_power - width_of_object_pointer + 8, lower_bound_of_scale, power + 8, upper_bound_of_scale);
        graph_power = power;
    }
    if (graph_power > power) {
        myGLCD.setColor (VGA_BLACK);
        myGLCD.fillRect (power - width_of_object_pointer + 8, lower_bound_of_scale, graph_power + 8, upper_bound_of_scale);
        graph_power = power;
    }
    if (graph_object != object) {
        if (graph_object > graph_power) {
            myGLCD.setColor (VGA_BLACK);
            myGLCD.fillRect (graph_object - width_of_object_pointer + 8, lower_bound_of_scale, graph_object + 8, upper_bound_of_scale);
        } else {
            myGLCD.setColor (VGA_LIME);
            myGLCD.fillRect (graph_object - width_of_object_pointer + 8, lower_bound_of_scale, graph_object + 8, upper_bound_of_scale);
        }
        graph_object = object;
        if (graph_object != graph_power){
            myGLCD.setColor (VGA_RED);
            myGLCD.fillRect (graph_object - width_of_object_pointer + 8, lower_bound_of_scale, graph_object + 8, upper_bound_of_scale);
        }
    }
    
}

void Monitor ()
{
    if (graph_object != object) {
        object_in_percent = object * 100 / 255;
        if (object_in_percent == 100)
            object_in_percent = 99;
        myGLCD.setColor (VGA_RED);
        //myGLCD.print("OBJECT: ", 20, 40);
        myGLCD.printNumI(object_in_percent, 270, 191, 2);
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
