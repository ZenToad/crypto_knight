from pyray import *
import prototypes.util.util as util

rows = 8
cols = 8

#  Function to display the grid
def display_grid(grid):
    for row in grid:
        print(' '.join(map(str, row)))
    print()


def draw_grid(grid, offsetx, offsety, width, height):

    for i, row in enumerate(grid):
        for j, cell in enumerate(row):
            draw_rectangle(offsetx + j * width, offsety + i * height, width, height, grid[i][j])

    for i, row in enumerate(grid):
        draw_line(offsetx + i * width, offsety, offsetx + i*width, offsety + len(row) * height, RED)
        for j, cell in enumerate(row):
            draw_line(offsetx, offsety + j * height, offsetx + len(row) * width, offsety + j * height, RED)



def test_proto1():
    window_width = 800
    window_height = 450
    init_window(window_width, window_height, "Hello Pyray")
    set_target_fps(60)
    print(WHITE)

    grid = [[BLACK for _ in range(cols)] for _ in range(rows)]

    # Display the grid
    delta = 0
    while not window_should_close():
        delta += 1
        if delta % 60 == 0:
            grid[get_random_value(0, rows-1)][get_random_value(0, cols-1)] = util.get_random_color()

        # Update
        # ------------------------------------------
        # there is nothing magical about clicking to move
        # can we enter a sequence of keys to move the player?
        # 
        #  if is_key_down(KEY_A):
        #      grid_scalar = 4
        #  elif is_key_down(KEY_S):
        #      grid_scalar = 3
        #  elif is_key_down(KEY_D):
        #      grid_scalar = 2
        #  elif is_key_down(KEY_F):
        #      grid_scalar = 1
        #
        #  if is_key_pressed(KEY_K):
        #      playery -= 20 * grid_scalar
        #  if is_key_pressed(KEY_J):
        #      playery += 20 * grid_scalar
        #  if is_key_pressed(KEY_H):
        #      playerx -= 20 * grid_scalar
        #  if is_key_pressed(KEY_L):
        #      playerx += 20 * grid_scalar
        #
        #  if playerx < 0:
        #      playerx = 0
        #  if playerx > 800 - playerw:
        #      playerx = 800 - playerw
        #  if playery < 0:
        #      playery = 0
        #  if playery > 450 - playerh:
        #      playery = 450 - playerh
        # ------------------------------------------

        # Draw
        # ------------------------------------------
        begin_drawing()
        clear_background(BLACK)
        draw_grid(grid, window_width // 4, window_height // 8, 42, 42)

        #  draw_rectangle(playerx, playery, playerw, playerh, BLUE)
        #  draw_rectangle(enemyx, enemyy, enemyw, enemyh, RED)


        end_drawing()
        ## ------------------------------------------

    close_window()
