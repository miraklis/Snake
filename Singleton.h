#ifndef SINGLETON_H
#define SINGLETON_H

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

#endif //SINGLETON_H