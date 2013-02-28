#include "Land.hpp"

void Land::writePixelRectangle(int x, int y, int w, int h, pixelType type, pixelForegroundType fType, bool changeFTypeOnlyIfPreviousIsSolid){
  // We possibly modify x, y, w, h to correct them is we're trying to write anything outside the land
  if(x < 0){ w += x; x = 0; }
  if(y < 0){ h += y; y = 0; }
  if(x + w >= width) w = width - x;
  if(y + h >= height) h = height - y;
  
  // We write
  for(int i = x; i < x + w; ++i){
    for(int j = y; j < y + h; ++j){
      writeSinglePixel(i, j, type, fType, changeFTypeOnlyIfPreviousIsSolid);
    }
  }
}

void Land::writeEverythingBetweenTwoOrientedIdenticalSquares(int x1, int y1, int x2, int y2, int size, pixelType type, pixelForegroundType fType, bool changeFTypeOnlyIfPreviousIsSolid){
  // If we're trying to draw a line, then we just draw a line
  if(size == 1)
    writePixelLine(x1, y1, x2, y2, type, fType);
  // Else, it's gonna be a bit more complex
  else{
    // Calculate points positions
    bool diagonalsAreOrientedUpLeftAndDownRight;
    int p1x, p2x, p1y, p2y;
    if(x1 < x2){
      if(y1 < y2){
        diagonalsAreOrientedUpLeftAndDownRight = true;
        p1x = x1; p1y = y1;
        p2x = x2 + size-1; p2y = y2 + size-1;
      }
      else{
        diagonalsAreOrientedUpLeftAndDownRight = false;
        p1x = x1; p1y = y1 + size-1;
        p2x = x2 + size-1; p2y = y2;
      }
    }
    else{
      if(y2 < y1){
        diagonalsAreOrientedUpLeftAndDownRight = true;
        p1x = x2; p1y = y2;
        p2x = x1 + size-1; p2y = y1 + size-1;
      }
      else{
        diagonalsAreOrientedUpLeftAndDownRight = false;
        p1x = x2; p1y = y2 + size-1;
        p2x = x1 + size-1; p2y = y1;
      }
    }
    // Draw
    if(diagonalsAreOrientedUpLeftAndDownRight){
      for(int i = 0; i < size; ++i)
        writePixelLine(p1x, p1y + i, p2x - i, p2y, type, fType, changeFTypeOnlyIfPreviousIsSolid);
      for(int i = 1; i < size; ++i)
        writePixelLine(p1x + i, p1y, p2x, p2y - i, type, fType, changeFTypeOnlyIfPreviousIsSolid);
    }
    else{
      for(int i = 0; i < size; ++i)
        writePixelLine(p1x, p1y - i, p2x - i, p2y, type, fType, changeFTypeOnlyIfPreviousIsSolid);
      for(int i = 1; i < size; ++i)
        writePixelLine(p1x + i, p1y, p2x, p2y + i, type, fType, changeFTypeOnlyIfPreviousIsSolid);
    }
  }
}

void Land::writeSinglePixel(int x, int y, pixelType type, pixelForegroundType fType, bool changeFTypeOnlyIfPreviousIsSolid){
  if(x >= 0 && y >= 0 && x < width && y < height){
    p[x][y].create(*this, x, y, type, fType, changeFTypeOnlyIfPreviousIsSolid);
    notifyForUpdatingAroundThisPixel(x, y);
  }
}

void Land::writePixelLine(int x1, int y1, int x2, int y2, pixelType type, pixelForegroundType fType, bool changeFTypeOnlyIfPreviousIsSolid){
  int dx, dy;
  
  if((dx = x2 - x1) != 0){
    if(dx > 0){
      if((dy = y2 - y1) != 0){
        if(dy > 0){
          // Oblique vector in the first quadrant
          if(dx >= dy){
            // Oblique vector in the first octant
            int e;
            dx = (e = dx) * 2;
            dy *= 2;
            bool stop = false;
            while(stop == false){ // Horizontal movement
              writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
              if((x1 = x1 + 1) == x2) stop = true;
              if((e -= dy) < 0){
                y1++; // Diagonal movement
                e += dx;
              }
            }
          }
          else{
            // Oblique vector in the second octant
            int e;
            dy = (e = dy) * 2;
            dx *= 2;
            bool stop = false;
            while(stop == false){ // Vertical movement
              writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
              if((y1 = y1 + 1) == y2) stop = true;
              if((e = e - dx) < 0){
                x1++; // Diagonal movement
                e += dy;
              }
            }
          }
        }
        else{ // dy < 0 (and dx > 0)
          // Oblique vector in the fourth quadrant
          if(dx >= -dy){
            // Oblique vector in the eighth octant
            int e;
            dx = (e = dx) * 2;
            dy *= 2;
            bool stop = false;
            while(stop == false){ // Horizontal movement
              writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
              if((x1 = x1 + 1) == x2) stop = true;
              if((e = e + dy) < 0){
                y1--; // Diagonal movement
                e += dx;
              }
            }
          }
          else{ // Oblique vector in the seventh octant
            int e;
            dy = (e = dy) * 2;
            dx *= 2;
            bool stop = false;
            while(stop == false){ // Vertical movement
              writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
              if((y1 = y1 - 1) == y2) stop = true;
              if((e = e + dx) > 0){
                x1++; // Diagonal movement
                e += dy;
              }
            }
          }
        }
      }
      else{ // dy = 0 (and dx > 0)
        // Horizontal vector to the right
        do{
          writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
        }while((x1 = x1 + 1) != x2);
      }
    }
    else{ // dx < 0
      if((dy = y2 - y1) != 0){
        if(dy > 0){
          // Oblique vector in the second quadrant
          if(-dx >= dy){
            // Oblique vector in the fourth octant
            int e;
            dx = (e = dx) * 2;
            dy *= 2;
            bool stop = false;
            while(stop == false){ // Horizontal movement
              writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
              if((x1 = x1 - 1) == x2) stop = true;
              if((e = e + dy) >= 0){
                y1++; // Diagonal movement
                e += dx;
              }
            }
          }
          else{
            // Oblique vector in the third octant
            int e;
            dy = (e = dy) * 2;
            dx *= 2;
            bool stop = false;
            while(stop == false){ // Vertical movement
              writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
              if((y1 = y1 + 1) == y2) stop = true;
              if((e = e + dx) <= 0){
                x1--; // Diagonal movement
                e += dy;
              }
            }
          }
        }
        else{ // dy < 0 (and dx < 0)
          // Oblique vector in the third quadrant
          if(dx <= dy){
            // Oblique vector in the fifth octant
            int e;
            dx = (e = dx) * 2;
            dy *= 2;
            bool stop = false;
            while(stop == false){ // Horizontal movement
              writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
              if((x1 = x1 - 1) == x2) stop = true;
              if((e = e - dy) >= 0){
                y1--; // Diagonal movement
                e += dx;
              }
            }
          }
          else{ // Oblique vector in the sixth octant
            int e;
            dy = (e = dy) * 2;
            dx = dx * 2;
            bool stop = false;
            while(stop == false){ // Vertical movement
              writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
              if((y1 = y1 - 1) == y2) stop = true;
              if((e = e - dx) >= 0){
                x1--; // Diagonal movement
                e += dy;
              }
            }
          }
        }
      }
      else{ // dy = 0 (and dx < 0)
        // Horizontal vector to the left
        do{
          writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
        }while((x1 = x1 - 1) != x2);
        
      }
    }
  }
  else{ // dx = 0
    if((dy = y2 - y1) != 0){
      if(dy > 0){
        do{
          writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
        }while((y1 += 1) != y2);
      }
      else{ // dy < 0 (and dx = 0)
        do{
          writeSinglePixel(x1, y1, type, fType, changeFTypeOnlyIfPreviousIsSolid);
        }while((y1 -= 1) != y2);
      }
    } 
  }
}
