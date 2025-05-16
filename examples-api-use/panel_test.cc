#include "led-matrix.h"
#include "graphics.h"
#include <iostream>
#include <unistd.h>  // For sleep()

using namespace rgb_matrix;
using namespace std;

// Random binary matrix (4 rows × 40 columns)
const int binary_rows = 4;
const int binary_cols = 40;

int binary_matrix[binary_rows][binary_cols] = {
    {1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1},
    {0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0},
    {1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1},
    {0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1}
};

int main(int argc, char *argv[]) {
    // Configuration for the LED matrix
    RGBMatrix::Options defaults;
    defaults.rows = 32;           // 32 rows per panel
    defaults.cols = 32;           // 32 columns per panel
    defaults.chain_length = 10;   // 10 panels chained (so total width = 320)
    defaults.parallel = 1;        // Number of parallel chains
    defaults.hardware_mapping = "adafruit-hat";  // Set your hardware mapping
    defaults.brightness = 100;
    defaults.multiplexing = 1;    // Multiplexing if needed

    // Parse command-line flags
    rgb_matrix::RuntimeOptions runtime_defaults;
    if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &defaults, &runtime_defaults)) {
        return 1;
    }

    // Create the LED matrix
    RGBMatrix *matrix = rgb_matrix::CreateMatrixFromOptions(defaults, runtime_defaults);
    if (matrix == NULL) {
        return 1;
    }

    // Block size is 8x8 pixels as per your requirement
    int block_height = 8;
    int block_width = 8;

    // Total rows and columns based on chain and parallel configuration
    int total_cols = defaults.cols * defaults.chain_length;
    int total_rows = defaults.rows * defaults.parallel;

    // Create a canvas to draw on
    rgb_matrix::FrameCanvas *canvas = matrix->CreateFrameCanvas();

    // Draw the binary matrix onto the canvas
    for (int row = 0; row < binary_rows; ++row) {
        for (int col = 0; col < binary_cols; ++col) {
            Color color;
            if (binary_matrix[row][col] == 1) {
                color = Color(255, 255, 255);  // White color for '1'
            } else {
                color = Color(0, 0, 0);        // Black color for '0'
            }

            // Draw the block
            int x_start = col * block_width;
            int y_start = row * block_height;

            for (int x = x_start; x < x_start + block_width; ++x) {
                for (int y = y_start; y < y_start + block_height; ++y) {
                    if (x >= 0 && x < total_cols && y >= 0 && y < total_rows) {
                        canvas->SetPixel(x, y, color.r, color.g, color.b);
                    }
                }
            }
        }
    }

    // Swap the canvas to display the image
    matrix->SwapOnVSync(canvas);

    // Keep the display on until interrupted
    cout << "Press CTRL-C to exit." << endl;
    while (true) {
        sleep(1);
    }

    // Clean up (unreachable code in this example)
    delete matrix;

    return 0;
}
