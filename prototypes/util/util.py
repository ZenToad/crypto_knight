from pyray import *

colors = [
LIGHTGRAY,
GRAY,
DARKGRAY,
YELLOW,  
GOLD,       
ORANGE,     
PINK,       
RED,        
MAROON,     
GREEN,      
LIME,       
DARKGREEN,  
SKYBLUE,    
BLUE,       
DARKBLUE,   
PURPLE,     
VIOLET,     
DARKPURPLE, 
BEIGE,      
BROWN,      
DARKBROWN,  
WHITE,      
BLACK,      
BLANK,      
MAGENTA,    
RAYWHITE]  

def get_random_color():
    return colors[get_random_value(0, len(colors)-1)]
