/**********************************************************************************
 *                                                                                *
 *  DynELA Finite Element Code v.4.0                                              *
 *  by Olivier PANTALE                                                            *
 *  Olivier.Pantale@enit.fr                                                       *
 *                                                                                *
 *********************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

#include <ColorMap.h>

/*
@LABEL:ColorMap::ColorMap()
@SHORT:Default constructor of the ColorMap class.
@RETURN:ColorMap & The new ColorMap object created by the constructor.
This is the default constructor of the ColorMap class. By default, the associated color map is the deep color map.
@END
*/
//-----------------------------------------------------------------------------
ColorMap::ColorMap()
//-----------------------------------------------------------------------------
{
  // Set deep color map by default
  setDeepColorMap();
}

//  Default destructor
//-----------------------------------------------------------------------------
ColorMap::~ColorMap()
//-----------------------------------------------------------------------------
{
}

/*
@LABEL:ColorMap::clearColorMap()
@SHORT:Clears the current color map.
@END
*/
//-----------------------------------------------------------------------------
void ColorMap::clearColorMap()
//-----------------------------------------------------------------------------
{
  _redComponent.flush();
  _greenComponent.flush();
  _blueComponent.flush();
}

/*
@LABEL:ColorMap::setColorMap()
@SHORT:Select the color map.
Select the color map for drawings.
The color map is the one represented here after, from blue for low values (left side of the proposed bar) to red for high values (right side of the proposed bar).
\begin{center}
\includegraphics{Figures/ProgrammingLanguage/ColorMap}
\end{center}
@END
*/
//-----------------------------------------------------------------------------
void ColorMap::setColorMap()
//-----------------------------------------------------------------------------
{
  clearColorMap();
  double last = double(_levels - 1) / _levels;
  _currentColorMap = NormalCM;

  // 0.00 - Blue color
  _redComponent.add(0.00, BLUE(0));
  _greenComponent.add(0.00, BLUE(1));
  _blueComponent.add(0.00, BLUE(2));

  // 0.25 - Cyan color
  _redComponent.add(last / 4, CYAN(0));
  _greenComponent.add(last / 4, CYAN(1));
  _blueComponent.add(last / 4, CYAN(2));

  // 0.50 - Green color
  _redComponent.add(last / 2, GREEN(0));
  _greenComponent.add(last / 2, GREEN(1));
  _blueComponent.add(last / 2, GREEN(2));

  // 0.75 - Yellow color
  _redComponent.add(3 * last / 4, YELLOW(0));
  _greenComponent.add(3 * last / 4, YELLOW(1));
  _blueComponent.add(3 * last / 4, YELLOW(2));

  // 1.00 - Red color
  _redComponent.add(last, RED(0));
  _greenComponent.add(last, RED(1));
  _blueComponent.add(last, RED(2));

  // 1.00 - Red color
  _redComponent.add(1.00, RED(0));
  _greenComponent.add(1.00, RED(1));
  _blueComponent.add(1.00, RED(2));
}

/*
@LABEL:ColorMap::setReverseDeepColorMap()
@SHORT:Select the reversed deep color map.
Select the reversed deep color map for drawings.
The reversed deep color map is the one represented here after, from dark red for low values (left side of the proposed bar) to dark blue for high values (right side of the proposed bar).
\begin{center}
\includegraphics{Figures/ProgrammingLanguage/ReverseDeepColorMap}
\end{center}
@END
*/
//-----------------------------------------------------------------------------
void ColorMap::setReverseDeepColorMap()
//-----------------------------------------------------------------------------
{
  clearColorMap();
  double last = double(_levels - 1) / _levels;
  _currentColorMap = RDeepCM;

  // 0.00 - Dark Blue color
  _redComponent.add(0, DARKRED(0));
  _greenComponent.add(0, DARKRED(1));
  _blueComponent.add(0, DARKRED(2));

  // 0.00 - Blue color
  _redComponent.add(last / 6, RED(0));
  _greenComponent.add(last / 6, RED(1));
  _blueComponent.add(last / 6, RED(2));

  // 0.25 - Cyan color
  _redComponent.add(2 * last / 6, YELLOW(0));
  _greenComponent.add(2 * last / 6, YELLOW(1));
  _blueComponent.add(2 * last / 6, YELLOW(2));

  // 0.50 - Green color
  _redComponent.add(last / 2, GREEN(0));
  _greenComponent.add(last / 2, GREEN(1));
  _blueComponent.add(last / 2, GREEN(2));

  // 0.75 - Yellow color
  _redComponent.add(2 * last / 3, CYAN(0));
  _greenComponent.add(2 * last / 3, CYAN(1));
  _blueComponent.add(2 * last / 3, CYAN(2));

  // 1.00 - Red color
  _redComponent.add(5 * last / 6, BLUE(0));
  _greenComponent.add(5 * last / 6, BLUE(1));
  _blueComponent.add(5 * last / 6, BLUE(2));

  // 1.00 - Red color
  _redComponent.add(last, DARKBLUE(0));
  _greenComponent.add(last, DARKBLUE(1));
  _blueComponent.add(last, DARKBLUE(2));

  // 1.00 - Red color
  _redComponent.add(1.00, DARKBLUE(0));
  _greenComponent.add(1.00, DARKBLUE(1));
  _blueComponent.add(1.00, DARKBLUE(2));
}

/*
@LABEL:ColorMap::setDeepColorMap()
@SHORT:Select the deep color map.
Select the deep color map for drawings.
The deep color map is the one represented here after, from dark blue for low values (left side of the proposed bar) to dark red for high values (right side of the proposed bar).
\begin{center}
\includegraphics{Figures/ProgrammingLanguage/DeepColorMap}
\end{center}
@END
*/
//-----------------------------------------------------------------------------
void ColorMap::setDeepColorMap()
//-----------------------------------------------------------------------------
{
  clearColorMap();
  double last = double(_levels - 1) / _levels;
  _currentColorMap = DeepCM;

  // 0.00 - Dark Blue color
  _redComponent.add(0, DARKBLUE(0));
  _greenComponent.add(0, DARKBLUE(1));
  _blueComponent.add(0, DARKBLUE(2));

  // 0.00 - Blue color
  _redComponent.add(last / 6, BLUE(0));
  _greenComponent.add(last / 6, BLUE(1));
  _blueComponent.add(last / 6, BLUE(2));

  // 0.25 - Cyan color
  _redComponent.add(2 * last / 6, CYAN(0));
  _greenComponent.add(2 * last / 6, CYAN(1));
  _blueComponent.add(2 * last / 6, CYAN(2));

  // 0.50 - Green color
  _redComponent.add(last / 2, GREEN(0));
  _greenComponent.add(last / 2, GREEN(1));
  _blueComponent.add(last / 2, GREEN(2));

  // 0.75 - Yellow color
  _redComponent.add(2 * last / 3, YELLOW(0));
  _greenComponent.add(2 * last / 3, YELLOW(1));
  _blueComponent.add(2 * last / 3, YELLOW(2));

  // 1.00 - Red color
  _redComponent.add(5 * last / 6, RED(0));
  _greenComponent.add(5 * last / 6, RED(1));
  _blueComponent.add(5 * last / 6, RED(2));

  // 1.00 - Red color
  _redComponent.add(last, DARKRED(0));
  _greenComponent.add(last, DARKRED(1));
  _blueComponent.add(last, DARKRED(2));

  // 1.00 - Red color
  _redComponent.add(1.00, DARKRED(0));
  _greenComponent.add(1.00, DARKRED(1));
  _blueComponent.add(1.00, DARKRED(2));
}

/*
@LABEL:ColorMap::setReverseColorMap()
@SHORT:Select the color map.
Select the color map for drawings.
The color map is the one represented here after, from red for low values (left side of the proposed bar) to blue for high values (right side of the proposed bar).
\begin{center}
\includegraphics{Figures/ProgrammingLanguage/ReverseColorMap}
\end{center}
@END
*/
//-----------------------------------------------------------------------------
void ColorMap::setReverseColorMap()
//-----------------------------------------------------------------------------
{
  clearColorMap();
  double last = double(_levels - 1) / _levels;
  _currentColorMap = RNormalCM;

  // 0.00 - Blue color
  _redComponent.add(0, RED(0));
  _greenComponent.add(0, RED(1));
  _blueComponent.add(0, RED(2));

  // 0.25 - Cyan color
  _redComponent.add(last / 4, YELLOW(0));
  _greenComponent.add(last / 4, YELLOW(1));
  _blueComponent.add(last / 4, YELLOW(2));

  // 0.50 - Green color
  _redComponent.add(last / 2, GREEN(0));
  _greenComponent.add(last / 2, GREEN(1));
  _blueComponent.add(last / 2, GREEN(2));

  // 0.75 - Yellow color
  _redComponent.add(3 * last / 4, CYAN(0));
  _greenComponent.add(3 * last / 4, CYAN(1));
  _blueComponent.add(3 * last / 4, CYAN(2));

  // 1.00 - Red color
  _redComponent.add(last, BLUE(0));
  _greenComponent.add(last, BLUE(1));
  _blueComponent.add(last, BLUE(2));

  // 1.00 - Red color
  _redComponent.add(1.00, BLUE(0));
  _greenComponent.add(1.00, BLUE(1));
  _blueComponent.add(1.00, BLUE(2));
}

/*
@LABEL:ColorMap::setGrayMap()
@SHORT:Select the gray color map.
Select the gray color map for drawings.
The gray color map is the one represented here after, from black for low values (left side of the proposed bar) to white for high values (right side of the proposed bar).
\begin{center}
\includegraphics{Figures/ProgrammingLanguage/GrayMap}
\end{center}
@END
*/
//-----------------------------------------------------------------------------
void ColorMap::setGrayMap()
//-----------------------------------------------------------------------------
{
  clearColorMap();
  double last = double(_levels - 1) / _levels;
  _currentColorMap = GrayCM;

  // 0.00 - Blue color
  _redComponent.add(0, BLACK(0));
  _greenComponent.add(0, BLACK(1));
  _blueComponent.add(0, BLACK(2));

  _redComponent.add(last, WHITE(0));
  _greenComponent.add(last, WHITE(1));
  _blueComponent.add(last, WHITE(2));

  _redComponent.add(1.00, WHITE(0));
  _greenComponent.add(1.00, WHITE(1));
  _blueComponent.add(1.00, WHITE(2));
}

/*
@LABEL:ColorMap::setReverseGrayMap()
@SHORT:Select the gray color map.
Select the gray color map for drawings.
The gray color map is the one represented here after, from white for low values (left side of the proposed bar) to black for high values (right side of the proposed bar).
\begin{center}
\includegraphics{Figures/ProgrammingLanguage/ReverseGrayMap}
\end{center}
@END
*/
//-----------------------------------------------------------------------------
void ColorMap::setReverseGrayMap()
//-----------------------------------------------------------------------------
{
  clearColorMap();
  double last = double(_levels - 1) / _levels;
  _currentColorMap = RGrayCM;

  // 0.00 - Blue color
  _redComponent.add(0, WHITE(0));
  _greenComponent.add(0, WHITE(1));
  _blueComponent.add(0, WHITE(2));

  // 1.00 - Red color
  _redComponent.add(last, BLACK(0));
  _greenComponent.add(last, BLACK(1));
  _blueComponent.add(last, BLACK(2));

  // 1.00 - Red color
  _redComponent.add(1.00, BLACK(0));
  _greenComponent.add(1.00, BLACK(1));
  _blueComponent.add(1.00, BLACK(2));
}

/*
@LABEL:ColorMap::resetColorMap()
@SHORT:Reset and recomputes the color map.
@END
*/
//-----------------------------------------------------------------------------
void ColorMap::resetColorMap()
//-----------------------------------------------------------------------------
{
  switch (_currentColorMap)
  {
  case DeepCM:
    setDeepColorMap();
    break;
  case RDeepCM:
    setReverseDeepColorMap();
    break;
  case NormalCM:
    setColorMap();
    break;
  case RNormalCM:
    setReverseColorMap();
    break;
  case GrayCM:
    setGrayMap();
    break;
  case RGrayCM:
    setReverseGrayMap();
    break;
  }
}

/*
@LABEL:ColorMap::setBounds(double min, double max)
@ARGS:double & min & Min value to use for the bounds of the color map.
@ARGS:double & max & Max value to use for the bounds of the color map.
@SHORT:Sets the boundary values of the current color map.
@END
*/
//-----------------------------------------------------------------------------
void ColorMap::setBounds(double min, double max)
//-----------------------------------------------------------------------------
{
  _min = min;
  _max = max;
}

/*
@LABEL:ColorMap::setLevels(int l)
@ARGS:int & l & Number of levels of the color map.
@SHORT:Sets the number of levels of the current color map.
@END
*/
//-----------------------------------------------------------------------------
void ColorMap::setLevels(int levels)
//-----------------------------------------------------------------------------
{
  _levels = levels;
  resetColorMap();
}

/*
@LABEL:ColorMap::getIntColor(double val)
@ARGS:double & val & Value color value from 0 to 1.
@RETURN: int & The index of the color.
@SHORT:Get the color index of a value.
@END
*/
//-----------------------------------------------------------------------------
int ColorMap::getIntColor(double value)
//-----------------------------------------------------------------------------
{
  // If less than min
  if (value < _min)
    return -1;

  // If more than max
  if (value > _max)
    return _levels;

  if (_max == _min)
    return 0;

  if (value == _max)
    return _levels - 1;

  // Compute color using palette
  return int((value - _min) / (_max - _min) * _levels);
}

/*
@LABEL:ColorMap::getVec3DColor(double val, bool s)
@ARGS:double & val & Value color value from 0 to 1.
@ARGS:double & s & If the steps have to be takent into account.
@RETURN: Vec3D & the RGB components of the color.
@SHORT:Get the color as RGB components.
If the color is out of range, this method returns black if the value is lower than the minimum value, white if it is larger than the highest value.
@END
*/
//-----------------------------------------------------------------------------
Vec3D ColorMap::getVec3DColor(double value, bool steps)
//-----------------------------------------------------------------------------
{
  if (_max - _min < 1e-8)
    return GRAY;

  // If less than min
  if (value < _min)
    return BLACK;

  // If more than max
  if (value > _max)
    return WHITE;

  if (value == _max)
    return Vec3D(_redComponent.getValue(1), _greenComponent.getValue(1), _blueComponent.getValue(1));

  // Compute color using palette
  double val = (value - _min) / (_max - _min);

  if (steps)
    val = double(int(val * _levels)) / _levels;

  return Vec3D(_redComponent.getValue(val), _greenComponent.getValue(val), _blueComponent.getValue(val));
}

/*
@LABEL:ColorMap::getStringColor(double val, bool s)
@ARGS:double & val & Value color value from 0 to 1.
@ARGS:double & s & If the steps have to be takent into account.
@RETURN: String & the String components of the color.
@SHORT:Get the color as String components.
If the color is out of range, this method returns black if the value is lower than the minimum value, white if it is larger than the highest value.
@END
*/
//-----------------------------------------------------------------------------
String ColorMap::getStringColor(double value, bool steps)
//-----------------------------------------------------------------------------
{
  char hex[3];
  Vec3D color = getVec3DColor(value, steps);
  String code = "#";

  sprintf(hex, "%02x", int(color(0) * 255));
  code += hex;
  sprintf(hex, "%02x", int(color(1) * 255));
  code += hex;
  sprintf(hex, "%02x", int(color(2) * 255));
  code += hex;
  return code;
}

/*
@LABEL:ColorMap::getBounds(double min, double, max, int l)
@ARGS:double & min & The minimum value of the range (returned value).
@ARGS:double & max & The maximum value of the range (returned value).
@ARGS:int & l & The number of levels (returned value).
@SHORT:Get the boundaries of the current color map.
@END
*/
//-----------------------------------------------------------------------------
void ColorMap::getBounds(double &min, double &max, int &levels)
//-----------------------------------------------------------------------------
{
  min = _min;
  max = _max;
  levels = _levels;
}

/*
@LABEL:ColorMap::getMax()
@RETURN:double & The maximum value of the range.
@SHORT:Get the max value of the current color map.
@END
*/
//-----------------------------------------------------------------------------
double ColorMap::getMax()
//-----------------------------------------------------------------------------
{
  return _max;
}

/*
@LABEL:ColorMap::getMin()
@RETURN:double & The minimum value of the range.
@SHORT:Get the min value of the current color map.
@END
*/
//-----------------------------------------------------------------------------
double ColorMap::getMin()
//-----------------------------------------------------------------------------
{
  return _min;
}

/*
@LABEL:ColorMap::getLevels()
@RETURN:int & The number of levels.
@SHORT:Get the number of levels of the current color map.
@END
*/
//-----------------------------------------------------------------------------
int ColorMap::getLevels()
//-----------------------------------------------------------------------------
{
  return _levels;
}
