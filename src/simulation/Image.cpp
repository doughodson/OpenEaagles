
#include "openeaagles/simulation/Image.h"


namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(Image,"SarImage")
EMPTY_SLOTTABLE(Image)
EMPTY_SERIALIZER(Image)


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Image::Image()
{
    STANDARD_CONSTRUCTOR()

    id = 0;
    xmitEnble = false;
    latitude = 0.0;
    longitude = 0.0;
    elevation = 0.0;
    orientation = 0.0;
    resolution = 0.0;

    data = nullptr;
    size = 0;
    width = 0;
    height = 0;
    depth = 0;

    origData = nullptr;
    origSize = 0;
    origCompressed = 0;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Image::copyData(const Image& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        data = nullptr;
        size = 0;
        origData = nullptr;
        origSize = 0;
    }

    // Copy the SAR data
    setDatalinkTransmitEnable( org.xmitEnble );
    setImageId( org.getImageId() );
    setLatitude( org.getLatitude() );
    setLongitude( org.getLongitude() );
    setElevation( org.getElevation() );
    setOrientation( org.getOrientation() );
    setResolution( org.getResolution() );

    // Copy the images
    setImageData(org.getImageData(), org.getWidth(), org.getHeight(), org.getDepth());
    setOriginalImageData(org.getOrigImageData(), org.getOrigImageSize(), org.isOrigImageDataCompressed());
}

void Image::deleteData()
{
    // Clear the images
    setImageData(nullptr, 0, 0, 0);
    setOriginalImageData(nullptr, 0, 0);
}

//------------------------------------------------------------------------------
// setImageData() -- Load image data
//------------------------------------------------------------------------------
bool Image::setImageData(const unsigned char* const image, const unsigned int w, const unsigned int h, const unsigned int d)
{
    bool ok = true;     // Assume all will go well.

    // Allocate space for and transfer the new image
    unsigned int s = (w*h*d);
    unsigned char* buff = nullptr;
    if (image != nullptr) {
        // Allocate space
        buff = new unsigned char[s];
        if (buff != nullptr) {
            // Transfer the image
            for (unsigned int i = 0; i < s; i++) buff[i] = image[i];
        }
        else ok = false;    // we failed to allocate space
    }

    // If all's well, set our member variables
    if (ok) {
        // Clear the old image (if any)
        if (data != nullptr) delete[] data;
        data = buff;
        size = s;
        width = w;
        height = h;
        depth = d;
    }

    return ok;
}

//------------------------------------------------------------------------------
// setOrigialImageData() -- Load original image data
//------------------------------------------------------------------------------
bool Image::setOriginalImageData(const unsigned char* const ti, const unsigned int s, const bool c)
{
    bool ok = true;     // Assume all will go well.

    // Allocate space for and transfer the new image
    unsigned char* buff = nullptr;
    if (ti != nullptr) {
        // Allocate space
        buff = new unsigned char[s];
        if (buff != nullptr) {
            // Transfer the image
            for (unsigned int i = 0; i < s; i++) buff[i] = ti[i];
        }
        else ok = false;    // we failed to allocate space
    }

    // If all's well, set our member variables
    if (ok) {
        // Clear the old image (if any)
        if (origData != nullptr) delete[] origData;
        origData = buff;
        origSize = s;
        origCompressed = c;
    }

    return ok;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool Image::testImage(const unsigned w0, const unsigned h0)
{
    unsigned int d0 = 1;
    unsigned char* image = new unsigned char[w0*h0*d0];

    for (unsigned int h = 0; h < h0; h++) {
        for (unsigned int w = 0; w < w0; w++) {

            // Edges
            unsigned char c = 0x00;
            if (h < 64 || h >= (h0 - 32)) c = 0x7f;
            if (w < 32 || w >= (w0 - 16)) c += 0x7f;

            // Cross hair
            if (h == (h0/2) || w == (w0/2)) c = 0xff;

            // Store the color
            unsigned int os = (h * (w0 * d0)) + (w * d0);
            image[os] = c;
        }
    }

    bool ok = setImageData(image, w0, h0, d0);
    delete [] image;
    return ok;
}

} // End simulation namespace
}
