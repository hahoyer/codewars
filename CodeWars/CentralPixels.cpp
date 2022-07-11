// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

namespace CentralPixels
{
#include <string>

  struct Image
  {
    unsigned* pixels;
    unsigned width, height;

    std::vector<unsigned> central_pixels(unsigned colour) const;
    // other functions ...
  };

  struct ImagePlus
  {
    Image const& Image;

    unsigned GetDistance(unsigned colour, unsigned x, unsigned y) const;

    ImagePlus(struct Image const& image)
      : Image(image)


    {
      for(size_t y = 0; y < Image.height; y++)
        distanceCache.push_back({Image.width});
    }

    mutable std::vector<std::vector<unsigned>> distanceCache;
  };


  std::vector<unsigned> Image::central_pixels(unsigned colour) const
  {
    const ImagePlus imagePlus(*this);
    auto maximalDistance = 0u;
    for(auto y = 0u; y < height; y++)
      for(auto x = 0u; x < width; x++)
      {
        if(pixels[x + y * width] == colour)
        {
          if(const auto distance = imagePlus.GetDistance(colour, x, y); maximalDistance < distance)
            maximalDistance = distance;
        }
      }

    std::vector<unsigned> result;
    for(auto y = 0u; y < height; y++)
      for(auto x = 0u; x < width; x++)
      {
        if(pixels[x + y * width] == colour)
        {
          if(imagePlus.GetDistance(colour, x, y) == maximalDistance)
            result.push_back(x + y * width);
        }
      }
    return result;
  }

  unsigned ImagePlus::GetDistance(unsigned colour, unsigned x, unsigned y) const
  {
    if(colour != Image.pixels[x + y * Image.width])
      return 0;
    auto& result = distanceCache[x][y];
    if(result != 0)
    {
      if(x == 0 || y == 0 || x == Image.width - 1 || y == Image.height - 1) result = 1;
      else
        result = std::min({
          GetDistance(colour, x - 1, y), GetDistance(colour, x + 1, y), GetDistance(colour, x, y - 1),
          GetDistance(colour, x, y + 1)
        }) + 1;
    }
    return result;
  }

  using namespace std;

  /* ---------------------------------------------------------------------------------- */
  /*                               TESTS                                                */
  /* ---------------------------------------------------------------------------------- */
#if 0
  Describe(Centre_of_attention)
  {
    It(Example_In_The_Picture)
    {
      Image image({
                    1, 1, 4, 4, 4, 4, 2, 2, 2, 2,
                    1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
                    1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
                    1, 1, 1, 1, 1, 3, 2, 2, 2, 2,
                    1, 1, 1, 1, 1, 3, 3, 3, 2, 2,
                    1, 1, 1, 1, 1, 1, 3, 3, 3, 3
                  }, 10, 6);

      // Only one red pixel has the maximum depth of 3:
      vector<unsigned> red_ctr = {32};
      Assert::That(image.central_pixels(1), Fulfills(Unordered_Match(red_ctr)));

      // Multiple blue pixels have the maximum depth of 2:
      vector<unsigned> blue_ctr = {16, 17, 18, 26, 27, 28, 38};
      Assert::That(image.central_pixels(2), Fulfills(Unordered_Match(blue_ctr)));

      // All the green pixels have depth 1, so they are all "central":
      vector<unsigned> green_ctr = {35, 45, 46, 47, 56, 57, 58, 59};
      Assert::That(image.central_pixels(3), Fulfills(Unordered_Match(green_ctr)));

      // Similarly, all the purple pixels have depth 1:
      vector<unsigned> purple_ctr = {2, 3, 4, 5};
      Assert::That(image.central_pixels(4), Fulfills(Unordered_Match(purple_ctr)));

      // There are no pixels with colour 5:
      vector<unsigned> non_existent_ctr = {};
      Assert::That(image.central_pixels(5), Fulfills(Unordered_Match(non_existent_ctr)));

      // Changing one pixel can make a big difference to the result:
      image.pixels[32] = 3;
      vector<unsigned> new_ctr = {11, 21, 41, 43};
      Assert::That(image.central_pixels(1), Fulfills(Unordered_Match(new_ctr)));
    }
  };
#endif
}
