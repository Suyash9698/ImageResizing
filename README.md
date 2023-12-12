
# Image Resizing using Seam Carving

This project implements image resizing using seam carving, a content-aware image resizing technique.

## Overview

The program takes an input image, performs seam carving to resize the image to the specified dimensions, and saves the resized image as output_image.jpg.

## Requirements

- C++ compiler
- OpenCV library

## Usage

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/your-repo.git
   cd your-repo
Build the program:

## bash
g++ -o image_resizer image_resizer.cpp -lopencv_core -lopencv_imgcodecs -lopencv_highgui

## bash
./image_resizer
Follow the prompts to enter the image path, new height, and new width.

## Example

```bash
Enter the path of the image: /path/to/your/image.jpg
Enter the new height: 300
Enter the new width: 400
```


### Notes
Ensure that the image path is correct and the image exists.
The new height and width should be within a reasonable range.
Credits
This project was developed by Suyash Khare.

## Author & Coder:
### Mr. Suyash Khare



