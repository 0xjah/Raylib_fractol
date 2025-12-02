#include "mandelbrot.h"

double mandelbrot_minX = -2;
double mandelbrot_minY = -2;
double mandelbrot_maxX = 2;
double mandelbrot_maxY = 2;
double max_iter = MAX_ITER;
bool invert_colors = false;

double map(double value, double min_input, double max_input, double min_output, double max_output){
  return (value - min_input) * (max_output - min_output) / (max_input - min_input) + min_output; 
}

void getMandelbrotSet(double complex z, double complex c, int *iter_count, Vector2 *points){

  if(cabs(z) > 2) return; //magnitude of the complex number, mag = sqrt(r**r + img**img)
  
  if(*iter_count == max_iter) return;
  
  double complex z_ = (z * z) + c;
  
  points[*iter_count] = (Vector2) {.x = map(creal(z_),mandelbrot_minX,mandelbrot_maxX,0,SCREEN_WIDTH) , .y = map(cimag(z_),mandelbrot_minY,mandelbrot_maxY,0,SCREEN_HEIGHT) }; 
  
  *(iter_count) += 1;
  getMandelbrotSet(z_,c, iter_count,points);
}

int getIterationCount(double complex c){
  double complex z = 0.0 + 0.0 * I;
  int iter_cnt = -1; // if the iteration count goes beyond MAX_ITER

  for(int i = 0; i < max_iter; ++i){
    double complex z_ = (z * z) + c;
    z = z_;
    if(cabs(z) > 2) return i; //magnitude of the complex number, mag = sqrt(r**r + img**img)
  }
  return iter_cnt;
}

Color getGruvboxColor(int iter_count, int max_iter){
  if(iter_count == -1){
    // Points in the set - use darkest Gruvbox color
    return (Color){29, 32, 33, 255}; // bg_h
  }
  
  // Map iteration count to Gruvbox palette
  double ratio = (double)iter_count / (double)max_iter;
  int color_index = (int)(ratio * (NUM_GRUVBOX_COLORS - 1));
  
  if(color_index >= NUM_GRUVBOX_COLORS){
    color_index = NUM_GRUVBOX_COLORS - 1;
  }
  
  return gruvbox_colors[color_index];
}

Color getGruvboxColorSmooth(int iter_count, int max_iter){
  if(iter_count == -1){
    // Points in the set
    return invert_colors ? (Color){251, 241, 199, 255} : (Color){29, 32, 33, 255};
  }
  
  double ratio = (double)iter_count / (double)max_iter;
  
  // Invert ratio if invert_colors is true
  if(invert_colors){
    ratio = 1.0 - ratio;
  }
  
  double scaled = ratio * (NUM_GRUVBOX_COLORS - 1);
  int color_index = (int)scaled;
  double t = scaled - color_index;
  
  if(color_index >= NUM_GRUVBOX_COLORS - 1){
    return gruvbox_colors[NUM_GRUVBOX_COLORS - 1];
  }
  
  Color c1 = gruvbox_colors[color_index];
  Color c2 = gruvbox_colors[color_index + 1];
  
  return (Color){
    (unsigned char)(c1.r + t * (c2.r - c1.r)),
    (unsigned char)(c1.g + t * (c2.g - c1.g)),
    (unsigned char)(c1.b + t * (c2.b - c1.b)),
    255
  };
}

void drawMandelbrotFractal(Color *pixels){

  for(int i = 0; i < SCREEN_WIDTH; ++i){
    for(int j = 0; j < SCREEN_HEIGHT; ++j){
        //DrawPixel(i,j,pixels[i * SCREEN_WIDTH + j]);
        DrawRectangle(i,j,1,1,pixels[i * SCREEN_WIDTH + j]);
      }
  }
}


void generateMandelbrotFractal(Color *pixels){
  for(int i = 0; i < SCREEN_WIDTH; ++i){
    for(int j = 0; j < SCREEN_HEIGHT; ++j){
      double x_m = map(i,0,SCREEN_WIDTH,mandelbrot_minX,mandelbrot_maxX);
      double y_m = map(j,0,SCREEN_HEIGHT,mandelbrot_minY,mandelbrot_maxY);

      double complex c = x_m + y_m * I;

      int iter_count = getIterationCount(c);
      
      Color color = getGruvboxColorSmooth(iter_count, max_iter);
      
      pixels[i * SCREEN_WIDTH + j] = color;
    }
  }
}



void drawMandelbrotPoints(Vector2 *points, int iter_cnt){
  for(int i = 0; i < iter_cnt; ++i){
    Color c = {254, 128, 25, 255}; 
		DrawCircleV(points[i], 3, c);
  }

}

void zoomMandelbrot(double mouseX, double mouseY, float zoom_factor){
  double mouseX_m = map(mouseX,0,SCREEN_WIDTH,mandelbrot_minX,mandelbrot_maxX);
  double mouseY_m = map(mouseY,0,SCREEN_HEIGHT,mandelbrot_minY,mandelbrot_maxY);

  double x_range = (mandelbrot_maxX - mandelbrot_minX) * zoom_factor;
  double y_range = (mandelbrot_maxY - mandelbrot_minY) * zoom_factor;

  mandelbrot_minX = mouseX_m - x_range/2;
  mandelbrot_minY = mouseY_m - y_range/2;

  mandelbrot_maxX = mouseX_m + x_range/2;
  mandelbrot_maxY = mouseY_m + y_range/2;

}

int main(void){
  InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"Gruvbox Mandelbrot");
  SetTargetFPS(60);

  double complex z = 0 + 0 * I;

  int iter_cnt = 0;
  Vector2 *points = calloc(10000, sizeof(Vector2)); // Allocate enough for maximum iterations

  Color *pixels = calloc(SCREEN_HEIGHT * SCREEN_WIDTH, sizeof(Color));

  generateMandelbrotFractal(pixels);

  Color gruvbox_bg = {40, 40, 40, 255};       
  Color gruvbox_blue = {131, 165, 152, 255};  
  Color gruvbox_fg = {235, 219, 178, 255};    
  Color gruvbox_orange = {254, 128, 25, 255}; 

  while(!WindowShouldClose()){
    BeginDrawing();
      ClearBackground(gruvbox_bg);
      DrawLine(0,SCREEN_WIDTH/2, SCREEN_WIDTH,SCREEN_HEIGHT/2, gruvbox_blue); // Horizontal
      DrawLine(SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2,SCREEN_HEIGHT, gruvbox_blue); // Vertical
      
      DrawCircleLines(400,400,400,gruvbox_blue);

      if(IsMouseButtonDown(0)){
        iter_cnt = 0;
        Vector2 mouse_pos = GetMousePosition(); // in image grid coordinates
      
        double mouse_pos_mX = map(mouse_pos.x,0,SCREEN_WIDTH,mandelbrot_minX,mandelbrot_maxX);
        double mouse_pos_mY = map(mouse_pos.y,0,SCREEN_HEIGHT,mandelbrot_minY,mandelbrot_maxY);

        double complex c = mouse_pos_mX+ mouse_pos_mY * I;
        getMandelbrotSet(z,c,&iter_cnt, points);

        DrawText(TextFormat("c = %f + %f I", mouse_pos_mX, mouse_pos_mY),mouse_pos.x,mouse_pos.y,20,gruvbox_fg);
        DrawText(TextFormat("Iter = %d",iter_cnt),600, 50,20,gruvbox_orange);
      }

      if(IsKeyPressed(KEY_UP)){
        Vector2 mouse_pos = GetMousePosition();
        float zoom_factor = 0.5f;
        zoomMandelbrot(mouse_pos.x,mouse_pos.y,zoom_factor);
        generateMandelbrotFractal(pixels);
      }
      if(IsKeyPressed(KEY_DOWN)){
        Vector2 mouse_pos = GetMousePosition();
        float zoom_factor = 2.0f;
        zoomMandelbrot(mouse_pos.x,mouse_pos.y,zoom_factor);
        generateMandelbrotFractal(pixels);
      }
      if(IsKeyPressed(KEY_R)){
        // Reset view
        mandelbrot_minX = -2;
        mandelbrot_minY = -2;
        mandelbrot_maxX = 2;
        mandelbrot_maxY = 2;
        generateMandelbrotFractal(pixels);
      }
      if(IsKeyPressed(KEY_I)){
        // Increase max iterations
        max_iter = max_iter + 500;
        generateMandelbrotFractal(pixels);
      }
      if(IsKeyPressed(KEY_D)){
        // Decrease max iterations (minimum 50)
        if(max_iter > 50){
          max_iter = max_iter - 500;
          generateMandelbrotFractal(pixels);
        }
      }
      if(IsKeyPressed(KEY_C)){
        // Toggle color inversion
        invert_colors = !invert_colors;
        generateMandelbrotFractal(pixels);
      }
      
      drawMandelbrotFractal(pixels);
      drawMandelbrotPoints(points,iter_cnt);
      
      DrawText("UP: Zoom In | DOWN: Zoom Out | R: Reset | I: More Detail | D: Less Detail | C: Invert Colors", 10, 10, 13, gruvbox_fg);
      DrawText(TextFormat("Max Iterations: %d", (int)max_iter), 10, 30, 15, gruvbox_orange);
      
    EndDrawing();
  }
  
  free(pixels);
  free(points);
  CloseWindow();
  
  return 0;
}
