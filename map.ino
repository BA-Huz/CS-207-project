// Arrays used by pointMap class
// having them within pointMap makes
// the class too large for arduino
int pointsArray1[160][40];
int pointsArray2[160][40];
int pointsArray3[160][40];
int pointsArray4[160][40];

// class for managing the map of distances
class pointMap
{
  public:
    pointMap()
    {
      for(int x = 0; x < 161; x++)
      {
        for(int y = 0; y < 40; y++)
        {
          pointsArray1[x][y] = 0;
          pointsArray2[x][y] = 0;
          pointsArray3[x][y] = 0;
          pointsArray4[x][y] = 0;
        }
        pointsArray4[x][40] = 0;
      }

    }
    
    ~pointMap()
    {}

    // function sets the value at (x, y) in the
    // 2d array collection to value
    void setPoint(int x, int y, int value)
    {
      if( y % 40 == 0)
        pointsArray1[x][y] = value;
      else if( y % 40 == 1)
        pointsArray2[x][y] = value;
      else if( y % 40 == 2)
        pointsArray2[x][y] = value;
      else if( y % 40 >= 3)
        pointsArray2[x][y] = value;
    }

    // function sets the value at (x, y) in the
    // 2d array collection
    int getPoint(int x, int y)
    {
      if( y % 40 == 0)
        return pointsArray1[x][y];
      else if( y % 40 == 1)
        return pointsArray2[x][y];
      else if( y % 40 == 2)
        return pointsArray2[x][y];
      else if( y % 40 >= 3)
        return pointsArray2[x][y];
    }

};
