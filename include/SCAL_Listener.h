#pragma once
#include <memory>

namespace scal
{
	class Listener
	{
	public:
		Listener();
		~Listener();

	private:
		class Listener_Impl;
		std::unique_ptr<Listener_Impl> impl_;
	};

}
