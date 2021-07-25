/*
 * texture.h
 *
 *  Created on: 26 Mar 2021
 *      Author: mchlp
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/glew.h>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D
{
public:
    // texture image dimensions
    unsigned int m_width, m_height; // width and height of loaded image in pixels
    // texture Format
    unsigned int m_internalFormat; // format of texture object
    unsigned int m_imageFormat; // format of loaded image
    // texture configuration
    unsigned int m_wrapS; // wrapping mode on S axis
    unsigned int m_wrapT; // wrapping mode on T axis
    unsigned int m_filterMin; // filtering mode if texture pixels < screen pixels
    unsigned int m_filterMax; // filtering mode if texture pixels > screen pixels
    // constructor (sets default texture modes)
    Texture2D();
    // generates texture from image data
    void generate(unsigned int width, unsigned int height, unsigned char* data);
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void bind() const;
    // returns the texture's ID
    unsigned int getID() { return m_ID; }
    // sets internal format
    void setInternalFormat(unsigned int format) { m_internalFormat = format; }
    // sets image format
    void setImageFormat(unsigned int format) { m_imageFormat = format; }
    // gets image width
    int getWidth() { return m_width; }
    // gets image height
    int getHeight() { return m_height; }

private:
    // holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    unsigned int m_ID;
};

#endif /* TEXTURE_H_ */
