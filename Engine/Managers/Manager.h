/*
Abstract class,
Base implementation for all managers.
*/

#pragma once
namespace CMP316engine {
	class Manager
	{
	public:
		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;
	};
}

