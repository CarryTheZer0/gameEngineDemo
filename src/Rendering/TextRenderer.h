/*
 * TextRenderer.h
 *
 *  Created on: 5 Aug 2021
 *      Author: mchlp
 */

#ifndef RENDERING_TEXTRENDERER_H_
#define RENDERING_TEXTRENDERER_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>

#include "Shader.h"
#include "Texture.h"

class TextRenderer
{
public:
    // Constructor (inits shaders/shapes)
	TextRenderer(Shader &shader);
    // Destructor
    ~TextRenderer();
    // Renders text
    void drawString(std::string text, float x, float y, float scale);
    void draw();
private:
    struct Character {
        unsigned int textureID;  // ID handle of the glyph texture
        glm::ivec2   size;       // Size of glyph
        glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
        unsigned int advance;    // Offset to advance to next glyph
    };

    // Render state
    Shader		 m_shader;
    unsigned int m_quadVAO;
    unsigned int m_quadVBO;
    unsigned int m_quadIBO;
    int m_texCount;
    int m_batchOffset;
    std::map<unsigned int, int> m_texIDs;

    int m_lineHeight;
    std::map<char, Character> m_characters;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
    void initFont();
};

#endif /* RENDERING_TEXTRENDERER_H_ */
