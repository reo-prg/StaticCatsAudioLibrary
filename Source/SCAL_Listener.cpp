#include "../include/SCAL_Listener.h"
#include <x3daudio.h>

namespace scal
{
	class Listener::Listener_Impl
	{
	public:
		Listener_Impl() = default;
		~Listener_Impl();



		X3DAUDIO_LISTENER listener_;


	};

	Listener::Listener_Impl::~Listener_Impl()
	{
		
	}


	Listener::Listener()
	{
	}
	Listener::~Listener()
	{
	}
}
