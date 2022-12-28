/**
 * @file Texture.cpp
 * @author strah19
 * @date October 3 2021
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License as
 * published by the Free Software Foundation.
 *
 * @section DESCRIPTION
 *
 * This file represents a Texture object that can be rendered in Ember.
 */

#include "texture.h"
#include "log.h"

#include <iostream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Mu {
	void Texture::Init(const char* file_path) {
		m_path = file_path;

		int w, h, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(file_path, &w, &h, &channels, 0);

		m_width = w;
		m_height = h;
		if (channels == 4) {
			m_internal_format = GL_RGBA8;
			m_data_format = GL_RGBA;
		}
		else if (channels == 3) {
			m_internal_format = GL_RGB8;
			m_data_format = GL_RGB;
		}

		if (data) {
			glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_id);
			glTextureStorage2D(m_texture_id, 1, m_internal_format, m_width, m_height);

			glTextureParameteri(m_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_texture_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
			MU_LOG("Loaded texture '%s'", file_path);
		}
		else
			MU_LOG_ERROR("Failed to load texture '%s'", file_path);

		stbi_image_free(data);
	}

	void Texture::Init(uint32_t m_width, uint32_t m_height) {
		m_internal_format = GL_RGBA8;
		m_data_format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_id);
		glTextureStorage2D(m_texture_id, 1, m_internal_format, m_width, m_height);

		glTextureParameteri(m_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &m_texture_id);
	}

	void Texture::SetData(void* data) {
		uint32_t bpp = m_data_format == GL_RGBA ? 4 : 3;
		glTextureSubImage2D(m_texture_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
	}

	void Texture::Bind(uint32_t slot) {
		glBindTextureUnit(slot, m_texture_id);
	}

	void Texture::UnBind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
