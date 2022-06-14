#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <GFX4d.h>
#include <SPI.h>
#include <SD.h>

File configFile;

GFX4d gfx = GFX4d();

#define LTBLUE 0xB6DF
#define LTTEAL 0xBF5F
#define LTGREEN 0xBFF7
#define LTCYAN 0xC7FF
#define LTRED 0xFD34
#define LTMAGENTA 0xFD5F
#define LTYELLOW 0xFFF8
#define LTORANGE 0xFE73
#define LTPINK 0xFDDF
#define LTPURPLE 0xCCFF
#define LTGREY 0xE71C

#define BLUE 0x001F
#define TEAL 0x0438
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define ORANGE 0xFC00
#define PINK 0xF81F
#define PURPLE 0x8010
#define GREY 0xC618
#define WHITE 0xFFFF
#define BLACK 0x0000

#define DKBLUE 0x000D
#define DKTEAL 0x020C
#define DKGREEN 0x03E0
#define DKCYAN 0x03EF
#define DKRED 0x6000
#define DKMAGENTA 0x8008
#define DKYELLOW 0x8400
#define DKORANGE 0x8200
#define DKPINK 0x9009
#define DKPURPLE 0x4010
#define DKGREY 0x4A49

#define ymax 100

bool resetNow = false;
uint16_t tx = 0;

double ox, oy;

char ip[16];
char URI[20];

unsigned char state, stateBefore = 0; // 0 => Daydata, 1 => Weekdata, 2 => Monthdata, 3 => Yeardata
unsigned long timeBefore;
bool redrawNow = true;

bool readSetting(char *fileContent, int contentLength, const char *settingName, char *returnBuff, int returnLength)
{
  // strncpy(buf, key, l);
  char ContentBuf[contentLength];
  strncpy(ContentBuf, fileContent, sizeof(ContentBuf));
  char *saveptr1;
  char *saveptr2;

  // loop over \n tokens
  char *tok1 = strtok_r(ContentBuf, "\n", &saveptr1);
  while (tok1 != NULL)
  {
    // loop over = token
    char *tok2 = strtok_r(tok1, "=", &saveptr2);
    if (tok2 != NULL)
    {
      if (strcmp(tok2, settingName) == 0)
      {
        tok2 = strtok_r(NULL, "=", &saveptr2);
        if (tok2 != NULL)
        {
          strncpy(returnBuff, tok2, returnLength - 1);
        }
        return 0;
      }
    }

    tok1 = strtok_r(NULL, "\n", &saveptr1);
  }
  return true;
}

bool readFileContents(const char *fileName, char *content, int length)
{
  if (!SD.begin(4))
  {
    Serial.println(F("INITIALIZING SD CARD FAILED"));
    return false;
  }
  else
  {
    // Serial.println("INITIALIZING SD CARD COMPLETED");
    // Serial.println("OPENING FILE: " + String(fileName));
    configFile = SD.open(fileName);
    if (!configFile)
    {
      // Serial.println("ERROR OPENING FILE: " + String(fileName));
      return false;
    }
    else
    {
      int i = 0;
      char c;
      while (configFile.available())
      {
        c = configFile.read();
        if (i < (length - 1))
        {
          content[i] = c;
          i++;
        }
      }
      configFile.close();
      return true;
    }
  }
}

/*
   HTTP functions
*/

bool httpGETRequest(String serverIP, String serverURI, String parameters, char *returnBuff, int len)
{
  HTTPClient http;
  http.setTimeout(11000);
  String serverName = "http://" + serverIP + serverURI + parameters; // http expects a String, not a char *
  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "";

  if (httpResponseCode < 0)
    return 1;

  payload = http.getString(); // if server response is good than read payload

  // Free resources
  http.end(); // end http server connection

  if (!((payload.length() + 1) <= len))
    return 1;                                // if the return buffer is not the required length than give error code
  strncpy(returnBuff, payload.c_str(), len); // return payload in char * form
  return 0;
}

/*--------------------------------------
   This function converts ASCII numbers
   separated by a ',' to a char arrey
   that uses an unsigned char (number from 0-255)
   --------------------------------------
*/

bool convertToInt(char *Content, unsigned char *returnBuff, int len)
{
  // strncpy(buf, key, l);

  // loop over , tokens
  char *tok;
  int i = 0;
  while ((tok = strsep(&Content, ",")) != NULL && (i < len))
  {
    if (strlen(tok) == 0)
    {
      returnBuff[i] = ymax + 1;
    }
    else
    {
      returnBuff[i] = atoi(tok);
    }
    i++;
  }
  return 0;
}

/*
  GLCD functions

*/
void initializeGLCD(bool scrolEnabled, bool backlightOn, bool touchEnable, unsigned char OrientationLCD, unsigned char scrollSpeed)
{
  gfx.begin();
  gfx.Cls();
  gfx.ScrollEnable(scrolEnabled);
  gfx.BacklightOn(backlightOn);
  gfx.touch_Set(touchEnable);
  gfx.Orientation(OrientationLCD);
  gfx.SmoothScrollSpeed(scrollSpeed);
}

/*
  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn
  huge arguement list
  &d name of your display object
  x = x data point
  y = y data point
  gx = x graph location (lower left)
  gy = y graph location (lower left)
  w = width of graph
  h = height of graph
  xlo = lower bound of x axis (min x-axis)
  xhi = upper bound of x asis (max x-axis)
  xinc = division of x axis (distance not count)
  ylo = lower bound of y axis (min y-axis)
  yhi = upper bound of y asis (max y-axis)
  yinc = division of y axis (distance not count)
  title = title of graph
  xlabel = x asis label
  ylabel = y asis label
  gcolor = graph line colors
  acolor = axi ine colors
  pcolor = color of your plotted data
  tcolor = text color
  bcolor = background color
  &redraw = flag to redraw graph on fist call only
*/

void Graph(GFX4d &d, double x, double y, double gx, double gy, double w, double h, double xlo, double xhi, double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor, unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, boolean &redraw)
{

  double ydiv, xdiv;
  // initialize old x and old y in order to draw the first point of the graph
  // but save the transformed value
  // note my transform funcition is the same as the map function, except the map uses long and we need doubles
  // static double ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
  // static double oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  int i;
  double temp;
  int rot, newrot;

  if (redraw == true)
  {
    redraw = false;
    d.Cls();
    ox = (x - xlo) * (w) / (xhi - xlo) + gx;
    oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    // draw y scale
    for (i = ylo; i <= yhi; i += yinc)
    {
      // compute the transform
      temp = (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

      if (i == 0)
      {
        d.Line(gx, temp, gx + w, temp, acolor);
      }
      else
      {
        d.Line(gx, temp, gx + w, temp, gcolor);
      }

      d.TextSize(1);
      d.TextColor(tcolor, bcolor);
      d.MoveTo(gx - 40, temp);
      // precision is default Arduino--this could really use some format control
      d.println(i);
    }
    // draw x scale
    for (i = xlo; i <= xhi; i += xinc)
    {

      // compute the transform

      temp = (i - xlo) * (w) / (xhi - xlo) + gx;
      if (i == 0)
      {
        d.Line(temp, gy, temp, gy - h, acolor);
      }
      else
      {
        d.Line(temp, gy, temp, gy - h, gcolor);
      }

      d.TextSize(1);
      d.TextColor(tcolor, bcolor);
      d.MoveTo(temp, gy + 10);
      // precision is default Arduino--this could really use some format control
      d.println(i);
    }

    // now draw the labels
    d.TextSize(2);
    d.TextColor(tcolor, bcolor);
    d.MoveTo(gx, gy - h - 30);
    d.println(title);

    d.TextSize(1);
    d.TextColor(acolor, bcolor);
    d.MoveTo(gx, gy + 20);
    d.println(xlabel);

    d.TextSize(1);
    d.TextColor(acolor, bcolor);
    d.MoveTo(gx - 30, gy - h - 10);
    d.println(ylabel);
  }

  // graph drawn now plot the data
  //  the entire plotting code are these few lines...
  //  recall that ox and oy are initialized as static above
  x = (x - xlo) * (w) / (xhi - xlo) + gx;
  y = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  d.Line(ox, oy, x, y, pcolor);
  d.Line(ox, oy + 1, x, y + 1, pcolor);
  d.Line(ox, oy - 1, x, y - 1, pcolor);
  ox = x;
  oy = y;
}

// Function that draws a graph with the handled data
void drawGraph(unsigned char *Data, unsigned char selector, int maximum, bool redraw)
{
  char title[10];
  switch (selector)
  {
  case 0:
    strcpy(title, "Daydata");
    break;
  case 1:
    strcpy(title, "Weekdata");
    break;
  case 2:
    strcpy(title, "Monthdata");
    break;
  case 3:
    strcpy(title, "Yeardata");
    break;
  }
  for (int x = maximum; x > 0; x--)
  {
    if (Data[maximum - x] < ymax + 1)
      Graph(gfx, x, Data[maximum - x], 20, 230, 280, 200, 0, 288, 16, 0, 100, 10, title, "Time", "Volume(%)", DKBLUE, RED, YELLOW, WHITE, BLACK, redraw);
  }
}

unsigned char menuSelecting(unsigned char menuStateBefore)
{
  gfx.touch_Update();
  tx = gfx.touch_GetX();
  if (gfx.touch_GetPen() == TOUCH_RELEASED)
  {
    resetNow = true;
    if (tx < 180)
    {
      if (menuStateBefore == 0)
      {
        return 3;
      }
      return (menuStateBefore - 1);
    }
    else if (tx > 180)
    {
      if (menuStateBefore == 3)
      {
        return 0;
      }
      return (menuStateBefore + 1);
    }
  }
  return menuStateBefore;
}

void setup()
{
  // inits
  initializeGLCD(false, true, TOUCH_ENABLE, 0, 5);
  Serial.begin(115200);
  
  // Getting config from SD
  char ssid[20];
  char pswd[30];
  char filecontents[200];
  memset(filecontents, 0, sizeof(filecontents));
  memset(ssid, 0, sizeof(ssid));
  memset(pswd, 0, sizeof(pswd));
  memset(ip, 0, sizeof(ip));
  memset(URI, 0, sizeof(URI));
  readFileContents("config.txt", filecontents, sizeof(filecontents));
  readSetting(filecontents, sizeof(filecontents), "SSID", ssid, sizeof(ssid));
  readSetting(filecontents, sizeof(filecontents), "PSWD", pswd, sizeof(pswd));
  readSetting(filecontents, sizeof(filecontents), "IP", ip, sizeof(ip));
  readSetting(filecontents, sizeof(filecontents), "GET", URI, sizeof(URI));
  
  // Connecting to wifi
  gfx.TextColor(WHITE);
  gfx.Font(1);
  gfx.TextSize(1);
  gfx.print(F("Connecting to: "));
  gfx.println(ssid);
  gfx.print(F("IP: "));
  gfx.print(ip);
  gfx.println(URI);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    gfx.print(F("."));
  }
  gfx.println(F("Connected"));
}

void loop()
{
  state = menuSelecting(stateBefore);
  stateBefore = state;
  Serial.println(state);

  if ((millis() >= (timeBefore + 60000)) || resetNow)
  {
    timeBefore = millis();
    resetNow = false;
    redrawNow = true;
  }

  if (redrawNow)
  {
    redrawNow = false;
    char temp[1158];
    memset(temp, '0', sizeof(temp));
    unsigned char Data[290];
    memset(Data, 0, sizeof(Data));
    char GetParameter[12];
    sprintf(GetParameter, "?selector=%i", state);
    Serial.print(F("getParameter="));
    Serial.println(GetParameter);
    httpGETRequest(ip, URI, GetParameter, temp, sizeof(temp));
    convertToInt(temp, Data, sizeof(Data));
    drawGraph(Data, state, 288, true);
  }

  yield();
}
