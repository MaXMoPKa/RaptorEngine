#pragma once

#include <iostream>
#include <array>
#include <memory>

#include <stb_image/stb_image.h>

#include "SDL2/SDL_log.h"

#include "defines.hpp"

#include "render/texture_program.hpp"

#include "utils/file_system.h"

namespace raptor_engine
{
namespace render
{

class texture_manager
{
public:
	texture_manager() 
		: textures {}
		#if TESTS
		    , is_texture_file_opened {false}
		#endif
	{

	}

	texture_manager(texture_manager&& manager_) noexcept			= default;
	texture_manager& operator=(texture_manager&& manager_) noexcept = default;

	texture_manager(const texture_manager&)			   = delete;
	texture_manager& operator=(const texture_manager&) = delete;

	~texture_manager() = default;

public:
	texture_program_sptr add_texture(const texture_config_sptr& texture_)
	{
		int width, height, nrChannels;
		std::string	   path = file_system::get_path(texture_->get_texture_name()).c_str();
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		if (!data) 
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
							"ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ: can't read from file %s",
							texture_->get_texture_name());
			return std::make_shared<texture_program>();
		}
		#if TESTS
		    is_texture_file_opened = true;
		#endif

	    unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D,
						GL_TEXTURE_WRAP_S,
						GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 texture_->get_internal_format(),
					 width,
					 height,
					 0,
					 texture_->get_format(),
					 GL_UNSIGNED_BYTE,
					 data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		textures.emplace_back(std::make_shared<texture_program>(texture));

		return textures.back();
	}

public:
	#if TESTS
	    bool get_is_texture_file_opened() const
		{
			return this->is_texture_file_opened;
		}
	#endif


public:
	const std::vector<texture_program_sptr>& get_textures() const
	{
		return this->textures;
	}

private:
	std::vector<texture_program_sptr> textures;

	#if TESTS
	    bool is_texture_file_opened;
	#endif
};

using texture_manager_uptr = std::unique_ptr<texture_manager>;
using texture_manager_sptr = std::shared_ptr<texture_manager>;
using texture_manager_wptr = std::weak_ptr<texture_manager>;

} // namespace render
} // namespace raptor_engine