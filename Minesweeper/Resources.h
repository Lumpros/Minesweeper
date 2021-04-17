#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "Texture.h"

namespace Game
{
	template <typename Resource> class ResourceHolder
	{
		friend class Resources;

	private:
		std::unordered_map<std::string, std::unique_ptr<Resource>> resources;

	public:
		void Add(const std::string& name, Resource* resource) noexcept
		{
			if (resources.find(name) == resources.end())
			{
				resources.insert(std::make_pair(name, resource));
			}
		}

		Resource* Get(const std::string& name) noexcept
		{
			auto find_resource = resources.find(name);

			if (find_resource != resources.end())
			{
				return find_resource->second.get();
			}

			return nullptr;
		}
	};

	class Resources
	{
	private:
		Resources(void) noexcept;

	public:
		Resources(const Resources&) = delete;
		void operator=(const Resources&) = delete;

		static Resources* Get(void) noexcept;

		void ReleaseResources(void);

	public:
		ResourceHolder<Texture> gTextures;
	};
}

