#ifndef FRACTOL_H
#define FRACTOL_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<complex.h>
#include<raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAX_ITER 1000

static Color gruvbox_colors[] = {
    {29, 32, 33, 255},      
    {40, 40, 40, 255},      
    {60, 56, 54, 255},      
    {80, 73, 69, 255},      
    {102, 92, 84, 255},     
    {124, 111, 100, 255},   
    {146, 131, 116, 255},   
    {168, 153, 132, 255},   
    {189, 174, 147, 255},   
    {213, 196, 161, 255},   
    {235, 219, 178, 255},   
    {251, 241, 199, 255},   
    {251, 73, 52, 255},     
    {254, 128, 25, 255},    
    {250, 189, 47, 255},    
    {184, 187, 38, 255},    
    {142, 192, 124, 255},   
    {131, 165, 152, 255},   
    {211, 134, 155, 255},   
};

#define NUM_GRUVBOX_COLORS (sizeof(gruvbox_colors) / sizeof(gruvbox_colors[0]))

// Global variables
extern double mandelbrot_minX;
extern double mandelbrot_minY;
extern double mandelbrot_maxX;
extern double mandelbrot_maxY;
extern double max_iter;
extern bool invert_colors;

// Function prototypes
double map(double value, double min_input, double max_input, double min_output, double max_output);
void getMandelbrotSet(double complex z, double complex c, int *iter_count, Vector2 *points);
int getIterationCount(double complex c);
Color getGruvboxColor(int iter_count, int max_iter);
Color getGruvboxColorSmooth(int iter_count, int max_iter);
void drawMandelbrotFractal(Color *pixels);
void generateMandelbrotFractal(Color *pixels);
void drawMandelbrotPoints(Vector2 *points, int iter_cnt);
void zoomMandelbrot(double mouseX, double mouseY, float zoom_factor);

#endif 
