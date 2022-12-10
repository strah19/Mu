#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <memory>
#include <string>

namespace Mu {
	class Texture {
	public:
		Texture() = default;

		void Init(const char* file_path);
		void Init(uint32_t width, uint32_t height);

		virtual ~Texture();

		void Bind(uint32_t slot = 0);
		void UnBind();

		void SetData(void* data);

		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }
		uint32_t GetTextureId() const { return m_texture_id; }
	private:
		uint32_t m_texture_id = 0;

		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_internal_format = 0, m_data_format = 0;
		std::string m_path;
	};
}

#endif // !OPENGL_TEXTURE_H