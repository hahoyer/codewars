// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

namespace CentralPixels
{
#include <string>
  using namespace std;

  struct Image
  {
    unsigned* pixels;
    unsigned width, height;

    Image();
    Image(std::initializer_list<unsigned> data, unsigned w, unsigned h);
    ~Image();
    void resize(unsigned w, unsigned h);

    std::vector<unsigned> central_pixels(unsigned colour) const; // to be written by the codewarrior
  };

  /* ---------------------------------------------------------------------------------- */

  Image::Image()
    : pixels(nullptr), width(0), height(0)
  {}

  Image::Image(std::initializer_list<unsigned> data, unsigned w, unsigned h)
    : width(w), height(h)
  {
    pixels = new unsigned[w * h];
    unsigned* p = pixels;
    for(auto d : data)
      *p++ = d;
  }

  /* ---------------------------------------------------------------------------------- */

  Image::~Image()
  {
    delete [] pixels;
  }

  /* ---------------------------------------------------------------------------------- */
  // Resize the image. Existing pixel data is not preserved.

  void Image::resize(unsigned w, unsigned h)
  {
    width = w;
    height = h;
    delete [] pixels;
    pixels = new unsigned[w * h];
  }

  /* ---------------------------------------------------------------------------------- */
  /* ---------------------------------------------------------------------------------- */
  /* ---------------------------------------------------------------------------------- */

  struct ImagePlus
  {
  private:
    Image const& Image;
    unsigned width, height;
    std::vector<std::vector<unsigned>> distanceCache;
    mutable std::vector<string> distanceDump;
    mutable std::vector<string> pixelDump;
  public:
    explicit ImagePlus(struct Image const& image);

    void GetMaximalDistanceForward(unsigned color);
    unsigned GetMaximalDistanceBackward(unsigned color);
    vector<unsigned> CollectPixels(unsigned color, unsigned max) const;

  private:
    unsigned GetDistance(unsigned color, unsigned x, unsigned y) const;
    unsigned GetPixel(unsigned x, unsigned y) const;

    void OnDistanceChanged() const;
    void OnImageChanged() const;
  };

  std::vector<unsigned> Image::central_pixels(unsigned color) const
  {
    auto imagePlus = ImagePlus(*this);
    imagePlus.GetMaximalDistanceForward(color);
    const auto maximum = imagePlus.GetMaximalDistanceBackward(color);
    return imagePlus.CollectPixels(color, maximum);
  }

  ImagePlus::ImagePlus(struct Image const& image): Image(image), width(image.width), height(image.height)
  {
    OnImageChanged();
    for(size_t y = 0; y < height; y++)
      distanceCache.emplace_back(width);
    OnDistanceChanged();
  }

  void ImagePlus::GetMaximalDistanceForward(unsigned color)
  {
    for(auto y = 1u; y < height - 1; y++)
      for(auto x = 1u; x < width - 1; x++)
        if(GetPixel(x, y) == color)
        {
          distanceCache[y][x] = min(GetDistance(color, x - 1, y), GetDistance(color, x, y - 1));
          OnDistanceChanged();
        }
  }

  unsigned ImagePlus::GetMaximalDistanceBackward(unsigned color)
  {
    unsigned result = 0;

    for(auto y = height - 2; y > 0; y--)
      for(auto x = width - 2; x > 0; x--)
        if(GetPixel(x, y) == color)
        {
          const auto distance = min(GetDistance(color, x + 1, y), GetDistance(color, x, y + 1));
          if(distanceCache[y][x] >= distance)
          {
            distanceCache[y][x] = distance;
            OnDistanceChanged();
            if(result < distance)
              result = distance;
          }
        }

    return result;
  }

  vector<unsigned> ImagePlus::CollectPixels(unsigned color, unsigned max) const
  {
    std::vector<unsigned> result;
    for(auto y = 0u; y < height; y++)
      for(auto x = 0u; x < width; x++)
        if(GetPixel(x, y) == color && distanceCache[y][x] == max)
          result.push_back(x + y * width);
    return result;
  }

  unsigned ImagePlus::GetDistance(unsigned color, unsigned x, unsigned y) const
  {
    if(color == GetPixel(x, y))
      return distanceCache[y][x] + 1;
    return 0;
  }

  void ImagePlus::OnDistanceChanged() const
  {
    distanceDump = {};
    for(const auto& line : distanceCache)
    {
      string dump;
      for(const auto distance : line)
        dump += static_cast<char>('0' + distance);
      distanceDump.push_back(dump);
    }
  }

  void ImagePlus::OnImageChanged() const
  {
    pixelDump = {};
    for(auto y = 0u; y < height; y++)
    {
      string dump;
      for(auto x = 0u; x < width; x++)
        dump += static_cast<char>('0' + GetPixel(x, y));
      pixelDump.push_back(dump);
    }
  }

  unsigned ImagePlus::GetPixel(unsigned x, unsigned y) const
  {
    return Image.pixels[x + y * width];
  }

  using namespace std;

  /* ---------------------------------------------------------------------------------- */
  /*                               TESTS                                                */
  /* ---------------------------------------------------------------------------------- */
  /** Snowhouse Assert setup for checking solutions.
  Solutions are vectors (of unsigned int), but the order of elements within them isn't important. */

  struct Unordered_Match
  {
    Unordered_Match(const std::vector<unsigned>& expected);
    bool Matches(const std::vector<unsigned>& actual) const;

    std::vector<unsigned> expected_;
  };

  Unordered_Match::Unordered_Match(const std::vector<unsigned>& expected)
    : expected_(expected)
  {
    sort(expected_.begin(), expected_.end());
  }

  bool Unordered_Match::Matches(const std::vector<unsigned>& actual) const
  {
    if(actual.size() != expected_.size())
      return false;

    std::vector<unsigned> a = actual;
    sort(a.begin(), a.end());

    unsigned i;
    for(i = 0; i != a.size() && a[i] == expected_[i]; i++);
    return i == a.size();
  }

  std::ostream& operator<<(std::ostream& stm, const Unordered_Match& crit)
  {
    stm << Stringizer<std::vector<unsigned>>::ToString(crit.expected_);
    return stm;
  }

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

      // There are no pixels with color 5:
      vector<unsigned> non_existent_ctr = {};
      Assert::That(image.central_pixels(5), Fulfills(Unordered_Match(non_existent_ctr)));

      // Changing one pixel can make a big difference to the result:
      image.pixels[32] = 3;
      vector<unsigned> new_ctr = {11, 21, 41, 43};
      Assert::That(image.central_pixels(1), Fulfills(Unordered_Match(new_ctr)));
    }
  };
}
