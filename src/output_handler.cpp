/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/lite/micro/examples/hello_world/output_handler.h"
#include "tensorflow/lite/micro/examples/hello_world/constants.h"

//#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240

// The colors we'll draw
const uint16_t background_color = BLACK;  // Red
const uint16_t foreground_color = RED;  // Red

// The size of the dot we'll draw
const int dot_radius = 3;
// Track whether the function has run at least once
bool initialized = false;
// Size of the drawable area
int width;
int height;
// Midpoint of the y axis
int midpoint;
// Pixels per unit of x_value
int x_increment;

void HandleOutput(tflite::ErrorReporter* error_reporter, float x_value,
                  float y_value) {
  // Do this only once
  if (!initialized) {
    // Calculate the drawable area to avoid drawing off the edges
//    width = (int)lcd_get_width() - (dot_radius * 2);
//    height = (int)lcd_get_height() - (dot_radius * 2);
    width = WIDTH - (dot_radius * 2);
    height = HEIGHT - (dot_radius * 2);
    // Calculate the y axis midpoint
    midpoint = height / 2;
    // Calculate fractional pixels per unit of x_value
    x_increment = static_cast<float>(width) / kXrange;
    initialized = true;
  }

  // Log the current X and Y values
  error_reporter->Report("x_value: %f, y_value: %f\n", x_value, y_value);

  // Clear the previous drawing
  lcd_clear(background_color);

  // Calculate x position, ensuring the dot is not partially offscreen,
  // which causes artifacts and crashes
  int x_pos = dot_radius + static_cast<int>(x_value * x_increment);

  // Calculate y position, ensuring the dot is not partially offscreen
  int y_pos;
  if (y_value >= 0) {
    // Since the display's y runs from the top down, invert y_value
    y_pos = dot_radius + static_cast<int>(midpoint * (1.f - y_value));
  } else {
    // For any negative y_value, start drawing from the midpoint
    y_pos =
        dot_radius + midpoint + static_cast<int>(midpoint * (0.f - y_value));
  }

  // Draw the dot
  lcd_fill_rectangle(x_pos - dot_radius, y_pos - dot_radius,
                     x_pos + dot_radius, y_pos + dot_radius,
                     foreground_color);
}
