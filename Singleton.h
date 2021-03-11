#pragma once

namespace Snake {

	template<typename T>
	class Singleton
	{
	protected:
		Singleton() {}
	public:
		virtual ~Singleton() { }
		static T& GetInstance() {
			static T instance;
			return instance;
		}
	};

}