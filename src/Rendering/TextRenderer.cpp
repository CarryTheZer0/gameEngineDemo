/*
 * TextRenderer.cpp
 *
 *  Created on: 5 Aug 2021
 *      Author: mchlp
 */

#include "TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

TextRenderer::TextRenderer(Shader &shader) :
	m_batchOffset(0),
	m_texCount(0),
	m_shader(shader)
{
	initRenderData();
	initFont();
}

TextRenderer::~TextRenderer()
{
    glDeleteVertexArrays(1, &m_quadVAO);
}

void TextRenderer::drawString(std::string text, float x, float y, float scale)
{
	int textureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
	int maxTexUnits = 32; // temp line

	m_shader.use();
	int samplers[maxTexUnits];
	for (int i = 0; i < maxTexUnits; i++)
    	samplers[i] = i;
    m_shader.setIntegerList("text", maxTexUnits, samplers);

	m_shader.setVector3f("textColor", 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_quadVAO);

	for (char c : text)
	{
		// draw current batch if texture units are full
		if (m_texCount >= textureUnits || m_texCount >= maxTexUnits)
		{
			draw();
			m_texCount = 0;
			m_texIDs.clear();
		}

		Character ch = m_characters[c];

		// identify required texture unit
	    int textureSlot;
	    if(!(m_texIDs.count(ch.textureID)))
	    {
	    	// Add texture to map if not present
	    	m_texIDs.insert(std::pair<unsigned int, int>(ch.textureID, m_texCount));
	    	textureSlot = m_texCount;
	    	m_texCount++;
	    }
	    else
	    {
	    	// Get texture unit if already present
	        textureSlot = m_texIDs[ch.textureID];
	    }

		float xPos = x + ch.bearing.x * scale;
		float yPos = y + (ch.size.y - ch.bearing.y) * scale + m_lineHeight;


		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		// generate vertex data
		float vertices[] = {
			// pos         	 	// texPos   //texID
			xPos, yPos,   		0.0f, 1.0f, textureSlot,
			xPos, yPos - h, 	0.0f, 0.0f, textureSlot,
			xPos + w, yPos - h, 1.0f, 0.0f, textureSlot,
			xPos + w, yPos, 	1.0f, 1.0f, textureSlot
		};

		// generate index data
		uint32_t indices[] = {
	    		(m_batchOffset * 4) + 0, (m_batchOffset * 4) + 1, (m_batchOffset * 4) + 2,
				(m_batchOffset * 4) + 2, (m_batchOffset * 4) + 3, (m_batchOffset * 4) + 0
		};

		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, ch.textureID);

		// write to vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
		glBufferSubData(GL_ARRAY_BUFFER, m_batchOffset * sizeof(vertices), sizeof(vertices), vertices);

		// write to index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_batchOffset * sizeof(indices), sizeof(indices), indices);

		x += (ch.advance >> 6) * scale;

		m_batchOffset++;
	}

	draw();

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::draw()
{
	// draw 2 triangles per sprite
	m_shader.use();
	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);
	glDrawElements(GL_TRIANGLES, m_batchOffset * 6, GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_batchOffset = 0;
}

void TextRenderer::initRenderData()
{
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glGenBuffers(1, &m_quadIBO);

    float vertices[] = {
            // pos      // texPos
            0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    		1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    		1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 1000, nullptr, GL_DYNAMIC_DRAW);

    uint32_t indices[] = {
    		0, 1, 2, 2, 3, 0
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * 1000, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(m_quadVAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)8);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)16);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::initFont()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, "Resources/Fonts/arial.ttf", 0, &face))
    {
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    m_lineHeight = face->size->metrics.height / 64;

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            continue;
        }

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x
        };
        m_characters.insert(std::pair<char, Character>(c, character));
    }
}
