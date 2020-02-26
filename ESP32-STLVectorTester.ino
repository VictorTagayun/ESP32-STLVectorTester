#include "Matrix.h"
#include "DACGraphics.h"
#include "Mesh.h"

#include <driver/dac.h>
#include <soc/rtc.h>
#include <soc/sens_reg.h>
#include "DataTable.h"

//Variables
int           lastx, lasty;
unsigned long currentMillis  = 0;
unsigned long previousMillis = 0;
int           Timeout        = 20;
const    long interval       = 990; //milliseconds, you should twick this
//to get a better accuracy

namespace mesh0
{
#include "VictorTagayun.h"
}
namespace mesh1
{
#include "Victor1.h"
}
namespace mesh2
{
#include "ASMLogowText.h"
}
namespace mesh3
{
#include "ASMLogo3.h"
}
namespace mesh4
{
#include "V.h"
}
DACGraphics graphics;

Mesh<DACGraphics> VictorTagayun(mesh0::vertexCount, mesh0::vertices, mesh0::edgeCount, mesh0::edges);
Mesh<DACGraphics> Victor1(mesh1::vertexCount, mesh1::vertices, mesh1::edgeCount, mesh1::edges);
Mesh<DACGraphics> ASMLogowText(mesh2::vertexCount, mesh2::vertices, mesh2::edgeCount, mesh2::edges);
Mesh<DACGraphics> ASMLogo3(mesh3::vertexCount, mesh3::vertices, mesh3::edgeCount, mesh3::edges);
Mesh<DACGraphics> V(mesh4::vertexCount, mesh4::vertices, mesh4::edgeCount, mesh4::edges);

void setup()
{
  Serial.begin(115200);

  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);
  Serial.println("CPU Clockspeed: ");
  Serial.println(rtc_clk_cpu_freq_value(rtc_clk_cpu_freq_get()));

  graphics.init();

}

Matrix perspective = Matrix::translation(120, 120, 0) * Matrix::scaling(120) * Matrix::perspective(90, 1, 10);

bool going_up, going_down, triggered = false;
int seq;
static float u = 0;

void loop()
{
  //  int part = (millis() / 5000) & 1;
  //
  //  if ((part == 1) && (going_up == true))
  //  {
  //    going_up = false;
  //    triggered = true;
  //  }
  //
  //  if ((part == 0) && (going_up == false))
  //    going_up = true;
  //
  //  if ((part == 1) && (triggered == true))
  //  {
  //    seq++;
  //    triggered = false;
  //    if (seq >= 4)
  //      seq = 0;
  //  }
  //  switch (seq)
  //  {
  //    case 0:
  //      {
  //        static float u = 0;
//  u += 0.02;
//  Matrix m0 = perspective *  Matrix::translation(0, 0, 6) * Matrix::rotation(3.1416, 1, 0, 0) * Matrix::rotation(3.1416, 0, 1, 0) * Matrix::scaling(2);
//  V.transform(m0);
//  graphics.begin();
//  V.drawVertices(graphics);
//  graphics.dot(255, 255);
//  graphics.end();
  //        break;
  //      }
  //    case 1:
  //      {
//          Matrix m0 = perspective * Matrix::translation(0, 0, 4.5) * Matrix::rotation(0, 0, 0, 0) * Matrix::rotation(3.1416, 0, 0, 1) * Matrix::scaling(7);
//          ASMLogowText.transform(m0);
//          graphics.begin();
//          ASMLogowText.drawEdges(graphics);
//          graphics.dot(255, 255);
//          graphics.end();
  //        break;
  //      }
  //    case 2:
  //      {
          u += 0.03;
          Matrix m0 = perspective * Matrix::translation(0, 0, 6) * Matrix::rotation(u, 0, 1, 0) * Matrix::rotation(3.1416, 0, 0, 1) * Matrix::scaling(8);
          ASMLogowText.transform(m0);
          graphics.begin();
          ASMLogowText.drawEdges(graphics);
          graphics.dot(255, 255);
          graphics.end();
  //        break;
  //      }
  //    case 2:
  //      {
  //        static float u = 0;
  //        u += 0.02;
  //        Matrix m0 = perspective *  Matrix::translation(0, -1, 5.5) * Matrix::rotation(3.6, 1, 0, 0) * Matrix::rotation(u, 0, 1, 0) * Matrix::scaling(7);
  //        victor.transform(m0);
  //        graphics.begin();
  //        victor.drawVertices(graphics);
  //        graphics.dot(255, 255);
  //        graphics.end();
  //        break;
  //      }
  //    case 3:
  //      {
  //        static float u = 0;
  //        static float v = 0;
  //        static float w = 0;
  //        u += 0.01;
  //        v += 0.03;
  //        w += 0.05;
  //        Matrix m0 = perspective * Matrix::translation(0, 0, 5.5) * Matrix::rotation(u, 1, 0, 0) * Matrix::rotation(v, 0, 1, 0) * Matrix::rotation(w, 0, 0, 1) * Matrix::scaling(7);
  //        victor.transform(m0);
  //        graphics.begin();
  //        victor.drawVertices(graphics);
  //        graphics.dot(255, 255);
  //        graphics.end();
  //        break;
  //      }
  //  }
  //just to get a clean trigger
  while (millis() & 31);
}

//*****************************************************************************
// PlotTable
//*****************************************************************************

void PlotTable(byte *SubTable, int SubTableSize, int skip, int opt, int offset)
{
  int i = offset;
  while (i < SubTableSize) {
    if (SubTable[i + 2] == skip) {
      i = i + 3;
      if (opt == 1) if (SubTable[i] == skip) i++;
    }
    Line(SubTable[i], SubTable[i + 1], SubTable[i + 2], SubTable[i + 3]);
    if (opt == 2) {
      Line(SubTable[i + 2], SubTable[i + 3], SubTable[i], SubTable[i + 1]);
    }
    i = i + 2;
    if (SubTable[i + 2] == 0xFF) break;
  }
}

// End PlotTable
//*****************************************************************************



//*****************************************************************************
// Dot
//*****************************************************************************

inline void Dot(int x, int y)
{
  if (lastx != x) {
    lastx = x;
    dac_output_voltage(DAC_CHANNEL_1, x);
  }
#if defined EXCEL
  Serial.print("0x");
  if (x <= 0xF) Serial.print("0");
  Serial.print(x, HEX);
  Serial.print(",");
#endif
#if defined EXCEL
  Serial.print("0x");
  if (lasty <= 0xF) Serial.print("0");
  Serial.print(lasty, HEX);
  Serial.println(",");
#endif
  if (lasty != y) {
    lasty = y;
    dac_output_voltage(DAC_CHANNEL_2, y);
  }
#if defined EXCEL
  Serial.print("0x");
  if (x <= 0xF) Serial.print("0");
  Serial.print(x, HEX);
  Serial.print(",");
#endif
#if defined EXCEL
  Serial.print("0x");
  if (y <= 0xF) Serial.print("0");
  Serial.print(y, HEX);
  Serial.println(",");
#endif
}

// End Dot
//*****************************************************************************



//*****************************************************************************
// Line
//*****************************************************************************
// Bresenham's Algorithm implementation optimized
// also known as a DDA - digital differential analyzer

void Line(byte x1, byte y1, byte x2, byte y2)
{
  int acc;
  // for speed, there are 8 DDA's, one for each octant
  if (y1 < y2) { // quadrant 1 or 2
    byte dy = y2 - y1;
    if (x1 < x2) { // quadrant 1
      byte dx = x2 - x1;
      if (dx > dy) { // < 45
        acc = (dx >> 1);
        for (; x1 <= x2; x1++) {
          Dot(x1, y1);
          acc -= dy;
          if (acc < 0) {
            y1++;
            acc += dx;
          }
        }
      }
      else {   // > 45
        acc = dy >> 1;
        for (; y1 <= y2; y1++) {
          Dot(x1, y1);
          acc -= dx;
          if (acc < 0) {
            x1++;
            acc += dy;
          }
        }
      }
    }
    else {  // quadrant 2
      byte dx = x1 - x2;
      if (dx > dy) { // < 45
        acc = dx >> 1;
        for (; x1 >= x2; x1--) {
          Dot(x1, y1);
          acc -= dy;
          if (acc < 0) {
            y1++;
            acc += dx;
          }
        }
      }
      else {  // > 45
        acc = dy >> 1;
        for (; y1 <= y2; y1++) {
          Dot(x1, y1);
          acc -= dx;
          if (acc < 0) {
            x1--;
            acc += dy;
          }
        }
      }
    }
  }
  else { // quadrant 3 or 4
    byte dy = y1 - y2;
    if (x1 < x2) { // quadrant 4
      byte dx = x2 - x1;
      if (dx > dy) {  // < 45
        acc = dx >> 1;
        for (; x1 <= x2; x1++) {
          Dot(x1, y1);
          acc -= dy;
          if (acc < 0) {
            y1--;
            acc += dx;
          }
        }

      }
      else {  // > 45
        acc = dy >> 1;
        for (; y1 >= y2; y1--) {
          Dot(x1, y1);
          acc -= dx;
          if (acc < 0) {
            x1++;
            acc += dy;
          }
        }

      }
    }
    else {  // quadrant 3
      byte dx = x1 - x2;
      if (dx > dy) { // < 45
        acc = dx >> 1;
        for (; x1 >= x2; x1--) {
          Dot(x1, y1);
          acc -= dy;
          if (acc < 0) {
            y1--;
            acc += dx;
          }
        }

      }
      else {  // > 45
        acc = dy >> 1;
        for (; y1 >= y2; y1--) {
          Dot(x1, y1);
          acc -= dx;
          if (acc < 0) {
            x1--;
            acc += dy;
          }
        }
      }
    }

  }
}

// End Line
//*****************************************************************************
