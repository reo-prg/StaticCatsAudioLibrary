#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../include/SCAL_Math.h"

namespace scal
{
	class Listener
	{
	public:
		Listener();
		~Listener();

		void SetPosition(const Vector3& pos);

		void SetDefaultDirection(const Vector3& front, const Vector3& up);

		void SetRotate(const Vector3& axis, float rot);
		void SetRotate(float x, float y, float z);
		void SetRotateAsQuaternion(const DirectX::XMVECTOR& quat);
		void AddRotate(const Vector3& axis, float rot);
		void AddRotateAsQuaternion(const DirectX::XMVECTOR& quat);

		void EnableVelocity(bool enable = true);

		void UpdateVelocity(void);
	private:
		class Listener_Impl;
		std::unique_ptr<Listener_Impl> impl_;
	};

}
