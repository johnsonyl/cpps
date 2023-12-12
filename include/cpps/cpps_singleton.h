#pragma once
namespace cpps {

	template<class T>
	class Singleton {

	public:
		Singleton() {  };
		virtual ~Singleton() { 
		}
		static T* getInstance() {
			static T m_pInstance;
			return &m_pInstance;
		}

		Singleton(const Singleton&) = delete;
		Singleton& operator= (const Singleton) = delete;
	private:
		
	};
}
